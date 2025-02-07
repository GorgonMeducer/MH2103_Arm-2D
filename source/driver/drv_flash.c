/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.11.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#include "inc.h"
#include "drv_flash.h"

#undef SPI_SendData
#define sFLASH_SPI                           SPI3

#define SPI_SendData(SPIx,data)				(SPIx->DR = data)
#define SPI_GetData(SPIx)					((uint8_t)SPIx->DR)
#define SPI_FlagIsReset(SPIx,flag)			((SPIx->SR & flag) ? 0 : 1)

/* Select sFLASH: Chip Select pin low */
#define sFLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_6)
/* Deselect sFLASH: Chip Select pin high */
#define sFLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_6)

//复用jtag功能，flash的spi共用
void JtagRemap(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
//remap
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //PA13,PA14 for sw-dp,pa15,pb3,pb4 for io	
}

void sFLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStruct;

	JtagRemap();
  SPI_Cmd(sFLASH_SPI, DISABLE);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE); //spi3 at apb1

	//configure sck,di,do
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_5; //clk,miso,mosi
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; //cs
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  sFLASH_CS_HIGH();

  /*!< SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(sFLASH_SPI, &SPI_InitStructure);

   SPI_Cmd(sFLASH_SPI, ENABLE);
#ifdef DEBUG
  {
    UINT32 id;
   	
	  id = sFLASH_ReadID(); //flash复位好像没有完成，第一次读会失败
   id = sFLASH_ReadID();
	DebugPrintf("Flash id: %02X(%d)\r\n",id,id);
  }
#endif  
}

/**
  * @brief  Erases the specified FLASH sector.4KB
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void sFLASH_EraseSector(uint32_t SectorAddr)
{
	/*!< Send write enable instruction */
	sFLASH_WriteEnable();
	sFLASH_WaitForWriteEnd();


	sFLASH_CS_LOW();	
	sFLASH_SendByte(sFLASH_CMD_SE);
	sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	sFLASH_SendByte(SectorAddr & 0xFF);
	sFLASH_CS_HIGH();

	sFLASH_WaitForWriteEnd();
}

/**
  * @brief  Erases the specified FLASH block.64KB
  * @param  SectorAddr: address of the block to erase.
  * @retval None
  */
void sFLASH_EraseBlock(uint32_t BlockAddr)
{
	sFLASH_WriteEnable();
	sFLASH_WaitForWriteEnd();

	sFLASH_CS_LOW();	
	sFLASH_SendByte(sFLASH_CMD_64KBE);
	sFLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
	sFLASH_SendByte((BlockAddr & 0xFF00) >> 8);
	sFLASH_SendByte(BlockAddr & 0xFF);
	sFLASH_CS_HIGH();

	sFLASH_WaitForWriteEnd();

}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_EraseBulk(void)
{
  /*!< Send write enable instruction */
	sFLASH_WriteEnable();
	sFLASH_WaitForWriteEnd();
	
	sFLASH_CS_LOW();
	sFLASH_SendByte(sFLASH_CMD_BE);
	sFLASH_CS_HIGH();
	sFLASH_WaitForWriteEnd();

}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	DISABLE_INTERRUPT();
	
	sFLASH_WriteEnable();
	sFLASH_WaitForWriteEnd();

	sFLASH_CS_LOW();
	sFLASH_SendByte(sFLASH_CMD_WRITE);
	sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	sFLASH_SendByte(WriteAddr & 0xFF);

	while (NumByteToWrite--)
	{
        sFLASH_SendByte(*pBuffer);
        pBuffer++;
	}
	sFLASH_CS_HIGH();
	sFLASH_WaitForWriteEnd();
	ENABLE_INTERRUPT();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	UINT32 offset,firstbytes,page,endbytes;

	offset = WriteAddr % sFLASH_SPI_PAGESIZE;
	if(offset)	//非对齐地址，先写这一页数据
	{
		firstbytes = sFLASH_SPI_PAGESIZE - offset;	//data in current page
		if(firstbytes >= NumByteToWrite)
			firstbytes = NumByteToWrite;
		sFLASH_WritePage(pBuffer,WriteAddr,firstbytes);
		NumByteToWrite -= firstbytes;	//remain data
		pBuffer += firstbytes;		//set data offset
		WriteAddr += firstbytes;		//set address
	}
	if(NumByteToWrite) //
	{
		if(NumByteToWrite > sFLASH_SPI_PAGESIZE)	//over one page
		{
			page = NumByteToWrite / sFLASH_SPI_PAGESIZE;
			endbytes = NumByteToWrite - sFLASH_SPI_PAGESIZE * page;
			while(page--)
			{
				sFLASH_WritePage(pBuffer,WriteAddr,sFLASH_SPI_PAGESIZE);		//one page
				pBuffer += sFLASH_SPI_PAGESIZE;	
				WriteAddr += sFLASH_SPI_PAGESIZE;
			}
			if(endbytes)
				sFLASH_WritePage(pBuffer,WriteAddr,endbytes);
		}
		else //Less than a page or one page
		{
				sFLASH_WritePage(pBuffer,WriteAddr,NumByteToWrite);
		}
	}
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	sFLASH_CS_LOW();

	sFLASH_SendByte(sFLASH_CMD_READ);

	sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
	sFLASH_SendByte(ReadAddr & 0xFF);

	while (NumByteToRead--)
	{
        *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
        pBuffer++;
	}
	sFLASH_CS_HIGH();
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint32_t sFLASH_ReadID(void)
{
  uint32_t Temp, Temp0, Temp1, Temp2;

	sFLASH_CS_LOW();

	DISABLE_INTERRUPT();
	sFLASH_SendByte(0x9F);
	Temp0 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	Temp1 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	Temp2 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	
	ENABLE_INTERRUPT();
	sFLASH_CS_HIGH();
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}


/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sFLASH_SendByte(uint8_t byte)
{
	volatile uint32_t i; //-Ofast,不使用microlib会被编译器优化

	i = 0;
  while ((SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_TXE) == RESET) && (i++ < 5000)) __NOP(); //set timeout

  SPI_SendData(sFLASH_SPI, byte);

	i = 0;
  while ((SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET) && (i++ < 5000)) __NOP();//set timeout

  return (uint8_t)SPI_GetData(sFLASH_SPI);
}


/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_WriteEnable(void)
{
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_WREN);
  sFLASH_CS_HIGH();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void sFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_RDSR);
  do
  {
    flashstatus = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  }
  while ((flashstatus & sFLASH_WIP_FLAG) == SET);
  sFLASH_CS_HIGH();
}

