void endless_loop();

void kernel_entry() {
	*((short int*) 0xB8000) = 0;
	endless_loop();
}
