/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <string.h>


/*******************************************************************************
* Function Name: _write
********************************************************************************
*
* Summary: 
*   This version of _write sends text via UART. printf is redirected to 
*   this function when GCC compiler is used to print data to terminal using UART. 
*   Note that depending on your compiler you may need to rewrite a different
*   function that is linked to printf.
* 
* Parameters:
*   file: This variable is not used.
*   *ptr: Pointer to the data which will be transfered through UART.
*   len: Length of the data to be transfered through UART.
* 
* Returns: 
*   returns the number of characters transferred using UART.
*
*******************************************************************************/
#if defined (__GNUC__) 
    
    /* Add an explicit reference to the floating point printf library to allow 
       use of floating point conversion specifier. */     
    asm (".global _printf_float"); 
 
    /* For GCC compiler revise _write() function for printf functionality */     
	int _write(int file, char *ptr, int len) 
    {       
        /* Warning suppression for unused function parameter */
		file = file;
		
		int i; 
        /* Character is sent via UART */
        for (i = 0; i < len; i++) 
        { 
            UART_PORT_UartPutChar(*(ptr++)); 
        }          
        return(len); 
    } 
#endif /* (__GNUC__) */

void print_welcome_message()
{
    UART_PORT_UartPutString("\r\n**********************************************************************************\r\n");
    UART_PORT_UartPutString("If you see this text the terminal connection is configured correctly.\r\n");
    UART_PORT_UartPutString("This program sends entered data to connected i2c device\r\n");    
    UART_PORT_UartPutString("Enter command string in format.\r\n");
    UART_PORT_UartPutString("<command><address><data>\r\n");
    UART_PORT_UartPutString("Where\r\n");
    UART_PORT_UartPutString("\t <command>: s to send data or r to receive data,\r\n");
    UART_PORT_UartPutString("\t <address> device address - 2 hex digits,\r\n");
    UART_PORT_UartPutString("\t <data> data to send/receive - 2 hex digits.\r\n");
    UART_PORT_UartPutString("\r\n");

}
const char LETTER_S = 115;
const uint32 delay_time = 1000;
char buffer[80];
int operation_code_received(char received_symbol)
{
    sprintf(buffer,"Received symbol: %c  \r\n",received_symbol);
    UART_PORT_UartPutString(buffer);
    return (received_symbol == LETTER_S);
}
int send_operation(char op_code)
{
    return (op_code == LETTER_S);
}

uint32 get_address()
{
    uint32 byte_1;
    uint32 byte_2;
    byte_1 = UART_PORT_UartGetByte();
    sprintf(buffer,"Received symbol: %lx  \r\n",byte_1);
    UART_PORT_UartPutString(buffer);
    byte_2 = UART_PORT_UartGetByte();
    sprintf(buffer,"Received symbol: %lx  \r\n",byte_2);
    UART_PORT_UartPutString(buffer);
    return (byte_1*16 + byte_2);
}

uint32 get_data()
{
    
    return get_address();
}

int main(void)
{
    char operation;
    
    uint32 address;
    uint8 data;   
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_PORT_Start();
    print_welcome_message();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        operation = UART_PORT_UartGetChar();
        /* Place your application code here. */
        if (operation_code_received(operation))
        {
            address = get_address();
            if (send_operation(operation))
            {
                data = get_data();
                I2C_PORT_I2CMasterWriteBuf(address,&data,1,0);
            }
            else
            {
                data = I2C_PORT_I2CMasterReadBuf(address,&data,1,0);
                sprintf(buffer,"Received data: %x \r\n",data);
                UART_PORT_UartPutString(buffer);
            }        
        
        }
        CyDelay(delay_time);
    }
}

/* [] END OF FILE */
