#include "UART_Driver.h"

/*UARTA0 Ring Buffer Global Variables*/
volatile uint8_t UARTA0Data[UARTA0_BUFFERSIZE];
volatile uint32_t UARTA0ReadIndex;
volatile uint32_t UARTA0WriteIndex;

#define UARTA0_ADVANCE_READ_INDEX          MAP_Interrupt_disableMaster(); UARTA0ReadIndex = (UARTA0ReadIndex + 1) % UARTA0_BUFFERSIZE; MAP_Interrupt_enableMaster();
#define UARTA0_ADVANCE_WRITE_INDEX         UARTA0WriteIndex = (UARTA0WriteIndex + 1) % UARTA0_BUFFERSIZE
#define UARTA0_BUFFER_EMPTY                UARTA0ReadIndex == UARTA0WriteIndex ? true : false
#define UARTA0_BUFFER_FULL                 (UARTA0WriteIndex + 1) % UARTA0_BUFFERSIZE == UARTA0ReadIndex ? true : false

/*UARTA2 Ring Buffer Global Variables*/
volatile uint8_t UARTA2Data[UARTA2_BUFFERSIZE];
volatile uint32_t UARTA2ReadIndex;
volatile uint32_t UARTA2WriteIndex;

#define UARTA2_ADVANCE_READ_INDEX          MAP_Interrupt_disableMaster(); UARTA2ReadIndex = (UARTA2ReadIndex + 1) % UARTA2_BUFFERSIZE; MAP_Interrupt_enableMaster();
#define UARTA2_ADVANCE_WRITE_INDEX         UARTA2WriteIndex = (UARTA2WriteIndex + 1) % UARTA2_BUFFERSIZE
#define UARTA2_BUFFER_EMPTY                UARTA2ReadIndex == UARTA2WriteIndex ? true : false
#define UARTA2_BUFFER_FULL                 (UARTA2WriteIndex + 1) % UARTA2_BUFFERSIZE == UARTA2ReadIndex ? true : false
#define UARTA2_AVAILABLE                   UARTA2WriteIndex - UARTA2ReadIndex

void UART_Init(uint32_t UART, eUSCI_UART_Config UARTConfig)
{
    switch(UART)
    {
    case EUSCI_A0_BASE:
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
        MAP_UART_initModule(UART, &UARTConfig);
        MAP_UART_enableModule(UART);
        MAP_UART_enableInterrupt(UART, EUSCI_A_UART_RECEIVE_INTERRUPT);
        MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
        break;
    case EUSCI_A2_BASE:
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
        MAP_UART_initModule(UART, &UARTConfig);
        MAP_UART_enableModule(UART);
        MAP_UART_enableInterrupt(UART, EUSCI_A_UART_RECEIVE_INTERRUPT);
        MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
        break;
    /*Add more UART modules initialization modules here*/
    default:
        break;
    }
}

void UART_Write(uint32_t UART, uint8_t *Data, uint32_t Size)
{
    uint32_t i;
    for(i = 0; i < Size; i++)
    {
        MAP_UART_transmitData(UART, Data[i]);
    }
}

uint32_t UART_Read(uint32_t UART, uint8_t *Data, uint32_t Size)
{
    uint32_t i;
    int8_t c;

    switch(UART)
    {
    case EUSCI_A0_BASE:
        for(i = 0; i < Size; i++)
        {
            if(UARTA0_BUFFER_EMPTY)
            {
                return i;
            }
            else
            {
                c = UARTA0Data[UARTA0ReadIndex];
                UARTA0_ADVANCE_READ_INDEX;

                Data[i] = c;
            }
        }
    break;

    case EUSCI_A2_BASE:
        for(i = 0; i < Size; i++)
        {
            if(UARTA2_BUFFER_EMPTY)
            {
                return i;
            }
            else
            {
                c = UARTA2Data[UARTA2ReadIndex];
                UARTA2_ADVANCE_READ_INDEX;

                Data[i] = c;
            }
        }
    break;
    /*More UART reading modules go here*/
    default:
        return 0;
    }

    return i;
}

uint32_t UART_Available(uint32_t UART)
{
    switch(UART)
    {
    case EUSCI_A0_BASE:
        return UARTA0WriteIndex - UARTA0ReadIndex;
    case EUSCI_A2_BASE:
        return UARTA2WriteIndex - UARTA2ReadIndex;
    /*More UART available modules go here*/
    default:
        return 0;
    }
}

void UART_Flush(uint32_t UART)
{
    switch(UART)
    {
    case EUSCI_A0_BASE:
        UARTA0WriteIndex = UARTA0ReadIndex = 0;
        break;
    case EUSCI_A2_BASE:
        UARTA2WriteIndex = UARTA2ReadIndex = 0;
        break;
    }
}

void EUSCIA0_IRQHandler(void)
{
    uint8_t c;
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        c = MAP_UART_receiveData(EUSCI_A0_BASE);

        if(UARTA0_BUFFER_FULL)
        {
            /*TODO: Buffer Overflow, add handler here*/
        }
        else
        {
            UARTA0Data[UARTA0WriteIndex] = c;
            UARTA0_ADVANCE_WRITE_INDEX;

            /*Transmit data only if it made it to the buffer*/
            MAP_UART_transmitData(EUSCI_A0_BASE, c);
        }
    }
}

void EUSCIA2_IRQHandler(void)
{
    uint8_t c;
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
    {
        c = MAP_UART_receiveData(EUSCI_A2_BASE);

        if(UARTA2_BUFFER_FULL)
        {
            /*TODO: Buffer Overflow, add handler here*/
        }
        else
        {
            UARTA2Data[UARTA2WriteIndex] = c;
            UARTA2_ADVANCE_WRITE_INDEX;
        }
    }
}
