section .text

extern HuntSyscall
global hookedCallback

hookedCallback:
    push rcx
    push rdx
    mov rdx, [r10-0x10]
    call HuntSyscall
    pop rdx
    pop rcx
    ret