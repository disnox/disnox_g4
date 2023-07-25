#include "flash_driver.h"
#include "string.h"
/**
***********************************************************************
* @brief:      flash_erase_page(uint8_t page)
* @param:		   page��flashҳ��
* @retval:     i 0��ʧ�� 1���ɹ�
* @details:    ��ҳ�����������ҳ
***********************************************************************
**/
uint8_t flash_erase_page(uint32_t page)
{
	uint8_t i;
	i = flash_bsp_erase_page(page);
	return i;
}
/**
***********************************************************************
* @brief:      flash_erase_pages(uint32_t start_addr, uint32_t end_addr)
* @param:		   start_addr��flash�׵�ַ
* @param:		   end_addr��flashβ��ַ
* @retval:     i 0��ʧ�� 1���ɹ�
* @details:    ��ҳ���ַ����������ҳ
***********************************************************************
**/
uint8_t flash_erase_pages(uint32_t start_add, uint32_t end_add)
{
	uint8_t i;
	i	= flash_bsp_erase_pages(start_add, end_add);
	return i;
}
/**
***********************************************************************
* @brief:      flash_write_data(uint32_t addr, void *data, uint32_t size)
* @param:		   addr��flash ��ַ
* @param:		   data��д�������
* @param:		   size��д������ݵ��ֽ���
* @retval:     void
* @details:    ��flash�д�������
***********************************************************************
**/
void flash_write_data(uint32_t addr, void *data, uint32_t size)
{
	flash_bsp_write_data(addr, data, size);
}
/**
***********************************************************************
* @brief:      flash_read_data(uint32_t addr, void *data, uint32_t size)
* @param:		   addr��flash ��ַ
* @param:		   data����ȡ������
* @param:		   size����ȡ�������ֽ���
* @retval:     void
* @details:    ��flash�ж�ȡ����
***********************************************************************
**/
void flash_read_data(uint32_t addr ,uint32_t *data,uint32_t size)
{
	memcpy(data, (uint32_t *)addr, size);
}

