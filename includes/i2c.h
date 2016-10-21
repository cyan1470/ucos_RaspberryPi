#ifndef _I2C_H_
#define _I2C_H_

struct bcm2835_i2c_ctrl
{
  char *buf;
  unsigned int buf_remaining;
};

int bcm2835_i2c_xfer(unsigned int slave, char *value, unsigned int count,int write);
int bcm2835_i2c_init(void);

#endif  //_I2C_H_
