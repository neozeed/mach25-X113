/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	hd.c,v $
 * Revision 1.8.1.16  91/07/15  00:16:46  rvb
 * 	Rationalize p_flag
 * 	[91/07/10            rvb]
 * 
 * 	Kill nuisance print out.
 * 	[91/07/09            rvb]
 * 
 * 	Use BIOS magic to find number of drives.  CMOS RAM does not
 * 	always work.
 * 	[91/06/28            rvb]
 * 
 * Revision 1.8.1.15  90/11/27  13:42:04  rvb
 * 	Synched 2.5 & 3.0 at I386q (r1.8.1.15) & XMK35 (r2.4)
 * 	[90/11/15            rvb]
 * 
 * Revision 1.8.1.14  90/09/18  08:38:49  rvb
 * 	Typo & vs && at line 592.		[contrib]
 * 	Make Status printout on error only conditional on hd_print_error.
 * 	So we can get printout during clobber_my_disk.
 * 	[90/09/08            rvb]
 * 
 * Revision 1.8.1.13  90/08/25  15:44:38  rvb
 * 	Use take_<>_irq() vs direct manipulations of ivect and friends.
 * 	[90/08/20            rvb]
 * 
 * Revision 1.8.1.12  90/07/27  11:25:30  rvb
 * 	Fix Intel Copyright as per B. Davies authorization.
 * 	Let anyone who as opened the disk do absolute io.
 * 	[90/07/27            rvb]
 * 
 * Revision 1.8.1.11  90/07/10  11:43:22  rvb
 * 	Unbelievable bug in setcontroller.
 * 	New style probe/slave/attach.
 * 	[90/06/15            rvb]
 * 
 * Revision 1.8.1.10  90/03/29  19:00:00  rvb
 * 	Conditionally, print out state info for "state error".
 * 	[90/03/26            rvb]
 * 
 * Revision 1.8.1.8  90/03/10  00:27:20  rvb
 * 	Fence post error iff (bp->b_blkno + hh.blocktotal ) > partition_p->p_size)
 * 	[90/03/10            rvb]
 * 
 * Revision 1.8.1.7  90/02/28  15:49:35  rvb
 * 	Fix numerous typo's in Olivetti disclaimer.
 * 	[90/02/28            rvb]
 * 
 * Revision 1.8.1.6  90/01/16  15:54:14  rvb
 * 	FLush pdinfo/vtoc -> evtoc
 * 	[90/01/16            rvb]
 * 
 * 	Must be able to return "dos{cyl,head,sector}"
 * 	[90/01/12            rvb]
 * 
 * 	Be careful about p_size bound's checks if B_MD1 is true.
 * 	[90/01/12            rvb]
 * 
 * Revision 1.8.1.5  90/01/08  13:29:29  rvb
 * 	Add Intel copyright.
 * 	Add Olivetti copyright.
 * 	[90/01/08            rvb]
 * 
 * 	It is no longer possible to set the start and size of disk
 * 	partition "PART_DISK" -- it is always loaded from the DOS
 * 	partition data.
 * 	[90/01/08            rvb]
 * 
 * Revision 1.8.1.4  90/01/02  13:54:58  rvb
 * 	Temporarily regress driver to one that is known to work with Vectra's.
 * 
 */
 
/*
  Copyright 1988, 1989 by Intel Corporation, Santa Clara, California.

		All Rights Reserved

Permission to use, copy, modify, and distribute this software and
its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appears in all
copies and that both the copyright notice and this permission notice
appear in supporting documentation, and that the name of Intel
not be used in advertising or publicity pertaining to distribution
of the software without specific, written prior permission.

INTEL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL INTEL BE LIABLE FOR ANY SPECIAL, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT,
NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
 *  AT Hard Disk Driver
 *  Copyright Ing. C. Olivetti & S.p.A. 1989
 *  All rights reserved.
 *
 */
/*
  Copyright 1988, 1989 by Olivetti Advanced Technology Center, Inc.,
Cupertino, California.

		All Rights Reserved

  Permission to use, copy, modify, and distribute this software and
its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appears in all
copies and that both the copyright notice and this permission notice
appear in supporting documentation, and that the name of Olivetti
not be used in advertising or publicity pertaining to distribution
of the software without specific, written prior permission.

  OLIVETTI DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL OLIVETTI BE LIABLE FOR ANY SPECIAL, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT,
NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUR OF OR IN CONNECTION
WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


typedef long paddr_t;

#include <hd.h>

#ifdef	MACH_KERNEL
#include <sys/types.h>
#include <device/buf.h>
#include <device/errno.h>
#else	MACH_KERNEL
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/buf.h>
#include <sys/errno.h>
#include <sys/user.h>
#include <sys/vmparam.h>
#include <sys/uio.h>
#endif	MACH_KERNEL

#include <i386/ipl.h>
#include <i386at/atbus.h>
#include <i386at/hdreg.h>
#include <i386at/disk.h>

#define WHOLE_DISK(unit) ( (unit << 4) + PART_DISK )

#define NOP_DELAY {asm("nop");}

#define PRIBIO	20
#define PAGESIZ 	4096

#define CMOS_ADDR	0x70	/* I/O port address for CMOS ram addr */
#define CMOS_DATA	0x71	/* I/O port address for CMOS ram data */
#define HDTBL		0x12	/* byte offset of the disk type in CMOS ram */

/*
 * Use ATA LBA28 task-file addressing for normal read/write I/O.
 *
 * The old driver always decomposes a block number into CHS and puts
 * cylinder/head/sector into the task-file registers.  That works only
 * while the fake disk geometry can describe the requested block.
 *
 * Setting the LBA bit in the drive/head register makes the same READ
 * and WRITE commands interpret sector/cylinder/head as a 28-bit LBA:
 *
 *   sector        = LBA bits  0..7
 *   cylinder low  = LBA bits  8..15
 *   cylinder high = LBA bits 16..23
 *   drive/head    = 0xe0 | drive | LBA bits 24..27
 *
 * This gets us beyond the old CHS wall without changing the buffer or
 * interrupt path.  LBA28 tops out at 0x0fffffff sectors, i.e. about
 * 128GiB / 137GB with 512 byte sectors.
 */
#define HD_LBA		0x40
#define HD_LBA28_MAX	0x0fffffff
#define CMD_IDENTIFY	0xec

int hd_use_lba = 1;	/* set to 0 to force original CHS behaviour */


#ifndef	NULL
#define NULL	0
#endif

/* From sys/systm.h */
struct buf *geteblk();

#define paddr(X)	(paddr_t)(X->b_un.b_addr)
#define b_cylin b_resid

struct hh 				hh;
struct isa_dev *hdinfo[NHD];
struct alt_info 			alt_info[NDRIVES];
struct buf rhdbuf[NDRIVES];
struct buf hdbuf[NDRIVES]; 	/* buffer for raw io */
struct buf hdunit[NDRIVES];

struct buf *bp1, *bp2, *bp3;

partition_t	partition_struct[NDRIVES][V_NUMPAR];

typedef struct {

	unsigned short	ncylinders;
	unsigned short	nheads;
	unsigned short	precomp;
	unsigned short	landzone;
	unsigned short	nsecpertrack;
		} hdisk_t;
hdisk_t	hdparams[NDRIVES];	
hdisk_t rlparams[NDRIVES];

/* Total 28-bit LBA sectors reported by ATA IDENTIFY, if available. */
unsigned long hdlbasize[NDRIVES];

