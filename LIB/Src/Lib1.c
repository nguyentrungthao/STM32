#include "Lib1.h"

// int var_a;
// int var_b;

void blinkled(GPIO_TypeDef *port, uint16_t pin, uint16_t timeToggle) {
	static uint32_t tBlink = 0;
	if(HAL_GetTick() - tBlink >= timeToggle){
		tBlink = HAL_GetTick();
	    HAL_GPIO_TogglePin(port, pin);
	}
}
