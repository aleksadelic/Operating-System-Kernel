#include "System.h"
#include <dos.h>
#include "STDIO.H"
#include "STDARG.H"
#include "PCB.h"
#include "SCHEDULE.H"
#include "IdleThr.h"
#include "KSemList.h"
#include "IVTEntry.h"

PCBList allPCB;
KSemList allSem;
PCB* mainPCB = 0;
IdleThread* idleThread = 0;
pIntr oldTimer;

// sets new interrupt routine
void init() {

#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(0x8);
	setvect(0x8, timer);
	setvect(0x60, oldTimer);
#endif

	mainPCB = new PCB(0, 0, 1);
	PCB::running = mainPCB;
	PCB::running->state = RUNNING;
	allPCB.add(PCB::running);
	idleThread = new IdleThread();
	idleThread->myPCB->state = IDLE;
}

// restores old interrupt routine
void restore() {

#ifndef BCC_BLOCK_IGNORE
	setvect(8, oldTimer);
#endif

	delete idleThread;
	delete mainPCB;
}

// auxiliary variables for timer interrupt
volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;

volatile int cntr = 2;
volatile int context_switch_on_demand = 0;

void tick();

// new timer interrupt routine
void interrupt timer(...) {

	if (!context_switch_on_demand) {
		cntr--;
		tick();
		KernelSem::updateWaitTimes();
	}
	if (cntr == 0 || context_switch_on_demand) {

		asm{
			// saves sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		// scheduler
		if (PCB::running->state == RUNNING) {
			PCB::running->state = READY;
			Scheduler::put(PCB::running);
		}

		PCB::running = Scheduler::get();

		if (PCB::running == 0)
			PCB::running = idleThread->myPCB;
		else
			PCB::running->state = RUNNING;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		cntr = PCB::running->quantum;

		asm{
			// restores sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}

	if (!context_switch_on_demand) asm int 60h;

	context_switch_on_demand = 0;
}

void setContextSwitchOnDemand() {
	context_switch_on_demand = 1;
}