int ndrives = 1;

int domapping = 0;	/* handle bad block mapping or not */

unsigned hddebug = 0;
int	reset_requested = 0;

/*
 * IDE controller helpers.
 *
 * The original driver was effectively single-controller: most of the
 * transfer path used absolute 0x1f0/0x3f6 task-file ports, and the ATA
 * drive-select bit was derived from the global Mach unit number.  With
 * two IDE controllers the global units are:
 *
 *   hd0 = primary master      dev_slave 0
 *   hd1 = primary slave       dev_slave 1
 *   hd2 = secondary master    dev_slave 0
 *   hd3 = secondary slave     dev_slave 1
 *
 * So the controller base must come from the attached isa_ctlr, and the
 * ATA drive-select bit must come from dev_slave, not from unit number.
 */
hd_ctlr_base(unit)
int unit;
{
	if (unit >= 0 && unit < NDRIVES && hdinfo[unit] && hdinfo[unit]->dev_mi)
		return (int)hdinfo[unit]->dev_mi->ctlr_addr;
	return (int)PORT_DATA;
}

hdport(unit, port)
int unit, port;
{
	return hd_ctlr_base(unit) + (port - PORT_DATA);
}

hd_slave_bit(unit)
int unit;
{
	if (unit >= 0 && unit < NDRIVES && hdinfo[unit])
		return hdinfo[unit]->dev_slave & 1;
	return unit & 1;
}

hd_same_ctlr(unit1, unit2)
int unit1, unit2;
{
	if (unit1 < 0 || unit1 >= NDRIVES || unit2 < 0 || unit2 >= NDRIVES)
		return 0;
	if (!hdinfo[unit1] || !hdinfo[unit2])
		return 0;
	return hdinfo[unit1]->dev_mi == hdinfo[unit2]->dev_mi;
}

hd_is_ata_disk(unit)
int unit;
{
        int i;
        unsigned char s, sc, sn, cl, ch;

        waitcontroller(unit);

        outb(hdport(unit, PORT_DRIVE_HEADREGISTER),
             FIXEDBITS | (hd_slave_bit(unit) << 4));

        for (i = 0; i < 10000; i++)
                NOP_DELAY;

        waitcontroller(unit);

        /*
         * Check ATAPI signature registers.  A QEMU IDE CD-ROM will usually
         * present 01 01 14 eb after reset/select.
         */
        sc = inb(hdport(unit, PORT_NSECTOR));
        sn = inb(hdport(unit, PORT_SECTOR));
        cl = inb(hdport(unit, PORT_CYLINDERLOWBYTE));
        ch = inb(hdport(unit, PORT_CYLINDERHIBYTE));

        if (sc == ATAPI_SIG_SC && sn == ATAPI_SIG_SN &&
            cl == ATAPI_SIG_CL && ch == ATAPI_SIG_CH) {
                return 0;
        }

        /*
         * Try ATA IDENTIFY.  If this is ATAPI, command 0xec often aborts.
         */
        outb(hdport(unit, PORT_COMMAND), CMD_IDENTIFY);

        for (i = 0; i < PATIENCE; i++) {
                s = inb(hdport(unit, PORT_STATUS));

                if (s & STAT_ERROR) {
                        sc = inb(hdport(unit, PORT_NSECTOR));
                        sn = inb(hdport(unit, PORT_SECTOR));
                        cl = inb(hdport(unit, PORT_CYLINDERLOWBYTE));
                        ch = inb(hdport(unit, PORT_CYLINDERHIBYTE));

                        if (cl == ATAPI_SIG_CL && ch == ATAPI_SIG_CH)
                                return 0;

                        return 0;
                }

                if ((s & STAT_DATAREQUEST) && !(s & STAT_BUSY))
                        return 1;
        }

        return 0;
}

int hdstrategy(), hdminphys(), hdrawio();
int hdprobe(), hdslave(), hdattach();
int hdintr(); int hdintr2();

int (*hdintrs[])() = {hdintr, 0};
#if NHD>2
int (*hdintrs2[])() = {hdintr2, 0};
#endif

struct	isa_driver	hddriver = {
	hdprobe, hdslave, hdattach, "hd", 0, 0, 0};


hdprobe(ctlr)
struct isa_ctlr *ctlr;
{
    int port_status  = (int)ctlr->ctlr_addr + (PORT_STATUS - PORT_DATA);
    int port_ctrl    = (int)ctlr->ctlr_addr + (FIXED_DISK_REG - PORT_DATA);
    int port_drvhead = (int)ctlr->ctlr_addr + (PORT_DRIVE_HEADREGISTER - PORT_DATA);
    u_char stat;
    int i;

    /* Select drive 0 - required before status register responds */
    outb(port_drvhead, 0xa0);   /* drive 0, head 0 */

    /* Short delay after drive select */
    for (i = 0; i < 10000; i++) inb(port_ctrl);

    /* Poll for READY */
    for (i = 0; i < PATIENCE; i++) {
        stat = inb(port_status);
        if (stat == 0xff) return 0;      /* floating bus, no controller */
        if (stat & STAT_BUSY) continue;  /* busy, keep waiting */
        if (stat & STAT_READY) break;
    }
	if ((stat & STAT_READY) == STAT_READY) {
		take_ctlr_irq(ctlr);
		printf("hdc%d: port = %x, spl = %d, pic = %d.\n",
			ctlr->ctlr_ctlr, ctlr->ctlr_addr, ctlr->ctlr_spl, ctlr->ctlr_pic);
		return 1;
	} else
		return 0;

}


/*
 * hdslave:
 *
 *	Actually should be thought of as a slave probe.  Since we are
 *	a driver for an AT, and only one ESDI controller lives in the
 *	normal AT, we assume the controller and check for one or two
 *	hard disks that may be attached.
 *
 */

hdslave(dev)
struct isa_dev *dev;
{
        int unit = dev->dev_unit;

        /*
         * Temporarily record this so hdport()/hd_slave_bit() can work
         * during slave probe, before hdattach().
         */
        hdinfo[unit] = dev;

        if (!hd_is_ata_disk(unit)) {
                printf("hd%d: not an ATA disk, ignored. (controller %d, slave %d)\n",
                        unit, dev->dev_ctlr, dev->dev_slave);
                hdinfo[unit] = 0;
                return 0;
        }

        return 1;
}


/*
 * hdattach:
 *
 *	Attach the drive unit that has been successfully probed.  For the
 *	AT ESDI drives we will initialize all driver specific structures
 *	and complete the controller attach of the drive.
 *
 */

hd_bios_params(unit, dev)
int unit;
struct isa_dev *dev;
{
	unsigned long n;
	unsigned char *tbl;

	n = *(unsigned long *)phystokv(dev->dev_addr);
	tbl = (unsigned char *)(phystokv((n&0xffff) + ((n >> 12)&0xffff0) ));

	hdparams[unit].ncylinders = *tbl++;
	hdparams[unit].ncylinders |= *tbl++ << 8;
	hdparams[unit].nheads = *tbl++ & 0x00ff;
	tbl +=2;
	hdparams[unit].precomp = *tbl++;
	hdparams[unit].precomp |= *tbl++ << 8;
	tbl +=5;
	hdparams[unit].landzone = *tbl++;
	hdparams[unit].landzone |= *tbl++ << 8;
	hdparams[unit].nsecpertrack = *tbl;
	rlparams[unit] = hdparams[unit];
}

