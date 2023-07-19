#include "led_bsp.h"
#include "gpio.h"

static led_bsp_gpio_t* led_ports[] = {
  GPIOC,
	GPIOC,	
};

static uint16_t led_pins[] = {
	GPIO_PIN_14, 
	GPIO_PIN_13,  
};

led_bsp_t led_bsp;

void led_bsp_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* LED GPIO Periph clock enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	for (int i=0; i<sizeof(led_ports)/sizeof(led_ports[0]); i++) 
	{
		GPIO_InitStruct.Pin = led_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(led_ports[i], &GPIO_InitStruct);
	}
}

void led_bsp_on(led_bsp_gpio_t *port, uint16_t pin)
{
	HAL_GPIO_WritePin(port, pin, led_set);
}

void led_bsp_off(led_bsp_gpio_t *port, uint16_t pin)
{
	HAL_GPIO_WritePin(port, pin, led_reset);
}

void led_bsp_toggle(led_bsp_gpio_t *port, uint16_t pin)
{
	HAL_GPIO_TogglePin(port, pin);
}

