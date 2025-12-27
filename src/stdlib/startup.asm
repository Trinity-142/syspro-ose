[BITS 32]
[EXTERN main]
[EXTERN exit]
[GLOBAL startup]
startup:
    sub esp, 8
    push ecx
    push eax
    call main
    push eax
    call exit