hd_fake_params(unit)
int unit;
{
	/*
	 * Last-ditch geometry for an IDE disk that probes but does not answer
	 * IDENTIFY.  Real I/O still uses LBA if hd_use_lba is enabled; this is
	 * mostly for labels, sorting, old ioctls, and boot-time printouts.
	 */
	hdparams[unit].ncylinders = 1024;
	hdparams[unit].nheads = 16;
	hdparams[unit].nsecpertrack = 63;
	hdparams[unit].precomp = 0xffff;
	hdparams[unit].landzone = hdparams[unit].ncylinders;
	rlparams[unit] = hdparams[unit];
}

hd_identify(unit)
int unit;
{
	unsigned short id[256];
	unsigned long nsect;
	unsigned long cyls;
	unsigned int n;
	u_char stat;

	/* Select this drive on this controller. */
	waitcontroller(unit);
	outb(hdport(unit, PORT_DRIVE_HEADREGISTER),
	     FIXEDBITS | (hd_slave_bit(unit) << 4));
	for (n = 0; n < 10000; n++)
		NOP_DELAY

	waitcontroller(unit);
	outb(hdport(unit, PORT_COMMAND), CMD_IDENTIFY);

	for (n = 0; n < PATIENCE; n++) {
		stat = inb(hdport(unit, PORT_STATUS));
		if (stat == 0xff || stat == 0)
			return 0;
		if (stat & STAT_ERROR)
			return 0;
		if ((stat & STAT_DATAREQUEST) && !(stat & STAT_BUSY))
			break;
		NOP_DELAY
	}
	if (n == PATIENCE)
		return 0;

	linw(hdport(unit, PORT_DATA), id, 256);

	hdparams[unit].ncylinders = id[1];
	hdparams[unit].nheads = id[3];
	hdparams[unit].nsecpertrack = id[6];

	/* LBA28 total sectors, ATA IDENTIFY words 60/61. */
	nsect = ((unsigned long)id[61] << 16) | id[60];
	hdlbasize[unit] = nsect;

	/* Some emulated disks report useless CHS.  Keep 16/63 as a safe base. */
	if (hdparams[unit].nheads == 0 || hdparams[unit].nheads > 16)
		hdparams[unit].nheads = 16;
	if (hdparams[unit].nsecpertrack == 0 || hdparams[unit].nsecpertrack > 63)
		hdparams[unit].nsecpertrack = 63;

	if (nsect != 0) {
		cyls = nsect /
		       ((unsigned long)hdparams[unit].nheads *
			(unsigned long)hdparams[unit].nsecpertrack);
		if (cyls == 0)
			cyls = 1;
		if (cyls > 65535)
			cyls = 65535;
		hdparams[unit].ncylinders = cyls;
	}

	hdparams[unit].precomp = 0xffff;
	hdparams[unit].landzone = hdparams[unit].ncylinders;
	rlparams[unit] = hdparams[unit];
	return 1;
}

hdattach(dev)
struct	isa_dev	*dev;
{
	int	unit = dev->dev_unit;
	unsigned long meg;

	hdinfo[unit] = dev;
	hdlbasize[unit] = 0;

	ndrives = (ndrives >= unit + 1) ? ndrives : unit + 1;

	/*
	 * Best path: ask the drive itself.  QEMU does not provide useful BIOS
	 * fixed-disk parameter mappings for secondary IDE disks, so using the
	 * old 0x104/0x118 table for hd2/hd3 makes them look like hd0/hd1.
	 */
	if (!hd_identify(unit)) {
		if (dev->dev_ctlr == 0)
			hd_bios_params(unit, dev);
		else
			hd_fake_params(unit);
	}

	printf("hd%d:  stat = %x, spl = %d, pic = %d. (controller %d, slave %d)\n",
		unit, dev->dev_addr, dev->dev_spl, dev->dev_pic,
		dev->dev_ctlr, dev->dev_slave);

	if (hdlbasize[unit])
		meg = hdlbasize[unit] / 1953;
	else
		meg = ((unsigned long)hdparams[unit].ncylinders *
		       (unsigned long)hdparams[unit].nheads *
		       (unsigned long)hdparams[unit].nsecpertrack) / 1953;

	printf("hd%d:   %dMeg, cyls %d, heads %d, secs %d, precomp %d, landzone %d",
		unit, (int)meg,
		hdparams[unit].ncylinders, hdparams[unit].nheads, hdparams[unit].nsecpertrack,
		hdparams[unit].precomp, hdparams[unit].landzone);
	if (hdlbasize[unit])
		printf(", lba %d", (int)hdlbasize[unit]);
	printf("\n");

	hdunit[unit].b_active = 0;
	hdunit[unit].b_actf = hdunit[unit].b_actl = 0;
	setcontroller(unit);
	return;

}

hdopen(dev, flags)
{
	unsigned char unit, part, n;
	int	errcode = 0;

	unit = UNIT(dev);
	part = PARTITION(dev);

	if (unit >= ndrives || part >= V_NUMPAR || !hdinfo[unit] || !hdinfo[unit]->dev_alive)
		return(ENXIO);

	if (!partition_struct[unit][PART_DISK].p_size) {
		domapping = 0;
		getvtoc(unit);
		domapping = 1;
	}

	if (partition_struct[unit][part].p_flag & V_VALID)
		partition_struct[unit][part].p_flag |= V_OPEN;
	else
		errcode = ENXIO;

open1:
	return(errcode);
}


hdclose(dev)
{
	unsigned char unit, part;
	unsigned int old_priority;

	unit = UNIT(dev);
	part = PARTITION(dev);

	partition_struct[unit][part].p_flag &= ~V_OPEN;
}

#ifdef	MACH_KERNEL
/*
 *	No need to limit IO size to 4096 bytes.
 */
int hdread(dev, ior)
	dev_t	dev;
	io_req_t ior;
{
	return (block_io(hdstrategy, minphys, ior));
}

int hdwrite(dev, ior)
	dev_t	dev;
	io_req_t ior;
{
	return (block_io(hdstrategy, minphys, ior));
}

#else	MACH_KERNEL
hdread(dev,uio)
register short  dev;
struct uio 	*uio;
{
	return(physio(hdstrategy, &hdbuf[UNIT(dev)], dev, B_READ, hdminphys, uio));
}

hdwrite(dev,uio)
dev_t	 	dev;
struct uio	*uio;
{
	return(physio(hdstrategy, &hdbuf[UNIT(dev)], dev, B_WRITE, hdminphys, uio));
}
#endif	MACH_KERNEL

/* Trim buffer length if buffer-size is bigger than page size */
hdminphys(bp)
struct buf	*bp;
{
	if (bp->b_bcount > PAGESIZ)
		bp->b_bcount = PAGESIZ;
}

