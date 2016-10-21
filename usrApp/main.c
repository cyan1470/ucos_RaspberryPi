#include "uart.h"
#include "regs.h"
#include "timer.h"
#include "i2c.h"
#include "ucos/includes.h"

OS_STK  userAppTaskStk1[1000];
OS_STK  userAppTaskStk2[1000];

extern void userApp1(void *);
extern void userApp2(void *);

void main()
{
	char test = 117;
	uart_init();

	InitInterruptController();

	DisableInterrupts();

	timer_init();

    bcm2835_i2c_init();

    bcm2835_i2c_xfer(0x68, &test, 1, 0);

	OSInit();

	OSTaskCreate(userApp1, (void *) 0, &userAppTaskStk1[1000-1],5);

	OSTaskCreate(userApp2, (void *) 0, &userAppTaskStk2[1000-1],6);

	OSStart();

	while(1);
}
