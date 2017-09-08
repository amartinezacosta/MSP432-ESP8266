#ifndef HARDWARE_GPIO_DRIVER_H_
#define HARDWARE_GPIO_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

void GPIO_Init(uint32_t GPIO_Port, uint16_t pins);
void GPIO_High(uint32_t GPIO_Port, uint16_t pins);
void GPIO_Low(uint32_t GPIO_Port, uint16_t pins);


#endif /* HARDWARE_GPIO_DRIVER_H_ */
