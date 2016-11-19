
/*
 * Configuration for nuc97xx 
 *
 * SPDX-License-Identifier: GPL-2.0+ 
 *
 */

#ifndef NUC97XX_H_
#define NUC97XX_H_

/* Miscellaneous configurable options */

#if 0
//#define CONFIG_SYS_TEXT_BASE		0xA0000000    /* NOR boot */
//#define CONFIG_SYS_TEXT_BASE		0x8000        /* RAM boot */
//#define CONFIG_SYS_TEXT_BASE      0x200         /* SPI/NAND boot */
#endif

#define CONFIG_BOARD_EARLY_INIT_F

#define CONFIG_SYS_TEXT_BASE		0x800000  /* 0xE00000 */

#define CONFIG_BAUDRATE             115200

#define CONFIG_SYS_ONENAND_BASE     0

#define CONFIG_SYS_MALLOC_F_LEN     0x400
/*
 * Flash Configuration
 */
#define CONFIG_SYS_MAX_FLASH_BANKS 1
#define CONFIG_SYS_MAX_NAND_DEVICE 1

#define CONFIG_SYS_NAND_BASE       0xB000D000

#define CONFIG_ENV_IS_IN_NAND      1
#define CONFIG_ENV_OFFSET          0x80000
#define CONFIG_ENV_SIZE            0x10000  /* 64K */
#define CONFIG_ENV_OVERWRITE

/*
 * Memory Configuration
 */
#define CONFIG_NR_DRAM_BANKS		2  /* there are 2 sdram banks for nuc900 */
#define CONFIG_SYS_SDRAM_BASE		0

/*
 * Boot option
 */

/* default load address */
#define CONFIG_SYS_LOAD_ADDR        0x8000
#define CONFIG_SYS_INIT_SP_ADDR     0xBC008000

/*
 * Shell Settings
 */
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE		256	/* Console I/O Buffer Size */
#define CONFIG_SYS_MAXARGS		16	/* max number of command args */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		\
		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	ROUND(3 * CONFIG_ENV_SIZE + 128*1024, 0x1000)

#define CONFIG_STACKSIZE	(32*1024)	/* regular stack */

#endif

