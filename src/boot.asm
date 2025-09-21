[BITS 16]

cli

xor ax, ax
mov ss, ax
mov sp, 0x7C00

mov ax, 0x7C0
mov ds, ax 

mov bx, msg
print:
  mov al, byte [bx] 
  test al, al
  jz loop
  mov ah, 0x0E
  int 0x10
  inc bx 
  jmp print

loop:
  jmp loop


msg: db "Hello, world!", 0x0A, 0x0D

times 510-($-$$) db 0
dw 0xAA55
