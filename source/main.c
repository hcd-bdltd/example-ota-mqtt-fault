/******************************************************************************
* File Name: main.c
*
* Description: This code example demonstrates OTA update with PSoC 6 MCU and
* CYW43xxx connectivity devices. The device establishes a connection with the
* designated MQTT Broker (Mosquitto is used in this example) and subscribes to
* a topic. It periodically checks the job document to see if a new update is
* available. When a new update is available, it will be downloaded and written
* to the secondary slot. On the next reboot, MCUBoot will copy the new image
* over to the primary slot and run the application.
*
* Related Document: See Readme.md
********************************************************************************
* Copyright 2020-2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/* Header file includes */
#include "cy_log.h"
#include "cy_retarget_io.h"
#include "cybsp.h"
#include "cyhal.h"

#include "led_task.h"
#include "ota_task.h"

/* FreeRTOS header file */
#include <FreeRTOS.h>
#include <task.h>

/*******************************************************************************
* Macros
********************************************************************************/
/* OTA task configurations */
#define OTA_TASK_STACK_SIZE                 (1024 * 6)
#define OTA_TASK_PRIORITY                   (configMAX_PRIORITIES - 3)

/* OTA task configurations */
#define LED_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE)
#define LED_TASK_PRIORITY                   (configMAX_PRIORITIES - 3)

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void configure_fault_register(void);

/*******************************************************************************
* Global Variables
********************************************************************************/
/* LED task handle */
TaskHandle_t led_task_handle;

/* OTA task handle */
TaskHandle_t ota_task_handle;

/*******************************************************************************
 * Function Name: main
 ********************************************************************************
 * Summary:
 *  System entrance point. This function sets up OTA task and starts
 *  the RTOS scheduler.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  int
 *
 *******************************************************************************/
int main(void)
{
    cy_rslt_t result = CY_RSLT_TYPE_ERROR;

    /* Prevent the WDT from timing out and resetting the device. */
    /* Watchdog timer started by the bootloader */
    cyhal_wdt_kick(NULL);

    /* Initialize the board support package */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);

    /* Retarget-io init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* To avoid compiler warning */
    (void)result;

    /* Enable global interrupts. */
    __enable_irq();

    /* default for all logging to WARNING */
    cy_log_init(CY_LOG_WARNING, NULL, NULL);

    configure_fault_register();

    printf("\r===============================================================\n");
    printf("TEST Application: OTA Update version: %d.%d.%d\n",
            APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
    printf("===============================================================\n\n");

    /* Update watchdog timer to mark successful start up of application */
    /* Watchdog timer started by the bootloader */
    cyhal_wdt_free(NULL);
    printf("\nWatchdog timer started by the bootloader is now turned off!!!\n\n");

    /* Create the tasks */
    xTaskCreate(led_task, "LED", LED_TASK_STACK_SIZE, NULL,
                LED_TASK_PRIORITY, &led_task_handle);
    xTaskCreate(ota_task, "OTA", OTA_TASK_STACK_SIZE, NULL,
                OTA_TASK_PRIORITY, &ota_task_handle);

    /* Start the FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Should never get here. */
    CY_ASSERT(0);
}

/*******************************************************************************
* Function Name: configure_fault_register
********************************************************************************
* Summary:
*  This function configures the fault registers(bus fault and usage fault). See
*  the Arm documentation for more details on the registers.
*
*******************************************************************************/
static void configure_fault_register(void)
{
    /* Set SCB->SHCSR.BUSFAULTENA so that BusFault handler instead of the
     * HardFault handler handles the BusFault.
     */
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;

    /* If ACTLR.DISDEFWBUF is not set to 1, the imprecise BusFault will occur.
     * For the imprecise BusFault, the fault stack information won't be accurate.
     * Setting ACTLR.DISDEFWBUF bit to 1 so that bus faults will be precise.
     * Refer Arm documentation for detailed explanation on precise and imprecise
     * BusFault.
     * WARNING: This will decrease the performance because any store to memory
     * must complete before the processor can execute the next instruction.
     * Don't enable always, if it is not necessary.
     */
    SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk;

    /* Enable UsageFault when processor executes an divide by 0 */
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;

    /* Set SCB->SHCSR.USGFAULTENA so that faults such as DIVBYZERO, UNALIGNED,
     * UNDEFINSTR etc are handled by UsageFault handler instead of the HardFault
     * handler.
     */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
}

/*******************************************************************************
* Function Name: void Cy_SysLib_ProcessingFault(void)
********************************************************************************
* Summary:
*  This function prints out the stack register at the moment the hard fault
*  occurred. cy_syslib.c defines this as a  __WEAK function, so this function
*  replaces the weak function. Cy_SysLib_ProcessingFault() is called at the
*  end of Cy_SysLib_FaultHandler() function, which is the default exception
*  handler set for hard faults.
*
*******************************************************************************/
void Cy_SysLib_ProcessingFault(void)
{
    printf("\r\nCM4 FAULT!!\r\n");
    printf("SCB->CFSR = 0x%08lx\r\n", (unsigned long) cy_faultFrame.cfsr.cfsrReg);

    /* If MemManage fault valid bit is set to 1, print MemManage fault address */
    if ((cy_faultFrame.cfsr.cfsrReg & SCB_CFSR_MMARVALID_Msk)
            == SCB_CFSR_MMARVALID_Msk)
    {
        printf("MemManage Fault! Fault address = 0x%08lx\r\n", (unsigned long)SCB->MMFAR);
    }

    /* If Bus Fault valid bit is set to 1, print BusFault Address */
    if ((cy_faultFrame.cfsr.cfsrReg & SCB_CFSR_BFARVALID_Msk)
            == SCB_CFSR_BFARVALID_Msk)
    {
        printf("Bus Fault! \r\nFault address = 0x%08lx\r\n", (unsigned long)SCB->BFAR);
    }

    /* Print Fault Frame */
    printf("r0 = 0x%08lx\r\n", (unsigned long)cy_faultFrame.r0);
    printf("r1 = 0x%08lx\r\n", (unsigned long)cy_faultFrame.r1);
    printf("r2 = 0x%08lx\r\n", (unsigned long)cy_faultFrame.r2);
    printf("r3 = 0x%08lx\r\n", (unsigned long)cy_faultFrame.r3);
    printf("r12 = 0x%08lx\r\n", (unsigned long)cy_faultFrame.r12);
    printf("lr = 0x%08lx\r\n", (unsigned long)cy_faultFrame.lr);
    printf("pc = 0x%08lx\r\n", (unsigned long)cy_faultFrame.pc);
    printf("psr = 0x%08lx\r\n", (unsigned long)cy_faultFrame.psr);

    while (1);
}

/* [] END OF FILE */
