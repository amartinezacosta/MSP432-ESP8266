#include <Hardware/CS_Driver.h>
#include <Devices/MSPIO.h>
#include <Devices/ESP8266.h>

/* VCC = 3.3V
 * GND = GND
 * TX = P3.2
 * RX = P3.3
 * RST = P6.1
 * CH_PD = 3.3V */


/*We are connecting to api.themoviedb.org, ESP8266 can resolve DNS, pretty cool huh!*/
char HTTP_WebPage[] = "api.themoviedb.org";
/*HTTP is TCP port 80*/
char Port[] = "80";
/*Data that will be sent to the HTTP server. This will allow us to query movie data. Get an api key from api.themoviedb.org*/
char HTTP_Request[] = "GET /3/search/movie?api_key=yourkey&query=a+beautiful+mind HTTP/1.0\r\n\r\n";
/*Subtract one to account for the null character*/
uint32_t HTTP_Request_Size = sizeof(HTTP_Request) - 1;


/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
eUSCI_UART_Config UART0Config =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     13,
     0,
     37,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
eUSCI_UART_Config UART2Config =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     13,
     0,
     37,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};

void main(void)
{
	MAP_WDT_A_holdTimer();

    /*MSP432 Running at 24 MHz*/
	CS_Init();

	/*Initialize required hardware peripherals for the ESP8266*/
	UART_Init(EUSCI_A0_BASE, UART0Config);
	UART_Init(EUSCI_A2_BASE, UART2Config);
    GPIO_Init(GPIO_PORT_P6, GPIO_PIN1);

    MAP_Interrupt_enableMaster();

    /*Pointer to ESP8266 global buffer*/
    char *ESP8266_Data = ESP8266_GetBuffer();

    /*Hard Reset ESP8266*/
    ESP8266_HardReset();
    __delay_cycles(48000000);
    /*flush reset data, we do this because a lot of data received cannot be printed*/
    UART_Flush(EUSCI_A2_BASE);

    /*Check UART connection to MSP432*/
    if(!ESP8266_CheckConnection())
    {
        MSPrintf(EUSCI_A0_BASE, "Failed MSP432 UART connection\r\n");
        /*Trap MSP432 if failed connection*/
        while(1);
    }

    MSPrintf(EUSCI_A0_BASE, "Nice! We are connected to the MSP432\r\n\r\n");
    MSPrintf(EUSCI_A0_BASE, "Checking available Access Points\r\n\r\n");

    /*Check available Access Points*/
    if(!ESP8266_AvailableAPs())
    {
        MSPrintf(EUSCI_A0_BASE, "Failed to obtain Access Points\n\r ERROR: %s \r\n", ESP8266_Data);
        /*Trap MSP432 if failed to obtain access points*/
        while(1);
    }

    /*Print received Access Points Data to console*/
    MSPrintf(EUSCI_A0_BASE, "Got it! Here are the available Access Points: %s\r\n\r\n", ESP8266_Data);

    /*Connect to Access Point if necessary here*/
    //if(!ESP8266_ConnectToAP(YOUR_SSID, YOUR_SSID_PASSWORD))
    //{
        /*Trap MSP432 we are not connected*/
       //while(1);
    //}


    /*Enable multiple connections, up to 4 according to the internet*/
    if(!ESP8266_EnableMultipleConnections(true))
    {
        MSPrintf(EUSCI_A0_BASE, "Failed to set multiple connections\r\n");
        while(1);
    }

    MSPrintf(EUSCI_A0_BASE, "Multiple connections enabled\r\n\r\n");

    /*Try to establish TCP connection to a HTTP server*/
    if(!ESP8266_EstablishConnection('0', TCP, HTTP_WebPage, Port))
    {
        MSPrintf(EUSCI_A0_BASE, "Failed to connect to: %s\r\nERROR: %s\r\n", HTTP_WebPage, ESP8266_Data);
        while(1);
    }

    MSPrintf(EUSCI_A0_BASE, "Connected to: %s\r\n\r\n", HTTP_WebPage);

    /*Query data to connected HTTP server, in order to do this look for an API and request a key*/
    if(!ESP8266_SendData('0', HTTP_Request, HTTP_Request_Size))
    {
        MSPrintf(EUSCI_A0_BASE, "Failed to send: %s to %s \r\nError: %s\r\n", HTTP_Request, HTTP_WebPage, ESP8266_Data);
        while(1);
    }

    /*We succesfully sent the data to the HTTP server, we are now going to receive a lot of data from the ESP8266 (or depending
     * on what information you are querying, CHANGE ESP8266 BUFFER SIZE AND UARTA2 BUFFER SIZE, OTHERWISE YOU WILL LOSS DATA!!!!!).*/
    MSPrintf(EUSCI_A0_BASE, "Data sent: %s to %s\r\n\r\nESP8266 Data Received: %s\r\n", HTTP_Request, HTTP_WebPage, ESP8266_Data);

    while(1)
    {
        /*You can query data here by sending the HTTP request string every n seconds.
         * Be careful about the amount of times per second that you can query data from an API
         */
    }
}


