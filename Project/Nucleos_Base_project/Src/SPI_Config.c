#include "SPI_Config.h"

SPI_HandleTypeDef SpiHandle_2;

//function prototypes
void SPI_Init(void);
static uint8_t SPI_SendByte(uint8_t byte);

void SPI_Read(uint8_t* pBuffer, uint16_t NumByteToRead);
void SPI_Write(uint8_t* Buffer, uint16_t NumByteToWrite);
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data);
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);

void SPI_Init(){

	/* Handle structure declarations */ 
	GPIO_InitTypeDef GPIO_InitStructure_2;

	/* Enable SPIx and GPIOx interface clock */ 
	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
		
	/* SPI SCK pin configuration */
  GPIO_InitStructure_2.Pin = NUCLEO_SPI_SCK_PIN;
	GPIO_InitStructure_2.Alternate = NUCLEO_SPI_AF;
	GPIO_InitStructure_2.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure_2.Pull = GPIO_NOPULL;
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
	GPIO_InitStructure_2.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure_2.Pull = GPIO_PULLUP;
	GPIO_InitStructure_2.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(NUCLEO_SPI_CS_GPIO_PORT, &GPIO_InitStructure_2);
	
	/* Deselect : Chip Select high */
  HAL_GPIO_WritePin(NUCLEO_SPI_CS_GPIO_PORT, NUCLEO_SPI_CS_PIN, GPIO_PIN_SET);
	
	/*  */ 
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
  SpiHandle_2.Init.Mode 							= SPI_MODE_MASTER;
	if(HAL_SPI_Init(&SpiHandle_2) != HAL_OK) printf ("ERROR: Error in initialising SPI2 \n");
  
	__HAL_SPI_ENABLE(&SpiHandle_2);		
}

static uint8_t SPI_SendByte(uint8_t byte){
	__IO uint32_t  LIS3DSHTimeout = 10000;
	while (__HAL_SPI_GET_FLAG(&SpiHandle_2, SPI_FLAG_TXE) == RESET){
    if((LIS3DSHTimeout--) == 0) return 0;
  }
  SPI_SendData(&SpiHandle_2, byte);
	
	LIS3DSHTimeout = 10000;
	while (__HAL_SPI_GET_FLAG(&SpiHandle_2, SPI_FLAG_RXNE) == RESET){
    if((LIS3DSHTimeout--) == 0){
			return 0;
		}
  }
	return SPI_ReceiveData(&SpiHandle_2);
}

void SPI_Read(uint8_t* pBuffer, uint16_t NumByteToRead){
	SPI_CS_LOW();
	while(NumByteToRead > 0x00){
			*pBuffer = SPI_SendByte(DUMMY_BYTE);
			NumByteToRead--;
			pBuffer++;
	}
	SPI_CS_HIGH();
	
}

void SPI_Write(uint8_t* Buffer, uint16_t NumByteToWrite){
	while(NumByteToWrite >= 0x01){
		SPI_SendByte(*Buffer);
		NumByteToWrite--;
		Buffer++;
	}
}
	



