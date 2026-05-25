set architecture i386
target remote localhost:1234
symbol-file vmunix.sys
break ../../bsd/subr_prf.c:887
break high_start
continue