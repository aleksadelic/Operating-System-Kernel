#ifndef SYSTEM_H_
#define SYSTEM_H_

// disables interrupts
#define lock asm{\
	pushf;\
	cli;\
}

// enables interrupts
#define unlock asm popf

void init();
void restore();

void interrupt timer(...);

void setContextSwitchOnDemand();

#endif /* SYSTEM_H_ */
