set architecture i386
target remote localhost:1234
symbol-file vmunix.sys
break _panic
break panic
continue