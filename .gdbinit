target remote localhost:1234
set disassembly-flavor intel
b *0x7c00
c
display/i $pc
x/20i $pc

# set arch i80386
