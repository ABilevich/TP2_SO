#include <timet.h>
#include <keyboard.h>
#include <scheduler.h>
#include <stdint.h>
#include <interrupts.h>
#include <stdint.h>

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			//int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

void * int_20(void * rsp) {

	// printString("printing: ", 10);
	// print64Hex((uint64_t)rsp);
	// printNewLine();

	void * new_rsp = scheduler(rsp);
	//print64Hex((uint64_t)new_rsp);
	timer_handler();
	return new_rsp;
}

void int_21() {
	keyboard_handler();
}