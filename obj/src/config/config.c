
# line 32 "../../../src/config/config.y"
typedef union  {
	char	*str;
	int	val;
	struct	file_list *file;
	struct	idlst *lst;
} YYSTYPE;
# define ADDRMOD 257
# define AND 258
# define ANY 259
# define ARGS 260
# define AT 261
# define BIN 262
# define COMMA 263
# define CONFIG 264
# define CONTROLLER 265
# define CPU 266
# define CSR 267
# define DEVICE 268
# define DISK 269
# define DRIVE 270
# define DST 271
# define DUMPS 272
# define EQUALS 273
# define FLAGS 274
# define HZ 275
# define IDENT 276
# define INIT 277
# define MACHINE 278
# define MAJOR 279
# define MASTER 280
# define MAXUSERS 281
# define MAXDSIZ 282
# define MBA 283
# define MBII 284
# define MINOR 285
# define MINUS 286
# define NEXUS 287
# define ON 288
# define OPTIONS 289
# define MAKEOPTIONS 290
# define PRIORITY 291
# define PSEUDO_DEVICE 292
# define ROOT 293
# define SEMICOLON 294
# define SIZE 295
# define SLAVE 296
# define SWAP 297
# define TIMEZONE 298
# define TRACE 299
# define UBA 300
# define VECTOR 301
# define VME 302
# define VME16D16 303
# define VME24D16 304
# define VME32D16 305
# define VME16D32 306
# define VME24D32 307
# define VME32D32 308
# define LUN 309
# define SLOT 310
# define TAPE 311
# define DEVICE_DRIVER 312
# define SCSIBUS 313
# define TARGET 314
# define ID 315
# define NUMBER 316
# define FPNUMBER 317

# line 121 "../../../src/config/config.y"
/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*
 * HISTORY
 * Revision 2.6.1.1  90/02/01  13:37:20  rvb
 * 	Cast all sprintf's (void).
 * 	[90/01/22            rvb]
 * 
 * 12-Feb-90  Paul Parker (parker) at Carnegie-Mellon University
 *	Added "sun4c" machine type.
 *
 * $Log:	config.y,v $
 * Revision 2.10  91/04/03  13:11:53  mbj
 * 	Omron changes[sic]
 * 
 * Revision 2.8.1.1  91/03/06  15:29:10  rvb
 * 	Omron changes[sic]
 * 
 * Revision 2.9  91/04/02  12:16:59  mbj
 * 	Cast all sprintf's (void).
 * 	[90/01/22            rvb]
 * 
 * Revision 2.8  90/12/17  17:15:39  berman
 * 	Added "sun4c" machine type.
 * 	[90/02/12            parker]
 * 
 * Revision 2.7.2.1  90/12/17  12:29:30  berman
 * 	Added "sun4c" machine type.
 * 	[90/02/12            parker]
 * 
 * Revision 2.7.1.1  90/12/16  17:30:53  berman
 * 	Added "sun4c" machine type.
 * 	[90/02/12            parker]
 * 
 * Revision 2.7  90/08/30  11:49:14  bohman
 * 	Changes for mac2.
 * 	[90/08/28            bohman]
 * 
 * Revision 2.6.2.1  90/08/30  09:46:01  bohman
 * 	Changes for mac2.
 * 	[90/08/28            bohman]
 * 
 * Revision 2.6  89/02/25  19:21:38  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.5  89/02/07  22:56:11  mwyoung
 * Code cleanup cataclysm.
 * 
 * Revision 2.4  89/01/31  01:21:37  rpd
 * 	Use unsigned format to save option values.
 * 	[89/01/26            dlb]
 * 
 * Revision 2.3  89/01/23  22:22:14  af
 * 	Acumulate options and makeoptions in forward order.
 * 	Recognize MAXDSIZ special (as time zone, ...) not as an option.
 * 	Add MIPS machine and rules.
 * 	Add i386 machine.
 * 	[89/01/09            rvb]
 * 
 * 03-Mar-88  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Made changes for Sun 4.
 *
 * 01-Dec-87  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Initialize machine type field "d_mach" and bus type field
 *	"d_bus" to zero in init_dev().
 *
 * 08-Jan-87  Robert Beck (beck) at Sequent Computer Systems, Inc.
 *	Sense machine type "sqt", set machine = MACHINE_SQT
 *	check_slot() checks "slots" for MACHINE_SQT
 *	Mods for variable device fields parsing for MACHINE_SQT
 *
 * 27-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in David Black's changes for the Multimax.
 *
 * 22-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Merged in rules and actions for Sun 2 and 3.
 *
 * 14-Oct-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Picked up Jonathan's bug fix to properly initialize d_next
 *	field in newdev.
 *
 */

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)config.y	5.2 (Berkeley) 4/18/86
 */

#include "config.h"
#include <ctype.h>
#include <stdio.h>

struct	device cur;
struct	device *curp = 0;
char	*temp_id;
char	*val_id;
char	*malloc();

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 881 "../../../src/config/config.y"


yyerror(s)
	char *s;
{

	fprintf(stderr, "config: line %d: %s\n", yyline, s);
}

/*
 * return the passed string in a new space
 */
char *
ns(str)
	register char *str;
{
	register char *cp;

	cp = malloc((unsigned)(strlen(str)+1));
	(void) strcpy(cp, str);
	return (cp);
}

/*
 * add a device to the list of devices
 */
newdev(dp)
	register struct device *dp;
{
	register struct device *np;

	np = (struct device *) malloc(sizeof *np);
	*np = *dp;
	if (curp == 0)
		dtab = np;
	else
		curp->d_next = np;
	curp = np;
	curp->d_next = 0;
}

/*
 * note that a configuration should be made
 */
mkconf(sysname)
	char *sysname;
{
	register struct file_list *fl, **flp;

	fl = (struct file_list *) malloc(sizeof *fl);
	fl->f_type = SYSTEMSPEC;
	fl->f_needs = sysname;
	fl->f_rootdev = NODEV;
	fl->f_argdev = NODEV;
	fl->f_dumpdev = NODEV;
	fl->f_fn = 0;
	fl->f_next = 0;
	for (flp = confp; *flp; flp = &(*flp)->f_next)
		;
	*flp = fl;
	confp = flp;
}

struct file_list *
newswap()
{
	struct file_list *fl = (struct file_list *)malloc(sizeof (*fl));

	fl->f_type = SWAPSPEC;
	fl->f_next = 0;
	fl->f_swapdev = NODEV;
	fl->f_swapsize = 0;
	fl->f_needs = 0;
	fl->f_fn = 0;
	return (fl);
}

/*
 * Add a swap device to the system's configuration
 */
