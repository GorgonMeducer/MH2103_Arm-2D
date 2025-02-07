/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.5.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#ifndef __DRV_FLASH_H__
#define	__DRV_FLASH_H__
#include "inc.h"


/* M25P SPI Flash supported commands */  
#define sFLASH_CMD_WRITE          0x02  /* Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /* Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /* Write enable instruction */
#define sFLASH_CMD_READ           0x03  /* Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /* Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /* Read identification */
#define sFLASH_CMD_SE             0x20  /* Sector Erase instruction */
#define sFLASH_CMD_32KBE          0x52  /* Sector block instruction */
#define sFLASH_CMD_64KBE          0xD8  /* Sector block instruction */
#define sFLASH_CMD_BE             0x60	//0xC7  /* chip Erase instruction */
#define sFLASH_WIP_FLAG           0x01  /* Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0xFF
#define sFLASH_SPI_PAGESIZE       0x100		//256 bytes
#define sFLASH_SPI_SECTOR_SIZE    0x1000	//4K bytes
#define sFLASH_SPI_BLOCK_SIZE     0x10000	//64K bytes
#define sFLASH_SPI_CHIP_SIZE      0x800000      //8M

#define W25QXX_TOTAL_BLOCK          (sFLASH_SPI_CHIP_SIZE / sFLASH_SPI_BLOCK_SIZE)
#define W25QXX_SECTOR_OF_BLOCK      (sFLASH_SPI_BLOCK_SIZE / sFLASH_SPI_SECTOR_SIZE)
#define W25QXX_PAGE_OF_SECTOR       (sFLASH_SPI_SECTOR_SIZE / sFLASH_SPI_PAGESIZE)
#define W25QXX_PAGE_SIZE            sFLASH_SPI_PAGESIZE
    
#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P64_ID          0x202017
#define sFLASH_W25Q16FV_ID        0xEF4015
#define sFLASH_W25Q32FV_ID        0xEF4016
#define sFLASH_W25Q64FV_ID        0xEF4017
#define sFLASH_W25Q128FV_ID       0xEF4018
  
/* flash section define ------------------------------------------------------*/
#define NORFLASHE_ADDR_PICTURE		(1024*256)  //图片资源的起始地址0x40000
#define NORFLASHE_ADDR_CONFIG 		(0)			//配置信息的其实地址
#define NORFLASHE_ADDR_LOGO			(1024*4)    //开机动画的地址0x1000
/* Exported functions ------------------------------------------------------- */

/* High layer functions  */
void sFLASH_DeInit(void);
extern void sFLASH_Init(void);
extern void sFLASH_EraseSector(uint32_t SectorAddr);
extern void sFLASH_EraseBlock(uint32_t SectorAddr);
extern void sFLASH_EraseChip(void);
extern void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
extern void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
extern void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
extern uint32_t sFLASH_ReadID(void);
extern void sFLASH_StartReadSequence(uint32_t ReadAddr);
extern void spiFlashTest(void);

/* Low layer functions */
extern uint8_t sFLASH_ReadByte(void);
extern uint8_t sFLASH_SendByte(uint8_t byte);
extern uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);
extern void sFLASH_WriteEnable(void);
extern void sFLASH_WaitForWriteEnd(void);

#endif 

