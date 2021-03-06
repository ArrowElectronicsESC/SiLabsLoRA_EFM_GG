#include "em_device.h"
#include "em_chip.h"

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <bsp_os.h>
#include  "bsp.h"
#include  "bspconfig.h"
#include  "em_gpio.h"
#include  "pg_retargetswo.h"
#include  <cpu/include/cpu.h>
#include  <kernel/include/os.h>
#include  <kernel/include/os_trace.h>
#include  <common/include/common.h>
#include  <common/include/lib_def.h>
#include  <common/include/rtos_utils.h>
#include  <common/include/toolchains.h>

#include <stdio.h>

#include "../lorawan/LoRaWANInterface.h"
#include "../lorawan/system/lorawan_data_structures.h"
#include "../events/EventQueue.h"

// Application helpers
#include "trace.h"
#include "../lora_radio_helper.h"

using namespace events;

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  MAIN_START_TASK_PRIO              0u
#define  MAIN_START_TASK_STK_SIZE         512u

#define  LORA_TASK_PRIO                    2u
#define  LORA_TASK_STK_SIZE               2048u

#define  RADIO_TASK_PRIO                    1u
#define  RADIO_TASK_STK_SIZE               1024u


/*
 * Sets up an application dependent transmission timer in ms. Used only when Duty Cycling is off for testing
 */
#define TX_TIMER                        10000

/**
 * Maximum number of events for the event queue.
 * 10 is the safe number for the stack events, however, if application
 * also uses the queue for whatever purposes, this number should be increased.
 */
#define MAX_NUMBER_OF_EVENTS            10

/**
 * Maximum number of retries for CONFIRMED messages before giving up
 */
#define CONFIRMED_MSG_RETRY_COUNTER     3

#define DEBUG_BREAK           __asm__("BKPT #0");


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

                                                                /* Start Task Stack.                                    */
static  CPU_STK  MainStartTaskStk[MAIN_START_TASK_STK_SIZE];
                                                                /* Start Task TCB.                                      */
static  OS_TCB   MainStartTaskTCB;

static  CPU_STK  LoRaTaskStk[LORA_TASK_STK_SIZE];
                                                                /* LoRa Task TCB.                                      */
static  OS_TCB   LoRaTaskTCB;

static  CPU_STK  RadioTaskStk[RADIO_TASK_STK_SIZE];

static  OS_TCB   RadioTaskTCB;                                  /* Radio Task TCB.                                      */

/* Counts 1ms timeTicks */
volatile uint32_t msTicks = 0;

static OS_SEM    App_SemRadio;


// Max payload size can be LORAMAC_PHY_MAXPAYLOAD.
// This example only communicates with much shorter messages (<256 bytes).
// If longer messages are used, these buffers must be changed accordingly.
uint8_t tx_buffer[256];
uint8_t rx_buffer[256];

/*
********************************************************************************************************
*********************************************************************************************************
*                                       EXPORTED FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void Delay(uint32_t dlymsTicks);

uint32_t readmsTicks(void);

void gpioCallback(uint8_t pin);
/*
********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

static  void  MainStartTask (void  *p_arg);

static  void  LoRaTask (void  *p_arg);

static  void  RadioTask (void  *p_arg);


/**
* This event queue is the global event queue for both the
* application and stack. To conserve memory, the stack is designed to run
* in the same thread as the application and the application is responsible for
* providing an event queue to the stack that will be used for ISR deferment as
* well as application information event queuing.
*/
static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS *EVENTS_EVENT_SIZE);

/**
 * Event handler.
 *
 * This will be passed to the LoRaWAN stack to queue events for the
 * application which in turn drive the application.
 */
static void lora_event_handler(lorawan_event_t event);

/**
 * Application specific callbacks
 */
static lorawan_app_callbacks_t callbacks;

static LoRaWANInterface *p_lorawan;

static SX126X_LoRaRadio *p_radio;



void App_OS_TimeTickHook(void)
{
      /* Increment counter necessary in Delay()*/
      msTicks++;
}

