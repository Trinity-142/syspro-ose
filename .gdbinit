file .tmp/os.elf
target remote localhost:1234
set disassembly-flavor intel
set arch i386
br kernel_entry 
display/i $pc
c
