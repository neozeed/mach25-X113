# 1 "../../machine/genassym.c"
# 1 "../../machine/../i386/genassym.c" 1
 






 





























# 1 "confdep.h" 1




# 38 "../../machine/../i386/genassym.c" 2


 







# 1 "../../sys/param.h" 1
 







 














































































































































 

















# 1 "../../sys/types.h" 1
 







 













































































 












 



 


 


 


# 1 "../../mach/machine/vm_types.h" 1
# 1 "../../mach/machine/../i386/vm_types.h" 1
 






 
















 













typedef	unsigned int	vm_offset_t;
typedef	unsigned int	vm_size_t;


# 1 "../../mach/machine/vm_types.h" 2

# 113 "../../sys/types.h" 2


typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;		 

typedef	unsigned char	unchar;		 









typedef	struct	_physadr { int r[1]; } *physadr;
typedef	struct	label_t	{
	int	val[14];
} label_t;





















# 165 "../../sys/types.h"


typedef	struct	_quad { u_long val[2]; } quad;



typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	u_long	ino_t;
typedef	long	swblk_t;



typedef	unsigned long	size_t;




typedef	long	time_t;


typedef	short	dev_t;

typedef	u_long	off_t;



typedef	u_short	uid_t;
typedef	u_short	gid_t;


 









typedef long	fd_mask;





typedef	struct fd_set {
	fd_mask	fds_bits[((( 256  )+((  (sizeof(fd_mask) * 8 )  )-1))/(  (sizeof(fd_mask) * 8 )  )) ];
} fd_set;






# 230 "../../sys/types.h"



# 170 "../../sys/param.h" 2

# 1 "../../machine/machparam.h" 1
# 1 "../../machine/../i386/machparam.h" 1
 






 
























 
 







 














 


 



 




 


 


 









int	cpuspeed;






# 1 "../../machine/machparam.h" 2

# 171 "../../sys/param.h" 2




 



 










 

















 



# 1 "../../sys/signal.h" 1
 







 






















































































































































 



























# 206 "../../sys/signal.h"











 





# 237 "../../sys/signal.h"









































































 










# 334 "../../sys/signal.h"


 



struct	sigvec {

	void	(*sv_handler)();	 



	int	sv_mask;		 
	int	sv_flags;		 
};





 



struct	sigstack {
	char	*ss_sp;			 
	int	ss_onstack;		 
};

 






struct	sigcontext {
	int	sc_onstack;		 
	int	sc_mask;		 
# 405 "../../sys/signal.h"


































# 456 "../../sys/signal.h"



	int	sc_gs;
	int	sc_fs;
	int	sc_es;
	int	sc_ds;
	int	sc_edi;
	int	sc_esi;
	int	sc_ebp;
	int	sc_esp;
	int	sc_ebx;
	int	sc_edx;
	int	sc_ecx;
	int	sc_eax;
	int	sc_trapno;	 
	int	sc_err;		 
	int	sc_eip;
	int	sc_cs;
	int	sc_efl;
	int	sc_uesp;
	int	sc_ss;

# 503 "../../sys/signal.h"









};

















# 541 "../../sys/signal.h"






 






 









# 212 "../../sys/param.h" 2




 






 







 








 

















 






















 





 


































 







 













 















 





 







 











# 48 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/buf.h" 1
 







 






























































 












 




















 




struct bufhd
{
	long	b_flags;		 
	struct	buf *b_forw, *b_back;	 
};

struct buf
{
	long	b_flags;		 
	struct	buf *b_forw, *b_back;	 
	struct	buf *av_forw, *av_back;	 


	long	b_bcount;		 
	long	b_bufsize;		 

	short	b_error;		 
	dev_t	b_dev;			 
	union {
	    caddr_t b_addr;		 
	    int	*b_words;		 
	    struct fs *b_fs;		 
	    struct csum *b_cs;		 
	    struct cg *b_cg;		 
	    struct dinode *b_dino;	 
	    daddr_t *b_daddr;		 
	} b_un;
	daddr_t	b_blkno;		 
	long	b_resid;		 

	struct  proc *b_proc;		 
	struct	inode *b_ip;		 
	int	(*b_iodone)();		 
	int	b_pfcent;		 









};



















extern struct	buf *buf;	 
extern char	*buffers;
extern int	nbuf;		 
extern int	bufpages;	 

extern struct	bufhd bufhash[512 ];	 
extern struct	buf bfreelist[4 ];	 

extern struct	buf *alloc();
extern struct	buf *realloccg();
extern struct	buf *baddr();
extern struct	buf *getblk();
extern struct	buf *geteblk();
extern struct	buf *getnewbuf();
extern struct	buf *bread();
extern struct	buf *breada();

extern unsigned minphys();


 














































# 1 "../../kern/macro_help.h" 1
 






 






















 









# 1 "../../mach/boolean.h" 1
 







 

























 









 




# 1 "../../mach/machine/boolean.h" 1
# 1 "../../mach/machine/../i386/boolean.h" 1
 





 












 








typedef int		boolean_t;


# 1 "../../mach/machine/boolean.h" 2

# 50 "../../mach/boolean.h" 2




 

















# 41 "../../kern/macro_help.h" 2
















# 241 "../../sys/buf.h" 2


 

















 

























 















 











# 49 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/vmparam.h" 1
 






 



















 










 



# 1 "../../machine/vmparam.h" 1
# 1 "../../machine/../i386/vmparam.h" 1
 






 


















 
 
 







 


 










extern	int sigcode[5]; 






 

















# 1 "../../machine/vmparam.h" 2

# 43 "../../sys/vmparam.h" 2



# 50 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/dir.h" 1
 







 










































 













 




























struct	direct {
	u_long	d_ino;			 
	u_short	d_reclen;		 
	u_short	d_namlen;		 
	char	d_name[255  + 1];	 
};


 









# 157 "../../sys/dir.h"



 




struct dirtemplate {
	u_long	dot_ino;
	short	dot_reclen;
	short	dot_namlen;
	char	dot_name[4];		 
	u_long	dotdot_ino;
	short	dotdot_reclen;
	short	dotdot_namlen;
	char	dotdot_name[4];		 
};


# 51 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/user.h" 1
 







 
















































































































































 











# 1 "mach_rfs.h" 1

# 166 "../../sys/user.h" 2

# 1 "vice.h" 1

# 167 "../../sys/user.h" 2



# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 170 "../../sys/user.h" 2



# 1 "../../sys/time.h" 1
 







 























 










 



struct timeval {
	long	tv_sec;		 
	long	tv_usec;	 
};

struct timezone {
	int	tz_minuteswest;	 
	int	tz_dsttime;	 
};









 










 







struct	itimerval {
	struct	timeval it_interval;	 
	struct	timeval it_value;	 
};


 


struct tm {
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};














# 173 "../../sys/user.h" 2

# 1 "../../sys/resource.h" 1
 







 



































 













 









 






struct	rusage {
	struct timeval ru_utime;	 
	struct timeval ru_stime;	 
	long	ru_maxrss;

	long	ru_ixrss;		 
	long	ru_idrss;		 
	long	ru_isrss;		 
	long	ru_minflt;		 
	long	ru_majflt;		 
	long	ru_nswap;		 
	long	ru_inblock;		 
	long	ru_oublock;		 
	long	ru_msgsnd;		 
	long	ru_msgrcv;		 
	long	ru_nsignals;		 
	long	ru_nvcsw;		 
	long	ru_nivcsw;		 

};

 













struct rlimit {
	int	rlim_cur;		 
	int	rlim_max;		 
};


 



struct rusage_dev {
	struct rusage ru_rusage;
	dev_t	      ru_dev;
};




 



















# 174 "../../sys/user.h" 2

# 1 "../../sys/namei.h" 1
 







 















































 











