 /*
  *-----------------------------------------------------------------------------
  *
  *  File:         
  *
  *  Compilers:     IAR
  *
  *  Controller:    STM8S103
  *
  *  Programmer:    Vladlen
  *                  
  *  Description:   tank rx controller with two pwm outputs
  *
  *-----------------------------------------------------------------------------
  */
#include "main.h"
#include "functions.h"
#include "nrf24l01.h"
//****************************************************************************** 
//      DEFINES
//******************************************************************************
#define TRANSMITTER     //comment-uncomment to select between TX and RX mode

#define _SPEED_CENTER_  (1550+256)
#define _WHEEL_CENTER_  (1300+900)

#define _SPEED_REVERS_MAX_  600
#define _SPEED_FORWARD_MAX_ 2200

#define flip_flag       my_flag.bits.flip

#define LeftSpeedSet(a)         TIM2_SetCompare2(a)
#define RightSpeedSet(a)        TIM2_SetCompare3(a)

#define BackLightOn()     GPIO_WriteLow(GPIOB, GPIO_PIN_5)
#define BackLightOff()    GPIO_WriteHigh(GPIOB, GPIO_PIN_5)
#define BackLightTogle()  GPIO_WriteReverse(GPIOB, GPIO_PIN_5)


//****************************************************************************** 
//      Global variables
//******************************************************************************

volatile flags  my_flag;
volatile u16    count = 0;
extern volatile u32  ID1;
char Char_arr[]="0123456789ABCDEF";

//****************************************************************************** 
//      Function prototypes
//******************************************************************************


//********************************************************************************                                                                            *
//                 Function:       main                                          *                                                                               *
// ------------------------------------------------------------------------------/
int  main (void)
{
  /* MCU initialization */
  Init_CLK();
  Init_GPIO();
  TIM4_Config();
  Init_ITC();
  Init_ADC();
  Init_SPI();
  
  delay(300);
  
  /* Radio initialization */
  u8 address[] = {0xC1,0xC2,0xC3,0xC4,0xC5};
  
  NRF24L01_Set_Power(_POWER_UP);
    
  NRF24L01_Disable_All_Pipes();
  NRF24L01_Init(_RX_MODE, 45, _1Mbps, address, sizeof(address), 4);
  NRF24L01_Flush_RX(); 
  NRF24L01_Clear_Interrupts();
  NRF24L01_CE_HIGH;
  
  
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 1200);
  TIM2_OC2Init(TIM2_OCMODE_PWM1, 
               TIM2_OUTPUTSTATE_ENABLE, 
               0,
               TIM2_OCPOLARITY_HIGH);
  TIM2_OC3Init(TIM2_OCMODE_PWM1, 
               TIM2_OUTPUTSTATE_ENABLE, 
               0,
               TIM2_OCPOLARITY_HIGH);
  TIM2_Cmd(ENABLE); 
  
   /* Main loop */   
  
  while(1)
  {    
    static u8 channel_is_broken_cnt = 0;
    u8 buff[5];
    s16 speed = 0;
    s16 wheel = 0;
    buff[5]=0;

    u8 status =  NRF24L01_ReadReg(FIFO_STATUS);  

    if(status & 0x02)
    {   
      channel_is_broken_cnt = 60;
              
      NRF24L01_Read_RX_Buf(buff,sizeof(buff));
      NRF24L01_Flush_RX();
      NRF24L01_Clear_Interrupts(); 
      
      speed |= (u16)buff[0];
      speed <<= 8;
      speed |= (u16)buff[1];
      
      wheel |= (u16)buff[2];
      wheel <<= 8;
      wheel |= (u16)buff[3];
      
      if(speed < 515)
      {
        BackLightOff();
      }
      else
      {                
        if(speed > 970)
        {
          BackLightOn();
        }
      }
       
      speed -=512;
      if(speed < 0)speed = 0;
      speed /= 8; 
      
      wheel -= 512;
      wheel /= 4;
      
      u16 left = 0;
      u16 right = 0;
      
      if(speed>2)
      {
        speed += 192;
        left   = (u16)speed;
        right  = (u16)speed;
      }
      else
      {
        left = 0;
        right = 0;
      }
      
      if (wheel > 2)
      {
        wheel = ((256 - (wheel)) * 100) / 256;
        left *= (u16)wheel;
        left /= 100;
      }
      else if(wheel < -2)
      {
        wheel = (((((256) - (~(wheel)+1))) * 100) / 256);
        right *= (u16)wheel;
        right /= 100;
      }
      
      LeftSpeedSet(right); 
      RightSpeedSet(left);
    }

    NRF24L01_Get_Status();
    
    if(channel_is_broken_cnt)
    {
      channel_is_broken_cnt--;     
    }
    else 
    {
      u8 static tmr = 50;
      
      LeftSpeedSet(0);

      if(tmr)tmr--;
      else
      {
        tmr = 50;
        BackLightTogle();
      }      
    }

    delay(10);  
  }
    
}
//******************************************************************************
//  THE END
//******************************************************************************



