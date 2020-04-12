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
  *  Description:   car rx controller with pwm output signals for SG90 servo 
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

#define _SPEED_CENTER_  (1060 + 256)
#define _WHEEL_CENTER_  (600)

//#define _SPEED_REVERS_MAX_  2200
//#define _SPEED_FORWARD_MAX_ 600

#define flip_flag       my_flag.bits.flip

#define SpeedSet(a)     TIM2_SetCompare2(a)
#define WheelSet(a)     TIM2_SetCompare3(a)

#define BackLightOn()     GPIO_WriteLow(GPIOB, GPIO_PIN_4)
#define BackLightOff()    GPIO_WriteHigh(GPIOB, GPIO_PIN_4)
#define BackLightTogle()  GPIO_WriteReverse(GPIOB, GPIO_PIN_4)


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
  
  TIM2_Config();
  WheelSet(2000);
  delay(500);
  WheelSet(1300);
  delay(500);
  
  while(1)
  {    
    static u8 channel_is_broken_cnt = 0;
    u8 buff[5];
    u16 speed = 0;
    u16 wheel = 0;
    buff[5]=0;

    u8 status =  NRF24L01_ReadReg(FIFO_STATUS);  

    if(status & 0x02)
    {
      channel_is_broken_cnt = 25;
      
      NRF24L01_Read_RX_Buf(buff,sizeof(buff));
      NRF24L01_Flush_RX();
      NRF24L01_Clear_Interrupts(); 
      
      speed |= (u16)buff[0];
      speed <<= 8;
      speed |= (u16)buff[1];
      
      wheel |= (u16)buff[2];
      wheel <<= 8;
      wheel |= (u16)buff[3];
      wheel *= 17;
      wheel /= 10;
      
      if((speed < 495)&&(speed > 485))
      {
        SpeedSet(0);
        BackLightOff();
      }
      else
      {
        if((speed < 490))
        {
          BackLightOn();             
        }
        else
        {
          BackLightOff();   
        }
        
        if(speed > 1010)
        {
          SpeedSet(2200);   //speed forward max
        }
        else if(speed < 500)
        {
          if(speed < 5)
          {
            SpeedSet(600); //speed revers max
          }
          else
          {
            SpeedSet(_SPEED_CENTER_  + (speed/2)); 
          }
        }
        else
        {
          SpeedSet(_SPEED_CENTER_ + (speed/2)); 
        }
      }
       
      if((wheel < 516)&&(wheel > 508))
      {
        WheelSet(0);
      }
      else
      {
        WheelSet(_WHEEL_CENTER_ + wheel);
      }     
    }

    NRF24L01_Get_Status();
    
    if(channel_is_broken_cnt) //stop and blynk backlights when rc channel is off
    {
      channel_is_broken_cnt--;     
    }
    else 
    {
      u8 static tmr = 50;
      
      SpeedSet(0);

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



