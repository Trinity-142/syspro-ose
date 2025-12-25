#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#define INIT_PAGING(n) PAGING(n)
#define PAGING(n) INIT_PAGING_##n

#define ALLOC_USER_STACK(n) USER_STACK(n)
#define USER_STACK(n) ALLOC_USER_STACK_##n

#define USERSPACE_PROCESS(n) USER(n)
#define USER(n) USERSPACE_PROCESS_##n

#define PAGE_FAULT_HANDLER(n) PF(n)
#define PF(n) PAGE_FAULT_HANDLER_##n

#define EXP_SETUP                                       															\
	init_paging();																									\
    vga_clear_screen();																								\
	init_interrupts(INTERRUPT);																						\
	set_interrupt_dpl(WRITE_VECTOR, USER_PL);																		\
	set_interrupt_dpl(EXIT_VECTOR, USER_PL);																		\
	bool auto_eoi = true;																							\
	pic8259_init_master(auto_eoi);																					\
	pic8259_init_slave(auto_eoi);																					\
	pic8259_turn(TIMER, true);																						\
	sti();																											\
	param = 0;																										\
	start_usercode();																								\
	endless_loop();																									\
//================================================================================================================== 1
#define INIT_PAGING_1                                   															\
	pd = calloc_page();																								\
    pd->pt_addr = 0;			                        															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = true;                                     															\
    pd->ps = true;                                      															\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_1																							\
	return malloc_undead(4096, 16) + 4096;
#define USERSPACE_PROCESS_1																							\
	for (;;) {																										\
		write(global);																								\
		global++;																									\
	}
#define PAGE_FAULT_HANDLER_1
//================================================================================================================== 2
#define INIT_PAGING_2                                   															\
	pd = calloc_page();																								\
    pd->pt_addr = 0;			                        															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = false;                                    															\
    pd->ps = true;                                      															\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_2 ALLOC_USER_STACK_1
#define USERSPACE_PROCESS_2 USERSPACE_PROCESS_1
#define PAGE_FAULT_HANDLER_2
//================================================================================================================== 3
#define INIT_PAGING_3																								\
    pd = calloc_page();																								\
    PageTableEntry* pt = calloc_page();																				\
    pd->pt_addr = (u32) pt >> 12;                       															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = true;                                     															\
    pd->ps = false;                                     															\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (i * PAGE) >> 12;            															\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        pt[i].u_s = true;																							\
    }                                                   															\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_3 ALLOC_USER_STACK_1
#define USERSPACE_PROCESS_3 USERSPACE_PROCESS_1
#define PAGE_FAULT_HANDLER_3
//================================================================================================================== 4
#define INIT_PAGING_4																								\
    pd = calloc_page();																								\
    PageTableEntry* pt = calloc_page();																				\
    pd->pt_addr = (u32) pt >> 12;                       															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = false;                                    															\
    pd->ps = false;                                     															\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (i * PAGE) >> 12;            															\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        pt[i].u_s = true;																							\
    }                                                   															\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_4	ALLOC_USER_STACK_1
#define USERSPACE_PROCESS_4	USERSPACE_PROCESS_1
#define PAGE_FAULT_HANDLER_4
//================================================================================================================== 5
#define INIT_PAGING_5																								\
    pd = calloc_page();																								\
    PageTableEntry* pt = calloc_page();																				\
    pd->pt_addr = (u32) pt >> 12;                       															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = true;	                                    															\
    pd->ps = false;                                     															\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (i * PAGE) >> 12;            															\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        pt[i].u_s = false;																							\
    }                                                   															\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_5	ALLOC_USER_STACK_1
#define USERSPACE_PROCESS_5	USERSPACE_PROCESS_1
#define PAGE_FAULT_HANDLER_5
//================================================================================================================== 6
#define INIT_PAGING_6																								\
    pd = calloc_page();																								\
    PageTableEntry* pt = calloc_page();																				\
    pd->pt_addr = (u32) pt >> 12;                       															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = true;	                                    															\
    pd->ps = false;                                     															\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (i * PAGE) >> 12;            															\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        if (pt[i].frame_addr >= 0x80 && pt[i].frame_addr < 0x100) {													\
            pt[i].u_s = false;                          															\
        } else pt[i].u_s = true;																					\
    }                                                   															\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_6	ALLOC_USER_STACK_1
#define USERSPACE_PROCESS_6 USERSPACE_PROCESS_1
#define PAGE_FAULT_HANDLER_6
//================================================================================================================== 7
#define INIT_PAGING_7 INIT_PAGING_6
#define ALLOC_USER_STACK_7 ALLOC_USER_STACK_1
#define USERSPACE_PROCESS_7																							\
	for (;;) {																										\
		printf("%d ", global);																						\
		global++;																									\
	}
