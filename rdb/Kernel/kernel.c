#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <video_vm.h>
#include <screen.h>
#include <sound.h>
#include <timet.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {

	char buffer[10];

	ncPrint(cpuVendor(buffer));
	
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	//ncPrintHex((uint64_t)&text);
	
	//ncPrintHex((uint64_t)&rodata);
	
	//ncPrintHex((uint64_t)&data);
	
	//ncPrintHex((uint64_t)&bss);
	
	return getStackBase();
}

int main()
{
	init_VM_Driver();
	init_screen();
	load_idt();

	//ncPrintHex((uint64_t)sampleCodeModuleAddress);

	// void (*scm)(void);
    // scm = &sampleCodeModuleAddress;
	// printString("asd1", 4);
    // printNewLine();
 	// void * rsp = createProcess(scm, 27);
	// printString("asd2", 4);
    // printNewLine();
	// _set_rsp_and_halt(rsp);
	// printString("asd3", 4);
    // printNewLine();

	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	
	//ncPrint((char*)sampleDataModuleAddress);

	return 0;
}
