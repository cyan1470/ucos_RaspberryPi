#include "regs.h"
#include "i2c.h"
#include "uart.h"
#include "interrupts.h"
#include "utils/printf.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define BSC_C_READ_FILED_MASK BIT(0)
#define BSC_C_READ_FILED_SHIFT 0
#define BSC_C_CLEAR_FILED_MASK (BIT(5) | BIT(4))
#define BSC_C_CLEAR_FILED_SHIFT 4
#define BSC_C_ST_FILED_MASK BIT(7)
#define BSC_C_ST_FILED_SHIFT 7
#define BSC_C_INTD_FILED_MASK BIT(8)
#define BSC_C_INTD_FILED_SHIFT 8
#define BSC_C_INTT_FILED_MASK BIT(9)
#define BSC_C_INTT_FILED_SHIFT 9
#define BSC_C_INTR_FILED_MASK BIT(10)
#define BSC_C_INTR_FILED_SHIFT 10
#define BSC_C_I2CEN_FILED_MASK BIT(15)
#define BSC_C_I2CEN_FILED_SHIFT 15

#define BSC_S_TA_FILED_MASK BIT(0)
#define BSC_S_TA_FILED_SHIFT 0
#define BSC_S_DONE_FILED_MASK BIT(1)
#define BSC_S_DONE_FILED_SHIFT 1
#define BSC_S_TXW_FILED_MASK BIT(2)
#define BSC_S_TXW_FILED_SHIFT 2
#define BSC_S_RXR_FILED_MASK BIT(3)
#define BSC_S_RXR_FILED_SHIFT 3
#define BSC_S_TXD_FILED_MASK BIT(4)
#define BSC_S_TXD_FILED_SHIFT 4
#define BSC_S_RXD_FILED_MASK BIT(5)
#define BSC_S_RXD_FILED_SHIFT 5
#define BSC_S_TXE_FILED_MASK BIT(6)
#define BSC_S_TXE_FILED_SHIFT 6
#define BSC_S_RXF_FILED_MASK BIT(7)
#define BSC_S_RXF_FILED_SHIFT 7
#define BSC_S_ERR_FILED_MASK BIT(8)
#define BSC_S_ERR_FILED_SHIFT 8
#define BSC_S_CLKT_FILED_MASK BIT(9)
#define BSC_S_CLKT_FILED_SHIFT 9

static void bsc_debug_print_status()
{
  unsigned int v = 0;
  v = GET32(BSC0_S_REG);
  printf("BSC_S_TA_FILED is %d",v && BSC_S_TA_FILED_MASK);
  printf("BSC_S_DONE_FILED is %d",v && BSC_S_DONE_FILED_MASK);
  printf("BSC_S_TXW_FILED is %d",v && BSC_S_TXW_FILED_MASK);
  printf("BSC_S_RXR_FILED is %d",v && BSC_S_RXR_FILED_MASK);
  printf("BSC_S_TXD_FILED is %d",v && BSC_S_TXD_FILED_MASK);
  printf("BSC_S_RXD_FILED is %d",v && BSC_S_RXD_FILED_MASK);
  printf("BSC_S_RXF_FILED is %d",v && BSC_S_RXF_FILED_MASK);
  printf("BSC_S_ERR_FILED is %d",v && BSC_S_ERR_FILED_MASK);
  printf("BSC_S_CLKT_FILED is %d",v && BSC_S_CLKT_FILED_MASK);

  return;
}

static void bsc_set_reg_mask(int addr, int mask, int shift, unsigned int value)
{
  unsigned int v = 0;
  v = GET32(addr);
  printf("cyan addr=%d",addr);
  printf("cyan v=%d",v);
  v &= ~mask;
  v |= ((value << shift) & mask);
  PUT32(addr, v);
  v = GET32(addr);
  printf("cyan v=%d",v);
  return;
}

static void bsci2cHandler(int nIRQ, void *pParam) 
{
  unsigned int v = 0;
  v = GET32(BSC0_S_REG);
  if( v && BSC_S_DONE_FILED_MASK )
  {
      printf("translate is done");
  }
  return;
}

int bsci2c_init(void)
{
  RegisterInterrupt(BCM2835_IRQ_ID_I2C, bsci2cHandler, NULL);
#define BSC0_C_REG_INIT_VALUE 0x00008710
  PUT32(BSC0_C_REG,BSC0_C_REG_INIT_VALUE);
  bsc_debug_print_status();
  return 0;
}

