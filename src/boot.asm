[BITS 16]

cli

xor ax, ax
mov ss, ax
mov sp, 0x7C00

mov ax, 0x7C0
mov ds, ax 

mov ah, 0x2 ; 0x2 - read sectors from drive
mov al, 1   ; number of sectors
mov cl, 2   ; sector number
xor ch, ch  ; cylinder number
xor dh, dh  ; head number
mov ax, 0x7E0 
mov es, ax
int 0x13
jnc loop 

read_error:
  mov ah, 0x0E
  mov al, 'E'
  int 0x10
  ret
 

loop:
  jmp loop

times 510-($-$$) db 0
dw 0xAA55
