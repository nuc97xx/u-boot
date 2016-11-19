

#include <common.h>


flash_info_t flash_info[1];


unsigned long flash_init(void)
{
	return 0;
}

void flash_print_info(flash_info_t *info)
{
	return;
}

int flash_erase(flash_info_t *info, int first, int last)
{

	return 0;
}

int write_buff(flash_info_t *info, uchar *src, ulong addr, ulong cnt)
{

	return 0;
}