# 1 "../../sys/uio.h" 1
 







 






























 












struct iovec {
	caddr_t	iov_base;
	int	iov_len;
};

struct uio {
	struct	iovec *uio_iov;
	int	uio_iovcnt;
	off_t	uio_offset;
	int	uio_segflg;
	int	uio_resid;
};

enum	uio_rw { UIO_READ, UIO_WRITE };

 












# 69 "../../sys/namei.h" 2



 




struct nameidata {
	caddr_t	ni_dirp;		 
	short	ni_nameiop;		 
	short	ni_error;		 
	off_t	ni_endoff;		 
	struct	inode *ni_pdir;		 
	struct	iovec ni_iovec;		 
	struct	uio ni_uio;		 
	struct	direct ni_dent;		 
};










# 1 "mach_rfs.h" 1

# 97 "../../sys/namei.h" 2

# 1 "vice.h" 1

# 98 "../../sys/namei.h" 2


 









# 136 "../../sys/namei.h"



 



struct	namecache {
	struct	namecache *nc_forw;	 
	struct	namecache *nc_back;	 
	struct	namecache *nc_nxt;	 
	struct	namecache **nc_prev;	 
	struct	inode *nc_ip;		 
	ino_t	nc_ino;			 
	dev_t	nc_dev;			 
	dev_t	nc_idev;		 
	long	nc_id;			 
	char	nc_nlen;		 

	char	nc_name[15 ];	 
};


extern struct	namecache *namecache;
extern int	nchsize;


 


struct	nchstats {
	long	ncs_goodhits;		 
	long	ncs_badhits;		 
	long	ncs_falsehits;		 
	long	ncs_miss;		 
	long	ncs_long;		 
	long	ncs_pass2;		 
	long	ncs_2passes;		 
};

# 186 "../../sys/namei.h"



# 175 "../../sys/user.h" 2

# 1 "../../machine/pcb.h" 1
# 1 "../../machine/../i386/pcb.h" 1
 






 


































































 
 

























# 1 "../../i386/fpreg.h" 1
 





 
























 




























extern char   fp_kind;			 
extern struct thread  *fp_thread;      




 









 



 



 
typedef u_char i387_reg[10];
 





 
struct i387_env {
	u_int	control	: 16;
	u_int		: 16;
	u_int	status	: 16;
	u_int		: 16;
	u_int	tags	: 16;
	u_int		: 16;
	u_int	ip	: 32;
	u_int	cs_sel	: 16;		 
	u_int	opcode	: 11;
	u_int		: 5;
	u_int	data	: 32;		 
	u_int	data_sel : 16;		 
	u_int		: 16;
};

 




struct i387_state {
	struct i387_env	env;
	i387_reg stack[8];
};






 













 














 





# 102 "../../machine/../i386/pcb.h" 2


 

typedef struct tss386_flags {
	u_int	fl_cf	:  1,		 
			:  1,		 
		fl_pf	:  1,		 
			:  1,		 
		fl_af	:  1,		 
			:  1,		 
		fl_zf	:  1,		 
		fl_sf	:  1,		 
		fl_tf	:  1,		 
		fl_if	:  1,		 
		fl_df	:  1,		 
		fl_of	:  1,		 
		fl_iopl :  2,		 
		fl_nt	:  1,		 
			:  1,		 
		fl_rf	:  1,		 
		fl_vm	:  1,		 
		fl_res	: 14;		 
} tss386_flags_t;















 




struct tss386 {
	u_long	t_link;
	u_long	t_esp0;
	u_long	t_ss0;
	u_long	t_esp1;
	u_long	t_ss1;
	u_long	t_esp2;
	u_long	t_ss2;
	u_long	t_cr3;
	u_long	t_eip;
	u_long	t_eflags;
	u_long	t_eax;
	u_long	t_ecx;
	u_long	t_edx;
	u_long	t_ebx;
	u_long	t_esp;
	u_long	t_ebp;
	u_long	t_esi;
	u_long	t_edi;
	u_long	t_es;
	u_long	t_cs;
	u_long	t_ss;
	u_long	t_ds;
	u_long	t_fs;
	u_long	t_gs;
	u_long	t_ldt;
	u_short	t_t;
	u_short	t_bitmapbase;
	u_char	t_bitmap[(0x3ff +1)>>3];
};

 



struct pcb {

	struct tss386 pcb_tss;

	 






	struct pt_entry *pcb_cr3;	 
	int	pcb_context[7];		 
	int	pcb_fpvalid;		 
	struct	fpstate {
		u_char	state[sizeof(struct i387_state) ];	
					 
		int	status;		 
	} pcb_fps;
	int	pcb_flags;
	 




	int	pcb_last_uip;		 
	int	pcb_ip_same;		 
	 
};






 











 



extern void pcb_synch();


extern void pcb_init( );
extern void ldt_init( );



# 1 "../../machine/pcb.h" 2

# 176 "../../sys/user.h" 2



# 1 "../../kern/lock.h" 1
 







 













































































 











# 1 "cpus.h" 1

# 99 "../../kern/lock.h" 2

# 1 "mach_ldebug.h" 1

# 100 "../../kern/lock.h" 2


# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 102 "../../kern/lock.h" 2




 



struct slock {
	int		lock_data;	 
};

typedef struct slock	simple_lock_data_t;
typedef struct slock	*simple_lock_t;

# 129 "../../kern/lock.h"

 











 









struct lock {
# 163 "../../kern/lock.h"

# 180 "../../kern/lock.h"

# 193 "../../kern/lock.h"

	 



	boolean_t	want_write;	 
	boolean_t	want_upgrade;	 
	boolean_t	waiting;	 
	boolean_t	can_sleep;	 
	int		read_count;	 



	char		*thread;
		 
		 

	int		recursion_depth; 



	 



	 

};

typedef struct lock	lock_data_t;
typedef struct lock	*lock_t;

 

extern void		lock_init();
extern void		lock_sleepable();
extern void		lock_write();
extern void		lock_read();
extern void		lock_done();
extern boolean_t	lock_read_to_write();
extern void		lock_write_to_read();
extern boolean_t	lock_try_write();
extern boolean_t	lock_try_read();
extern boolean_t	lock_try_read_to_write();




extern void		lock_set_recursive();
extern void		lock_clear_recursive();


# 179 "../../sys/user.h" 2








# 202 "../../sys/user.h"


 







 





struct uthread {
	int	*uu_ar0;		 

 
	 
	int	uu_arg[8];		 
	int	*uu_ap;			 
	label_t	uu_qsave;		 
	union {				 
		struct	{
			int	R_val1;
			int	R_val2;
		} u_rv;


		off_t	r_off;
		time_t	r_time;
	} uu_r;
	char	uu_error;		 
	char	uu_eosys;		 
	uid_t	uu_VOPuid;		 
	struct fs *uu_rpsfs;		 
	char	uu_rpswhich;		 





	u_char	uu_rpause;		 





	short	uu_rfscode;		 
	short	uu_rfsncnt;		 

	 
	short	uu_rmt_code;		 
	short	uu_rmt_ncnt;		 

	char	uu_rmt_requested;	 

	u_char  uu_rmt_follow1  ;	 


	u_char  uu_rmt_follow2  ;	 
	struct	buf *uu_rmt_path_buf;	 

	char	*uu_rmt_path;		 

					 
	struct	rmt_saved_data *uu_rmt_saved_data;

 
	struct unameicache {		 
		int nc_prevoffset;	 
		ino_t nc_inumber;	 
		dev_t nc_dev;		 
		time_t nc_time;		 
	} uu_ncache;
	struct	nameidata uu_nd;

 
	int	uu_code;			 
	char uu_cursig;				 
	int  uu_sig;				 
};

 


