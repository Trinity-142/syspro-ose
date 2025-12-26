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

[GLOBAL n_rec]
n_rec:
    mov eax, [esp + 4]
    cmp eax, 0
    jz to_ret
        sub esp, 4088 ;40952 ;4088 ;40952
        sub eax, 1
        push eax
        call n_rec
        add esp, 4092 ; 40956 ;4092 ;40956
    to_ret:
        ret