/*
*********************************************************************************************************
*                                          LoRaTask()
*
* Description : This task executes LoRaWAN mbed application.
*
* Argument(s) : p_arg   Argument passed from task creation. Unused, in this case.
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/
static void LoRaTask (void  *p_arg)
{
    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

    /* Enable GPIO in CMU */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Initialize GPIO interrupt dispatcher */
    GPIOINT_Init();

    SX126X_LoRaRadio radio(MBED_CONF_APP_LORA_SPI_MOSI,
                           MBED_CONF_APP_LORA_SPI_MISO,
                           MBED_CONF_APP_LORA_SPI_SCLK,
						   MBED_CONF_APP_LORA_CS,
                           MBED_CONF_APP_LORA_RESET,
                           MBED_CONF_APP_LORA_DIO1,
                           MBED_CONF_APP_LORA_BUSY,
                           MBED_CONF_APP_LORA_FREQ_SELECT,
                           MBED_CONF_APP_LORA_DEVICE_SELECT,
                           MBED_CONF_APP_LORA_CRYSTAL_SELECT,
                           MBED_CONF_APP_LORA_ANT_SWITCH);

    /* dio1 interrupt pin mode set */
    GPIO_PinModeSet(gpioPortA, 13, gpioModeInput, 0);
    GPIOINT_CallbackRegister(13, gpioCallback);

    /**
     * Constructing Mbed LoRaWANInterface and passing it the radio object from lora_radio_helper.
     */
    LoRaWANInterface lorawan(radio);

    p_radio = &radio;

    p_lorawan = &lorawan;

    while (DEF_ON) {

        // stores the status of a call to LoRaWAN protocol
        lorawan_status_t retcode;

        // Initialize LoRaWAN stack
        if ( p_lorawan->initialize(&ev_queue) != LORAWAN_STATUS_OK) {
            printf("\r\n LoRa initialization failed! \r\n");
            return;
        }
        /* Test Code to set Device Class to CLASS_C mode */
#if 0
        // Set device class to CLASS_C
        if (p_lorawan->set_device_class(CLASS_C) != LORAWAN_STATUS_OK) {
            printf("\r\n Device Class C setting failed! \r\n");
            return;
        }
#endif
        printf("\r\n Mbed LoRaWANStack initialized \r\n");

        // prepare application callbacks
        callbacks.events = mbed::callback(lora_event_handler);
        p_lorawan->add_app_callbacks(&callbacks);

        // Set number of retries in case of CONFIRMED messages
        if (p_lorawan->set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER)
                != LORAWAN_STATUS_OK) {
            printf("\r\n set_confirmed_msg_retries failed! \r\n\r\n");
            return;
        }

        printf("\r\n CONFIRMED message retries : %d \r\n",
               CONFIRMED_MSG_RETRY_COUNTER);

        // Enable adaptive data rate
        if (p_lorawan->enable_adaptive_datarate() != LORAWAN_STATUS_OK) {
            printf("\r\n enable_adaptive_datarate failed! \r\n");
            return;
        }

        printf("\r\n Adaptive data  rate (ADR) - Enabled \r\n");

        retcode = p_lorawan->connect();

        if (retcode == LORAWAN_STATUS_OK ||
                retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
        } else {
            printf("\r\n Connection error, code = %d \r\n", retcode);
            return;
        }

        printf("\r\n Connection - In Progress ...\r\n");

        // make your event queue dispatching events forever
        ev_queue.dispatch_forever();

        return;
    }
}

/**
 * Sends a message to the Network Server
 */
static void send_message()
{
    uint16_t packet_len;
    int16_t retcode;
    char buf[] = "LORAWANNODELORAWANNODE";

    packet_len = sprintf((char *) tx_buffer, buf);

    printf("\n Send_data = %s \n", tx_buffer);

    retcode = p_lorawan->send(MBED_CONF_LORA_APP_PORT, tx_buffer, packet_len,
                           MSG_UNCONFIRMED_FLAG);

    if (retcode < 0) {
        retcode == LORAWAN_STATUS_WOULD_BLOCK ? printf("send - WOULD BLOCK\r\n")
        : printf("\r\n send() - Error code %d \r\n", retcode);

        if (retcode == LORAWAN_STATUS_WOULD_BLOCK) {
            //retry in 3 seconds
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                ev_queue.call_in(3000, send_message);
            }
        }
        return;
    }

    printf("\r\n %d bytes scheduled for transmission \r\n", retcode);
    memset(tx_buffer, 0, sizeof(tx_buffer));
}