struct utask {
	struct	proc *uu_procp;		 
	char	uu_comm[16  + 1];

 



	struct identity *uu_identity;	 

# 321 "../../sys/user.h"


 
	size_t	uu_tsize;		 
	size_t	uu_dsize;		 
	size_t	uu_ssize;		 
	caddr_t	uu_text_start;		 
	caddr_t	uu_data_start;		 
	caddr_t	uu_stack_start;		 
	caddr_t	uu_stack_end;		 
	boolean_t uu_stack_grows_up;	 
	time_t	uu_outime;		 

 
	int	(*uu_signal[32 +1])();	 
	int	uu_sigmask[32 +1];	 

	int	(*uu_sigreturn)();







	int	uu_sigonstack;		 
	int	uu_sigintr;		 
	int	uu_oldmask;		 
	struct	sigstack uu_sigstack;	 

 
	struct	file *uu_ofile[64 ];	 
	char	uu_pofile[64 ];	 
	int	uu_lastfile;		 



	struct	inode *uu_cdir;		 
	struct	inode *uu_rdir;		 
	short	uu_cmask;		 

 
	struct	rusage uu_ru;		 
	struct	rusage uu_cru;		 
	struct	itimerval uu_timer[3];
	struct	timeval uu_start;
	short	uu_acflag;

	struct uuprof {			 
		simple_lock_data_t *pr_lock;	 
		short	*pr_base;	 
		unsigned pr_size;	 
		unsigned pr_off;	 
		unsigned pr_scale;	 
	} uu_prof;
	short	uu_aid;			 
	u_short	uu_maxuprc;		 
	u_char	uu_rpause;		 




	 
	char	uu_rfs;			 




 
	struct	rlimit uu_rlimit[6 ];
	struct	quota *uu_quota;	 
	int	uu_qflags;		 

	char	uu_modes;		 
	 
	struct  urmtWd {		 
	    dev_t dev;			 
	    long fid[3];		 
	} uu_rmtWd;
	struct  file *uu_textfile;	 
	dev_t	uu_rmt_dev;		 
	long	uu_rmt_pag;		 
};



struct	user {
	struct	pcb u_pcb;
	struct	proc *u_procp;		 
	int	*u_ar0;			 
	char	u_comm[16  + 1];

 
	 
	int	u_arg[8];		 
	int	*u_ap;			 
	label_t	u_qsave;		 
	union {				 
		struct	{
			int	R_val1;
			int	R_val2;
		} u_rv;


		off_t	r_off;
		time_t	r_time;
	} u_r;
	char	u_error;		 
	char	u_eosys;		 
	uid_t	u_VOPuid;		 

 




	struct identity *u_identity;	 

# 449 "../../sys/user.h"


 
	size_t	u_tsize;		 
	size_t	u_dsize;		 
	size_t	u_ssize;		 
	caddr_t	u_text_start;		 
	caddr_t	u_data_start;		 
	caddr_t	u_stack_start;		 
	caddr_t	u_stack_end;		 
	int	u_stack_grows_up;	 
	time_t	u_outime;		 

 
	int	(*u_signal[32 +1])();	 
	int	u_sigmask[32 +1];	 

	int	(*u_sigreturn)();







	int	u_sigonstack;		 
	int	u_sigintr;		 
	int	u_oldmask;		 
	int	u_code;			 
	struct	sigstack u_sigstack;	 



 
	struct	file *u_ofile[64 ];	 
	char	u_pofile[64 ];	 
	int	u_lastfile;		 


	struct	inode *u_cdir;		 
	struct	inode *u_rdir;		 


	short	u_cmask;		 

 
	struct	rusage u_ru;		 
	struct	rusage u_cru;		 
	struct	itimerval u_timer[3];
	int	u_XXX[3];
	struct	timeval u_start;
	short	u_acflag;

	struct uprof {			 
		short	*pr_base;	 
		unsigned pr_size;	 
		unsigned pr_off;	 
		unsigned pr_scale;	 
	} u_prof;
	short	u_aid;			 
	u_short	u_maxuprc;		 
	struct fs *u_rpsfs;		 
	char	u_rpswhich;		 





	u_char	u_rpause;		 




	char	u_modes;		 
	 
	char	u_rfs;			 



	short	u_rfscode;		 
	short	u_rfsncnt;		 
	 
	short	u_rmt_code;		 
	short	u_rmt_ncnt;		 
	struct  rmtWd {			 
	    dev_t dev;			 
	    long fid[3];		 
	} u_rmtWd;
	struct  file *u_textfile;	 
	char	u_rmt_requested;	 

	u_char u_rmt_follow1  ;	 


	u_char u_rmt_follow2  ;	 
	dev_t	u_rmt_dev;		 
	struct	buf *u_rmt_path_buf;	 

	char	*u_rmt_path;		 

	long	u_rmt_pag;		 
					 
	struct	rmt_saved_data *u_rmt_saved_data;

 
	struct	rlimit u_rlimit[6 ];
	struct	quota *u_quota;		 
	int	u_qflags;		 

 
	struct nameicache {		 
		int nc_prevoffset;	 
		ino_t nc_inumber;	 
		dev_t nc_dev;		 
		time_t nc_time;		 
	} u_ncache;
	struct	nameidata u_nd;

	int	u_stack[1];
};


 
struct identity {
	u_short	id_ref;			 
	uid_t	id_uid;			 
	uid_t	id_ruid;		 
	gid_t	id_gid;			 
	gid_t	id_rgid;		 
	gid_t	id_groups[16 ];	 
	long	id_pag;			 
};


extern struct identity *alloc_identity();
extern struct identity *change_identity();
extern struct identity *copy_identity();
extern struct identity *root_identity;


 




 
# 1 "../../sys/errno.h" 1
 







 


























































 











# 1 "mach_nfs.h" 1

# 80 "../../sys/errno.h" 2



 





































 



 





 

	 













	 













	 



 




 




 
 

















 





 









 



# 222 "../../sys/errno.h"


# 233 "../../sys/errno.h"



# 595 "../../sys/user.h" 2



# 1 "../../kern/thread.h" 1
 







 











































































































 










# 1 "cpus.h" 1

# 128 "../../kern/thread.h" 2

# 1 "mach_fixpri.h" 1

# 129 "../../kern/thread.h" 2

# 1 "mach_host.h" 1

# 130 "../../kern/thread.h" 2

# 1 "mach_km.h" 1

# 131 "../../kern/thread.h" 2


# 1 "../../mach/port.h" 1
 







 































































 












# 1 "mach_ipc_xxxhack.h" 1

# 86 "../../mach/port.h" 2



typedef int 		port_name_t;		 
typedef port_name_t	port_set_name_t;	 
typedef port_name_t	*port_name_array_t;

typedef int		port_type_t;		 
typedef port_type_t	*port_type_array_t;

	 











typedef	port_name_t	port_t;			 
typedef	port_t		port_rcv_t;		 
typedef	port_t		port_own_t;		 
typedef	port_t		port_all_t;		 
typedef	port_t		*port_array_t;










# 133 "../../kern/thread.h" 2

# 1 "../../mach/message.h" 1
 







 













































# 1 "mach_ipc_xxxhack.h" 1

# 55 "../../mach/message.h" 2





 







typedef	unsigned int	msg_size_t;

typedef	struct {
		unsigned int	msg_unused : 24,
				msg_simple : 8;
		msg_size_t	msg_size;
		int		msg_type;
		port_t		msg_local_port;
		port_t		msg_remote_port;
		int		msg_id;
} msg_header_t;



 











 

















typedef struct  {
	unsigned int	msg_type_name : 8,		 
			msg_type_size : 8,		 
			msg_type_number : 12,		 
			msg_type_inline : 1,		 
			msg_type_longform : 1,		 
			msg_type_deallocate : 1,	 
			msg_type_unused : 1;
} msg_type_t;

