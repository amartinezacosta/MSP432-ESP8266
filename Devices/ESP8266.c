#include "ESP8266.h"

#define RESET_PIN       GPIO_PIN1
#define RESET_PORT      GPIO_PORT_P6
#define AT              "AT\r\n"
#define AT_RST          "AT+RST\r\n"
#define AT_GMR          "AT+GMR\r\n"
#define AT_GSLP         "AT+GSLP\r\n"
#define ATE             "ATE\r\n"
#define AT_CWMODE       "AT+CWMODE\r\n"
#define AT_CWJAP        "AT+CWJAP"
#define AT_CWLAP        "AT+CWLAP\r\n"
#define AT_CWQAP        "AT+CWQAP\r\n"
#define AT_CWLIF        "AT+CWLIF\r\n"
#define AT_CWDHCP       "AT+CWDHCP\r\n"
#define AT_CIPSTAMAC    "AT+CIPSTAMAC\r\n"
#define AT_CIPAPMAC     "AT+CIPAPMAC\r\n"
#define AT_CIPSTA       "AT+CIPSTA\r\n"
#define AT_CIPAP        "AT+CIPAP"
#define AT_CIPSTATUS    "AT+CIPSTATUS\r\n"
#define AT_CIPSTART     "AT+CIPSTART"
#define AT_CIPSEND      "AT+CIPSEND"
#define AT_CIPCLOSE     "AT+CIPSEND\r\n"
#define AT_CIFSR        "AT+CIFSR\r\n"
#define AT_CIPMUX       "AT+CIPMUX"
#define AT_CIPSERVER    "AT+CIPSTO\r\n"
#define AT_CIPMODE      "AT+CIPMODE\r\n"
#define AT_CIPSTO       "AT+CIPSTO\r\n"
#define AT_CIUPDATE     "AT+CIUPDATE\r\n"
#define IPD             "+IPD\r\n"

char ESP8266_Buffer[ESP8266_BUFFER_SIZE];

bool ESP8266_WaitForAnswer(uint32_t Tries)
{
    uint32_t c;
    uint32_t i = 0;

    while(Tries)
    {
        if(UART_Available(EUSCI_A2_BASE)){
            while(UART_Available(EUSCI_A2_BASE)){
                UART_Read(EUSCI_A2_BASE, (uint8_t*)&c, 1);

                if(i > ESP8266_BUFFER_SIZE)
                {
                    return false;
                }
                else
                {
                    ESP8266_Buffer[i++] = c;
                }
            }

            ESP8266_Buffer[i++] = 0;
            return true;
        }
        Tries--;
        __delay_cycles(2400);
    }

    return false;
}

bool ESP8266_CheckConnection(void)
{
    MSPrintf(EUSCI_A2_BASE, AT);
    __delay_cycles(12000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

bool ESP8266_AvailableAPs(void)
{
    MSPrintf(EUSCI_A2_BASE, AT_CWLAP);
    __delay_cycles(48000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

bool ESP8266_ConnectToAP(char *SSID, char *Password)
{
    MSPrintf(EUSCI_A2_BASE, "%s=\"%s\",\"%s\"\r\n", AT_CWJAP, SSID, Password);

    __delay_cycles(12000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

bool ESP8266_EnableMultipleConnections(bool Enable)
{
    char c;

    switch(Enable)
    {
    case 0:
        c = '0';
        break;
    case 1:
        c = '1';
        break;
    }

    MSPrintf(EUSCI_A2_BASE, "%s=%c\r\n", AT_CIPMUX, c);

    __delay_cycles(12000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

bool ESP8266_EstablishConnection(char ID, uint8_t type, char *address, char *port)
{
    char ct[3];

    switch(type)
    {
    case TCP:
        ct[0] = 'T';
        ct[1] = 'C';
        ct[2] = 'P';
        break;
    case UDP:
        ct[0] = 'U';
        ct[1] = 'D';
        ct[2] = 'P';
        break;
    }

    MSPrintf(EUSCI_A2_BASE, "%s=%c,\"%s\",\"%s\",%s\r\n", AT_CIPSTART, ID, ct, address, port);

    __delay_cycles(24000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

bool ESP8266_SendData(char ID, char *Data, uint32_t DataSize)
{
    char size[5];

    ltoa(DataSize, size);
    MSPrintf(EUSCI_A2_BASE, "%s=%c,%s\r\n", AT_CIPSEND, ID, size);

    __delay_cycles(24000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, ">") == NULL)
    {
        return false;
    }

    MSPrintf(EUSCI_A2_BASE, Data);

    __delay_cycles(48000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

void ESP8266_Terminal(void)
{
    while(1)
    {
        MSPgets(EUSCI_A0_BASE, ESP8266_Buffer, 128);
        MSPrintf(EUSCI_A2_BASE, ESP8266_Buffer);

        __delay_cycles(48000000);
        if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))
        {
            MSPrintf(EUSCI_A0_BASE, "ESP8266 receive timeout error\r\n");
        }
        else
        {
            MSPrintf(EUSCI_A0_BASE, ESP8266_Buffer);
        }

    }
}

char *ESP8266_GetBuffer(void)
{
    return ESP8266_Buffer;
}

void ESP8266_HardReset(void)
{
    GPIO_Low(RESET_PORT, RESET_PIN);

    __delay_cycles(24000000);

    GPIO_High(RESET_PORT, RESET_PIN);
}