mkswap(system, fl, size)
	struct file_list *system, *fl;
	int size;
{
	register struct file_list **flp;
	char *cp, name[80];

	if (system == 0 || system->f_type != SYSTEMSPEC) {
		yyerror("\"swap\" spec precedes \"config\" specification");
		return;
	}
	if (size < 0) {
		yyerror("illegal swap partition size");
		return;
	}
	/*
	 * Append swap description to the end of the list.
	 */
	flp = &system->f_next;
	for (; *flp && (*flp)->f_type == SWAPSPEC; flp = &(*flp)->f_next)
		;
	fl->f_next = *flp;
	*flp = fl;
	fl->f_swapsize = size;
	/*
	 * If first swap device for this system,
	 * set up f_fn field to insure swap
	 * files are created with unique names.
	 */
	if (system->f_fn)
		return;
	if (eq(fl->f_fn, "generic"))
		system->f_fn = ns(fl->f_fn);
	else
		system->f_fn = ns(system->f_needs);
}

/*
 * find the pointer to connect to the given device and number.
 * returns 0 if no such device and prints an error message
 */
struct device *
connect(dev, num)
	register char *dev;
	register int num;
{
	register struct device *dp;
	struct device *huhcon();

	if (num == QUES)
		return (huhcon(dev));
	for (dp = dtab; dp != 0; dp = dp->d_next) {
		if ((num != dp->d_unit) || !eq(dev, dp->d_name))
			continue;
		if (dp->d_type != CONTROLLER && dp->d_type != MASTER &&
			(machine == MACHINE_SUN4C && 
				dp->d_type != DEVICE_DRIVER)) {
			 (void) sprintf(errbuf,
			    "%s connected to non-controller", dev);
			 yyerror(errbuf);
			return (0);
		}
		return (dp);
	}
	(void) sprintf(errbuf, "%s %d not defined", dev, num);
	yyerror(errbuf);
	return (0);
}

/*
 * connect to an unspecific thing
 */
struct device *
huhcon(dev)
	register char *dev;
{
	register struct device *dp, *dcp;
	struct device rdev;
	int oldtype;

	/*
	 * First make certain that there are some of these to wildcard on
	 */
	for (dp = dtab; dp != 0; dp = dp->d_next)
		if (eq(dp->d_name, dev))
			break;
	if (dp == 0) {
		(void) sprintf(errbuf, "no %s's to wildcard", dev);
		yyerror(errbuf);
		return (0);
	}
	oldtype = dp->d_type;
	dcp = dp->d_conn;
	/*
	 * Now see if there is already a wildcard entry for this device
	 * (e.g. Search for a "uba ?")
	 */
	for (; dp != 0; dp = dp->d_next)
		if (eq(dev, dp->d_name) && dp->d_unit == -1)
			break;
	/*
	 * If there isn't, make one because everything needs to be connected
	 * to something.
	 */
	if (dp == 0) {
		dp = &rdev;
		init_dev(dp);
		dp->d_unit = QUES;
		dp->d_name = ns(dev);
		dp->d_type = oldtype;
		newdev(dp);
		dp = curp;
		/*
		 * Connect it to the same thing that other similar things are
		 * connected to, but make sure it is a wildcard unit
		 * (e.g. up connected to sc ?, here we make connect sc? to a
		 * uba?).  If other things like this are on the NEXUS or
		 * if they aren't connected to anything, then make the same
		 * connection, else call ourself to connect to another
		 * unspecific device.
		 */
		if (dcp == TO_NEXUS || dcp == 0)
			dp->d_conn = dcp;
		else
			dp->d_conn = connect(dcp->d_name, QUES);
	}
	return (dp);
}

init_dev(dp)
	register struct device *dp;
{

	dp->d_name = "OHNO!!!";
	dp->d_type = DEVICE;
	dp->d_conn = 0;
	dp->d_vec = 0;
	dp->d_addr = dp->d_pri = dp->d_flags = dp->d_dk = 0;
	dp->d_slave = dp->d_drive = dp->d_unit = UNKNOWN;
	if (machine == MACHINE_SUN2 || 
	    machine == MACHINE_SUN3 ||
	    machine == MACHINE_SUN4 ||
	    machine == MACHINE_SUN4C){
		dp->d_addr = UNKNOWN;
		dp->d_mach = dp->d_bus = 0;
	}
	if (machine == MACHINE_MIPSY || machine == MACHINE_MIPS){
		dp->d_addrmod = 0;
	}
	dp->d_init = dp->d_intr = 0;
}

/*
 * make certain that this is a reasonable type of thing to connect to a nexus
 */
check_nexus(dev, num)
	register struct device *dev;
	int num;
{

	switch (machine) {

	case MACHINE_VAX:
		if (!eq(dev->d_name, "uba") && !eq(dev->d_name, "mba"))
			yyerror("only uba's and mba's should be connected to the nexus");
		if (num != QUES)
			yyerror("can't give specific nexus numbers");
		break;

	case MACHINE_SUN:
		if (!eq(dev->d_name, "mb"))
			yyerror("only mb's should be connected to the nexus");
		break;

	case MACHINE_ROMP:
		if (!eq(dev->d_name, "iocc"))
			yyerror("only iocc's should be connected to the nexus");
		break;
        case MACHINE_SUN2:
		if (!eq(dev->d_name, "virtual") &&
		    !eq(dev->d_name, "obmem") &&
		    !eq(dev->d_name, "obio") &&
		    !eq(dev->d_name, "mbmem") &&
		    !eq(dev->d_name, "mbio") &&
		    !eq(dev->d_name, "vme16d16") &&
		    !eq(dev->d_name, "vme24d16")) {
			(void)sprintf(errbuf,
			    "unknown bus type `%s' for nexus connection on %s",
			    dev->d_name, machinename);
			yyerror(errbuf);
		}

	case MACHINE_MMAX:
		yyerror("don't grok 'nexus' on mmax -- try 'slot'.");
		break;
        case MACHINE_SUN3:
        case MACHINE_SUN4:
		if (!eq(dev->d_name, "virtual") &&
		    !eq(dev->d_name, "obmem") &&
		    !eq(dev->d_name, "obio") &&
		    !eq(dev->d_name, "mbmem") &&
		    !eq(dev->d_name, "mbio") &&
		    !eq(dev->d_name, "vme16d16") &&
		    !eq(dev->d_name, "vme24d16") &&
                    !eq(dev->d_name, "vme32d16") &&
		    !eq(dev->d_name, "vme16d32") &&
		    !eq(dev->d_name, "vme24d32") &&
		    !eq(dev->d_name, "vme32d32")) {
			(void)sprintf(errbuf,
			    "unknown bus type `%s' for nexus connection on %s",
			    dev->d_name, machinename);
			yyerror(errbuf);
		}
		break;
	case MACHINE_SUN4C:
		if (!eq(dev->d_name, "somewhere")) {
			(void)sprintf(errbuf,
			    "unknown bus type `%s' for nexus connection on %s",
			    dev->d_name, machinename);
			yyerror(errbuf);
		}
		break;
	case MACHINE_MIPSY:
	case MACHINE_MIPS:
		if (!eq(dev->d_name, "vme") && !eq(dev->d_name, "mbii"))
			yyerror("only vme's and mbii's should be connected to the nexus");
		if (num != QUES)
			yyerror("can't give specific nexus numbers");
		break;
	case MACHINE_LUNA88K:
		if (!eq(dev->d_name, "obio") && !eq(dev->d_name, "obmem") &&
				!eq(dev->d_name, "xp"))
			yyerror("only obio's, obmem's and xp's should be connected to the nexus");
		break;
	}
}