typedef	struct	{
	msg_type_t	msg_type_header;
	unsigned short	msg_type_long_name;
	unsigned short	msg_type_long_size;
	unsigned int	msg_type_long_number;
} msg_type_long_t;

 
























 












 











 



typedef	unsigned int	msg_timeout_t;

 






typedef	int		msg_option_t;










				 

 











 







typedef	int		msg_return_t;
















 
























 



msg_return_t	msg_send(





		);


msg_return_t	msg_receive(





		);


msg_return_t	msg_rpc(







		);



# 134 "../../kern/thread.h" 2

# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 135 "../../kern/thread.h" 2

# 1 "../../mach/machine/vm_types.h" 1

# 136 "../../kern/thread.h" 2

# 1 "../../kern/ast.h" 1
 





 



















 






 



















# 1 "cpus.h" 1

# 54 "../../kern/ast.h" 2

# 1 "hw_ast.h" 1

# 55 "../../kern/ast.h" 2


# 1 "../../machine/cpu.h" 1
# 1 "../../machine/../i386/cpu.h" 1
 





 













int cpu;

int master_cpu;
# 1 "../../machine/cpu.h" 2

# 57 "../../kern/ast.h" 2


 






 

 








extern int	need_ast[1 ];

 






 






 









 















# 137 "../../kern/thread.h" 2

# 1 "../../kern/queue.h" 1
 







 



















































 








# 1 "../../mach/machine/vm_types.h" 1

# 70 "../../kern/queue.h" 2




 


















 



struct queue_entry {
	struct queue_entry	*next;		 
	struct queue_entry	*prev;		 
};

typedef struct queue_entry	*queue_t;
typedef	struct queue_entry	queue_head_t;
typedef	struct queue_entry	queue_chain_t;
typedef	struct queue_entry	*queue_entry_t;

 








extern void		enqueue_head();
extern void		enqueue_tail();
extern queue_entry_t	dequeue_head();
extern queue_entry_t	dequeue_tail();
extern void		remqueue();

 









 









 







 










 









# 188 "../../kern/queue.h"

 








 






# 214 "../../kern/queue.h"

 





















 









# 264 "../../kern/queue.h"

 









 








 


















 


struct mpqueue_head {
	struct queue_entry	head;		 
	struct slock		lock;		 
};

typedef struct mpqueue_head	mpqueue_head_t;

















# 337 "../../kern/queue.h"

 



 













 





struct Queue
{
    struct Queue * F;
    struct Queue * B;
};








# 138 "../../kern/thread.h" 2

# 1 "../../kern/processor.h" 1
 





 


















































 






 



# 1 "cpus.h" 1

# 69 "../../kern/processor.h" 2

# 1 "mach_fixpri.h" 1

# 70 "../../kern/processor.h" 2


# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 72 "../../kern/processor.h" 2




# 1 "../../kern/sched.h" 1
 







 

































































 











# 1 "cpus.h" 1

# 87 "../../kern/sched.h" 2

# 1 "mach_fixpri.h" 1

# 88 "../../kern/sched.h" 2

# 1 "simple_clock.h" 1

# 89 "../../kern/sched.h" 2

# 1 "stat_time.h" 1

# 90 "../../kern/sched.h" 2












 















struct run_queue {
	queue_head_t		runq[32 ];	 
	 		 
	int			low;		 
	int			count;		 
};

typedef struct run_queue	*run_queue_t;


# 150 "../../kern/sched.h"









 



extern struct run_queue	*rem_runq();
extern struct thread	*choose_thread();
extern queue_head_t	action_queue;	 
 ;

extern int		min_quantum;	 

 





 





 




struct shift {
	int	shift1;
	int	shift2;
};

typedef	struct shift	*shift_t, shift_data_t;

 



extern unsigned	sched_tick;




 




# 220 "../../kern/sched.h"











# 76 "../../kern/processor.h" 2






struct processor_set {
	struct	run_queue	runq;		 
	queue_head_t		idle_queue;	 
	int			idle_count;	 
	simple_lock_data_t	idle_lock;	 
	queue_head_t		processors;	 
	int			processor_count;	 
	boolean_t		empty;		 
	queue_head_t		tasks;		 
	int			task_count;	 
	queue_head_t		threads;	 
	int			thread_count;	 
	int			ref_count;	 
	queue_chain_t		all_psets;	 
	boolean_t		active;		 
	simple_lock_data_t	lock;		 
	port_t			pset_self;	 
	port_t			pset_name_self;	 
	int			max_priority;	 



	int			set_quantum;	 





	long			mach_factor;	 
	long			load_average;	 
	long			sched_load;	 
};

typedef	struct processor_set *processor_set_t;



extern struct processor_set	default_pset;

struct processor {
	struct run_queue runq;		 
		 
	queue_chain_t	processor_queue;  
	int		state;		 
	struct thread	*next_thread;	 
	struct thread	*idle_thread;	 
	int		quantum;	 
	boolean_t	first_quantum;	 
	int		last_quantum;	 

	processor_set_t	processor_set;	 
	processor_set_t processor_set_next;	 
	queue_chain_t	processors;	 
	simple_lock_data_t	lock;
	port_t		processor_self;	 
	int		slot_num;	 



	 
};

typedef struct processor *processor_t;



extern struct processor	processor_array[1 ];

 


extern queue_head_t		all_psets;
extern int			all_psets_count;
 ;

 



extern processor_t	master_processor;

 












 

















 






extern processor_t	processor_ptr[1 ];






 




 







typedef port_t	*processor_array_t;
typedef port_t	*processor_set_name_array_t;


# 139 "../../kern/thread.h" 2

# 1 "../../kern/task.h" 1
 






 











































































































 










# 1 "mach_emulation.h" 1

# 127 "../../kern/task.h" 2

# 1 "mach_ipc_xxxhack.h" 1

# 128 "../../kern/task.h" 2

# 1 "mach_ipc_tcache.h" 1

# 129 "../../kern/task.h" 2


# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 131 "../../kern/task.h" 2


# 1 "../../mach/time_value.h" 1
 







 























 



struct time_value {
	long	seconds;
	long	microseconds;
};
typedef	struct time_value	time_value_t;

 























# 133 "../../kern/task.h" 2



# 1 "../../kern/mach_param.h" 1
 







 









 











# 1 "mach_np.h" 1

# 31 "../../kern/mach_param.h" 2


# 1 "../../mach/mach_param.h" 1
 







 





































































 

















# 33 "../../kern/mach_param.h" 2











					 


					 








# 136 "../../kern/task.h" 2

# 1 "../../kern/kern_obj.h" 1
 







 








































 








# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 59 "../../kern/kern_obj.h" 2


# 1 "../../kern/zalloc.h" 1
 







 










































































 









# 1 "../../mach/machine/vm_types.h" 1

# 94 "../../kern/zalloc.h" 2





 







typedef struct zone {
	 	 
	int		count;		 
	vm_offset_t	free_elements;
	vm_size_t	cur_size;	 
	vm_size_t	max_size;	 
	vm_size_t	elem_size;	 
	vm_size_t	alloc_size;	 
	boolean_t	doing_alloc;	 
	char		*zone_name;	 
	unsigned int
	 	pageable :1,	 
	 	sleepable :1,	 
	  exhaustible :1;	 
	lock_data_t	complex_lock;	 
	struct zone *	next_zone;	 
} *zone_t;



extern vm_offset_t	zalloc();
extern vm_offset_t	zget();
extern zone_t		zinit();
extern void		zfree();
extern void		zchange();

























# 166 "../../kern/zalloc.h"










extern void		zcram();
extern void		zone_bootstrap();
extern void		zone_init();


# 61 "../../kern/kern_obj.h" 2


