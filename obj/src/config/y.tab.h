
typedef union  {
	char	*str;
	int	val;
	struct	file_list *file;
	struct	idlst *lst;
} YYSTYPE;
extern YYSTYPE yylval;
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