/*
 * make certain that this is a reasonable type of thing to connect to a slot
 */

check_slot(dev, num)
	register struct device *dev;
	int num;
{

	switch (machine) {

	case MACHINE_MMAX:
		if (!eq(dev->d_name, "emc"))
			yyerror("only emc's plug into backplane slots.");
		if (num == QUES)
			yyerror("specific slot numbers must be given");
		break;

	case MACHINE_SQT:
		if (!eq(dev->d_name, "mbad") &&
		    !eq(dev->d_name, "zdc") &&
		    !eq(dev->d_name, "sec")) {
			(void)sprintf(errbuf,
			    "unknown bus type `%s' for slot on %s",
			    dev->d_name, machinename);
			yyerror(errbuf);
		}
		break;

	case MACHINE_LUNA88K:
		if (!eq(dev->d_name, "obio") && !eq(dev->d_name, "obmem") &&
				!eq(dev->d_name, "xp"))
			yyerror("only obio's, obmem's and xp's should be connected to the nexus");
		break;

	default:
		yyerror("don't grok 'slot' for this machine -- try 'nexus'.");
		break;
	}
}

/*
 * Check the timezone to make certain it is sensible
 */

check_tz()
{
	if (abs(timezone) > 12 * 60)
		yyerror("timezone is unreasonable");
	else
		hadtz = 1;
}

/*
 * Check system specification and apply defaulting
 * rules on root, argument, dump, and swap devices.
 */
checksystemspec(fl)
	register struct file_list *fl;
{
	char buf[BUFSIZ];
	register struct file_list *swap;
	int generic;

	if (fl == 0 || fl->f_type != SYSTEMSPEC) {
		yyerror("internal error, bad system specification");
		exit(1);
	}
	swap = fl->f_next;
	generic = swap && swap->f_type == SWAPSPEC && eq(swap->f_fn, "generic");
	if (fl->f_rootdev == NODEV && !generic) {
		yyerror("no root device specified");
		exit(1);
	}
	/*
	 * Default swap area to be in 'b' partition of root's
	 * device.  If root specified to be other than on 'a'
	 * partition, give warning, something probably amiss.
	 */
	if (swap == 0 || swap->f_type != SWAPSPEC) {
		dev_t dev;

		swap = newswap();
		dev = fl->f_rootdev;
		if (minor(dev) & DEV_MASK) {
			(void) sprintf(buf,
"Warning, swap defaulted to 'b' partition with root on '%c' partition",
				(minor(dev) & DEV_MASK) + 'a');
			yyerror(buf);
		}
		swap->f_swapdev =
		   makedev(major(dev), (minor(dev) &~ DEV_MASK) | ('b' - 'a'));
		swap->f_fn = devtoname(swap->f_swapdev);
		mkswap(fl, swap, 0);
	}
	/*
	 * Make sure a generic swap isn't specified, along with
	 * other stuff (user must really be confused).
	 */
	if (generic) {
		if (fl->f_rootdev != NODEV)
			yyerror("root device specified with generic swap");
		if (fl->f_argdev != NODEV)
			yyerror("arg device specified with generic swap");
		if (fl->f_dumpdev != NODEV)
			yyerror("dump device specified with generic swap");
		return;
	}
	/*
	 * Default argument device and check for oddball arrangements.
	 */
	if (fl->f_argdev == NODEV)
		fl->f_argdev = swap->f_swapdev;
	if (fl->f_argdev != swap->f_swapdev)
		yyerror("Warning, arg device different than primary swap");
	/*
	 * Default dump device and warn if place is not a
	 * swap area or the argument device partition.
	 */
	if (fl->f_dumpdev == NODEV)
		fl->f_dumpdev = swap->f_swapdev;
	if (fl->f_dumpdev != swap->f_swapdev && fl->f_dumpdev != fl->f_argdev) {
		struct file_list *p = swap->f_next;

		for (; p && p->f_type == SWAPSPEC; p = p->f_next)
			if (fl->f_dumpdev == p->f_swapdev)
				return;
		(void) sprintf(buf, "Warning, orphaned dump device, %s",
			"do you know what you're doing");
		yyerror(buf);
	}
}

/*
 * Verify all devices specified in the system specification
 * are present in the device specifications.
 */
verifysystemspecs()
{
	register struct file_list *fl;
	dev_t checked[50], *verifyswap();
	register dev_t *pchecked = checked;

	for (fl = conf_list; fl; fl = fl->f_next) {
		if (fl->f_type != SYSTEMSPEC)
			continue;
		if (!finddev(fl->f_rootdev))
			deverror(fl->f_needs, "root");
		*pchecked++ = fl->f_rootdev;
		pchecked = verifyswap(fl->f_next, checked, pchecked);
#define	samedev(dev1, dev2) \
	((minor(dev1) &~ DEV_MASK) != (minor(dev2) &~ DEV_MASK))
		if (!alreadychecked(fl->f_dumpdev, checked, pchecked)) {
			if (!finddev(fl->f_dumpdev))
				deverror(fl->f_needs, "dump");
			*pchecked++ = fl->f_dumpdev;
		}
		if (!alreadychecked(fl->f_argdev, checked, pchecked)) {
			if (!finddev(fl->f_argdev))
				deverror(fl->f_needs, "arg");
			*pchecked++ = fl->f_argdev;
		}
	}
}

/*
 * Do as above, but for swap devices.
 */
dev_t *
verifyswap(fl, checked, pchecked)
	register struct file_list *fl;
	dev_t checked[];
	register dev_t *pchecked;
{

	for (;fl && fl->f_type == SWAPSPEC; fl = fl->f_next) {
		if (eq(fl->f_fn, "generic"))
			continue;
		if (alreadychecked(fl->f_swapdev, checked, pchecked))
			continue;
		if (!finddev(fl->f_swapdev))
			fprintf(stderr,
			   "config: swap device %s not configured", fl->f_fn);
		*pchecked++ = fl->f_swapdev;
	}
	return (pchecked);
}

/*
 * Has a device already been checked
 * for it's existence in the configuration?
 */
alreadychecked(dev, list, last)
	dev_t dev, list[];
	register dev_t *last;
{
	register dev_t *p;

	for (p = list; p < last; p++)
		if (samedev(*p, dev))
			return (1);
	return (0);
}

deverror(systemname, devtype)
	char *systemname, *devtype;
{

	fprintf(stderr, "config: %s: %s device not configured\n",
		systemname, devtype);
}

/*
 * Look for the device in the list of
 * configured hardware devices.  Must
 * take into account stuff wildcarded.
 */
finddev(dev)
	dev_t dev;
{

	/* punt on this right now */
	return (1);
}

