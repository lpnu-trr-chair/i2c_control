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

int main(void)
{
    char operation;
    const char LETTER_S=115;
    char* MY_STR = "Received char:";
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_PORT_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_PORT_UartPutString("\r\n**********************************************************************************\r\n");
    UART_PORT_UartPutString("This example demonstrates UART operation with printf \r\n");
    UART_PORT_UartPutString("If you see this text the terminal connection is configured correctly.\r\n");
    UART_PORT_UartPutString("Push button to see printf function used to print on terminal.\r\n");
    UART_PORT_UartPutString("\r\n");
    for(;;)
    {
        operation = UART_PORT_UartGetChar();
        /* Place your application code here. */
        if (operation == LETTER_S)
        {
        UART_PORT_UartPutString("Received character: ");
        UART_PORT_UartPutChar(operation);
        UART_PORT_UartPutString("\r\n");
        //printf("Hello\r\n");
        
        //    printf("R");
        }
        CyDelay(1000);
    }
}

/* [] END OF FILE */
