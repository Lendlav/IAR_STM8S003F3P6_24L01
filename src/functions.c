/**
  ******************************************************************************
  * @file     function.c
  * @author   MCD Application Team
  * @version  V1.1.0
  * @date     11-March-2011
  * @brief    This file contains some functions that can be used in the demo.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "functions.h"
#include "SSD1306.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern volatile flags             my_flag;
extern volatile u16               count;
const u8 Chars[] = "0123456789ABCDEF"; 

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

//-----------------------------
void delay(u16 time)
{
  count = time;
  while(count)
  {
    continue;
  }
}
//---------------------------------------------
//
//---------------------------------------------
const u32 step[] = {100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
void IntToString(u32 data, u8 *buffer, u8 length)
{
   u8 atemp;
   u32 temp;
   u8 i = (u8)(9 - length);
   
   while(*buffer != '\0')
   {
     temp = step[i];
     atemp = 0;
     while (data >= temp) 
     {
       atemp++;
       data -= temp;
     }
     *buffer = Chars[atemp];  
     buffer++;
     i++;
   }
}
//---------------------------------------------
//
//---------------------------------------------


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