#ifdef	MACH_KERNEL
#else	MACH_KERNEL
hdioctl(dev, cmd, arg, mode)
dev_t dev;
int cmd;
caddr_t arg;
{
	unsigned char unit, part;
	union io_arg  *arg_kernel; 
	unsigned int i, snum, old_priority;
	struct absio *absio_kernel;
	union vfy_io *vfy_io_kernel;
	int xcount, errcode = 0;
	int intlv;

	unit = UNIT(dev);
	part = PARTITION(dev);

	switch (cmd) {
	case V_CONFIG:
		arg_kernel = (union io_arg *)arg;
		if (arg_kernel->ia_cd.secsiz != SECSIZE) {
			/* changing sector size NOT allowed */
			/* u.u_error = EINVAL; */ 
		  	errcode = EINVAL;
			break;
		}
		hdparams[unit].ncylinders=(unsigned short)arg_kernel->ia_cd.ncyl;
		hdparams[unit].nheads = (unsigned short)arg_kernel->ia_cd.nhead;
		hdparams[unit].nsecpertrack = (unsigned short)arg_kernel->ia_cd.nsec;
		setcontroller(unit);
		break;

	case V_REMOUNT:
		getvtoc(unit);	
		break;

	case V_ADDBAD:

		/* this adds a bad block to IN CORE alts table ONLY */
		arg_kernel = (union io_arg *)arg;
		alt_info[unit].alt_sec.alt_used++;
		alt_info[unit].alt_sec.alt_bad[
		alt_info[unit].alt_sec.alt_used]=  arg_kernel->ia_abs.bad_sector;
		break;

	case V_GETPARMS:
		{
		struct disk_parms *disk_parms = (struct disk_parms *)arg;
		
		disk_parms->dp_type = DPT_WINI;
		disk_parms->dp_heads = hdparams[unit].nheads;
		disk_parms->dp_cyls = hdparams[unit].ncylinders;
		disk_parms->dp_sectors  = hdparams[unit].nsecpertrack;
  		disk_parms->dp_dosheads = rlparams[unit].nheads;
		disk_parms->dp_doscyls = rlparams[unit].ncylinders;
		disk_parms->dp_dossectors  = rlparams[unit].nsecpertrack;
		disk_parms->dp_secsiz = SECSIZE;
		disk_parms->dp_ptag = 0;
		disk_parms->dp_pflag =partition_struct[unit][part].p_flag;
		disk_parms->dp_pstartsec = partition_struct[unit][part].p_start;
		disk_parms->dp_pnumsec =partition_struct[unit][part].p_size;
		break;
		}

	case V_FORMAT:
		arg_kernel = (union io_arg *)arg;
		hh.format_request = 1;		
		hh.controller_busy = 1;
		for (i =0; i < SECSIZE; i++) 
			hh.interleave_tab[i] = 0; /* 0 means not taken */

		/* start formatting at sector one, not sector zero */
		intlv = arg_kernel->ia_fmt.intlv;
		i = 0;
		snum = 1;
		hh.interleave_tab[i] = snum++;	
		do {
			i = (i + intlv) % hdparams[unit].nsecpertrack;
			while (hh.interleave_tab[i] != 0) { /* taken */
				i = (i + 1) % 
					(hdparams[unit].nsecpertrack );	
				if (snum > hdparams[unit].nsecpertrack)
					goto fmt1;
			}
			hh.interleave_tab[i] = snum++;	
		
		} while (snum <= hdparams[unit].nsecpertrack);
fmt1:
		/* format all tracks in request */

		hh.format_track = arg_kernel->ia_fmt.start_trk;
		hh.curdrive = unit;
		for (i=0; i <arg_kernel->ia_fmt.num_trks; i++,hh.format_track++ ) {
			hh.retry_count = 0;
			badblock_mapping();
			format_command();
			sleep(&hh.interleave_tab[0], PRIBIO);
		}
		hh.format_request = 0;	
		hh.controller_busy = 0;	
		break;

	case V_PDLOC:
		{
		unsigned int *pd_loc;
		
		pd_loc = (unsigned int *)arg;
		*pd_loc = (unsigned int)(hh.start_of_unix[unit]) + PDLOCATION; 
		break;
		}
	case V_RDABS:
		/* V_RDABS is relative to head 0, sector 0, cylinder 0 */
		bp1 = geteblk(SECSIZE);
		absio_kernel = (struct absio *)arg;
		bp1->b_flags = B_READ | B_MD1;	/* MD1 is be absolute */
		bp1->b_blkno = absio_kernel->abs_sec;
		bp1->b_dev = WHOLE_DISK(unit);	/* 4 lsb's = 0 = partit 0 = RDABS */	
		bp1->b_bcount = SECSIZE;
		/* no badblock_mapping(), no offset by partition_p->p_start */
		hdstrat2(bp1);
		biowait(bp1);
		bp1->b_flags &= ~B_MD1;
		if (bp1->b_flags & B_ERROR) {
			printf("hd: read failure on ioctl\n");
			errcode = ENXIO;
			brelse(bp1);
			break;
		}
		if ( copyout( (caddr_t)paddr(bp1), absio_kernel->abs_buf, 
				SECSIZE) !=0 ) {
			errcode = ENXIO;
		}
		brelse(bp1);
		break;

	case V_WRABS:

		/* V_WRABS is relative to head 0, sector 0, cylinder 0 */
		bp1 = geteblk(SECSIZE);
		absio_kernel = (struct absio *)arg;
		if ( copyin( absio_kernel->abs_buf, (caddr_t)paddr(bp1), 
				SECSIZE) !=0 ) {
			/* u.u_error =ENXIO; */
			errcode = ENXIO;
			brelse(bp1);
			break;
		}
		bp1->b_flags = B_WRITE | B_MD1;	/* MD1 is be absolute */
		bp1->b_blkno = absio_kernel->abs_sec;
		bp1->b_dev = WHOLE_DISK(unit);	/* 4 lsb's = 0 = partit 0 = RDABS */	
		bp1->b_bcount = SECSIZE;
		/* no badblock_mapping(), no offset by partition_p->p_start */
		hdstrat2(bp1);
		biowait(bp1);
		bp1->b_flags &= ~B_MD1;
		if (bp1->b_flags & B_ERROR) {
			printf("hd: write failure on ioctl\n");
			/* u.u_error =ENXIO; */
			errcode = ENXIO;
		}
		brelse(bp1);
		break;

	case V_VERIFY:

		if (u.u_uid != 0) {
			errcode = ENXIO;
			break;
		}

		bp1 = geteblk(PAGESIZ);
		vfy_io_kernel = (union vfy_io *)arg;
		bp1->b_flags = B_READ;
		bp1->b_blkno = vfy_io_kernel->vfy_in.abs_sec;
		bp1->b_dev = WHOLE_DISK(unit);	/* 4 lsb's = 0 = partit 0 = RDABS */	
		xcount = vfy_io_kernel->vfy_in.num_sec;
		vfy_io_kernel->vfy_out.err_code = 0;
		snum = PAGESIZ >> 9;
		while (xcount > 0) {
			i = (xcount > snum) ? snum : xcount;
			bp1->b_bcount = i << 9;
			bp1->b_flags |= B_MD1;
			hdstrat2(bp1);
			biowait(bp1);
			bp1->b_flags &= ~B_MD1;
			if (bp1->b_flags & B_ERROR) {
				vfy_io_kernel->vfy_out.err_code = BAD_BLK;
				break;
			}
			xcount -= i;
			bp1->b_blkno += i;
			bp1->b_flags &= ~B_DONE;
		}
		brelse(bp1);
		break;

	default:
		printf("hdioctl(): do not recognize ioctl of 0x%x \n", cmd);
		/* u.u_error = EINVAL; */
		errcode = EINVAL;
	}
	return(errcode);
}
#endif	MACH_KERNEL

hdstrategy(bp)
struct	buf	*bp;
{
	/* this will allow bufs to drain so that a close open or 
 	   ioctl can procede without interference from users of hdstrategy() 
	*/
	hdstrat2(bp);
}

