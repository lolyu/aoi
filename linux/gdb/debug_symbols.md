# debug symbols

## how to use debug file for linkmgrd
```
(gdb) exec-file usr/sbin/linkmgrd
(gdb) symbol-file usr/
lib/   sbin/  share/
(gdb) symbol-file usr/lib/debug/.build-id/7a/19e9cd44f81ddb0bbc3eab757319675ae606de.debug
Reading symbols from usr/lib/debug/.build-id/7a/19e9cd44f81ddb0bbc3eab757319675ae606de.debug...
```


## references
* https://wiki.ubuntu.com/Debug%20Symbol%20Packages
* https://developers.redhat.com/articles/2022/01/10/gdb-developers-gnu-debugger-tutorial-part-2-all-about-debuginfo
* https://blog.csdn.net/nirendao/article/details/104107608
* https://stackoverflow.com/questions/866721/how-to-generate-gcc-debug-symbol-outside-the-build-target
* https://sourceware.org/gdb/onlinedocs/gdb/Separate-Debug-Files.html
