# gdb basics

## gdb startup scripts
* `gdb` starts up and read script files:
    * `/etc/gdb/gdbinit`
    * `$HOME/.gdbinit
    * `./.gdbinit`

## gdb help && apropos
```
(gdb) apropos symbol-file
add-symbol-file -- Load symbols from FILE, assuming FILE has been dynamically loaded.
add-symbol-file-from-memory -- Load the symbols out of memory from a dynamically loaded object file.
remove-symbol-file -- Remove a symbol file added via the add-symbol-file command.
set print symbol-filename -- Set printing of source filename and line number with <SYMBOL>.
show print symbol-filename -- Show printing of source filename and line number with <SYMBOL>.
symbol-file -- Load symbol table from executable file FILE.
(gdb) help symbol-file
Load symbol table from executable file FILE.
Usage: symbol-file [-readnow | -readnever] [-o OFF] FILE
...
(gdb) help add-symbol-file
Load symbols from FILE, assuming FILE has been dynamically loaded.
Usage: add-symbol-file FILE [-readnow | -readnever] [-o OFF] [ADDR] [-s SECT-NAME SECT-ADDR]...
...
```

## run with args
```
# gdb --args linkmgrd-test --gtest_filter="*InitializeSendBuffer"
```

## run with args and GDB commands
```
# gdb -ex="set confirm off" -ex=r -ex=bt -ex=q --args linkmgrd-test --gtest_filter="MuxPortTest*"
```

## references
* https://developers.redhat.com/blog/2021/04/30/the-gdb-developers-gnu-debugger-tutorial-part-1-getting-started-with-the-debugger#why_another_gdb_tutorial_
