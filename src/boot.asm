[BITS 16]

cli

; stack setup
mov ax, 0x7C0
mov ss, ax
xor sp, sp

; read buffer setup
mov ax, 0x7E0
mov es, ax
xor ax, ax
mov bx, ax

; reading from floppy disk
mov ah, 0x2 ; 0x2 - read sectors from drive
mov al, 1   ; number of sectors
mov cl, 2   ; sector number
xor ch, ch  ; cylinder number
xor dh, dh  ; head number
int 0x13

jnc loop

disk_read_error:
  mov ah, 0xE
  mov al, 'E'
  int 0x10
  hlt 

loop:
  jmp loop

times 510-($-$$) db 0
dw 0xAA55
