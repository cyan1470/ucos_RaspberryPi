
#ifndef REGS_H_
#define REGS_H_

#define BIT(x)  (1UL << (x))

#define NULL ((void*)0) 

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

//bsc i2c
#define BSC0_C_REG      0X20205000
#define BSC0_S_REG      0X20205004
#define BSC0_DLEN_REG   0X20205008
#define BSC0_A_REG      0X2020500C
#define BSC0_FIFO_REG   0X20205010
#define BSC0_DIV_REG    0X20205004
#define BSC0_DEL_REG    0X20205008
#define BSC0_CLKT_REG   0X2020500C

//interrupt control regs
#define BASE_INTC			(0x2000B200)

typedef struct {
	unsigned long	IRQBasic;	// Pending 0
	unsigned long	Pending1;
	unsigned long	Pending2;
	unsigned long	FIQCtrl;
	unsigned long	Enable1;
	unsigned long	Enable2;
	unsigned long	EnableBasic;
	unsigned long	Disable1;
	unsigned long	Disable2;
	unsigned long	DisableBasic;
} INTC_REGS;

static volatile INTC_REGS * const intcRegs = (INTC_REGS *) (BASE_INTC);

//timer regs
#define portTIMER_BASE                    		( (unsigned long ) 0x2000B400 )

typedef struct _BCM2835_TIMER_REGS {
	unsigned long LOD;
	unsigned long VAL;
	unsigned long CTL;
	unsigned long CLI;
	unsigned long RIS;
	unsigned long MIS;
	unsigned long RLD;
	unsigned long DIV;
	unsigned long CNT;
} TIMER_REGS;

static volatile TIMER_REGS * const timerRegs = (TIMER_REGS *) (portTIMER_BASE);

#endif /* REGS_H_ */
