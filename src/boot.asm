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
;mov si, 0x7C0
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

lgdt [gdt_descriptor]

cld

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
	mov esp, 0x7C00
	and esp, 0xFFFFFFF0

[EXTERN kernel_entry]
jmp CODE:kernel_entry

gdt_descriptor:
	dw 0x17
	dd gdt

align 8

gdt:
	dq 0x0
	dq 0x00CF9A000000FFFF
	dq 0x00CF92000000FFFF

CODE equ 0b01000
DATA equ 0b10000

[BITS 32]

[GLOBAL endless_loop]
endless_loop:
	jmp endless_loop

times 510-($-$$) db 0
dw 0xAA55
