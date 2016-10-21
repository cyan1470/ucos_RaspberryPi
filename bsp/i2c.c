#include "regs.h"
#include "i2c.h"
#include "uart.h"
#include "interrupts.h"
#include "utils/printf.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

static void bcm2835_i2c_writel(unsigned int addr, unsigned int value)
{
  PUT32(addr,value);
}

static int bcm2835_i2c_readl(unsigned int addr)
{
  return GET32(addr);
}

#define BCM2835_I2C_C_READ  (BIT(0))
#define BCM2835_I2C_C_CLEAR (BIT(5) | BIT(4))
#define BCM2835_I2C_C_ST    (BIT(7))
#define BCM2835_I2C_C_INTD  (BIT(8))
#define BCM2835_I2C_C_INTT  (BIT(9))
#define BCM2835_I2C_C_INTR  (BIT(10))
#define BCM2835_I2C_C_I2CEN (BIT(15))

#define BCM2835_I2C_S_TA    (BIT(0))
#define BCM2835_I2C_S_DONE  (BIT(1))
#define BCM2835_I2C_S_TXW   (BIT(2))
#define BCM2835_I2C_S_RXR   (BIT(3))
#define BCM2835_I2C_S_TXD   (BIT(4))
#define BCM2835_I2C_S_RXD   (BIT(5))
#define BCM2835_I2C_S_TXE   (BIT(6))
#define BCM2835_I2C_S_RXF   (BIT(7))
#define BCM2835_I2C_S_ERR   (BIT(8))
#define BCM2835_I2C_S_CLKT  (BIT(9))

static struct bcm2835_i2c_ctrl bcm2835_i2c_dev;

static struct bcm2835_i2c_ctrl *bcm2835_i2c_get_ctrl(void)
{
  return &bcm2835_i2c_dev;
}

static void bcm2835_i2c_debug_print_status()
{
  unsigned int v = 0;
  v = bcm2835_i2c_readl(BSC0_S_REG);
  printf("BCM2835_I2C_S_TA is %d",v && BCM2835_I2C_S_TA);
  printf("BCM2835_I2C_S_DONE is %d",v && BCM2835_I2C_S_DONE);
  printf("BCM2835_I2C_S_TXW is %d",v && BCM2835_I2C_S_TXW);
  printf("BCM2835_I2C_S_RXR is %d",v && BCM2835_I2C_S_RXR);
  printf("BCM2835_I2C_S_TXD is %d",v && BCM2835_I2C_S_TXD);
  printf("BCM2835_I2C_S_RXD is %d",v && BCM2835_I2C_S_RXD);
  printf("BCM2835_I2C_S_RXF is %d",v && BCM2835_I2C_S_RXF);
  printf("BCM2835_I2C_S_ERR is %d",v && BCM2835_I2C_S_ERR);
  printf("BCM2835_I2C_S_CLKT is %d",v && BCM2835_I2C_S_CLKT);

  return;
}

//static int bcm2835_i2c_drain_rxfifo(struct bcm2835_i2c_ctrl *bcm2835_i2c)
//{
//
//}

static int bcm2835_i2c_fill_txfifo(struct bcm2835_i2c_ctrl *bcm2835_i2c)
{
  int state = 0;

  while(bcm2835_i2c->buf_remaining)
  {
    state = bcm2835_i2c_readl(BSC0_S_REG);
    if( state && BCM2835_I2C_S_TXD )
    {
      break;
    }

    bcm2835_i2c_writel(BSC0_FIFO_REG,*(bcm2835_i2c->buf));
    bcm2835_i2c->buf++;
    bcm2835_i2c->buf_remaining--;
  }
  return 0;
}

static void bcm2835_i2c_isr(int nIRQ, void *pParam) 
{
  unsigned int v = 0;

  //struct bcm2835_i2c_ctrl *bcm2835_i2c = (struct bcm2835_i2c_ctrl *)pParam;

  v = bcm2835_i2c_readl(BSC0_S_REG);

  if( v && ( BCM2835_I2C_S_ERR | BCM2835_I2C_S_CLKT ) )
  {
    printf("there are soem errs, please check");
  }
  else if( v && BCM2835_I2C_S_RXD )
  {
    printf("RXD need to do something");
  }
  else if( v && BCM2835_I2C_S_DONE )
  {
    printf("translate is done");
  }
  else if( v && BCM2835_I2C_S_TXD )
  {
    printf("TXD need to do something");
    //bcm2835_i2c_fill_txfifo(bcm2835_i2c);
  }
  else
  {
    printf("Oops");
  }

  return;
}

static int wait_for_bcm2835_i2c_completion(struct bcm2835_i2c_ctrl *bcm2835_i2c)
{
  unsigned int v = 0;
  while(1)
  {
    v = bcm2835_i2c_readl(BSC0_S_REG);

    if( v && ( BCM2835_I2C_S_ERR | BCM2835_I2C_S_CLKT ) )
    {
      printf("there are soem errs, please check");
      break;
    }
    else if( v && BCM2835_I2C_S_RXD )
    {
      printf("RXD");

    }
    else if( v && BCM2835_I2C_S_DONE )
    {
      printf("translate is done");
      break;
    }
    else if( v && BCM2835_I2C_S_TXD )
    {
      printf("TXD need to do something");
      bcm2835_i2c_fill_txfifo(bcm2835_i2c);
      continue;
    }
    else
    {
      printf("Oops");
      break;
    }
  }
  
  return 0;
}

int bcm2835_i2c_xfer(unsigned int slave, char *value, unsigned int count,int write)
{
  unsigned int c = 0;

  struct bcm2835_i2c_ctrl *bcm2835_i2c = bcm2835_i2c_get_ctrl();
  bcm2835_i2c->buf = value;
  bcm2835_i2c->buf_remaining = count;

  if ( write )
  {
    c = BCM2835_I2C_C_INTT;
    bcm2835_i2c_fill_txfifo(bcm2835_i2c);
  }
  else
  {
    c = BCM2835_I2C_C_READ | BCM2835_I2C_C_INTR;
  }

  c |= BCM2835_I2C_C_ST | BCM2835_I2C_C_INTD | BCM2835_I2C_C_I2CEN;
  bcm2835_i2c_writel(BSC0_A_REG,slave);
  bcm2835_i2c_writel(BSC0_DLEN_REG,count);
  bcm2835_i2c_writel(BSC0_C_REG, c);

  wait_for_bcm2835_i2c_completion(bcm2835_i2c);
  return 0;
}

int bcm2835_i2c_init(void)
{
  struct bcm2835_i2c_ctrl *bcm2835_i2c = bcm2835_i2c_get_ctrl();
  RegisterInterrupt(BCM2835_IRQ_ID_I2C, bcm2835_i2c_isr, bcm2835_i2c);
  bcm2835_i2c_debug_print_status();
  return 0;
}