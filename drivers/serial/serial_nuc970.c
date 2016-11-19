/*
 * Copyright (c) 2014	Nuvoton Technology Corp.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Description:   NUC970 UART driver
 */
#include <common.h>
#include <asm/io.h>
#include <dm.h>
#include <serial.h>

#include "serial_nuc970.h"
//#include <asm/arch/serial_nuc970.h>

#define CONFIG_EXT_CLK	                12000000        /* 12 MHz crystal */
#define CONFIG_TMR_DIV                  120             /* timer prescaler */

DECLARE_GLOBAL_DATA_PTR;

typedef struct
{
  	union
  	{
		volatile unsigned int RBR;		    	        /*!< Offset: 0x0000   Receive Buffer Register			                 */
		volatile unsigned int THR;			            /*!< Offset: 0x0000   Transmit Holding Register 		                 */
  	} x;
  	volatile unsigned int IER;				        /*!< Offset: 0x0004   Interrupt Enable Register 		                 */
  	volatile unsigned int FCR;				        /*!< Offset: 0x0008   FIFO Control Register 			                 */
  	volatile unsigned int LCR;				        /*!< Offset: 0x000C   Line Control Register				                 */
  	volatile unsigned int MCR;			        	/*!< Offset: 0x0010   Modem Control Register 			                 */
  	volatile unsigned int MSR;				        /*!< Offset: 0x0014   Modem Status Register 			                 */
  	volatile unsigned int FSR;				        /*!< Offset: 0x0018   FIFO Status Register 				                 */
  	volatile unsigned int ISR;				        /*!< Offset: 0x001C   Interrupt Status Register 		                 */
  	volatile unsigned int TOR;				        /*!< Offset: 0x0020   Time Out Register 				                 */
  	volatile unsigned int BAUD;				        /*!< Offset: 0x0024   Baud Rate Divisor Register		                 */
  	volatile unsigned int IRCR;				        /*!< Offset: 0x0028   IrDA Control Register 			                 */
  	volatile unsigned int ALTCON;			            /*!< Offset: 0x002C   Alternate Control/Status Register	                 */
  	volatile unsigned int FUNSEL;			            /*!< Offset: 0x0030   Function Select Register			                 */
} UART_TypeDef;

#define REG_PCLKEN0     0xB0000218 /* clock */

#define GCR_BA    0xB0000000 /* Global Control */
#define REG_MFP_GPE_L	(GCR_BA+0x090)  /* GPIOE Low Byte Multiple Function Control Register */
#define UART0_BA  0xB8000000 /* UART0 Control (High-Speed UART) */
#define UART0	   ((UART_TypeDef *)UART0_BA) 

/*
 * Initialise the serial port with the given baudrate. The settings are always 8n1.
 */

u32 baudrate = CONFIG_BAUDRATE;
u32 ext_clk  = CONFIG_EXT_CLK;

int nuc97xx_serial_init (void)
{
	__raw_writel((__raw_readl(REG_MFP_GPE_L) & 0xffffff00) | 0x99, REG_MFP_GPE_L); // UART0 multi-function

	/* UART0 line configuration for (115200,n,8,1) */
	UART0->LCR |=0x07;	
	UART0->BAUD = 0x30000066; /* 12MHz reference clock input, 115200 */
	
	return 0;
}

void nuc97xx_serial_putc (const char ch)
{
	//volatile int loop;
	while ((UART0->FSR & 0x800000)); //waits for TX_FULL bit is clear
	UART0->x.THR = ch;
	if(ch == '\n')
	{
		while((UART0->FSR & 0x800000)); //waits for TX_FULL bit is clear
		UART0->x.THR = '\r';
	}

}

void nuc97xx_serial_puts (const char *s)
{
	while (*s) {
		nuc97xx_serial_putc (*s++);
	}
}

int nuc97xx_serial_getc (void)
{
	int i;
	while (1)
	{
		for(i=0;i<0x1000;i++);
		if (!(UART0->FSR & (1 << 14)))
		{
			return (UART0->x.RBR);
		}
	}
}

int nuc97xx_serial_tstc (void)
{
	return (!((__raw_readl(UART0_BASE + REG_COM_MSR) & RX_FIFO_EMPTY)>>14));
}

void nuc97xx_serial_setbrg (void)
{
        return;
}

static int nuc970_serial_setbrg(struct udevice *dev, int baudrate)
{
	return 0;
}

static int nuc970_serial_getc(struct udevice *dev)
{
    if(UART0->FSR & (1 << 14))
    {
        return -EAGAIN;
    }

    return (UART0->x.RBR);
}

static int nuc970_serial_putc(struct udevice *dev, const char c)
{
#if 0
	//volatile int loop;
	while ((UART0->FSR & 0x800000)); //waits for TX_FULL bit is clear
	UART0->x.THR = c;
	if(c == '\n')
	{
		while((UART0->FSR & 0x800000)); //waits for TX_FULL bit is clear
		UART0->x.THR = '\r';
	}
#else

    if(UART0->FSR & 0x800000)
    {
        return -EAGAIN;
    }

    UART0->x.THR = c;
    
#endif
	return 0;
}

static int nuc970_serial_pending(struct udevice *dev, bool input)
{
    if(input)
    {
        //return (UART0->FSR & (1 << 14)) ? 0 : ((UART0->FSR & 0x3F00) >> 8);
        return (UART0->FSR & (1 << 14)) ? 0 : 1;
    }
    else
    {
        return (UART0->FSR & 0x800000) ? 0 : 1;
    }
}

static const struct dm_serial_ops nuc970_serial_ops = {
	.putc = nuc970_serial_putc,
	.pending = nuc970_serial_pending,
	.getc = nuc970_serial_getc,
	.setbrg = nuc970_serial_setbrg,
};

static int nuc970_serial_probe(struct udevice *dev)
{
    __raw_writel((__raw_readl(REG_MFP_GPE_L) & 0xffffff00) | 0x99, REG_MFP_GPE_L); // UART0 multi-function

    /* UART0 line configuration for (115200,n,8,1) */
    UART0->LCR |=0x07;
    UART0->BAUD = 0x30000066; /* 12MHz reference clock input, 115200 */

    return 0;
}

U_BOOT_DRIVER(nuc970_serial) = {
	.name = "nuc970_serial",
	.id = UCLASS_SERIAL,
	.ops = &nuc970_serial_ops,
	.probe = nuc970_serial_probe,
	.flags = DM_FLAG_PRE_RELOC,
};

U_BOOT_DEVICE(stm32_serials) = {
	.name = "nuc970_serial",
};
