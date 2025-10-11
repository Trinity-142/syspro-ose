[BITS 16]

%define NUM_OF_SECTORS ((KERNEL_SIZE) / 512 + 1) 
%define MAX_SECTOR_NUM 18
%define MAX_HEAD_NUM 1
%define MAX_CYLINDER_NUM 79

cli

; stack init 
xor ax, ax
mov ss, ax
mov sp, 0x7C00

; data init
xor si, si
mov ds, si

; read buffer init 
mov si, 0x7E0
mov es, si 
xor bx, bx

; reading from floppy disk
mov al, 1   ; number of sectors
mov cl, 2   ; sector number
xor ch, ch  ; cylinder number
xor dh, dh  ; head number

mov di, NUM_OF_SECTORS

read:
    mov ah, 0x2
    int 0x13
    jc disk_read_error
    dec di
    jz continue 

    ; shift read buffer by 512 bytes (1 sector)
    add si, 0x20
    mov es, si

    ; change sector
    inc cl
    cmp cl, MAX_SECTOR_NUM 
    jle read
    mov cl, 1 

    ; change head 
    inc dh
    cmp dh, MAX_HEAD_NUM
    jle read
    xor dh, dh

    ; change cylinder
    inc ch
    cmp ch, MAX_CYLINDER_NUM
    jle read

disk_read_overflow:
    mov ah, 0xE
    mov al, 'O'
    int 0x10
    hlt

disk_read_error:
    mov ah, 0xE
    mov al, 'E'
    int 0x10
    hlt


continue:

; load gdtr
lgdt [gdt_descriptor]

; clear df flag
cld

; set protection enable flag
mov eax, cr0 
or eax, 0x1
mov cr0, eax

; segment registers init
jmp CODE:next
[BITS 32]
next:
    mov eax, DATA       
    mov ds, eax 
    mov ss, eax 
    mov es, eax
    mov fs, eax 
    mov gs, eax 
    and esp, 0xFFFFFFF0

; jmp to C
[EXTERN kernel_entry]
jmp CODE:kernel_entry

; pseudo gdt descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; gdt limit
    dd gdt_start               ; gdt base address

align 8

; gdt init
gdt_start:
    ; null descriptor
    .null: dq 0x0

    ; code segment descriptor
    .gdt_code:          
        dw 0xFFFF       ; limit[15:00]
        dw 0x0          ; base[15:00]
        db 0x0          ; base[23:16]
        db 0b1001_1010  ; segment-present flag, dpl, descriptor type flag
                        ; code flags: executable, conforming, read-enable, accessed
        db 0b1100_1111  ; granularity flag, D, L flag, available flag, limit[19:16]
        db 0x0          ; base[31:24]

    ; data segment descriptor
    .gdt_data:
        dw 0xFFFF       ; limit[15:00]
        dw 0x0          ; base[15:00]
        db 0x0          ; base[23:16]
        db 0b1001_0010  ; segment-present flag, dpl, descriptor type flag
                        ; code flags: executable, E, write-enable, accessed
        db 0b1100_1111  ; granularity flag, B, L flag, available flag, limit[19:16]
        db 0x0          ; base[31:24]
gdt_end:

; segment selectors init
CODE equ 0x8
DATA equ 0x10 

[GLOBAL endless_loop]
endless_loop:
    jmp endless_loop

times 510-($-$$) db 0
dw 0xAA55