hdstrat2(bp)
struct	buf	*bp;
{
	struct	buf	*dp;
	partition_t	*partition_p;
	unsigned char unit, partition;
	unsigned int opri;

	if (bp->b_bcount == 0) {
		goto done;
	}

	unit = UNIT((bp->b_dev));
	partition = PARTITION((bp->b_dev));

	partition_p= &(partition_struct[unit][partition]);

	if ( !(partition_p->p_flag & V_VALID))
	{
		bp->b_error = ENXIO;
		goto bad;
	}

	/* if request is off the end or trying to write last block on out */

	if (bp->b_flags & B_MD1) {
		if (bp->b_blkno > partition_struct[unit][PART_DISK].p_start +
				  partition_struct[unit][PART_DISK].p_size - 1) {
			bp->b_error = ENXIO;
			goto bad;
		}
	} else {
		if ( (bp->b_blkno >  partition_p->p_size) ||
		     (bp->b_blkno == partition_p->p_size && !(bp->b_flags & B_READ))) {
			bp->b_error = ENXIO;
			goto bad;
		}
		if (bp->b_blkno == partition_p->p_size) {
		/* indicate (read) EOF by setting b_resid to b_bcount on last block */ 
			bp->b_resid = bp->b_bcount;
			goto done;
		}
	}

	bp->b_cylin = ( ((bp->b_flags & B_MD1) ? 0 : partition_p->p_start) + bp->b_blkno) /
		      (hdparams[hh.curdrive].nsecpertrack * hdparams[hh.curdrive].nheads);

	opri = spl5();
	dp = &hdunit[unit];

	disksort(dp, bp);

	if (!hh.controller_busy)
		hdstart();

	splx(opri);
	return;
bad:
	bp->b_flags |= B_ERROR;
done:
	iodone(bp);
	return;
}

/* hdstart() is called at spl5 */
hdstart()
{
	partition_t	*partition_p;
	int drivecount;
	register struct buf *bp, *dp;

	if (hh.controller_busy) {
		return;
	}

	if(reset_requested) {
		reset_requested = 0;
		reset_controller();
	}
	
	for (drivecount = 0; drivecount < NDRIVES; drivecount++) {
		if ( hh.curdrive < (NDRIVES-1) )
			hh.curdrive++;
		else
			hh.curdrive = 0;

		dp = &hdunit[hh.curdrive];
		if ((bp = dp->b_actf) != NULL)
			break;
	}
	if (drivecount == NDRIVES) {
		return;
	}

	hh.controller_busy = 1;
	hh.blocktotal = (bp->b_bcount + 511) >> 9;

	partition_p = &partition_struct[UNIT(bp->b_dev)][PARTITION(bp->b_dev)];
	
	/* see V_RDABS and V_WRABS in hdioctl() */
	if (bp->b_flags & B_MD1) {
		int end = partition_struct[hh.curdrive][PART_DISK].p_start +
				  partition_struct[hh.curdrive][PART_DISK].p_size - 1;

		hh.physblock = bp->b_blkno;
		if ((bp->b_blkno + hh.blocktotal ) > end)
			hh.blocktotal = end - bp->b_blkno + 1;
	} else {
 		hh.physblock = partition_p->p_start + bp->b_blkno;
		if ((bp->b_blkno + hh.blocktotal ) > partition_p->p_size)
			hh.blocktotal = partition_p->p_size - bp->b_blkno + 1;
	}

	hh.blockcount = 0;
	hh.rw_addr = (paddr_t) bp->b_un.b_addr;
	hh.retry_count = 0;

if (hddebug & 1) printf("[");

	start_rw(bp->b_flags & B_READ); 
}


int hd_print_error = 0;
hdintr()
{
	register struct buf *bp, *dp;

	if (!hh.controller_busy) {
		if (hd_print_error)
			printf("hd: false interrupt\n");
		return;
	}
	waitcontroller(hh.curdrive);
	hh.status = inb(hdport(hh.curdrive, PORT_STATUS));	

	dp = &hdunit[hh.curdrive];
	bp = dp->b_actf;

	if (hh.restore_request == 1) { /* Restore command has completed */
		hh.restore_request = 0;
		if (hh.status & STAT_ERROR)
			hderror(bp);
		else {
			if (bp != NULL)	
				start_rw(bp->b_flags & B_READ);
			else {
				if (hh.format_request)	
					format_command(); 
			}
		}
		return;
	}

	if (hh.status & STAT_WRITEFAULT) {
		panic("hd: write fault\n");
	}

	if (hh.status & STAT_ERROR) {
		if (hd_print_error) {
			 printf("hdintr: state error %x, error = %x\n",
			 	hh.status, inb(hdport(hh.curdrive, PORT_ERROR)));
			 printf("hdintr: state error. block %d, count %d, total %d\n",
			 	hh.physblock, hh.blockcount, hh.blocktotal);
			 printf("hdintr: state error. cyl %d, head %d, sector %d\n",
			 	hh.cylinder, hh.head, hh.sector);

		}
		hderror(bp);
		return;
	}

	if (hh.format_request) {
		printf("hdintr: format request\n");
		wakeup(&hh.interleave_tab[0]);
		return;
	}

	if (hh.status & STAT_ECC) 
		printf("hd: ECC soft error fixed, unit %d, partition %d, physical block %d \n",
			hh.curdrive, PARTITION(bp->b_dev), hh.physblock);

	if (bp == NULL ) {
		/* there should be a read/write buffer queued at this point */
		printf("hdintr: no bp buffer to read or write\n");
		return;	
	}

	if (bp->b_flags & B_READ) {
if (hddebug & 4) printf("hd.hdintr(): reading a sector into 0x%x\n", hh.rw_addr);
		while ((inb(hdport(hh.curdrive, PORT_STATUS)) & STAT_DATAREQUEST) == 0) {
NOP_DELAY
		}
		linw(hdport(hh.curdrive, PORT_DATA), hh.rw_addr, SECSIZE/2); 
	}

	if ( ++hh.blockcount == hh.blocktotal ) {
		dp->b_actf = bp->av_forw;
		bp->b_resid = 0;
		iodone(bp);
		hh.controller_busy = 0;

if (hddebug & 1) printf("]");

		hdstart();
	} else {
		hh.rw_addr += SECSIZE;
		hh.physblock++;
		if (hh.single_mode)
			start_rw(bp->b_flags & B_READ);
		else if (!(bp->b_flags & B_READ)) {
			/* Load sector into controller for next write */
			waitcontroller(hh.curdrive);
			while ((inb(hdport(hh.curdrive, PORT_STATUS)) & STAT_DATAREQUEST) == 0 ) {
NOP_DELAY
			}
			loutw(hdport(hh.curdrive, PORT_DATA), hh.rw_addr, SECSIZE/2);
		}
	}
}