/*
 * bi_info gives the magic number used to construct the token for
 * the autoconf code.  bi_max is the maximum value (across all
 * machine types for a given architecture) that a given "bus 
 * type" can legally have.
 */
struct bus_info {
	char    *bi_name;
	u_short bi_info;
	u_int   bi_max;
};

struct bus_info sun2_info[] = {
	{ "virtual",    0x0001, (1<<24)-1 },
	{ "obmem",      0x0002, (1<<23)-1 },
	{ "obio",       0x0004, (1<<23)-1 },
	{ "mbmem",      0x0010, (1<<20)-1 },
	{ "mbio",       0x0020, (1<<16)-1 },
	{ "vme16d16",   0x0100, (1<<16)-1 },
	{ "vme24d16",   0x0200, (1<<24)-(1<<16)-1 },
	{ (char *)0,    0,      0 }
};

struct bus_info sun3_info[] = {
	{ "virtual",    0x0001, (1<<32)-1 },
	{ "obmem",      0x0002, (1<<32)-1 },
	{ "obio",       0x0004, (1<<21)-1 },
	{ "vme16d16",   0x0100, (1<<16)-1 },
	{ "vme24d16",   0x0200, (1<<24)-(1<<16)-1 },
	{ "vme32d16",   0x0400, (1<<32)-(1<<24)-1 },
	{ "vme16d32",   0x1000, (1<<16) },
	{ "vme24d32",   0x2000, (1<<24)-(1<<16)-1 },
	{ "vme32d32",   0x4000, (1<<32)-(1<<24)-1 },
	{ (char *)0,    0,      0 }
};

struct bus_info sun4_info[] = {
	{ "virtual",    0x0001, 0xffffffff },
	{ "obmem",      0x0002, 0xffffffff },
	{ "obio",       0x0004, 0xffffffff },
	{ "vme16d16",   0x0100, (1<<16)-1 },
	{ "vme24d16",   0x0200, (1<<24)-(1<<16)-1 },
	{ "vme32d16",   0x0400, 0xfeffffff },
	{ "vme16d32",   0x1000, (1<<16) },
	{ "vme24d32",   0x2000, (1<<24)-(1<<16)-1 },
	{ "vme32d32",   0x4000, 0xfeffffff },
	{ (char *)0,    0,      0 }
};

struct bus_info sun4c_info[] = {
	{ (char *)0,	0,	0 }
};

bus_encode(addr, dp)
        u_int addr;
	register struct device *dp;
{
	register char *busname;
	register struct bus_info *bip;
	register int num;

	if (machine == MACHINE_SUN2)
		bip = sun2_info;
	else if (machine == MACHINE_SUN3)
		bip = sun3_info;
	else if (machine == MACHINE_SUN4)
		bip = sun4_info;
	else if (machine == MACHINE_SUN4C)
		bip = sun4c_info;
	else {
		yyerror("bad machine type for bus_encode");
		exit(1);
	}

        if (dp->d_conn == TO_NEXUS || dp->d_conn == 0) {
		yyerror("bad connection");
		exit(1);
	}

        busname = dp->d_conn->d_name;
        num = dp->d_conn->d_unit;

        for (; bip->bi_name != 0; bip++)
                if (eq(busname, bip->bi_name))
                        break;

        if (bip->bi_name == 0) {
                (void)sprintf(errbuf, "bad bus type '%s' for machine %s",
                        busname, machinename);
                yyerror(errbuf);
        } else if (addr > bip->bi_max) {
                (void)sprintf(errbuf,
                        "0x%x exceeds maximum address 0x%x allowed for %s",
                        addr, bip->bi_max, busname);
                yyerror(errbuf);
        } else {
                dp->d_bus = bip->bi_info;       /* set up bus type info */
                if (num != QUES)
                        /*
                         * Set up cpu type since the connecting
                         * bus type is not wildcarded.
                         */
                        dp->d_mach = num;
        }
}

/*
 * find the pointer to a scsibus to connect this scsi device to.
 * returns 0 if no such device and prints an error message
 */