# 1 "../../kern/assert.h" 1
 







 











































 

 






# 1 "mach_assert.h" 1

# 62 "../../kern/assert.h" 2




# 82 "../../kern/assert.h"






# 63 "../../kern/kern_obj.h" 2



typedef struct kern_obj {
	 
	boolean_t obj_in_use;
	int obj_references;
	zone_t obj_home_zone;
	queue_head_t obj_translations;
} *kern_obj_t;


















































# 137 "../../kern/task.h" 2

# 1 "../../kern/kern_set.h" 1
 







 




























 








# 1 "../../mach/kern_return.h" 1
 








 












































 











# 1 "../../mach/machine/kern_return.h" 1
# 1 "../../mach/machine/../i386/kern_return.h" 1
 





 















 












typedef	int		kern_return_t;


# 1 "../../mach/machine/kern_return.h" 2

# 67 "../../mach/kern_return.h" 2





		 



		 




		 





		 




		 



		 




		 




		 



		 





		 







		 



		 



		 



		 



 




		 





# 47 "../../kern/kern_set.h" 2


# 1 "../../kern/msg_queue.h" 1
 







 











































 








typedef struct {
	queue_head_t messages;
	 
	queue_head_t blocked_threads;
} msg_queue_t;












# 49 "../../kern/kern_set.h" 2



typedef struct kern_set {
	struct kern_obj set_obj;

	struct task *set_owner;	 
	port_name_t set_local_name;

	msg_queue_t set_messages;
	queue_head_t set_members;
	struct kern_port *set_traversal;  
} *kern_set_t;


















extern void set_reference();
extern void set_release();
extern kern_return_t set_alloc();
extern void set_destroy();
extern void set_add_member();
extern void set_remove_member();


# 138 "../../kern/task.h" 2


# 1 "../../kern/syscall_emulation.h" 1
 







 


















 







typedef	 unsigned long 	eml_routine_t;

typedef struct eml_dispatch {
	int		eml_ref;	 
	int 		disp_count; 	 
	eml_routine_t	disp_vector[1];	 
					 
} *eml_dispatch_t;

typedef struct syscall_val {
	int	rv_val1;
	int	rv_val2;
} syscall_val_t;















# 140 "../../kern/task.h" 2

# 1 "../../vm/vm_map.h" 1
 







 


















































 















# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 76 "../../vm/vm_map.h" 2

# 1 "../../mach/machine/vm_types.h" 1

# 77 "../../vm/vm_map.h" 2

# 1 "../../mach/vm_prot.h" 1
 







 












 











 





typedef int		vm_prot_t;

 









 





 






# 78 "../../vm/vm_map.h" 2

# 1 "../../mach/vm_inherit.h" 1
 







 












 











 





typedef int		vm_inherit_t;	 

 











# 79 "../../vm/vm_map.h" 2

# 1 "../../mach/vm_statistics.h" 1
 







 





























 












struct vm_statistics {
	long	pagesize;		 
	long	free_count;		 
	long	active_count;		 
	long	inactive_count;		 
	long	wire_count;		 
	long	zero_fill_count;	 
	long	reactivations;		 
	long	pageins;		 
	long	pageouts;		 
	long	faults;			 
	long	cow_faults;		 
	long	lookups;		 
	long	hits;			 
};

typedef struct vm_statistics	*vm_statistics_t;
typedef struct vm_statistics	vm_statistics_data_t;


extern vm_statistics_data_t	vm_stat;


 






struct pmap_statistics {
	long		resident_count;	 
	long		wired_count;	 
};

typedef struct pmap_statistics	*pmap_statistics_t;


# 80 "../../vm/vm_map.h" 2

# 1 "../../vm/pmap.h" 1
 







 





























 












# 1 "../../machine/pmap.h" 1
# 1 "../../machine/../i386/pmap.h" 1
 






 














































 














# 1 "../../i386/seg.h" 1
 





 




















 



  
















 










 












 














 

















struct fakedesc {
	unsigned long f_base;		 
	unsigned long f_limacc;		 
};

 



struct fakegate {
	unsigned long  f_off;		 
	unsigned short f_sel;		 
	unsigned char  f_wcount;	 
	unsigned char  f_type;		 
};

 



struct desc {
	unsigned int	d_lim0015:16,
	 		d_base0015:16,
	  		d_base1623:8,
			d_acc0007:8,
			d_lim1619:4,
			d_acc0811:4,
			d_base2431:8;
};

 



struct gate {
	unsigned int	g_off0015:16,
			g_select:16,
			g_wordc:8,
			g_acc:8,
			g_off1631:16;
};

			

 







 



 



 



 











 





# 70 "../../machine/../i386/pmap.h" 2

# 1 "../../mach/i386/vm_param.h" 1
 






 




















 



















 







 
























 




 




 




 








 








# 71 "../../machine/../i386/pmap.h" 2



 



 






struct pt_entry {
	unsigned int
			valid:1,	 
			prot:2,		 
			:2,		 
			ref:1,		 
			modify:1,	 
			:2,		 

			wired:1,	 
			sw_valid:1,	 
			:1,		 
			pfn:20;		 
};

typedef struct pt_entry	pt_entry_t;







 









 














typedef	long		cpu_set;	 

struct pmap {
	pt_entry_t	*cr3;		 
	int		ref_count;	 
	struct fakedesc	ldt[0x07 ];	 
	simple_lock_data_t
			lock;		 
	struct pmap_statistics	stats;	 
	cpu_set		cpus_using;	 
};

typedef struct pmap	*pmap_t;



# 177 "../../machine/../i386/pmap.h"


 



pt_entry_t	*pmap_pte();
vm_offset_t	pmap_map_bd();		 
 



# 290 "../../machine/../i386/pmap.h"


 





# 307 "../../machine/../i386/pmap.h"







 

















# 1 "../../machine/pmap.h" 2

# 52 "../../vm/pmap.h" 2

# 1 "../../mach/machine/vm_types.h" 1

# 53 "../../vm/pmap.h" 2

# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 54 "../../vm/pmap.h" 2


 











 



extern void		pmap_bootstrap();	 


extern void		pmap_init();		 



extern vm_offset_t	pmap_map();		 



 


extern pmap_t		pmap_create();		 

extern pmap_t		pmap_kernel();		 

extern void		pmap_reference();	 
extern void		pmap_destroy();		 

extern void		pmap_enter();		 

 


extern void		pmap_remove();		 
extern void		pmap_protect();		 

 


extern void		pmap_activate();	 


extern void		pmap_deactivate();	 




 


extern void		pmap_page_protect();	 
extern boolean_t	pmap_valid_page();	 






 




extern void		pmap_clear_reference();	 

extern boolean_t	pmap_is_referenced();	 

extern void		pmap_clear_modify();	 
extern boolean_t	pmap_is_modified();	 


 


extern void		pmap_statistics();	 





 


extern vm_offset_t	pmap_extract();		 


extern boolean_t	pmap_access();		 


extern void		pmap_update();		 

extern void		pmap_collect();		 



extern void		pmap_change_wiring();	 










# 179 "../../vm/pmap.h"


 



extern void		pmap_copy();		 





 



extern pmap_t	kernel_pmap;			 


# 81 "../../vm/vm_map.h" 2

# 1 "../../vm/vm_object.h" 1
 







 








































 











# 1 "mach_pagemap.h" 1

# 62 "../../vm/vm_object.h" 2



# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 65 "../../vm/vm_object.h" 2

# 1 "../../mach/memory_object.h" 1
 






 
















 









 






typedef	port_t		memory_object_t;
					 
					 
					 
					 

typedef	port_t		memory_object_control_t;
					 
					 

typedef	port_t		memory_object_name_t;
					 
					 

