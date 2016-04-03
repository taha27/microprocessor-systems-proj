#include "gpio_config.h"

/**
	 * @brief This function configures the GPIO peripherals: it declares the necessary structures,   
	 * sets up the GPIO peripheral clock, defines the pinout number and other pertinent configurations
   * @param void
   * @retval void
   */	
void GPIO_Config(void){
	
	/* Configure the GPIO peripheral clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	/* Initializes GPIO PORTS(A, E, C & D) and specifies pin characteristics */
	GPIOA_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOA_Init.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8;
	GPIOA_Init.Pull = GPIO_NOPULL;
	GPIOA_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOA, &GPIOA_Init);
	
	GPIOE_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOE_Init.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIOE_Init.Pull = GPIO_NOPULL;
	GPIOE_Init.Speed = GPIO_SPEED_FREQ_HIGH;			
	HAL_GPIO_Init(GPIOE, &GPIOE_Init);
	
	GPIOD_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOD_Init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIOD_Init.Pull = GPIO_NOPULL;
	GPIOD_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOD, &GPIOD_Init);		
}
