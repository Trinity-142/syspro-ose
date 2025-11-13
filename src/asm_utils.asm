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