typedef	int		memory_object_copy_strategy_t;
					 

					 

					 

					 
					 




 








extern memory_object_t	memory_manager_default;


# 66 "../../vm/vm_object.h" 2


# 1 "../../mach/machine/vm_types.h" 1

# 68 "../../vm/vm_object.h" 2










 





struct vm_object {
	queue_chain_t		memq;		 
	 		 




	vm_size_t		size;		 



	short			ref_count;	 
	short			resident_page_count;
						 

	struct vm_object	*copy;		 


	struct vm_object	*shadow;	 
	vm_offset_t		shadow_offset;	 

	memory_object_t		pager;		 
	vm_offset_t		paging_offset;	 
	memory_object_control_t	pager_request;	 
	memory_object_name_t	pager_name;	 

	unsigned int
	 		pager_created:1, 
	 		pager_initialized:1, 
	 		pager_ready:1;	 

	memory_object_copy_strategy_t
				copy_strategy;	 

	unsigned int
				absent_count;	 







	unsigned int  
				all_wanted;	 





	unsigned int
				paging_in_progress:16,
						 





	 		can_persist:1,	 





	 		internal:1,	 



	 		temporary:1,	 






	 		alive:1,	 
	 		lock_in_progress : 1,
						 


	 		lock_restart : 1,
						 


	 		single_use:1;	 




	queue_chain_t		cached_list;	 



	vm_offset_t		last_alloc;	 



};

typedef struct vm_object	*vm_object_t;


extern
vm_object_t	kernel_object;		 

 



extern void		vm_object_init ();
extern void		vm_object_terminate();
extern vm_object_t	vm_object_allocate();
extern void		vm_object_reference();
extern void		vm_object_deallocate();
extern void		vm_object_pmap_protect();
extern void		vm_object_pmap_remove();
extern void		vm_object_page_remove();
extern void		vm_object_shadow();
extern void		vm_object_collapse();
extern vm_object_t	vm_object_lookup();
extern port_t		vm_object_name();
extern void		vm_object_remove();

extern boolean_t	vm_object_copy_temporary();
extern kern_return_t	vm_object_copy_strategically();
extern kern_return_t	vm_object_copy_slowly();

extern vm_object_t	vm_object_enter();
extern void		vm_object_pager_create();
extern void		vm_object_destroy();

extern void		vm_object_print();

extern vm_object_t	vm_object_request_object();

 






























 
















# 279 "../../vm/vm_object.h"
















 



# 317 "../../vm/vm_object.h"











# 82 "../../vm/vm_map.h" 2




 









 







typedef union vm_map_object {
	struct vm_object	*vm_object;	 
	struct vm_map		*share_map;	 
	struct vm_map		*sub_map;	 
} vm_map_object_t;

 












struct vm_map_links {
	struct vm_map_entry	*prev;		 
	struct vm_map_entry	*next;		 
	vm_offset_t		start;		 
	vm_offset_t		end;		 
};

struct vm_map_entry {
	struct vm_map_links	links;		 




	union vm_map_object	object;		 
	vm_offset_t		offset;		 
	unsigned int
	 		is_a_map:1,	 
	 		is_sub_map:1,	 
		 
	 		copy_on_write:1, 
	 		needs_copy:1,	 

		 
	 		single_use:1;	 

		 
	vm_prot_t		protection;	 
	vm_prot_t		max_protection;	 
	vm_inherit_t		inheritance;	 
	short			wired_count;	 
	short			user_wired_count;  
};

typedef struct vm_map_entry	*vm_map_entry_t;



 



















typedef struct vm_map {
	lock_data_t		lock;		 
	struct vm_map_links	links;		 


	int			nentries;	 
	pmap_t			pmap;		 
	vm_size_t		size;		 
	boolean_t		is_main_map;	 
	int			ref_count;	 
	 	 
	vm_map_entry_t		hint;		 
	 	 
	vm_map_entry_t		first_free;	 
	boolean_t		entries_pageable;  
	boolean_t		wait_for_space;	 
	unsigned int		timestamp;	 
	vm_statistics_data_t	vm_stat;
} *vm_map_t;







 


















typedef struct {
	unsigned int	main_timestamp;
	vm_map_t	share_map;
	unsigned int	share_timestamp;
} vm_map_version_t;

 






















typedef struct vm_map_copy {
	struct vm_map_links	links;
	unsigned int		nentries;
	boolean_t		entries_pageable;
} *vm_map_copy_t;










 





























 



extern void		vm_map_init();		 

extern vm_map_t		vm_map_create();	 
extern vm_map_t		vm_map_fork();		 


extern void		vm_map_reference();	 

extern void		vm_map_deallocate();	 

extern kern_return_t	vm_map_enter();		 
extern kern_return_t	vm_map_remove();	 
extern kern_return_t	vm_map_protect();	 
extern kern_return_t	vm_map_inherit();	 

extern kern_return_t	vm_map_find();		 
extern void		vm_map_print();		 

extern kern_return_t	vm_map_lookup();	 
extern boolean_t	vm_map_verify();	 

extern void		vm_map_verify_done();	 



extern kern_return_t	vm_map_copyin();	 
extern kern_return_t	vm_map_copyout();	 
extern kern_return_t	vm_map_copy_overwrite(); 

extern void		vm_map_copy_discard();	 


 



						 



						 


						 


 


extern kern_return_t	vm_map_pageable_common();







 



extern vm_object_t	vm_submap_object;


# 141 "../../kern/task.h" 2


struct task {
	 
	 	 
	int		ref_count;	 
	boolean_t	active;		 

	 
	vm_map_t	map;		 
	queue_chain_t	pset_tasks;	 
	int		suspend_count;	 

	 
	queue_head_t	thread_list;	 
	int		thread_count;	 
	   
	processor_set_t	processor_set;	 
	boolean_t	may_assign;	 
	boolean_t	assign_active;	 

	 
	struct utask	*u_address;
	int		proc_index;	 

	 
	int		user_stop_count;	 
	int		priority;		 

	 
	boolean_t	kernel_ipc_space;  
	boolean_t	kernel_vm_space;  

	 
	time_value_t	total_user_time;
				 
	time_value_t	total_system_time;
				 

	 
	port_t		task_self;	 
	port_t		task_tself;	 
	port_t		task_notify;	 
	port_t		exception_port;	 
	port_t		bootstrap_port;	 

	 
	boolean_t	ipc_privilege;	 
	 
	queue_head_t	ipc_translations;  
	boolean_t	ipc_active;	 
	port_name_t	ipc_next_name;	 

	kern_set_t	ipc_enabled;	 






	struct {
		port_name_t	name;
		kern_obj_t	object;
	}		obj_cache[010 ];
					 


	 
	boolean_t	ipc_intr_msg;	 
	port_t		ipc_ports_registered[4 ];





};

typedef struct task *task_t;



typedef	port_t	*task_array_t;







 



extern kern_return_t	task_create();
extern kern_return_t	task_terminate();
extern kern_return_t	task_suspend();
extern kern_return_t	task_resume();
extern kern_return_t	task_threads();
extern kern_return_t	task_ports();
extern kern_return_t	task_info();
extern kern_return_t	task_get_special_port();
extern kern_return_t	task_set_special_port();
extern kern_return_t	task_assign();
extern kern_return_t	task_assign_default();

 



extern void		task_init();
extern void		task_reference();
extern void		task_deallocate();
extern kern_return_t	task_hold();
extern kern_return_t	task_dowait();
extern kern_return_t	task_release();
extern kern_return_t	task_halt();

extern kern_return_t	task_suspend_nowait();

extern task_t	kernel_task;


# 140 "../../kern/thread.h" 2

# 1 "../../kern/timer.h" 1
 







 














































 




# 1 "cpus.h" 1

# 61 "../../kern/timer.h" 2

