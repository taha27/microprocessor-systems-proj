#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

extern SPI_HandleTypeDef SpiHandle_2;

#define FLAG_TIMEOUT         ((uint32_t)0x1000)

#define 																 HAL_SPI_MODULE_ENABLED
#define NUCLEO_SPI_AF                		 GPIO_AF5_SPI2

#define NUCLEO_SPI_GPIO_PORT         		 GPIOB
#define NUCLEO_SPI_SCK_PIN               GPIO_PIN_13
#define NUCLEO_SPI_MISO_PIN              GPIO_PIN_14
#define NUCLEO_SPI_MOSI_PIN              GPIO_PIN_15

#define NUCLEO_SPI_CS_PIN                GPIO_PIN_12

#define SPI_CS_LOW()       HAL_GPIO_WritePin(NUCLEO_SPI_GPIO_PORT, NUCLEO_SPI_CS_PIN, GPIO_PIN_RESET)
#define SPI_CS_HIGH()      HAL_GPIO_WritePin(NUCLEO_SPI_GPIO_PORT, NUCLEO_SPI_CS_PIN, GPIO_PIN_SET)

#define DUMMY_BYTE                 ((uint8_t)0x00)
#define READWRITE_CMD              ((uint8_t)0x80)
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)

void SPI_Init(void);
void SPI_Read(uint8_t* pBuffer, uint16_t NumByteToRead);
void SPI_Write(uint8_t* pBuffer, uint16_t NumByteToWrite);

uint8_t SendByte_SPI(uint8_t byte);
extern void SPI_SendData(SPI_HandleTypeDef *hspi, uint32_t Data);
extern uint32_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);

