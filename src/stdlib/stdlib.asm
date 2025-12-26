[BITS 32]
[GLOBAL print_char]
print_char:
    mov eax, [esp + 4]
    int 0x31
    ret

[GLOBAL exit]
exit:
    mov eax, [esp + 4]
    int 0x30
