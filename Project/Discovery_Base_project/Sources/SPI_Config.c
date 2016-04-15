#include "SPI_Config.h"

__IO uint32_t  Timeout = FLAG_TIMEOUT;

void SPI_Init(){
	
	/* Handle structure declarations */ 
	GPIO_InitTypeDef GPIO_InitStructure_2;
	

	/* Enable SPIx and GPIOx interface clock */ 
	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Configure SPI pins as alternate function push-pull */ 
	
	/* SPI SCK pin configuration */
  GPIO_InitStructure_2.Pin = NUCLEO_SPI_SCK_PIN;
	GPIO_InitStructure_2.Alternate = NUCLEO_SPI_AF;
	GPIO_InitStructure_2.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure_2.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure_2.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(NUCLEO_SPI_GPIO_PORT, &GPIO_InitStructure_2);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure_2.Pin =  NUCLEO_SPI_MOSI_PIN;
	GPIO_InitStructure_2.Alternate = NUCLEO_SPI_AF;
	GPIO_InitStructure_2.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure_2.Pull = GPIO_NOPULL;
	GPIO_InitStructure_2.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(NUCLEO_SPI_GPIO_PORT, &GPIO_InitStructure_2);

  /* SPI MISO pin configuration */
  GPIO_InitStructure_2.Pin = NUCLEO_SPI_MISO_PIN;
	GPIO_InitStructure_2.Alternate = NUCLEO_SPI_AF;
	GPIO_InitStructure_2.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure_2.Pull = GPIO_NOPULL;
	GPIO_InitStructure_2.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(NUCLEO_SPI_GPIO_PORT, &GPIO_InitStructure_2);
	
	/* Configure SPI nSS pin as output push-pull */ 
	GPIO_InitStructure_2.Pin = NUCLEO_SPI_CS_PIN;
	GPIO_InitStructure_2.Pull = GPIO_PULLUP;
	GPIO_InitStructure_2.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure_2.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(NUCLEO_SPI_GPIO_PORT, &GPIO_InitStructure_2);
	
	/* Deselect : Chip Select high */
  // HAL_GPIO_WritePin(NUCLEO_SPI_GPIO_PORT, NUCLEO_SPI_CS_PIN, GPIO_PIN_SET);
		
	/* SPI handle configuration */ 
	HAL_SPI_DeInit(&SpiHandle_2);
  SpiHandle_2.Instance 							  = SPI2;
  SpiHandle_2.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_256;
  SpiHandle_2.Init.Direction 					= SPI_DIRECTION_2LINES;
  SpiHandle_2.Init.CLKPhase 					= SPI_PHASE_1EDGE;
  SpiHandle_2.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  SpiHandle_2.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  SpiHandle_2.Init.CRCPolynomial 			= 7;
  SpiHandle_2.Init.DataSize 					= SPI_DATASIZE_8BIT;
  SpiHandle_2.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  SpiHandle_2.Init.NSS 								= SPI_NSS_SOFT;
  SpiHandle_2.Init.TIMode 						= SPI_TIMODE_DISABLED;
  SpiHandle_2.Init.Mode 							= SPI_MODE_SLAVE;
	if(HAL_SPI_Init(&SpiHandle_2) != HAL_OK) printf ("ERROR: Error in initialising SPI1 \n");
  
	__HAL_SPI_ENABLE(&SpiHandle_2);		
}

/**
  * @brief  Reads a block of data from the LIS3DSH.
  * @param  pBuffer : pointer to the buffer that receives the data read from the LIS3DSH.
  * @param  ReadAddr : LIS3DSH's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the LIS3DSH.
  * @retval None
  */
void SPI_Read(uint8_t* pBuffer, uint16_t NumByteToRead){
	while(NumByteToRead > 0x00){
		if(__HAL_SPI_GET_FLAG(&SpiHandle_2, SPI_FLAG_RXNE) == SET){
			if(HAL_GPIO_ReadPin(NUCLEO_SPI_GPIO_PORT, NUCLEO_SPI_CS_PIN) == GPIO_PIN_RESET){
				*pBuffer = SPI_ReceiveData(&SpiHandle_2);
				NumByteToRead--;
				pBuffer++;
			}
		}
	}
}

/**
  * @brief  Writes one byte to the LIS3DSH.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the LIS3DSH.
  * @param  WriteAddr : LIS3DSH's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void SPI_Write(uint8_t* pBuffer, uint16_t NumByteToWrite){  
	while(NumByteToWrite >= 0x01){
		if(__HAL_SPI_GET_FLAG(&SpiHandle_2, SPI_FLAG_TXE) == SET){
			// printf("checkpoint 1\n");
			if(HAL_GPIO_ReadPin(NUCLEO_SPI_GPIO_PORT, NUCLEO_SPI_CS_PIN) == GPIO_PIN_RESET){
				// printf("checkpoint 2");
				SPI_SendData(&SpiHandle_2, *pBuffer);
				printf("%u ", *pBuffer);
				NumByteToWrite--;
				pBuffer++;
			}
		}
	}
	printf("\n");
}


