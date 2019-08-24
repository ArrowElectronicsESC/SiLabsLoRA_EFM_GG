/**************************************************************************//**
 * @file
 * @brief helper functions for configuring SWO
 * @version 4.4.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_cmu.h"

int RETARGET_WriteChar(char c)
{
  return ITM_SendChar(c);
}

int RETARGET_ReadChar(void)
{
  return 0;
}

void setupSWOForPrint(void)
{
  /* Enable GPIO clock. */
	CMU_ClockEnable(cmuClock_GPIO, true);

  /* Enable Serial wire output pin */
  GPIO->ROUTEPEN |= GPIO_ROUTEPEN_SWVPEN;

    /* Set location 0 */
  GPIO->ROUTELOC0 = GPIO_ROUTELOC0_SWVLOC_LOC0;

  /* Enable output on pin - GPIO Port F, Pin 2 */
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;

  /* Enable debug clock AUXHFRCO */
  CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  /* Wait until clock is ready */
  while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));

  /* Enable trace in core debug */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  ITM->LAR  = 0xC5ACCE55;
  ITM->TER  = 0x0;
  ITM->TCR  = 0x0;
  TPI->SPPR = 2;
  TPI->ACPR = 0x15;	// changed from 0x0F on Giant, etc. to account for 19 MHz default AUXHFRCO frequency
  ITM->TPR  = 0x0;
  DWT->CTRL = 0x400003FE;
  ITM->TCR  = 0x0001000D;
  TPI->FFCR = 0x00000100;
  ITM->TER  = 0x1;
}
