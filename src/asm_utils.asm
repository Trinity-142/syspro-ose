[GLOBAL cli]
cli:
    cli
    ret

[GLOBAL endless_loop]
endless_loop:
    jmp endless_loop

[GLOBAL lidt]
lidt:
    mov eax, [esp+4]
    lidt [eax]
    ret

[GLOBAL set_gprs]
set_gprs:
    mov eax, 0
    mov ecx, 1
    mov edx, 2
	mov ebx, 3
	mov ebp, 4
	mov esi, 5
	mov edi, 6
	ret

[GLOBAL division_by_zero]
division_by_zero:
    div eax
    ret

[GLOBAL syscall]
syscall:
    int 142
    ret

[GLOBAL sti]
sti:
    sti
    ret

[EXTERN universal_handler]
[GLOBAL collect_context]
collect_context:
    ; push segment registers
    push ds
    push es
    push fs
    push gs
    ; push GPRs
    pusha

    ; clear df flag
    cld
    ; segment registers init
    mov eax, DATA
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    ; stack alignment
    mov ebx, esp
    and esp, 0xFFFFFFF0
    sub esp, 12
    push ebx
    call universal_handler

    ; resotre context
    mov esp, ebx
    popa
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 8
    iret
DATA equ 0x10
