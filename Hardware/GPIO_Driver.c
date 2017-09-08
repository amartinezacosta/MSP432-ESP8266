#include "GPIO_Driver.h"

void GPIO_Init(uint32_t GPIO_Port, uint16_t pins)
{
    MAP_GPIO_setAsOutputPin(GPIO_Port, pins);
}

void GPIO_High(uint32_t GPIO_Port, uint16_t pins)
{
    MAP_GPIO_setOutputHighOnPin(GPIO_Port, pins);
}

void GPIO_Low(uint32_t GPIO_Port, uint16_t pins)
{
    MAP_GPIO_setOutputLowOnPin(GPIO_Port, pins);
}

