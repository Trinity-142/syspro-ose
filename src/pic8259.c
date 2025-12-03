#include "pic8259.h"
#include <stdbool.h>
#include "asm_utils.h"

void pic8259_init_master(bool auto_eoi) {
    const u8 command_port = MASTER_COMMAND;
    const u8 data_port = MASTER_DATA;

    write_u8(data_port, 0b11111111);
    write_u8(command_port, 0b00010001);
    write_u8(data_port, IRQ0_VECTOR);
    write_u8(data_port, 1 << SLAVE_IRQ);
    write_u8(data_port, auto_eoi ? 0b0011 : 0b0001);
    write_u8(data_port, 0b11111111);
}

void pic8259_init_slave(bool auto_eoi) {
    const u8 command_port = SLAVE_COMMAND;
    const u8 data_port = SLAVE_DATA;

    write_u8(data_port, 0b11111111);
    write_u8(command_port, 0b00010001);
    write_u8(data_port, IRQ8_VECTOR);
    write_u8(data_port, SLAVE_IRQ);
    write_u8(data_port, auto_eoi ? 0b0011 : 0b0001);
    write_u8(data_port, 0b11111111);
}

void pic8259_turn(Device device, bool mode) {
    device = device < 8 ? device : device - 8;
    u8 current_mask = read_u8(MASTER_DATA);
    u8 new_mask = (mode == 1) ? (~(1 << device) & current_mask) : (1 << device | current_mask);
    write_u8(MASTER_DATA, new_mask);
}

void pic8259_send_EOI(Device irq) {
    write_u8(MASTER_COMMAND, 0x20);
    if (irq >= 8) {
        write_u8(SLAVE_COMMAND, 0x20);
    }
}