#ifndef MSPIO_H_
#define MSPIO_H_

#include <stdio.h>
#include <Hardware/UART_Driver.h>

void MSPrintf(uint32_t UART, const char *fs, ...);
int MSPgets(uint32_t UART, char *b, int size);

#endif /* MSPIO_H_ */