# 1 "stat_time.h" 1

# 62 "../../kern/timer.h" 2





 


















 




struct timer {
	unsigned	low_bits;
	unsigned	high_bits;
	unsigned	high_bits_check;
	unsigned	tstamp;
};

typedef struct timer		timer_data_t;
typedef	struct timer		*timer_t;

 





 



extern timer_t		current_timer[1 ];
extern timer_data_t	kernel_timer[1 ];

 




struct timer_save {
	unsigned	low;
	unsigned	high;
};

typedef struct timer_save	timer_save_data_t, *timer_save_t;

 











extern void		timer_read();
extern void		thread_read_times();
extern unsigned		timer_delta();


 

	








# 162 "../../kern/timer.h"


 










# 187 "../../kern/timer.h"


# 141 "../../kern/thread.h" 2



# 1 "../../kern/thread_modes.h" 1
 







 











 

 








 







# 144 "../../kern/thread.h" 2

# 1 "../../kern/kern_msg.h" 1
 







 






















































 








# 1 "mach_np.h" 1

# 73 "../../kern/kern_msg.h" 2






# 1 "../../kern/ipc_netport.h" 1
 







 









































 








# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 60 "../../kern/ipc_netport.h" 2


# 1 "../../mach/netport.h" 1
 







 










typedef unsigned long	netaddr_t;

 


typedef struct {
    long	np_uid_high;
    long	np_uid_low;
} np_uid_t;

typedef struct {
    netaddr_t	np_receiver;
    netaddr_t	np_owner;
    np_uid_t	np_puid;
    np_uid_t	np_sid;
} network_port_t;


# 62 "../../kern/ipc_netport.h" 2


 


extern int		np_flags;





 







 

















typedef struct {
    unsigned short	disp_type;		 
    unsigned short	src_format;		 
} disp_hdr_t, *disp_hdr_ptr_t;

 

















 


























typedef network_port_t *network_port_ptr_t;

typedef union {
    struct {
	unsigned char ia_net_owner;
	unsigned char ia_net_node_type;
	unsigned char ia_host_high;
	unsigned char ia_host_low;
    } ia_bytes;
    netaddr_t ia_netaddr;
} ip_addr_t;

 


typedef struct {
	disp_hdr_t      disp_hdr;	 
	network_port_t  local_port;
	network_port_t  remote_port;
	unsigned long   info;		 
	unsigned long   npd_size;	 
	unsigned long   inline_size;	 
	unsigned long   ool_size;	 
	unsigned long   ool_num;	 
	unsigned long	ipc_seq_no;	 
}               ipc_network_hdr_t;

 


typedef struct {
	disp_hdr_t      disp_hdr;	 
	network_port_t  local_port;
	network_port_t  remote_port;
	unsigned long   info;		 
	unsigned long   npd_size;	 
	unsigned long   inline_size;	 
	unsigned long   ool_size;	 
	unsigned long   ool_num;	 
	unsigned long	ipc_seq_no;	 
	msg_header_t	kmsg_header;
}               ipc_netmsg_hdr_t;

 





 









 







typedef struct tcp_ctl {
	int		ctl;
	unsigned long	trid;
	int		code;
	unsigned long	size;
	int		crypt_level;
} tcp_ctl_t, *tcp_ctl_ptr_t;



 





typedef struct {
    int		(*sendrequest)();
    int		(*sendreply)();
} transport_sw_entry_t;
extern transport_sw_entry_t transport_switch[2 ];


 


typedef struct {
	unsigned long	v1;
	unsigned long	v2;
} trid_t;


# 79 "../../kern/kern_msg.h" 2


typedef struct kern_msg {
		queue_chain_t		queue_head;	 
		struct port_hash *	sender_entry;
		zone_t			home_zone;
		boolean_t		kernel_message;
						 






		msg_header_t		kmsg_header;
} *kern_msg_t;




# 145 "../../kern/thread.h" 2

# 1 "../../machine/cpu.h" 1
# 1 "../../machine/../i386/cpu.h" 1
 





 













int cpu;

int master_cpu;
# 1 "../../machine/cpu.h" 2

# 146 "../../kern/thread.h" 2

# 1 "../../machine/thread.h" 1
# 1 "../../machine/../i386/thread.h" 1
 





 































 















extern	struct	u_address	U_ADDRESS;




# 1 "../../machine/thread.h" 2

# 147 "../../kern/thread.h" 2

# 1 "../../machine/pcb.h" 1

# 148 "../../kern/thread.h" 2







struct thread {
	 
	queue_chain_t	links;		 
	run_queue_t	runq;		 
 






	 
	task_t		task;		 
	queue_chain_t	thread_list;	 

	 
	queue_chain_t	pset_threads;	 


	 
	 
	int		ref_count;	 

	 
	struct pcb	*pcb;		 
	vm_offset_t	kernel_stack;	 

	 
	int		wait_event;	 
	int		suspend_count;	 
	boolean_t	interruptible;	 
	kern_return_t	wait_result;	 
	boolean_t	timer_set;	 
	boolean_t	wake_active;
	int		swap_state;	 
	int		state;		 
 








	 
	int		priority;	 
	int		max_priority;	 
	int		sched_pri;	 




	int		depress_priority;  
	unsigned int	cpu_usage;	 
	unsigned int	sched_usage;	 
	unsigned int	sched_stamp;	 

	 

	vm_offset_t	recover;	 
	boolean_t	vm_privilege;	 
	vm_offset_t	tmp_address;	 

	vm_object_t	tmp_object;	 

	

	 
	struct u_address {
		struct uthread	*uthread;
		struct utask	*utask;
	} u_address;
	int		unix_lock;	 

	 
	int		user_stop_count;	 

	 
	   
	port_t		thread_self;	 
	port_t		thread_tself;	 
	port_t		thread_reply;	 
	queue_chain_t	ipc_wait_queue;	 
	msg_return_t	ipc_state;	 
	union {
	msg_size_t	msize;		 
	kern_msg_t	kmsg;		 
	} ipc_data;
	 
	boolean_t	ipc_kernel;	 
	port_name_t	reply_port;	 
	int		messages_sent;	 
	int		messages_received;

	 
	timer_data_t	user_timer;	 
	timer_data_t	system_timer;	 
	timer_save_data_t user_timer_save;   
	timer_save_data_t system_timer_save;   
	unsigned int	cpu_delta;	 
	unsigned int	sched_delta;	 

	 
	port_t		exception_port;
	port_t		exception_clear_port;

	 
	boolean_t	active;		 
	boolean_t	halted;		 
	int		ast;    	 

	 
	processor_set_t	processor_set;	 
	processor_t	bound_processor;	 















};


typedef struct thread *thread_t;



typedef	port_t	*thread_array_t;	 


extern thread_t active_threads[];	 

 



extern kern_return_t	thread_create();
extern kern_return_t	thread_terminate();
extern kern_return_t	thread_suspend();
extern kern_return_t	thread_resume();
extern kern_return_t	thread_abort();
extern kern_return_t	thread_get_state();
extern kern_return_t	thread_set_state();
extern kern_return_t	thread_get_special_port();
extern kern_return_t	thread_set_special_port();
extern kern_return_t	thread_info();
extern kern_return_t	thread_assign();
extern kern_return_t	thread_assign_default();

 



extern void		thread_init();
extern void		thread_reference();
extern void		thread_deallocate();
extern void		thread_hold();
extern kern_return_t	thread_dowait();
extern void		thread_release();
extern void		thread_swappable();
extern kern_return_t	thread_halt();
extern void		thread_halt_self();
extern thread_t		kernel_thread();

extern void		reaper_thread();







 














 










# 598 "../../sys/user.h" 2


# 611 "../../sys/user.h"




















































































# 708 "../../sys/user.h"



















