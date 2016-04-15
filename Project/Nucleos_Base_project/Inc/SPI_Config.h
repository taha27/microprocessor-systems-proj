#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"


#define 																 HAL_SPI_MODULE_ENABLED
#define NUCLEO_SPI_AF                		 GPIO_AF5_SPI2

#define NUCLEO_SPI_GPIO_PORT         		 GPIOB
#define NUCLEO_SPI_SCK_PIN               GPIO_PIN_13
#define NUCLEO_SPI_MISO_PIN              GPIO_PIN_14
#define NUCLEO_SPI_MOSI_PIN              GPIO_PIN_15

#define NUCLEO_SPI_CS_PIN                GPIO_PIN_9
#define NUCLEO_SPI_CS_GPIO_PORT          GPIOB    

#define SPI_CS_LOW()       							 HAL_GPIO_WritePin(NUCLEO_SPI_CS_GPIO_PORT, NUCLEO_SPI_CS_PIN, GPIO_PIN_RESET)
#define SPI_CS_HIGH()      							 HAL_GPIO_WritePin(NUCLEO_SPI_CS_GPIO_PORT, NUCLEO_SPI_CS_PIN, GPIO_PIN_SET)

#define DUMMY_BYTE                 			 ((uint8_t)0x00)
	
#define READWRITE_CMD										 ((uint8_t)0x80)
#define MULTIPLEBYTE_CMD								 ((uint8_t)0x40)

extern SPI_HandleTypeDef SpiHandle_2;


void SPI_Init(void);
static uint8_t SPI_SendByte(uint8_t byte);

extern void SPI_Read(uint8_t* pBuffer, uint16_t NumByteToRead);
extern void SPI_Write(uint8_t* Buffer, uint16_t NumByteToWrite);
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data);
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);
