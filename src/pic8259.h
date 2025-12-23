#ifndef PIC8259_H
#define PIC8259_H
#include <stdbool.h>
#define IRQs 16
#define IRQ0_VECTOR 0x20
#define IRQ8_VECTOR 0x28
#define TIMER_VECTOR IRQ0_VECTOR
#define KEYBOARD_VECTOR 0x21
#define SLAVE_IRQ 2

typedef enum {
    TIMER = 0,
    KEYBOARD = 1
} Device;

typedef enum {
    MASTER,
    SLAVE
} PIC8259Type;

typedef enum {
    MASTER_COMMAND = 0x20,
    MASTER_DATA = 0x21,
    SLAVE_COMMAND = 0xA0,
    SLAVE_DATA = 0xA1,
    DELAY = 0x80
} Port;

void pic8259_init_master(bool auto_eoi);
void pic8259_init_slave(bool auto_eoi);
void pic8259_turn(Device device, bool mode);
void pic8259_send_EOI(Device irq);

#endif