/**
 * Receive a message from the Network Server
 */
static void receive_message()
{
    uint8_t port;
    int flags;

    int16_t retcode = p_lorawan->receive(rx_buffer, sizeof(rx_buffer), port, flags);

    if (retcode < 0) {
        printf("\r\n receive() - Error code %d \r\n", retcode);
        return;
    }

    printf(" RX Data on port %u (%d bytes): ", port, retcode);

    printf("\n Received_data = %s \n", rx_buffer);

    printf("\r\n");

    memset(rx_buffer, 0, sizeof(rx_buffer));
}

/**
 * Event handler
 */
static void lora_event_handler(lorawan_event_t event)
{
    switch (event) {
        case CONNECTED:
            printf("\r\n Connection - Successful \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
            	/* Test Code to set Fixed Data Rate */
#if 0
            	// Disable adaptive data rate
                if (p_lorawan->disable_adaptive_datarate() != LORAWAN_STATUS_OK) {
                    printf("\r\n disable_adaptive_datarate failed! \r\n");
                    return;
                }
                // Fixed data rate
                if (p_lorawan->set_datarate(DR_5) != LORAWAN_STATUS_OK) {
                    printf("\r\n Fixed_adaptive_datarate failed! \r\n");
                    return;
                }
#endif
                send_message();
            } else {
                ev_queue.call_every(TX_TIMER, send_message);
            }

            break;
        case DISCONNECTED:
            ev_queue.break_dispatch();
            printf("\r\n Disconnected Successfully \r\n");
            break;
        case TX_DONE:
            printf("\r\n Message Sent to Network Server \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
            	Delay(2);
                send_message();
            }
            break;
        case TX_TIMEOUT:
        case TX_ERROR:
        case TX_CRYPTO_ERROR:
        case TX_SCHEDULING_ERROR:
            printf("\r\n Transmission Error - EventCode = %d \r\n", event);
            // try again
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        case RX_DONE:
            printf("\r\n Received message from Network Server \r\n");
            receive_message();
            break;
        case RX_TIMEOUT:
        case RX_ERROR:
            printf("\r\n Error in reception - Code = %d \r\n", event);
            break;
        case JOIN_FAILURE:
            printf("\r\n OTAA Failed - Check Keys \r\n");
            break;
        case UPLINK_REQUIRED:
            printf("\r\n Uplink required by NS \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        default:
            MBED_ASSERT("Unknown Event");
    }
}

/*
*********************************************************************************************************
*                                          RadioTask()
*
* Description : This task polls radio event and increments the semaphore when it is occurred.
*
* Argument(s) : p_arg   Argument passed from task creation. Unused, in this case.
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/
static  void  RadioTask (void  *p_arg)
{
    RTOS_ERR  err;
    CPU_TS ts;

    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

    while (1) {

    	OSSemPend((OS_SEM *)&App_SemRadio,
    			(OS_TICK )0,
				(OS_OPT )OS_OPT_PEND_BLOCKING,
				(CPU_TS *)&ts,
					&err);
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);

        // call ISR function from radio
        p_radio->handle_dio1_irq();

    }
}

/*
 *****************************************************************************
 *                         gpioCallback(uint8_t )
 * @brief : Gpio Callback
 *
 * @param  : pin - pin which triggered interrupt
 *
 * @Return(s) : None.
 *****************************************************************************
 */
void gpioCallback(uint8_t pin)
{
  RTOS_ERR  err;
  if (pin == 13) {  // change this to dio pin

	  OSSemPost((OS_SEM *)&App_SemRadio,
			  	 (OS_OPT )OS_OPT_POST_1,
				 &err);
	  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

  }
}

/*
 *****************************************************************************
 *                         Delay(uint32_t )
 * @brief  Delay in ms
 *
 * @param  dlymsTicks - delay in ms for ticks
 *
 * @Return(s) : None.
 *****************************************************************************
 */
void Delay(uint32_t dlymsTicks)
{
	RTOS_ERR  err;

    OSTimeDly( (OS_TICK)dlymsTicks,                          /*   500 OS Ticks                                      */
               OS_OPT_TIME_DLY,                             /*   from now.                                          */
              &err);

    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
}

/*
 *****************************************************************************
 *                         readmsTicks()
 * @brief  Read ms ticks from OSTimeGet
 *
 * @param  None
 *
 * @Return(s) : uint32_t msTicks.
 *****************************************************************************
 */
uint32_t readmsTicks(void)
{
	RTOS_ERR  err;
	msTicks = OSTimeGet(&err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	return msTicks;
}

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C applications. It initializes the first task and
*                 the OS.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Note(s)     : None.
*********************************************************************************************************
*/
int  main (void)
{
    RTOS_ERR  err;

    /* Configures the SWO to output both printf-information, PC-samples and interrupt trace. */
    setupSWOForPrint();

    BSP_SystemInit();                                           /* Initialize System.                                   */
    CPU_Init();                                                 /* Initialize CPU.                                      */

    OS_TRACE_INIT();                                            /* Initialize trace if enabled                          */
    OSInit(&err);                                               /* Initialize the Kernel.                               */
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

    /* Initialization Code */
    OSSemCreate(&App_SemRadio,
    			"Radio ISR Sem",
				0,
				&err);
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

    OSTaskCreate(&MainStartTaskTCB,                          /* Create the Start Task.                               */
                 "Main Start Task",
                  MainStartTask,
                  DEF_NULL,
                  MAIN_START_TASK_PRIO,
                 &MainStartTaskStk[0],
                 (MAIN_START_TASK_STK_SIZE / 10u),
                  MAIN_START_TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

    OSStart(&err);                                              /* Start the kernel.                                    */
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

    return (1);
}

/*
*********************************************************************************************************
*                                          MainStartTask()
*
* Description : This is the task that will be called by the Startup when all services are initialized
*               successfully.  It toggles the LED.
*
* Argument(s) : p_arg   Argument passed from task creation. Unused, in this case.
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/
static  void  MainStartTask (void  *p_arg)
{
    RTOS_ERR  err;

    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

    BSP_TickInit();                                             /* Initialize Kernel tick source.                       */

    OSStatTaskCPUUsageInit(&err);                               /* Initialize CPU Usage.                                */
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);

    Common_Init(&err);                                          /* Call common module initialization example.           */
    APP_RTOS_ASSERT_CRITICAL(err.Code == RTOS_ERR_NONE, ;);

    BSP_OS_Init();                                              /* Initialize the BSP. It is expected that the BSP ...  */
                                                                /* ... will register all the hardware controller to ... */
                                                                /* ... the platform manager at this moment.             */

    OSTaskCreate(&LoRaTaskTCB,                                /* Create the LoRa Task.                               */
                     "LoRa Task",
                      LoRaTask,
                      DEF_NULL,
                      LORA_TASK_PRIO,
                     &LoRaTaskStk[0],
                     (LORA_TASK_STK_SIZE / 10u),
                      LORA_TASK_STK_SIZE,
                      0u,
                      0u,
                      DEF_NULL,
                     (OS_OPT_TASK_STK_CLR),
                     &err);
                                                                    /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000))
     {
           DEBUG_BREAK;
     }

    OSTaskCreate(&RadioTaskTCB,                          /* Create the Radio Task.                               */
                 "Radio Task",
                  RadioTask,
                  DEF_NULL,
                  RADIO_TASK_PRIO,
                 &RadioTaskStk[0],
                 (RADIO_TASK_STK_SIZE / 10u),
                  RADIO_TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

    while (DEF_ON) {

        BSP_LedToggle(1);
                                                                /* Delay Start Task execution for                       */
        OSTimeDly( 500,                                        /*   500 OS Ticks                                      */
                   OS_OPT_TIME_DLY,                             /*   from now.                                          */
                  &err);
                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }
}
// EOF