struct device *
find_scsibus(num)
	register int num;
{
	register struct device *dp;

	for (dp = dtab; dp != 0; dp = dp->d_next) {
		if (dp->d_type != SCSIBUS || num != dp->d_unit)
			continue;
		return (dp);
	}
	(void)sprintf(errbuf, "scsibus %d not defined", num);
	yyerror(errbuf);
	return (0);
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 128
# define YYLAST 325
short yyact[]={

 158,  55,  97,  98, 194, 162,  43, 182, 126, 127,
 155, 204, 203, 156, 202, 122, 197, 160, 195, 140,
 163, 190, 189, 188, 187, 186,  74,  82, 185, 184,
 183,  53,  54, 179, 193, 178, 177, 173, 172, 157,
 168, 167, 166,  73, 165,  81,  75,  76,  77,  78,
  79,  80, 159,   8, 121,  43,  43, 161,  83, 201,
 151,  28,  13,  18, 131,   9,  11, 130, 115,  90,
  57,  56,  23,  21,  52,  17, 198,  10,  25,  26,
 112,  51, 107, 108, 176,  93,  19,  20,  32,  15,
  31,   7, 108,  30,  29,  24,   6,  74,  82, 150,
 101, 113, 199, 122,  94,  92, 133, 132,  12,  14,
  16,  74,  82,  67,  73,  96,  81,  75,  76,  77,
  78,  79,  80,  95, 111,  66,  91, 114,  73,  83,
  81,  75,  76,  77,  78,  79,  80, 123,  71, 174,
 139, 152, 135,  83, 106,  72,  65, 138, 120, 125,
  64,  49, 105, 137,  46, 100,  59,  69,  42,  44,
  47,  50,  34, 154, 109,  70, 134,  63,  62,  68,
  33,  61,  60,  58,  27,  22,  48,  39,  40,  41,
  45,  35,  36,  37,  38,  88,  89,   5,   4,   3,
   2,   1, 192,  84,  85,  86,  87, 136, 144, 141,
 147, 175,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  99,   0, 110,   0,   0,
   0, 102, 103, 104,   0,   0,   0,   0,   0,   0,
   0,   0,  47,   0,  50,   0,   0, 116, 117, 118,
 119,   0,   0,   0, 129, 128, 124,   0,   0, 153,
 143, 146, 149,   0,   0, 164, 142, 145, 148,   0,
   0,   0,   0, 170,   0,   0,   0,   0,   0, 171,
 169,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 153, 181, 180,   0,   0,   0,
   0,   0,   0,   0, 191,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 196,   0,   0,
   0,   0,   0, 153, 200 };
short yypact[]={

-1000,-1000,-203,-1000,-200,-201,-204,-1000,-206,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-259,-259,-259,
-259,-234,-1000,-242,-285,-245,-246,-147,-259,-1000,
-1000,-1000,-1000,-123,-172,-123,-123,-123,-123,-172,
-172,-247,-1000,-1000,-1000,-137,-1000,-168,-178,-1000,
-169,-1000,-1000,-148,-156,-314,-1000,-1000,-147,-1000,
-1000,-1000,-1000,-1000,-188,-188,-188,-188,-1000,-209,
-1000,-186,-248,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-209,-209,-209,-209,-1000,-262,
-124,-259,-307,-259,-307,-249,-252,-164,-165,-1000,
-260,-1000,-260,-260,-260,-1000,-192,-256,-259,-257,
-272,-274,-1000,-275,-276,-1000,-1000,-1000,-1000,-1000,
-1000,-174,-259,-172,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-278,-279,-119,-1000,-211,-1000,-1000,-280,
-281,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-283,-218,-1000,-309,-1000,-286,-287,-288,-291,-292,
-293,-294,-295,-259,-282,-1000,-1000,-1000,-1000,-1000,
-1000,-298,-1000,-1000,-260,-1000,-300,-239,-183,-1000,
-1000,-1000,-259,-1000,-1000,-1000,-1000,-1000,-1000,-250,
-1000,-1000,-1000,-1000,-302,-1000,-1000,-1000,-1000,-304,
-1000,-305,-1000,-1000,-1000 };
short yypgo[]={

   0, 140, 149, 145, 141, 201, 153, 147, 200, 199,
 198, 197, 192, 191, 190, 189, 188, 187, 180, 176,
 175, 174, 173, 156, 172, 171, 168, 167, 155, 166,
 142, 154, 151, 170, 157, 152, 162, 148, 165, 164,
 163, 144 };
short yyr1[]={

   0,  13,  14,  14,  15,  15,  15,  15,  15,  17,
  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,
  20,  21,  22,  22,  23,  23,  23,  23,  24,  29,
  29,  30,  11,  11,  25,   9,   9,  26,  10,  10,
  27,   8,   8,   7,  28,  28,   5,   5,   6,   6,
   6,  18,  18,  31,  31,   2,   2,   2,   1,  19,
  19,  32,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,  16,  16,  16,  16,  16,  16,  16,
  16,  16,  16,  37,  37,  33,  36,  34,  34,  38,
  38,  38,  38,  38,  39,  39,  40,  40,  40,  40,
  40,  40,  40,  40,  40,  40,  12,  12,  35,  35,
  35,  35,  35,  41,   4,   4,   4,   4 };
short yyr2[]={

   0,   1,   2,   0,   2,   2,   2,   1,   2,   2,
   2,   2,   2,   2,   1,   2,   2,   4,   3,   2,
   4,   3,   3,   5,   4,   3,   5,   4,   2,   2,
   2,   2,   2,   1,   1,   1,   1,   1,   3,   3,
   1,   2,   1,   1,   3,   1,   1,   3,   1,   1,
   3,   1,   1,   4,   1,   0,   2,   0,   1,   2,
   3,   3,   1,   1,   3,   1,   1,   0,   1,   3,
   1,   3,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   4,   4,   4,   4,   4,   3,   4,
   5,   6,   5,   2,   0,   3,   0,   2,   0,   3,
   3,   2,   3,   3,   2,   0,   2,   2,   2,   2,
   2,   2,   4,   2,   2,   2,   1,   2,   1,   2,
   3,   3,   0,   2,   1,   2,   2,   3 };
short yychk[]={

-1000, -13, -14, -15, -16, -17, 299, 294, 256, 268,
 280, 269, 311, 265, 312, 292, 313, 278, 266, 289,
 290, 276, -20, 275, 298, 281, 282, -21, 264, 294,
 294, 294, 294, -33, -36, -33, -33, -33, -33, -36,
 -36, -36,  -1, 315,  -1, -18, -31,  -1, -19, -32,
  -1, 315, 316, 316, 317, 286, 316, 316, -22, -23,
 -24, -25, -26, -27, 297, 293, 272, 260,  -1, -34,
 -38, 261,  -3, 300, 283, 303, 304, 305, 306, 307,
 308, 302, 284, 315, -34, -34, -34, -34,  -3,  -3,
 316, 263, 273, 263, 273, 271, 271, 316, 317, -23,
 -28, 288, -28, -28, -28, -35, -41, 291, 301, -39,
  -3, 310, 266, 287, 313, 316, -35, -35, -35, -35,
 -37, 316, 277, 261, -31,  -2, 315, 316, -32,  -2,
 316, 316, 271, 271, -29, -30, -11,  -6,  -7,  -1,
 279,  -9,  -6,  -7, -10,  -6,  -7,  -8,  -6,  -7,
 291, 316,  -4,  -1, -40, 267, 270, 296, 257, 309,
 274, 314, 262, 277,  -3, 316, 316, 316, 316, -37,
  -1,  -3, 316, 316, 258,  -5, 295, 316, 316, 316,
 -41,  -4, 316, 316, 316, 316, 316, 316, 316, 316,
 316,  -1, -12, 316, 286, 316, -30, 316, 315, 285,
  -4, 309, 316, 316, 316 };
short yydef[]={

   3,  -2,   1,   2,   0,   0,   0,   7,   0,  96,
  96,  96,  96,  96,  96,  96,  96,   0,   0,   0,
   0,   0,  14,   0,   0,   0,   0,   0,   0,   4,
   5,   6,   8,  98,   0,  98,  98,  98,  98,   0,
   0,   0,   9,  68,  10,  11,  62,  63,  12,  70,
   0,  13,  15,  16,  19,   0,  28,  29,  30,  33,
  34,  35,  36,  37,  55,  55,  55,  55,  31, 122,
 105,   0,   0,  72,  73,  74,  75,  76,  77,  78,
  79,  80,  81,  82, 122, 122, 122, 122,  88,  94,
   0,   0,  67,   0,  67,  18,  21,  22,  25,  32,
   0,  54,   0,   0,   0,  83, 118,   0,   0,  97,
   0,   0, 101,   0,   0,  95,  84,  85,  86,  87,
  89,  94,   0,   0,  61,  64,  65,  66,  69,  71,
  17,  20,  24,  27,  38,  40,  57,  42,  43,  58,
   0,  44,  45,  46,  47,  48,  49,  50,  51,  52,
   0, 119, 123, 124, 104,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  99, 100, 102, 103,  90,
  93,  92,  23,  26,   0,  41,   0,  59,   0, 121,
 120, 125, 126, 106, 107, 108, 109, 110, 111,   0,
 113, 114, 115, 116,   0,  91,  39,  56,  60,   0,
 127,   0, 117,  53, 112 };
#ifndef lint
static char yaccpar_sccsid[] = "@(#)yaccpar	4.1	(Berkeley)	2/11/83";
#endif not lint

#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

#ifdef YYDEBUG
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

#ifdef YYDEBUG
			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

#ifdef YYDEBUG
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
#endif
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 1:
# line 233 "../../../src/config/config.y"
 { verifysystemspecs(); } break;
case 4:
# line 244 "../../../src/config/config.y"
 { newdev(&cur); } break;
case 6:
# line 248 "../../../src/config/config.y"
 { do_trace = !do_trace; } break;
case 9:
# line 256 "../../../src/config/config.y"
 {
		if (!strcmp(yypvt[-0].str, "vax")) {
			machine = MACHINE_VAX;
			machinename = "vax";
		} else if (!strcmp(yypvt[-0].str, "sun")) {
			/* default to Sun 3 */
			machine = MACHINE_SUN3;
			machinename = "sun3";
		} else if (!strcmp(yypvt[-0].str, "sun2")) {
			machine = MACHINE_SUN2;
			machinename = "sun2";
		} else if (!strcmp(yypvt[-0].str, "sun3")) {
			machine = MACHINE_SUN3;
			machinename = "sun3";
		} else if (!strcmp(yypvt[-0].str, "sun4")) {
			machine = MACHINE_SUN4;
			machinename = "sun4";
		} else if (!strcmp(yypvt[-0].str, "sun4c")) {
			machine = MACHINE_SUN4C;
			machinename = "sun4c";
		} else if (!strcmp(yypvt[-0].str, "romp")) {
			machine = MACHINE_ROMP;
			machinename = "romp";
		} else if (!strcmp(yypvt[-0].str, "ca")) {
			machine = MACHINE_ROMP;
			machinename = "ca";
		} else if (!strcmp(yypvt[-0].str, "mmax")) {
			machine = MACHINE_MMAX;
			machinename = "mmax";
		} else if (!strcmp(yypvt[-0].str, "sqt")) {
			machine = MACHINE_SQT;
			machinename = "sqt";
		} else if (!strcmp(yypvt[-0].str, "i")) {
			machine = MACHINE_I386;
			machinename = "i386";
		} else if (!strcmp(yypvt[-0].str, "i386")) {
			machine = MACHINE_I386;
			machinename = "i386";
		} else if (!strcmp(yypvt[-0].str, "ix")) {
			machine = MACHINE_IX;
			machinename = "ix";
		} else if (!strcmp(yypvt[-0].str, "mipsy")) {
			machine = MACHINE_MIPSY;
			machinename = "mipsy";
		} else if (!strcmp(yypvt[-0].str, "mips")) {
			machine = MACHINE_MIPS;
			machinename = "mips";
		} else if (!strcmp(yypvt[-0].str, "mac2")) {
			machine = MACHINE_MAC2;
			machinename = "mac2";
		} else if (!strcmp(yypvt[-0].str, "luna88k")) {
			machine = MACHINE_LUNA88K;
			machinename = "luna88k";
		} else
			yyerror("Unknown machine type");
	      } break;
case 10:
# line 313 "../../../src/config/config.y"
 {
		struct cputype *cp =
		    (struct cputype *)malloc(sizeof (struct cputype));
		cp->cpu_name = ns(yypvt[-0].str);
		cp->cpu_next = cputype;
		cputype = cp;
		free(temp_id);
	      } break;
case 13:
# line 326 "../../../src/config/config.y"
 { ident = ns(yypvt[-0].str); } break;
case 15:
# line 330 "../../../src/config/config.y"
 { yyerror("HZ specification obsolete; delete"); } break;
case 16:
# line 332 "../../../src/config/config.y"
 { timezone = 60 * yypvt[-0].val; check_tz(); } break;
case 17:
# line 334 "../../../src/config/config.y"
 { timezone = 60 * yypvt[-2].val; dst = yypvt[-0].val; check_tz(); } break;
case 18:
# line 336 "../../../src/config/config.y"
 { timezone = 60 * yypvt[-1].val; dst = 1; check_tz(); } break;
case 19:
# line 338 "../../../src/config/config.y"
 { timezone = yypvt[-0].val; check_tz(); } break;
case 20:
# line 340 "../../../src/config/config.y"
 { timezone = yypvt[-2].val; dst = yypvt[-0].val; check_tz(); } break;
case 21:
# line 342 "../../../src/config/config.y"
 { timezone = yypvt[-1].val; dst = 1; check_tz(); } break;
case 22:
# line 344 "../../../src/config/config.y"
 { timezone = -60 * yypvt[-0].val; check_tz(); } break;
case 23:
# line 346 "../../../src/config/config.y"
 { timezone = -60 * yypvt[-2].val; dst = yypvt[-0].val; check_tz(); } break;
case 24:
# line 348 "../../../src/config/config.y"
 { timezone = -60 * yypvt[-1].val; dst = 1; check_tz(); } break;
case 25:
# line 350 "../../../src/config/config.y"
 { timezone = -yypvt[-0].val; check_tz(); } break;
case 26:
# line 352 "../../../src/config/config.y"
 { timezone = -yypvt[-2].val; dst = yypvt[-0].val; check_tz(); } break;
case 27:
# line 354 "../../../src/config/config.y"
 { timezone = -yypvt[-1].val; dst = 1; check_tz(); } break;
case 28:
# line 356 "../../../src/config/config.y"
 { maxusers = yypvt[-0].val; } break;
case 29:
# line 358 "../../../src/config/config.y"
 { maxdsiz = yypvt[-0].val; } break;
case 30:
# line 362 "../../../src/config/config.y"
 { checksystemspec(*confp); } break;
case 31:
# line 367 "../../../src/config/config.y"
 { mkconf(yypvt[-0].str); } break;
case 41:
# line 393 "../../../src/config/config.y"
 { mkswap(*confp, yypvt[-1].file, yypvt[-0].val); } break;
case 42:
# line 398 "../../../src/config/config.y"
 {
			struct file_list *fl = newswap();

			if (eq(yypvt[-0].str, "generic"))
				fl->f_fn = yypvt[-0].str;
			else {
				fl->f_swapdev = nametodev(yypvt[-0].str, 0, 'b');
				fl->f_fn = devtoname(fl->f_swapdev);
			}
			yyval.file = fl;
		} break;
case 43:
# line 410 "../../../src/config/config.y"
 {
			struct file_list *fl = newswap();

			fl->f_swapdev = yypvt[-0].val;
			fl->f_fn = devtoname(yypvt[-0].val);
			yyval.file = fl;
		} break;
case 44:
# line 421 "../../../src/config/config.y"
 {
			struct file_list *fl = *confp;

			if (fl && fl->f_rootdev != NODEV)
				yyerror("extraneous root device specification");
			else
				fl->f_rootdev = yypvt[-0].val;
		} break;
case 45:
# line 433 "../../../src/config/config.y"
 { yyval.val = nametodev(yypvt[-0].str, 0, 'a'); } break;
case 47:
# line 439 "../../../src/config/config.y"
 {
			struct file_list *fl = *confp;

			if (fl && fl->f_dumpdev != NODEV)
				yyerror("extraneous dump device specification");
			else
				fl->f_dumpdev = yypvt[-0].val;
		} break;
case 48:
# line 452 "../../../src/config/config.y"
 { yyval.val = nametodev(yypvt[-0].str, 0, 'b'); } break;
case 50:
# line 458 "../../../src/config/config.y"
 {
			struct file_list *fl = *confp;

			if (fl && fl->f_argdev != NODEV)
				yyerror("extraneous arg device specification");
			else
				fl->f_argdev = yypvt[-0].val;
		} break;
case 51:
# line 470 "../../../src/config/config.y"
 { yyval.val = nametodev(yypvt[-0].str, 0, 'b'); } break;
case 53:
# line 476 "../../../src/config/config.y"
 { yyval.val = makedev(yypvt[-2].val, yypvt[-0].val); } break;
case 56:
# line 486 "../../../src/config/config.y"
 { yyval.val = yypvt[-0].val; } break;
case 57:
# line 488 "../../../src/config/config.y"
 { yyval.val = 0; } break;
case 58:
# line 493 "../../../src/config/config.y"
 { yyval.str = yypvt[-0].str; } break;
case 59:
# line 495 "../../../src/config/config.y"
 {
			char buf[80];

			(void) sprintf(buf, "%s%d", yypvt[-1].str, yypvt[-0].val);
			yyval.str = ns(buf); free(yypvt[-1].str);
		} break;
case 60:
# line 502 "../../../src/config/config.y"
 {
			char buf[80];

			(void) sprintf(buf, "%s%d%s", yypvt[-2].str, yypvt[-1].val, yypvt[-0].str);
			yyval.str = ns(buf); free(yypvt[-2].str);
		} break;
case 63:
# line 518 "../../../src/config/config.y"
 {
		struct opt *op = (struct opt *)malloc(sizeof (struct opt));
		op->op_name = ns(yypvt[-0].str);
		op->op_next = (struct opt *) 0;
		op->op_value = 0;
		if (opt == (struct opt *) 0)
			opt = op;
		else
			opt_tail->op_next = op;
		opt_tail = op;
		free(temp_id);
	      } break;
case 64:
# line 531 "../../../src/config/config.y"
 {
		struct opt *op = (struct opt *)malloc(sizeof (struct opt));
		op->op_name = ns(yypvt[-2].str);
		op->op_next = (struct opt *) 0;
		op->op_value = ns(yypvt[-0].str);
		if (opt == (struct opt *) 0)
			opt = op;
		else
			opt_tail->op_next = op;
		opt_tail = op;
		free(temp_id);
		if (val_id)
			free(val_id);
	      } break;
case 65:
# line 548 "../../../src/config/config.y"
 { yyval.str = val_id = ns(yypvt[-0].str); } break;
case 66:
# line 550 "../../../src/config/config.y"
 { char nb[16];

	          (void) sprintf(nb, "%u", yypvt[-0].val);
	      	  yyval.str = val_id = ns(nb); } break;
case 67:
# line 555 "../../../src/config/config.y"
 { yyval.str = val_id = ns(""); } break;
case 68:
# line 561 "../../../src/config/config.y"
 { yyval.str = temp_id = ns(yypvt[-0].str); } break;
case 71:
# line 572 "../../../src/config/config.y"
 {
		struct opt *op = (struct opt *)malloc(sizeof (struct opt));
		op->op_name = ns(yypvt[-2].str);
		op->op_next =  (struct opt *) 0;
		op->op_value = ns(yypvt[-0].str);
		if (mkopt == (struct opt *) 0)
			mkopt = op;
		else
			mkopt_tail->op_next = op;
		mkopt_tail = op;
		free(temp_id);
		if (val_id)
			free(val_id);
	      } break;
case 72:
# line 589 "../../../src/config/config.y"
 { yyval.str = ns("uba"); } break;
case 73:
# line 591 "../../../src/config/config.y"
 { yyval.str = ns("mba"); } break;
case 74:
# line 593 "../../../src/config/config.y"
 {
		if (machine != MACHINE_SUN2 && machine != MACHINE_SUN3
		    && machine != MACHINE_SUN4)
			yyerror("wrong machine type for vme16d16");
		yyval.str = ns("vme16d16");
		} break;
case 75:
# line 600 "../../../src/config/config.y"
 {
		if (machine != MACHINE_SUN2 && machine != MACHINE_SUN3
		    && machine != MACHINE_SUN4)
			yyerror("wrong machine type for vme24d16");
			yyval.str = ns("vme24d16");
		} break;
case 76:
# line 607 "../../../src/config/config.y"
 {
		if (machine != MACHINE_SUN3 && machine != MACHINE_SUN4)

                        yyerror("wrong machine type for vme32d16");
                yyval.str = ns("vme32d16");
                } break;
case 77:
# line 614 "../../../src/config/config.y"
 {
                if (machine != MACHINE_SUN3  && machine != MACHINE_SUN4)
                        yyerror("wrong machine type for vme16d32");
                yyval.str = ns("vme16d32");
                } break;
case 78:
# line 620 "../../../src/config/config.y"
 {
		if (machine != MACHINE_SUN3 && machine != MACHINE_SUN4)
			yyerror("wrong machine type for vme24d32");
		yyval.str = ns("vme24d32");
		} break;
case 79:
# line 626 "../../../src/config/config.y"
 {
		if (machine != MACHINE_SUN3 && machine != MACHINE_SUN4)
			yyerror("wrong machine type for vme32d32");
		yyval.str = ns("vme32d32");
		} break;
case 80:
# line 632 "../../../src/config/config.y"
 {
		if (machine != MACHINE_MIPSY && machine != MACHINE_MIPS)
			yyerror("wrong machine type for vme");
			yyval.str = ns("vme");
		} break;
case 81:
# line 638 "../../../src/config/config.y"
 {
		if (machine != MACHINE_MIPSY && machine != MACHINE_MIPS)
			yyerror("wrong machine type for mbii");
			yyval.str = ns("mbii");
		} break;
case 82:
# line 644 "../../../src/config/config.y"
 { yyval.str = ns(yypvt[-0].str); } break;
case 83:
# line 649 "../../../src/config/config.y"
 { cur.d_type = DEVICE; } break;
case 84:
# line 651 "../../../src/config/config.y"
 { cur.d_type = MASTER; } break;
case 85:
# line 653 "../../../src/config/config.y"
 { cur.d_dk = 1; cur.d_type = DEVICE; } break;
case 86:
# line 656 "../../../src/config/config.y"
 { cur.d_type = DEVICE; } break;
case 87:
# line 658 "../../../src/config/config.y"
 { cur.d_type = CONTROLLER; } break;
case 88:
# line 660 "../../../src/config/config.y"
 {
		cur.d_name = yypvt[-0].str;
		cur.d_type = DEVICE_DRIVER; /* WAS PSEUDO_DEVICE */
		cur.d_unit = 0;	/* DID NOT EXIST */
		} break;
case 89:
# line 666 "../../../src/config/config.y"
 {
		cur.d_name = yypvt[-1].str;
		cur.d_type = PSEUDO_DEVICE;
		} break;
case 90:
# line 671 "../../../src/config/config.y"
 {
		cur.d_name = yypvt[-2].str;
		cur.d_type = PSEUDO_DEVICE;
		cur.d_slave = yypvt[-1].val;
		} break;
case 91:
# line 677 "../../../src/config/config.y"
 {
			cur.d_name = "scsibus";
			cur.d_type = SCSIBUS;
			cur.d_unit = yypvt[-3].val;
			cur.d_conn = connect(yypvt[-1].str, yypvt[-0].val);
		} break;
case 92:
# line 684 "../../../src/config/config.y"
 {
			cur.d_name = "scsibus";
			cur.d_unit = yypvt[-2].val;
			cur.d_type = SCSIBUS;
			cur.d_conn = connect(yypvt[-0].str, 0);
		} break;
case 93:
# line 692 "../../../src/config/config.y"
 {
		cur.d_init = yypvt[-0].str;
		} break;
case 95:
# line 700 "../../../src/config/config.y"
 {
		cur.d_name = yypvt[-1].str;
		if (eq(yypvt[-1].str, "mba"))
			seen_mba = 1;
		else if (eq(yypvt[-1].str, "uba"))
			seen_uba = 1;
		else if (eq(yypvt[-1].str, "mbii"))
			seen_mbii = 1;
		else if (eq(yypvt[-1].str, "vme"))
			seen_vme = 1;
		cur.d_unit = yypvt[-0].val;
		} break;
case 96:
# line 715 "../../../src/config/config.y"
 { init_dev(&cur); } break;
case 99:
# line 725 "../../../src/config/config.y"
 {
		if (eq(cur.d_name, "mba") || eq(cur.d_name, "uba")
		    || eq(cur.d_name, "mbii") || eq(cur.d_name, "vme")) {
			(void) sprintf(errbuf,
			    "%s must be connected to a nexus", cur.d_name);
			yyerror(errbuf);
		}
		cur.d_conn = connect(yypvt[-1].str, yypvt[-0].val);
		if (machine == MACHINE_SQT)
			dev_param(&cur, "index", cur.d_unit);
		} break;
case 100:
# line 738 "../../../src/config/config.y"
 { 
		check_slot(&cur, yypvt[-0].val);
		cur.d_addr = yypvt[-0].val;
		cur.d_conn = TO_SLOT; 
		 } break;
case 101:
# line 744 "../../../src/config/config.y"
 { cur.d_conn = TO_CPU; } break;
case 102:
# line 746 "../../../src/config/config.y"
 { check_nexus(&cur, yypvt[-0].val); cur.d_conn = TO_NEXUS; } break;
case 103:
# line 748 "../../../src/config/config.y"
 {
		cur.d_conn = find_scsibus(yypvt[-0].val);
	      } break;
case 106:
# line 760 "../../../src/config/config.y"
 {
		cur.d_addr = yypvt[-0].val;
                if (machine == MACHINE_SUN2 || 
		    machine == MACHINE_SUN3 ||
		    machine == MACHINE_SUN4)
			bus_encode(yypvt[-0].val, &cur);
		if (machine == MACHINE_SQT) {
			dev_param(&cur, "csr", yypvt[-0].val);
		}
		} break;
case 107:
# line 771 "../../../src/config/config.y"
 {
			cur.d_drive = yypvt[-0].val;
			if (machine == MACHINE_SQT) {
				dev_param(&cur, "drive", yypvt[-0].val);
			}
		} break;
case 108:
# line 778 "../../../src/config/config.y"
 {
		if (cur.d_conn != 0 && cur.d_conn != TO_NEXUS &&
		    cur.d_conn->d_type == MASTER)
			cur.d_slave = yypvt[-0].val;
		else
			yyerror("can't specify slave--not to master");
		} break;
case 109:
# line 787 "../../../src/config/config.y"
 { cur.d_addrmod = yypvt[-0].val; } break;
case 110:
# line 790 "../../../src/config/config.y"
 {
		if ((cur.d_conn != 0) && (cur.d_conn != TO_SLOT) &&
			(cur.d_conn->d_type == CONTROLLER)) {
			cur.d_addr = yypvt[-0].val; 
		}
		else {
			yyerror("device requires controller card");
		    }
		} break;
case 111:
# line 800 "../../../src/config/config.y"
 {
		cur.d_flags = yypvt[-0].val;
		if (machine == MACHINE_SQT) {
			dev_param(&cur, "flags", yypvt[-0].val);
		}
	      } break;
case 112:
# line 807 "../../../src/config/config.y"
 {
		cur.d_type = DEVICE;
		cur.d_slave = yypvt[-2].val<<3 | yypvt[-0].val;
	      } break;
case 113:
# line 812 "../../../src/config/config.y"
 { 
		 if (machine != MACHINE_SQT)
			yyerror("bin specification only valid on Sequent Balance");
		 if (yypvt[-0].val < 1 || yypvt[-0].val > 7)  
			yyerror("bogus bin number");
		 else {
			cur.d_bin = yypvt[-0].val;
			dev_param(&cur, "bin", yypvt[-0].val);
		}
	       } break;
case 114:
# line 823 "../../../src/config/config.y"
 { cur.d_init = yypvt[-0].str; } break;
case 115:
# line 825 "../../../src/config/config.y"
 {
		if (machine != MACHINE_SQT)
			yyerror("bad device spec");
		dev_param(&cur, yypvt[-1].str, yypvt[-0].val);
		} break;
case 117:
# line 835 "../../../src/config/config.y"
 { yyval.val = -(yypvt[-0].val); } break;
case 118:
# line 840 "../../../src/config/config.y"
 { cur.d_pri = 0; } break;
case 119:
# line 842 "../../../src/config/config.y"
 { cur.d_pri = yypvt[-0].val; } break;
case 120:
# line 844 "../../../src/config/config.y"
 { cur.d_pri = yypvt[-1].val; } break;
case 121:
# line 846 "../../../src/config/config.y"
 { cur.d_pri = yypvt[-0].val; } break;
case 123:
# line 852 "../../../src/config/config.y"
 { cur.d_vec = yypvt[-0].lst; } break;
case 124:
# line 857 "../../../src/config/config.y"
 {
		struct idlst *a = (struct idlst *)malloc(sizeof(struct idlst));
		a->id = yypvt[-0].str; a->id_next = 0; yyval.lst = a;
		a->id_vec = 0;
		} break;
case 125:
# line 862 "../../../src/config/config.y"

		{
		struct idlst *a = (struct idlst *)malloc(sizeof(struct idlst));
	        a->id = yypvt[-1].str; a->id_next = yypvt[-0].lst; yyval.lst = a;
		a->id_vec = 0;
		} break;
case 126:
# line 869 "../../../src/config/config.y"
 {
		struct idlst *a = (struct idlst *)malloc(sizeof(struct idlst));
		a->id_next = 0; a->id = yypvt[-1].str; yyval.lst = a;
		a->id_vec = yypvt[-0].val;
		} break;
case 127:
# line 875 "../../../src/config/config.y"
 {
		struct idlst *a = (struct idlst *)malloc(sizeof(struct idlst));
		a->id_next = yypvt[-0].lst; a->id = yypvt[-2].str; yyval.lst = a;
		a->id_vec = yypvt[-1].val;
		} break;
		}
		goto yystack;  /* stack new state and value */

	}
