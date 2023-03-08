SYSWRITE = 4
.global mywrite, myadd
.text
mywrite:
    pushl   %ebp
    movl    %esp, %ebp
    pushl   %ebx
    movl    8(%ebp), %ebx
    movl    12(%ebp), %ecx
    movl    16(%ebp), %edx
    movl    $SYSWRITE, %eax
    int     $0x80
    popl    %ebx
    movl    %ebp, %esp
    popl    %ebp
    ret
myadd:
    pushl   %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %eax
    movl    12(%ebp), %edx
    xorl    %ecx, %ecx
    addl    %eax, %edx
    jo      1f
    movl    16(%ebp), %eax
    movl    %edx, (%eax)
    incl    %ecx
1:  movl    %ecx, %eax
    movl    %ebp, %esp
    popl    %ebp
    ret