/* same as above */
hdintr2()
{
	register struct buf *bp, *dp;

	if (!hh.controller_busy) {
		if (hd_print_error)
			printf("hd: false interrupt\n");
		return;
	}
	waitcontroller(hh.curdrive);
	hh.status = inb(hdport(hh.curdrive, PORT_STATUS));	

	dp = &hdunit[hh.curdrive];
	bp = dp->b_actf;

	if (hh.restore_request == 1) { /* Restore command has completed */
		hh.restore_request = 0;
		if (hh.status & STAT_ERROR)
			hderror(bp);
		else {
			if (bp != NULL)	
				start_rw(bp->b_flags & B_READ);
			else {
				if (hh.format_request)	
					format_command(); 
			}
		}
		return;
	}

	if (hh.status & STAT_WRITEFAULT) {
		panic("hd: write fault\n");
	}

	if (hh.status & STAT_ERROR) {
		if (hd_print_error) {
			 printf("hdintr: state error %x, error = %x\n",
			 	hh.status, inb(hdport(hh.curdrive, PORT_ERROR)));
			 printf("hdintr: state error. block %d, count %d, total %d\n",
			 	hh.physblock, hh.blockcount, hh.blocktotal);
			 printf("hdintr: state error. cyl %d, head %d, sector %d\n",
			 	hh.cylinder, hh.head, hh.sector);

		}
		hderror(bp);
		return;
	}

	if (hh.format_request) {
		printf("hdintr: format request\n");
		wakeup(&hh.interleave_tab[0]);
		return;
	}

	if (hh.status & STAT_ECC) 
		printf("hd: ECC soft error fixed, unit %d, partition %d, physical block %d \n",
			hh.curdrive, PARTITION(bp->b_dev), hh.physblock);

	if (bp == NULL ) {
		/* there should be a read/write buffer queued at this point */
		printf("hdintr: no bp buffer to read or write\n");
		return;	
	}

	if (bp->b_flags & B_READ) {
if (hddebug & 4) printf("hd.hdintr(): reading a sector into 0x%x\n", hh.rw_addr);
		while ((inb(hdport(hh.curdrive, PORT_STATUS)) & STAT_DATAREQUEST) == 0) {
NOP_DELAY
		}
		linw(hdport(hh.curdrive, PORT_DATA), hh.rw_addr, SECSIZE/2); 
	}

	if ( ++hh.blockcount == hh.blocktotal ) {
		dp->b_actf = bp->av_forw;
		bp->b_resid = 0;
		iodone(bp);
		hh.controller_busy = 0;

if (hddebug & 1) printf("]");

		hdstart();
	} else {
		hh.rw_addr += SECSIZE;
		hh.physblock++;
		if (hh.single_mode)
			start_rw(bp->b_flags & B_READ);
		else if (!(bp->b_flags & B_READ)) {
			/* Load sector into controller for next write */
			waitcontroller(hh.curdrive);
			while ((inb(hdport(hh.curdrive, PORT_STATUS)) & STAT_DATAREQUEST) == 0 ) {
NOP_DELAY
			}
			loutw(hdport(hh.curdrive, PORT_DATA), hh.rw_addr, SECSIZE/2);
		}
	}
}

hderror(bp)
struct buf *bp;
{

	if(++hh.retry_count > 3) {
		if(bp) {
			/************************************************
			* We have a problem with this block, set the	*
			* error flag, terminate the operation and move	*
			* on to the next request.			*
			* With every hard disk transaction error we set	*
			* the reset requested flag so that the contrlr	*
			* is reset before next operation is started.	*
			* A reset is a relatively long operation, the	*
			* upper level routines are better qualified for	*
			* such an operation than the interrupt service	*
			* routines.					*
			************************************************/

			hdunit[hh.curdrive].b_actf = bp->av_forw;
			bp->b_flags |= B_ERROR;
			bp->b_resid = 0;

			biodone(bp);
			hh.controller_busy = 0;
			reset_requested = 1;
			hdstart();
		}
		else {
			/* give up, its way beyond hope */
			if (hh.format_request) {
				printf("hd: can't format track number %d\n", hh.format_track);
				wakeup (&hh.interleave_tab[0]);
			}
		}
	}
	else {
		/* lets do a recalibration */
		waitcontroller(hh.curdrive);
		hh.restore_request = 1;
		outb(hdport(hh.curdrive, PORT_PRECOMP), hdparams[hh.curdrive].precomp >>2);
		outb(hdport(hh.curdrive, PORT_NSECTOR), hdparams[hh.curdrive].nsecpertrack);
		outb(hdport(hh.curdrive, PORT_SECTOR), hh.sector);
		outb(hdport(hh.curdrive, PORT_CYLINDERLOWBYTE), hh.cylinder & 0xff );
		outb(hdport(hh.curdrive, PORT_CYLINDERHIBYTE),  (hh.cylinder >> 8) & 0xff );
		outb(hdport(hh.curdrive, PORT_DRIVE_HEADREGISTER),
		     FIXEDBITS | (hd_slave_bit(hh.curdrive) << 4));
		outb(hdport(hh.curdrive, PORT_COMMAND), CMD_RESTORE);
	}
}

