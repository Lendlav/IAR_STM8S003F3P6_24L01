/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include <stdio.h>
#include "iostm8s.h"

/* Private defines -----------------------------------------------------------*/

CONST char array[]={0x09,"Kotenok my little"};
/* Private function prototypes -----------------------------------------------*/

void delay (void);

/* Private functions ---------------------------------------------------------*/

void main(void)
{
   uint8_t CRC_RESET_VALUE = 0x07;
   
   BEEP_DeInit();
   GPIO_DeInit(GPIOB);
   GPIO_DeInit(GPIOC);
   
   GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);   
   GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   
   GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);  //UART Pins
   GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);
   
   CLK_DeInit();
   CLK_HSICmd(ENABLE);
   CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV8);
   CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV32);
   CLK_CCOConfig(CLK_OUTPUT_CPU);
   
   CLK_AdjustHSICalibrationValue(CLK_HSITRIMVALUE_7);
   
   SPI_DeInit(); 
   SPI_Init(
            SPI_FIRSTBIT_LSB, 
            SPI_BAUDRATEPRESCALER_2,
            SPI_MODE_MASTER,  
            SPI_CLOCKPOLARITY_HIGH, 
            SPI_CLOCKPHASE_1EDGE , 
            SPI_DATADIRECTION_1LINE_TX, 
            SPI_NSS_SOFT, 
            CRC_RESET_VALUE
               );
   SPI_Cmd(ENABLE);
   
   UART1_DeInit();
   UART1_Init(2400, UART1_WORDLENGTH_8D, UART1_STOPBITS_1  , UART1_PARITY_NO, \
      UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);
   UART1_Cmd(ENABLE);
   
   
   
   /* Infinite loop */
   while (1)
   {
      uint8_t ch;
      
      
      SPI_SendData(0xAA);
      //delay();
      
      for(ch=0; ch<sizeof(array);ch++){
         GPIO_WriteReverse(GPIOB,GPIO_PIN_5);
         //UART1_SendData8(array[ch]);
         GPIO_WriteReverse(GPIOB,GPIO_PIN_5);
         delay();
      }
      delay();
      delay();
      for(ch=0; ch<sizeof(array);ch++){
         delay();
         UART1_SendData8(0x08);
         delay();
      }
      printf(array);
   }
   
}

void delay (void){
	
	u16 i;
	for (i=0; i<300; i++){
		continue;
	}
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
