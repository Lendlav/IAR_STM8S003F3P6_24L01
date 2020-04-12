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
  *  Description:   tx controller for both tank and car receiver
  *
  *-----------------------------------------------------------------------------
  */
#include "main.h"
#include "nrf24l01.h"
#include "functions.h"
//****************************************************************************** 
//      DEFINES
//******************************************************************************

#define flip_flag       my_flag.bits.flip

//****************************************************************************** 
//      Global variables
//******************************************************************************

volatile flags  my_flag;
volatile u16    count = 0;
char Char_arr[]="0123456789ABCDEF";

//****************************************************************************** 
//      Function prototypes
//******************************************************************************
void Chip_Staus(void);
u16 ADCRead_CH5(void);
u16 ADCRead_CH6(void);

//********************************************************************************                                                                            *
//                 Function:       main                                          *                                                                               *
// ------------------------------------------------------------------------------/
int  main (void)
{
  /* MCU initialization */
  Init_CLK();
  Init_GPIO();
  TIM4_Config();
  TIM1_Config();
  Init_ITC();
  Init_ADC();
  Init_SPI();
  
  delay(300);
  
  /* Radio initialization */
  u8 address[] = {0xC1,0xC2,0xC3,0xC4,0xC5};
  
  NRF24L01_Set_Power(_POWER_UP);
    
  NRF24L01_Disable_All_Pipes();
  NRF24L01_Init(_TX_MODE, 45, _1Mbps, address, sizeof(address), 4);
  NRF24L01_Flush_TX(); 
  NRF24L01_Clear_Interrupts();

  /* Main loop */
  
  while(1)
  {    
    u8 buff[4];
#define THRETH_STEP 6    //2,4,6,8....
    static s16 speedThresUp    = 512;
    static s16 speedThresDown  = 512;
    static s16 wheelThresUp    = 512;
    static s16 wheelThresDown  = 512;
    
    u16 speed = ADCRead_CH6();
    u16 wheel = ADCRead_CH5();
  
    for(u8 i = 0; i < 32; i++) //hysteresis 
    {
      if(speed < speedThresDown)
      {
        speedThresDown   = speed - THRETH_STEP/2;
        speedThresUp     = speedThresDown + THRETH_STEP;
      }
      if(speed > speedThresUp)
      {
        speedThresUp     = speed + THRETH_STEP/2;
        speedThresDown   = speedThresUp - THRETH_STEP;
      }    
      
      if(wheel < wheelThresDown)
      {
        wheelThresDown   = wheel - THRETH_STEP/2;
        wheelThresUp     = wheelThresDown + THRETH_STEP;
      }
      if(wheel > wheelThresUp)
      {
        wheelThresUp     = wheel + THRETH_STEP/2;
        wheelThresDown   = wheelThresUp - THRETH_STEP;
      }     
    }
    
    speed = (speedThresUp + speedThresDown)/2;
    wheel = (wheelThresUp + wheelThresDown)/2;
    
    buff[0] = speed >> 8;
    buff[1] = speed & 0x00FF;
    buff[2] = wheel >> 8;
    buff[3] = wheel & 0x00FF;
    
    NRF24L01_Write_TX_Buf(buff,4);
    NRF24L01_RF_TX();
    NRF24L01_Flush_TX();
    delay(20);
  } 
}
//---------------------------------------------
// simple reading the ADC channels
//---------------------------------------------
u16 ADCRead_CH5(void) { 
//  ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
            ADC1_CHANNEL_5, 
            ADC1_PRESSEL_FCPU_D12, 
            ADC1_EXTTRIG_TIM, DISABLE, 
            ADC1_ALIGN_RIGHT, 
            ADC1_SCHMITTTRIG_CHANNEL5,DISABLE
              );
  ADC1->CR1 |= ADC1_CR1_ADON;  //StartConversion
  while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC)==RESET);   
  u16 Din =  ADC1_GetConversionValue();
  static u16 Dacc = 0;
  static u16 Dout = 0;
  Dacc = Dacc + Din - Dout;
  Dout = Dacc/(u16)2;
  
  return (u16)Dout;   
} 
//---------------------------------------------
u16 ADCRead_CH6(void) { 
//  ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
            ADC1_CHANNEL_6, 
            ADC1_PRESSEL_FCPU_D12, 
            ADC1_EXTTRIG_TIM, DISABLE, 
            ADC1_ALIGN_RIGHT, 
            ADC1_SCHMITTTRIG_CHANNEL6,DISABLE
              );
  ADC1->CR1 |= ADC1_CR1_ADON;  //StartConversion 
  while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC)==RESET);   
  u16 Din =  ADC1_GetConversionValue();
  static u16 Dacc = 0;
  static u16 Dout = 0;
  Dacc = Dacc + Din - Dout;
  Dout = Dacc/(u16)4;
  
  return (u16)Dout;  
} 
//******************************************************************************
//  THE END
//******************************************************************************



