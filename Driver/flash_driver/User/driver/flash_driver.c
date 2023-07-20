#include "flash_driver.h"
#include "string.h"

void flash_erase_page(uint32_t page)
{
	flash_bsp_erase_page(page);
}

void flash_erase_pages(uint32_t start_add, uint32_t end_add)
{
	flash_bsp_erase_pages(start_add, end_add);
}

void flash_write_data(uint32_t addr, void *data, uint32_t size)
{
	flash_bsp_write_data(addr, data, size);
}

void flash_read_data(uint32_t addr ,uint32_t *data,uint32_t size)
{
	memcpy(data, (uint32_t *)addr, size);
}