# 52 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/map.h" 1
 







 






























 







 


























struct map {
	struct	mapent *m_limit;	 
	char	*m_name;		 
 




};
struct mapent
{
	int	m_size;		 
	int	m_addr;		 
};




extern vm_map_t	kernel_pageable_map;
extern vm_map_t	mb_map;







# 53 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/proc.h" 1
 







 








































































































































































































 










# 1 "../../mach/boolean.h" 1
 







 

























 






# 52 "../../mach/boolean.h"


 

















# 221 "../../sys/proc.h" 2














 







struct	proc {
	struct	proc *p_link;	 
	struct	proc *p_rlink;
	struct	proc *p_nxt;	 
	struct	proc **p_prev;		 





	char	p_usrpri;	 
	char	p_pri;		 
	char	p_cpu;		 

	char	p_stat;
	char	p_time;		 



	char	p_nice;		 

	char	p_slptime;	 
	char	p_cursig;
	int	p_sig;		 
	int	p_sigmask;	 
	int	p_sigignore;	 
	int	p_sigcatch;	 
	int	p_flag;
	uid_t	p_uid;		 
	short	p_pgrp;		 
	short	p_pid;		 
	short	p_ppid;		 
	u_short	p_xstat;	 
	struct	rusage *p_ru;	 
	size_t 	p_rssize; 	 
	size_t	p_maxrss;	 
	size_t	p_swrss;	 
	swblk_t	p_swaddr;	 
	int	p_stopsig;	 
	short	p_cpticks;	 
	long	p_pctcpu;	 
	short	p_ndx;		 
	short	p_idhash;	 
	struct	proc *p_pptr;	 
	struct	proc *p_cptr;	 
	struct	proc *p_osptr;	 
	struct	proc *p_ysptr;	 
	struct	itimerval p_realtimer;
	struct	quota *p_quota;	 
	dev_t	    p_logdev;	 
	dev_t       p_ttyd;	 
	struct tty *p_ttyp;	 
	struct task	*task;	 
	struct utask	*utask;  
	struct thread	*thread; 
	int	p_rmt_seq;	 



	simple_lock_data_t siglock;	 
	boolean_t	sigwait;	 
	struct thread	*exit_thread;	 






};




 




















 





# 372 "../../sys/proc.h"

 





















 



















extern short	pidhash[64 ];
extern struct	proc *pfind();
extern struct	proc *proc, *procNPROC;	 
extern struct	proc *freeproc, *zombproc, *allproc;
			 
extern int	nproc;


extern struct	prochd {
	struct	proc *ph_link;	 
	struct	proc *ph_rlink;
} qs[32 ];




 
 









 



















 











# 54 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/mbuf.h" 1
 







 

























































 












 












 



 



 


 


struct mbuf {
	struct	mbuf *m_next;		 
	u_long	m_off;			 
	short	m_len;			 
	short	m_type;			 
	union {
		u_char	mun_dat[(128 - 12 - 4 ) ];	 
		struct {
			short	mun_cltype;	 
			int	(*mun_clfun)();
			int	mun_clarg;
			int	(*mun_clswp)();
		} mun_cl;
	} m_un;
	struct	mbuf *m_act;		 
};







 



















 



 




 





 





 


struct mbstat {
	short	m_mbufs;	 
	short	m_clusters;	 
	short	m_clfree;	 
	short	m_drops;	 
	short	m_mtypes[256];	 
	short	m_space;
	short	m_drain;
	short	m_wait;
};





# 203 "../../sys/mbuf.h"

 









# 227 "../../sys/mbuf.h"





# 241 "../../sys/mbuf.h"


# 253 "../../sys/mbuf.h"


# 276 "../../sys/mbuf.h"

extern struct	mbuf *mbutl, *embutl;	 
extern struct	mbstat mbstat;
extern int	nmbclusters;
extern struct	mbuf *mfree, *mclfree;
extern char	mclrefcnt[512  + 1];
extern int	m_want;

extern struct	mbuf *m_get();
extern struct	mbuf *m_getclr();
extern struct	mbuf *m_free();
extern struct	mbuf *m_more();
extern struct	mbuf *m_copy();
extern struct	mbuf *m_pullup();
extern caddr_t	m_clalloc();


# 55 "../../machine/../i386/genassym.c" 2

# 1 "../../sys/msgbuf.h" 1
 





 
















 













struct	msgbuf {
	long	msg_magic;
	long	msg_bufx;
	long	msg_bufr;
	char	msg_bufc[(4096 - 3 * sizeof (long)) ];
};









extern struct	msgbuf msgbuf;



# 56 "../../machine/../i386/genassym.c" 2




# 1 "../../machine/pcb.h" 1

# 60 "../../machine/../i386/genassym.c" 2






thread_t	active_threads[1];

main()
{
	register struct proc *p = (struct proc *)0;
	register struct utask *utask = (struct utask *)0;
	register struct uthread *uthread = (struct uthread *)0;
	register struct pcb *pcb = (struct pcb *)0;
	register struct rusage *rup = (struct rusage *)0;
	struct rpb *rp = (struct rpb *)0;
	struct thread *thread = (struct thread *) 0;
	struct task *task = (struct task *) 0;
	struct uuprof *uprof = (struct uuprof *)0;




	printf("#ifdef LOCORE\n");
	printf("#define\tP_LINK %d\n", &p->p_link);
	printf("#define\tP_RLINK %d\n", &p->p_rlink);
	printf("#define\tP_PRI %d\n", &p->p_pri);
	printf("#define\tP_STAT %d\n", &p->p_stat);
	printf("#define\tP_CURSIG %d\n", &p->p_cursig);
	printf("#define\tP_SIG %d\n", &p->p_sig);
	printf("#define\tP_FLAG %d\n", &p->p_flag);
	printf("#define\tSSLEEP %d\n", 1 );
	printf("#define\tSRUN %d\n", 3 );
	printf("#define\tUPAGES %d\n", 16 );
	printf("#define\tU_PROCP %d\n", &utask->uu_procp);
	printf("#define\tU_RU %d\n", &utask->uu_ru);
	printf("#define\tRU_MINFLT %d\n", &rup->ru_minflt);
	printf("#define\tPR_BASE %d\n", &uprof->pr_base);
	printf("#define\tPR_SIZE %d\n", &uprof->pr_size);
	printf("#define\tPR_OFF %d\n", &uprof->pr_off);
	printf("#define\tPR_SCALE %d\n", &uprof->pr_scale);
	printf("#define\tU_ERROR %d\n", &uthread->uu_error);
	printf("#define\tU_AR0 %d\n", &uthread->uu_ar0);
	printf("#define\tpcb_fpvalid %d\n", &pcb->pcb_fpvalid);
	printf("#define\tpcb_fps %d\n", &pcb->pcb_fps);
	printf("#define\tUSER_FP 0x%x\n", 0x2f );
	printf("#define\tKDSSEL 0x%x\n", 0x160 );
	printf("#define\tKTSSSEL 0x%x\n", 0x150 );
	printf("#define\tJTSSSEL 0x%x\n", 0x170 );
	printf("#define\tTHREAD_PCB %d\n", &thread->pcb);
	printf("#define\tTHREAD_RECOVER %d\n", &thread->recover);
	printf("#define\tTHREAD_TASK %d\n", &thread->task);




	printf("#define\tUTHREAD %d\n", &thread->u_address.uthread);
	printf("#define\tUTASK %d\n", &thread->u_address.utask);





	printf("#else\n");
	printf("asm(\".set\tU_ARG,%d\");\n", uthread->uu_arg);
	printf("asm(\".set\tU_QSAVE,%d\");\n", &uthread->uu_qsave);
	printf("#endif\n");
	exit(0);
}
# 1 "../../machine/genassym.c" 2