#define PAGE_FAULT_HANDLER_7
//================================================================================================================== 8
#define INIT_PAGING_8																								\
	pd = calloc_page();																								\
																													\
    PageTableEntry* pt = calloc_page();																				\
    pd->pt_addr = (u32) pt >> 12;                       															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = true;	                                    															\
    pd->ps = false;                                     															\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (i * PAGE) >> 12;            															\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        if (pt[i].frame_addr < 0x7 || (pt[i].frame_addr >= 0x80 && pt[i].frame_addr < 0x400)) {						\
            pt[i].u_s = false;                          															\
        } else pt[i].u_s = true;																					\
    }                                                   															\
																													\
	pt = calloc_page();																								\
	pd[1].pt_addr = (u32) pt >> 12;																					\
	pd[1].p = true;																									\
	pd[1].r_w = true;																								\
	pd[1].u_s = true;																								\
	pd[1].ps = false;																								\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (u32) calloc_page() >> 12;																\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        pt[i].u_s = true;																							\
    }                                                   															\
																													\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_8 return (void*) USER_STACK_POINTER;
#define USERSPACE_PROCESS_8	USERSPACE_PROCESS_1;
#define PAGE_FAULT_HANDLER_8
//================================================================================================================== 9
#define INIT_PAGING_9																								\
    pd = calloc_page();																								\
    PageTableEntry* pt = calloc_page();																				\
    pd->pt_addr = (u32) pt >> 12;                       															\
    pd->p = true;                                       															\
    pd->r_w = true;                                     															\
    pd->u_s = true;                                     															\
    pd->ps = false;                                     															\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (i * PAGE) >> 12;            															\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        if (pt[i].frame_addr < 0x7 || (pt[i].frame_addr >= 0x80 && pt[i].frame_addr < 0x400)) {						\
            pt[i].u_s = false;                          															\
        } else {                                        															\
            pt[i].u_s = true;                           															\
        }                                               															\
    }                                                   															\
    turn_paging_on(pd);
#define ALLOC_USER_STACK_9																							\
	turn_paging_off();																								\
	PageTableEntry* pt = calloc_page();																				\
	pd[1].pt_addr = (u32) pt >> 12;																					\
	pd[1].p = true;																									\
	pd[1].r_w = true;																								\
	pd[1].u_s = true;																								\
	pd[1].ps = false;																								\
    for (u32 i = 0; i < 1024; i++) {                    															\
        pt[i].frame_addr = (u32) calloc_page() >> 12;																\
        pt[i].p = true;                                 															\
        pt[i].r_w = true;                               															\
        pt[i].u_s = true;																							\
    }                                                   															\
    turn_paging_on(pd);																								\
	return (void*) USER_STACK_POINTER;
#define USERSPACE_PROCESS_9																							\
	exit(param);
#define PAGE_FAULT_HANDLER_9
//================================================================================================================== 10
#define INIT_PAGING_10 INIT_PAGING_9
#define ALLOC_USER_STACK_10 ALLOC_USER_STACK_9
#define USERSPACE_PROCESS_10																						\
	switch (param % 4) {																							\
		case 1:																										\
			*((u32*) 0x42) = 42;																					\
		case 2:																										\
			endless_rec();																							\
		case 3:																										\
			*((u32*) 0x900000) = 42;																				\
		default:																									\
			exit(param);																							\
	}
#define PAGE_FAULT_HANDLER_10																						\
	u32 cr2 = get_cr2();																							\
	bool us = ctx->error_code & (1 << 2);																			\
	if (us == 0) kernel_panic("Kernel panic: UB in kernel space!\n");												\
	else {																											\
		if (cr2 < 0x7000) printf("NPE: ");																			\
		else if (cr2 >= 0x80000 && cr2 < 0x400000) printf("SOE: ");													\
		else printf("UB: ");																						\
		exit_impl(ctx);																								\
	}
//================================================================================================================== 11
#define INIT_PAGING_11 INIT_PAGING_9
#define ALLOC_USER_STACK_11																							\
	turn_paging_off();																								\
	PageTableEntry* pt = calloc_page();																				\
	pd[1].pt_addr = (u32) pt >> 12;																					\
	pd[1].p = true;																									\
	pd[1].r_w = true;																								\
	pd[1].u_s = true;																								\
	pd[1].ps = false;																								\
    turn_paging_on(pd);																								\
	return (void*) USER_STACK_POINTER;
#define USERSPACE_PROCESS_11																						\
    n_rec(param);																									\
    exit(param);
#define PAGE_FAULT_HANDLER_11																						\
	u32 cr2 = get_cr2();																							\
	bool us = ctx->error_code & (1 << 2);																			\
	if (us == 0) kernel_panic("Kernel panic: UB in kernel space!\n");												\
	else {																											\
		if (cr2 < 0x7000) printf("NPE: ");																			\
		else if (cr2 >= 0x80000 && cr2 < 0x400000) printf("SOE: ");													\
		else if (cr2 >= 0x400000 && cr2 < USER_STACK_POINTER) {														\
			expand_user_stack(cr2);																					\
			return;																									\
		} else printf("UB: ");																						\
		exit_impl(ctx);																								\
	}
#endif
