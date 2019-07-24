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
#include <stdlib.h>
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
const char LETTER_SMALL_S = 115;
const uint32 delay_time = 1000;
char buffer[80];
int operation_code_received(char received_symbol)
{
    //sprintf(buffer,"Received symbol: %c  \r\n",received_symbol);
    //UART_PORT_UartPutString(buffer);
    return (received_symbol == LETTER_SMALL_S);
}
int send_operation(char op_code)
{
    return (op_code == LETTER_SMALL_S);
}


uint32 get_address()
{
    char bytes[2]="00";
    uint32 result;
    
    bytes[0] = UART_PORT_UartGetChar();
    sprintf(buffer,"Received symbol: %lx  \r\n",bytes[0]);
    UART_PORT_UartPutString(buffer);
    
    bytes[1] = UART_PORT_UartGetChar();
    sprintf(buffer,"Received symbol: %lx  \r\n",bytes[1]);
    UART_PORT_UartPutString(buffer);
    
    result = strtoul(bytes,NULL,16);
    sprintf(buffer,"Converted value: %lu  \r\n",result);
    UART_PORT_UartPutString(buffer); 
    return result;//(byte_1*16 + byte_2);
}

uint32 get_data()
{
    
    return get_address();
}

void print_error_status(uint32 error_status)
{
    switch(error_status)
    {
        
        case I2C_PORT_I2C_MSTR_NO_ERROR:
            sprintf(buffer,"Absent I2C error.  \r\n");
            break;
        case I2C_PORT_I2C_MSTR_BUS_BUSY:
            sprintf(buffer,"I2C error: I2C_MSTR_BUS_BUSY  \r\n");
            break;
        case I2C_PORT_I2C_MSTR_NOT_READY:
            sprintf(buffer,"I2C error: I2C_MSTR_NOT_READY  \r\n");
            break;
        case I2C_PORT_I2C_MSTR_ERR_LB_NAK:
            sprintf(buffer,"I2C error: I2C_MSTR_ERR_LB_NAK  \r\n");
            break;
        case I2C_PORT_I2C_MSTR_ERR_ARB_LOST:
            sprintf(buffer,"I2C error: I2C_MSTR_ERR_ARB_LOST  \r\n");
            break;
        case I2C_PORT_I2C_MSTR_ERR_BUS_ERR:
            sprintf(buffer,"I2C error: I2C_MSTR_ERR_BUS_ERR  \r\n");            
            break;
        case I2C_PORT_I2C_MSTR_ERR_ABORT_START:
            sprintf(buffer,"I2C error: I2C_MSTR_ERR_ABORT_START  \r\n");  
            break;
        case I2C_PORT_I2C_MSTR_ERR_TIMEOUT:
            sprintf(buffer,"I2C error: I2C_MSTR_ERR_TIMEOUT  \r\n");  
            break;
            
    }
    UART_PORT_UartPutString(buffer);
}

int main(void)
{
    char operation;
    
    uint32 address;
    uint8 data_1;
    uint8 data_2;
    uint32 err;
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_PORT_Start();
    I2C_PORT_Start();
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
                data_1 = get_data();
                data_2 = get_data();
                sprintf(buffer,"Start I2C data transfer \r\n");
                UART_PORT_UartPutString(buffer);
                err =I2C_PORT_I2CMasterSendStart(address,0,100);
                print_error_status(err);
                err = I2C_PORT_I2CMasterWriteByte(data_1,100);
                print_error_status(err);
                err = I2C_PORT_I2CMasterWriteByte(data_2,100);
                print_error_status(err);
                err = I2C_PORT_I2CMasterSendStop(1000);
                print_error_status(err);
                sprintf(buffer,"End of I2C data transfer  \r\n");
                UART_PORT_UartPutString(buffer);
            }
            else
            {
                data_1 = I2C_PORT_I2CMasterReadBuf(address,&data_1,1,0);
                sprintf(buffer,"Received data: %x \r\n",data_1);
                UART_PORT_UartPutString(buffer);
            }        
        
        }
        CyDelay(delay_time);
    }
}

/* [] END OF FILE */