getvtoc(unit)
unsigned char	unit;
{
	unsigned char *c_p;
	unsigned int n, m;
	char *pt1, *pt2;
	struct ipart *ipart_p;
	struct evtoc *evp;
	unsigned long unix_size;

	bp1= geteblk(SECSIZE);		/* for evtoc */
	bp2 = geteblk(SECSIZE);		/* for alt_info */

	/* make partition 0 the whole disk in case of failure
  	   then get pdinfo 
	*/
	partition_struct[unit][PART_DISK].p_flag = V_OPEN|V_VALID;
	partition_struct[unit][PART_DISK].p_start = 0; 
	if (hdlbasize[unit])
		partition_struct[unit][PART_DISK].p_size = hdlbasize[unit];
	else
		partition_struct[unit][PART_DISK].p_size = hdparams[unit].ncylinders *
		   hdparams[unit].nheads * hdparams[unit].nsecpertrack;

	/* get active partition */
	bp1->b_flags = B_READ | B_MD1;
	bp1->b_blkno = 0;
	bp1->b_dev = WHOLE_DISK(unit);		/* partition is bottom 4 bits */
	bp1->b_bcount = SECSIZE;
	hdstrat2(bp1);
	biowait(bp1);
	bp1->b_flags &= ~B_MD1;
	if (bp1->b_flags & B_ERROR) {
		printf("hd: can not read sector 0 on drive %d, the boot record partition (see AT Tech. Ref.)\n", unit);
		return;
	}
	c_p = (unsigned char *)(paddr(bp1)) + 511;
	if ( *c_p != BOOTRECORDSIGNATURE) {
		printf("hd: sector 0 signature bad, can not read active partition on drive %d\n", unit);
		return;
	}
	c_p = (unsigned char *)(paddr(bp1)) + 446;
	ipart_p = (struct ipart *)(c_p);
	for (n=0; n<FD_NUMPART; n++, ipart_p++)
		if (ipart_p->bootid == ACTIVE) break;

	if (ipart_p->bootid != ACTIVE) {
		printf("hd: no active partition on drive %d\n", unit);
		return;
	}
	hh.start_of_unix[unit] = ipart_p->relsect;
	unix_size = ipart_p->numsect;
	
	/* set correct partition information */

	partition_struct[unit][PART_DISK].p_start = hh.start_of_unix[unit];
	if (unix_size)
		partition_struct[unit][PART_DISK].p_size = unix_size;
	else if (hdlbasize[unit])
		partition_struct[unit][PART_DISK].p_size = hdlbasize[unit] -
		   hh.start_of_unix[unit];
	else if (hdlbasize[unit])
		partition_struct[unit][PART_DISK].p_size = hdlbasize[unit] -
		   hh.start_of_unix[unit];
	else
		partition_struct[unit][PART_DISK].p_size = hdparams[unit].ncylinders *
		   hdparams[unit].nheads * hdparams[unit].nsecpertrack - 
		   hh.start_of_unix[unit];

	/* get evtoc out of active unix partition */
	bp1->b_flags = B_READ;
	bp1->b_blkno = PDLOCATION;
	bp1->b_dev = WHOLE_DISK(unit);		/* partition is bottom 4 bits */
	bp1->b_bcount = SECSIZE;
	hdstrat2(bp1);
	biowait(bp1);
	if (bp1->b_flags & B_ERROR) {
		printf("hd: can not read evtoc on drive %d\n", unit);
		return;
	}
	evp = (struct evtoc *)bp1->b_un.b_addr;

	if (evp->sanity != VTOC_SANE) {
		printf("hd: evtoc invalid on drive %d\n",unit);
		return;
	}

	/* pd info from disk must be more accurate than that in cmos
	   thus override hdparams and re- setcontroller()
	*/			
	hdparams[unit].ncylinders = evp->cyls;
	hdparams[unit].nheads = evp->tracks;
	hdparams[unit].nsecpertrack = evp->sectors;
#ifdef	OOPS
	printf("cyl = %d, heads = %d, sectors = %d\n",
		evp->cyls, evp->tracks, evp->sectors);
#else	OOPS
	if (unix_size)
		partition_struct[unit][PART_DISK].p_size = unix_size;
	else
		partition_struct[unit][PART_DISK].p_size = hdparams[unit].ncylinders *
		   hdparams[unit].nheads * hdparams[unit].nsecpertrack - 
		   hh.start_of_unix[unit];
	setcontroller(unit);
#endif	OOPS
		
	/* copy info on all valid partition, zero the others */
	for (n = 0; n < evp->nparts; n++) {
		if (n == PART_DISK)		/* "c" is sacred */
			continue;
		/* this is a STRUCTURE copy */
		partition_struct[unit][n] = evp->part[n];
		if (evp->part[n].p_start >= 0 && evp->part[n].p_size >= 0)
			partition_struct[unit][n].p_flag = V_VALID;
		else
			partition_struct[unit][n].p_flag = 0;
	}
	for ( ; n < V_NUMPAR; n++) {
		if (n == PART_DISK)
			continue;
		partition_struct[unit][n].p_flag = 0;
		partition_struct[unit][n].p_size = 0;
	}	
	/* leave partition 0 "open" for raw I/O */
	partition_struct[unit][PART_DISK].p_flag |= V_OPEN;

	/* get alternate sectors out of active unix partition */
	pt1 = (char *)&alt_info[unit];
	for (n = 0; n < 4; n++) {
		bp2->b_flags = B_READ;
		bp2->b_blkno = evp->alt_ptr/SECSIZE + n;
		bp2->b_dev = WHOLE_DISK(unit);	/* partition is bottom 4 bits */
		bp2->b_bcount = SECSIZE;
		hdstrat2(bp2);
		biowait(bp2);
		if (bp2->b_flags & B_ERROR) {
			printf("hd: can not read alternate sectors on drive %d\n", unit);
			return;
		}
		/* since struct altinfo is split over 4 bufs, can't do 
		   struct copy */
		pt2 = (char *)bp2->b_un.b_addr;
		for (m = 0; m < SECSIZE; m++, pt1++, pt2++)
			*pt1 = *pt2; 
	}

	if (alt_info[unit].alt_sanity != ALT_SANITY) {
		printf("hd: alternate sectors corrupted on drive %d\n", unit);
		return;
	}

	if (bp1 != NULL)
		brelse(bp1);
	if (bp2 != NULL)
		brelse(bp2);
}

format_command()
{
	unsigned int track;
		
	if ( hdparams[hh.curdrive].nheads > 8)
		outb(hdport(hh.curdrive, FIXED_DISK_REG), MORETHAN8HEADS);
	else
		outb(hdport(hh.curdrive, FIXED_DISK_REG), EIGHTHEADSORLESS);
	
	if (hh.block_is_bad)
		track = hh.substitutetrack;
	else
		track = hh.format_track;
	hh.head     = track   % hdparams[hh.curdrive].nheads; 
	hh.head = hh.head | (hd_slave_bit(hh.curdrive) << 4) | FIXEDBITS;
	hh.cylinder = track   / hdparams[hh.curdrive].nheads;

	waitcontroller(hh.curdrive);
	outb(hdport(hh.curdrive, PORT_PRECOMP), hdparams[hh.curdrive].precomp >>2);
	outb(hdport(hh.curdrive, PORT_NSECTOR), hdparams[hh.curdrive].nsecpertrack);
	/* Western Digital 1010 and 1005 want the following line */
	outb(hdport(hh.curdrive, PORT_SECTOR), 36);
	outb(hdport(hh.curdrive, PORT_CYLINDERLOWBYTE), hh.cylinder & 0xff );
	outb(hdport(hh.curdrive, PORT_CYLINDERHIBYTE),  (hh.cylinder >> 8) & 0xff );
	outb(hdport(hh.curdrive, PORT_DRIVE_HEADREGISTER), hh.head);
	outb(hdport(hh.curdrive, PORT_COMMAND), CMD_FORMAT);
	waitcontroller(hh.curdrive);
	loutw(hdport(hh.curdrive, PORT_DATA), hh.interleave_tab, SECSIZE/2);
}

reset_controller()
{
	int	i, unit;

	unit = hh.curdrive;
	outb(hdport(unit, FIXED_DISK_REG), 4);
	for(i = 0; i < 10000; i++);
	outb(hdport(unit, FIXED_DISK_REG), 0);
	waitcontroller(unit);
	if(1 != (i = inb(hdport(unit, PORT_ERROR))))
		printf("reset_controller(): unit %d error code %d\n", unit, i);

	/* Re-issue SET PARAMETERS for drives on the reset controller only. */
	for (i = 0; i < ndrives; i++)
		if (hdinfo[i] && hdinfo[i]->dev_alive && hd_same_ctlr(i, unit))
			setcontroller(i);
}

setcontroller(unit)
{
	unsigned char nheads;

	waitcontroller(unit);
	nheads = FIXEDBITS | (hdparams[unit].nheads -1);
	nheads |= (hd_slave_bit(unit) << 4);
	outb(hdport(unit, PORT_DRIVE_HEADREGISTER), nheads); 
	outb(hdport(unit, PORT_NSECTOR), hdparams[unit].nsecpertrack);
	outb(hdport(unit, PORT_COMMAND), CMD_SETPARAMETERS);
	waitcontroller(unit);
}

waitcontroller(unit)
int unit;
{
	unsigned int n;

	for (n = 0; n < PATIENCE; n++) {
		if ((inb(hdport(unit, PORT_STATUS)) & STAT_BUSY) == 0)
			return;
NOP_DELAY
	}
	printf("hard disk drive: waitcontroller() times out, unit %d, port %x\n",
		unit, hd_ctlr_base(unit));
	panic("hard disk drive: waitcontroller() times out");
}

