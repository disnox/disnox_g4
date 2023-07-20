#include "flash_bsp.h"

/* 获取地址代表的页 */
static uint32_t get_page(uint32_t addr)
{
  return (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
}

/* 按页面地址擦除，擦多页 */ 
uint8_t flash_bsp_erase_pages(uint32_t start_addr, uint32_t end_addr)
{
	uint32_t page_err = 0;
	
	FLASH_EraseInitTypeDef EraseInit;
	
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	
	uint32_t FirstPage = get_page(start_addr);
	uint32_t NbOfPages = get_page(end_addr) - FirstPage + 1;
	
	/* Fill EraseInit structure*/
  EraseInit.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInit.Page        = FirstPage;
  EraseInit.NbPages     = NbOfPages;
	if (HAL_FLASHEx_Erase(&EraseInit, &page_err) != HAL_OK) return 0;
	HAL_FLASH_Lock();
	
	return 1;
}

/* 按页面擦除，擦单页 */ 
uint8_t flash_bsp_erase_page(uint8_t page)
{
	uint32_t page_err = 0;
	
	FLASH_EraseInitTypeDef EraseInit;
	
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	
	/* Fill EraseInit structure*/
  EraseInit.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInit.Page        = page;
  EraseInit.NbPages     = 1;
	if (HAL_FLASHEx_Erase(&EraseInit, &page_err) != HAL_OK) return 0;
	HAL_FLASH_Lock();
	
	return 1;
}


void flash_bsp_write_data(uint32_t addr, void *data, uint32_t size)
{
	uint64_t *buffer = (uint64_t *)data;
	uint32_t temp_addr = addr;
	
	HAL_FLASH_Unlock();
	
	for (uint32_t i=0; i<size; i+=8)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, temp_addr + i, *buffer);
		buffer++;
	}
	
	HAL_FLASH_Lock();
}














