%include "src/consts.inc"

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

[GLOBAL write]
write:
    int WRITE_VECTOR
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
    mov eax, KERNEL_DATA
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
restore_context:
    popa
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 8
    iret

[GLOBAL write_u8]
write_u8:
    mov edx, [esp+4]
    mov eax, [esp+8]
    out dx, al
    call synchronize
    ret

[GLOBAL read_u8]
read_u8:
    xor eax, eax
    mov edx, [esp+4]
    in al, dx
    ret

synchronize:
    call cpuid
    call cpuid
    call cpuid
    ret

[GLOBAL cpuid]
cpuid:
    push ebx
    xor eax, eax
    cpuid
    pop ebx
    ret

[EXTERN gdt_descriptor]
[GLOBAL lgdt]
lgdt:
    lgdt [gdt_descriptor]
    ret

[GLOBAL ltr]
ltr:
    mov ax, TSS
    ltr ax
    ret

[GLOBAL cr]
cr:
    mov cr0, eax
    ret

[GLOBAL get_esp]
get_esp:
    mov eax, esp
    ret

[GLOBAL restore_user_context]
restore_user_context:
    mov esp, [esp + 4]
    jmp restore_context

[GLOBAL get_eflags]
get_eflags:
    pushfd
    pop eax
    ret

[GLOBAL turn_paging_on]
turn_paging_on:
    mov eax, [esp + 4]
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 4
    mov cr4, eax

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    ret

[GLOBAL turn_paging_off]
turn_paging_off:
    mov eax, cr0
    and eax, ~(1 << 31)
    mov cr0, eax
    ret

[GLOBAL get_cr2]
get_cr2:
    mov eax, cr2
    ret

[GLOBAL exit]
exit:
    mov eax, [esp + 4]
    int 0x31

[GLOBAL endless_rec]
endless_rec:
    sub esp, 4092
    call endless_rec

[GLOBAL n_rec]
n_rec:
    mov eax, [esp + 4]
    cmp eax, 0
    jz to_ret
        sub esp, 40952 ;4088 ;40952
        sub eax, 1
        push eax
        call n_rec
        add esp, 40956 ;4092 ;40956
    to_ret:
        ret