hd_lba_rw(read, disk_block, xblk)
int read;
unsigned long disk_block;
unsigned int xblk;
{
	waitcontroller(hh.curdrive);

	/*
	 * Keep these filled in for the existing error/debug printouts.
	 * In LBA mode they are not CHS values; they are the task-file
	 * bytes that make up the 28-bit LBA.
	 */
	hh.sector = disk_block & 0xff;
	hh.cylinder = (disk_block >> 8) & 0xffff;
	hh.head = FIXEDBITS | HD_LBA | (hd_slave_bit(hh.curdrive) << 4) |
		  ((disk_block >> 24) & 0x0f);

	outb(hdport(hh.curdrive, PORT_PRECOMP), 0);
	outb(hdport(hh.curdrive, PORT_NSECTOR), xblk);
	outb(hdport(hh.curdrive, PORT_SECTOR), hh.sector);
	outb(hdport(hh.curdrive, PORT_CYLINDERLOWBYTE), hh.cylinder & 0xff);
	outb(hdport(hh.curdrive, PORT_CYLINDERHIBYTE), (hh.cylinder >> 8) & 0xff);
	outb(hdport(hh.curdrive, PORT_DRIVE_HEADREGISTER), hh.head);

	if (read)
		outb(hdport(hh.curdrive, PORT_COMMAND), CMD_READ);
	else {
		outb(hdport(hh.curdrive, PORT_COMMAND), CMD_WRITE);
		waitcontroller(hh.curdrive);
		while ((inb(hdport(hh.curdrive, PORT_STATUS)) & STAT_DATAREQUEST) == 0) {
NOP_DELAY
		}
		loutw(hdport(hh.curdrive, PORT_DATA), hh.rw_addr, SECSIZE/2);
	}
}

start_rw(read)
{
	unsigned long track, disk_block;
	unsigned int xblk;
	struct buf *bp = hdunit[hh.curdrive].b_actf;

	if ( hdparams[hh.curdrive].nheads > 8)
		outb(hdport(hh.curdrive, FIXED_DISK_REG), MORETHAN8HEADS);
	else
		outb(hdport(hh.curdrive, FIXED_DISK_REG), EIGHTHEADSORLESS);

	disk_block = hh.physblock;

	xblk = hh.blocktotal - hh.blockcount;	/* # of blks to transfer */

	if ( (bp->b_flags & B_MD1) == 0 ) {
		xfermode();	/* determine the transfer mode */
		if(hh.single_mode) {
			xblk = 1;
			badblock_mapping();
			if(hh.block_is_bad)
				disk_block = hh.substituteblock;
		}
	}

	if (hd_use_lba &&
	    disk_block <= HD_LBA28_MAX &&
	    (disk_block + xblk - 1) <= HD_LBA28_MAX) {
		hd_lba_rw(read, disk_block, xblk);
		return;
	}

	/* disk is formatted starting sector 1, not sector 0 */
	hh.sector = (disk_block % hdparams[hh.curdrive].nsecpertrack) + 1;

	track = disk_block / hdparams[hh.curdrive].nsecpertrack;

	hh.head = track % hdparams[hh.curdrive].nheads; 
	hh.head = hh.head | (hd_slave_bit(hh.curdrive) << 4) | FIXEDBITS;
	hh.cylinder = track / hdparams[hh.curdrive].nheads;

	waitcontroller(hh.curdrive);
	outb(hdport(hh.curdrive, PORT_PRECOMP), hdparams[hh.curdrive].precomp >>2);
	outb(hdport(hh.curdrive, PORT_NSECTOR), xblk);
	outb(hdport(hh.curdrive, PORT_SECTOR), hh.sector);
	outb(hdport(hh.curdrive, PORT_CYLINDERLOWBYTE), hh.cylinder & 0xff );
	outb(hdport(hh.curdrive, PORT_CYLINDERHIBYTE),  (hh.cylinder >> 8) & 0xff );
	outb(hdport(hh.curdrive, PORT_DRIVE_HEADREGISTER), hh.head );
	if(read) 
		outb(hdport(hh.curdrive, PORT_COMMAND), CMD_READ);
 	else {
 		outb(hdport(hh.curdrive, PORT_COMMAND), CMD_WRITE); 
		waitcontroller(hh.curdrive);
		while ((inb(hdport(hh.curdrive, PORT_STATUS)) & STAT_DATAREQUEST) == 0) {
NOP_DELAY
		}
		loutw(hdport(hh.curdrive, PORT_DATA), hh.rw_addr, SECSIZE/2);
	}

}

badblock_mapping()
{
	unsigned short n;
	unsigned int track;
	struct buf *bp = hdunit[hh.curdrive].b_actf;

	hh.block_is_bad = 0;

	if (!domapping)
		return;

	/* partition 0 is whole disk, bad blocks and all */
	if ( PARTITION(bp->b_dev) == PART_DISK)
		return;

	/* to support V_RDABS and V_WRABS in hdioctl() */
	if (bp->b_flags & B_MD1)
		return;

	/* BAD TRACK MAPPING */
	track = hh.physblock / hdparams[hh.curdrive].nsecpertrack;	
	
	for (n = 0; n < alt_info[hh.curdrive].alt_trk.alt_used; n++) {
		if (track == alt_info[hh.curdrive].alt_trk.alt_bad[n]) {
			hh.block_is_bad = 1;
			hh.substituteblock =  
				alt_info[hh.curdrive].alt_trk.alt_base + 
				hdparams[hh.curdrive].nsecpertrack * n +
				(hh.physblock % hdparams[hh.curdrive].nsecpertrack);	
			hh.substitutetrack = track;
			return;
		}	
	} 


	/* BAD BLOCK MAPPING */
	/* add do while if substituteblock is bad !!!!!!!!!!  */

	for (n = 0; n < alt_info[hh.curdrive].alt_sec.alt_used; n++) {
		if (hh.physblock == alt_info[hh.curdrive].alt_sec.alt_bad[n]) {
			hh.block_is_bad = 1;
			hh.substituteblock = 
				alt_info[hh.curdrive].alt_sec.alt_base + n;
			break;
		}	
	} 
}

dynamic_badblock()
{
	printf("dynamic_badblock()	--not implemented yet\n");
}

hdsize()
{
	printf("hdsize()	-- not implemented\n");
}

hddump()
{
	printf("hddump()	-- not implemented\n");
}

/*
 * get the disk type from CMOS ram
 */
unsigned char
disktype()
{
	outb(CMOS_ADDR, HDTBL);
	return inb(CMOS_DATA);
}

/*
 *  determine single block or multiple blocks transfer mode
 */
xfermode()
{
	int n, bblk, eblk, btrk, etrk;
	struct buf *bp = hdunit[hh.curdrive].b_actf;

	hh.single_mode = 0;	/* default is multiple mode */

	if (!domapping)		/* if mapping is not on */
		return;

	bblk = hh.physblock;
	eblk = bblk + hh.blocktotal - 1;

	btrk = bblk / hdparams[hh.curdrive].nsecpertrack;	
	etrk = eblk / hdparams[hh.curdrive].nsecpertrack;	
	
	for (n = 0; n < alt_info[hh.curdrive].alt_trk.alt_used; n++) {
		if ((btrk <= alt_info[hh.curdrive].alt_trk.alt_bad[n]) &&
		     (etrk >= alt_info[hh.curdrive].alt_trk.alt_bad[n])) {
			hh.single_mode = 1;
			return;
		}	
	} 


	for (n = 0; n < alt_info[hh.curdrive].alt_sec.alt_used; n++) {
		if ((bblk <= alt_info[hh.curdrive].alt_sec.alt_bad[n]) &&
		    (eblk >= alt_info[hh.curdrive].alt_sec.alt_bad[n])) {
			hh.single_mode = 1;
			break;
		}	
	} 
}

