#include "vsprintf.h"
#include "string.h"
#include "printf.h"
#include "uart.h"

#define	OUTBUFSIZE	1024
#define	INBUFSIZE	1024


static unsigned char g_pcOutBuf[OUTBUFSIZE];
static unsigned char g_pcInBuf[INBUFSIZE];


int printf(const char *fmt, ...)
{
	int i;
	int len;
	va_list args;

	va_start(args, fmt);
	len = vsprintf(g_pcOutBuf,fmt,args);
	va_end(args);
	
    uart_string(g_pcOutBuf);

	return len;
}

int scanf(const char * fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;
	
    return 0;
	while(1)
	{
		//c = getchar();
		if((c == 0x0d) || (c == 0x0a))
		{
			g_pcInBuf[i] = '\0';
			break;
		}
		else
		{
			g_pcInBuf[i++] = c;
		}
	}
	
	va_start(args,fmt);
	i = vsscanf(g_pcInBuf,fmt,args);
	va_end(args);

	return i;
}

