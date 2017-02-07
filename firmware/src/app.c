/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void APP_Initialize ( void )
{
    /* Set the direction of the ChipKit GPIO pins 40-47 to output for the dbgOutputVal function */
    PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_A, 0x0400);
    PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_B, 0x3800);
    PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_D, 0x0040);
    PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_F, 0x0003);
    PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_G, 0x0100);
    
    /* Set the direction of the ChipKit GPIO pins 30-37 to output for the dbgOutputLoc function */
    PLIB_PORTS_DirectionOutputSet (PORTS_ID_0, PORT_CHANNEL_E, 0x00FF);
    
    DRV_TMR0_Initialize ();
    QInitialize(); 
}

void APP_Tasks ( void )
{
    dbgOutputLoc (ENTER_TASK); 
    DRV_TMR0_Start(); 
    dbgOutputLoc (BEFORE_WHILE);
    while (1) {
        dbgOutputLoc(BEFORE_RECEIVE); 
        unsigned char value = Q_Receive ();
        dbgOutputLoc (AFTER_RECEIVE);
        dbgOutputVal(value);
        dbgUARTVal(value);
    }
}

QueueHandle_t Q_Mile1;

void QInitialize ()
{
    Q_Mile1 = xQueueCreate (20, 1); 
    if (Q_Mile1 == 0) {
        error();
    }
}

void ISRQ_Send (unsigned char in)
{
    /* Check for whether the task needs to do a context switch */
    BaseType_t conSwitch;
    conSwitch = pdFALSE;
    
    /* Send to the Queue from an ISR */
    if ((xQueueSendFromISR (Q_Mile1, &in, &conSwitch)) == errQUEUE_FULL) {
        error();
    }
    
    /* Switch if needed */
   portEND_SWITCHING_ISR(conSwitch);
}

unsigned char Q_Receive ()
{
    unsigned char buffer;
    xQueueReceive (Q_Mile1, &buffer, portMAX_DELAY);
    return buffer;
}

/*******************************************************************************
 End of File
 */