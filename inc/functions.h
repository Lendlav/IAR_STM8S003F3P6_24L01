/**
  ********************************************************************************
  * @file     function.h
  * @author   MCD Application Team
  * @version  V1.1.0
  * @date     11-March-2011
  * @brief    This file contains functions prototypes for the functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void read_buttons       (void);
void delay              (u16 time);

void HELP_Exec          (void);

void PAGER_READ_Exec   (void);
void PAGER_SEND_Exec   (void);

void SETUP_ID_Exec      (void);
void SETUP_CH_Exec      (void);
void SETUP_TUNE_Exec    (void);

void ABOUT_Exec         (void);
void SHOW_UID           (void);
void IntToString        (u32 data, u8 *buffer, u8 length);
u8   MemCmp             (const u8 *mask, u8 *buff, u8 n);

#endif /* __FUNCTIONS_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
