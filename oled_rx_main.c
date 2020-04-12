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
  *  Description:   receiver with oled display for tuning 
  *
  *-----------------------------------------------------------------------------
  */

#include "main.h"
#include "functions.h"
#include "SSD1306.h"
#include "nrf24l01.h"
//****************************************************************************** 
//      DEFINES
//******************************************************************************

#define flip_flag       my_flag.bits.flip

//****************************************************************************** 
//      Global variables
//******************************************************************************

volatile flags  my_flag;
volatile u16    count = 0;
extern   u8     lcd_buff[];
char Char_arr[]="0123456789ABCDEF";

//****************************************************************************** 
//      Function prototypes
//******************************************************************************
void Chip_Staus(void);

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
  Init_I2C();
  Init_ADC();
  Init_SPI();
  
  delay(300);
  
  LCD_Init();
  LCD_Mode(0x00);
  LCD_Flip(RESET);
  LCD_Clear();
  
  /* Radio initialization */
  u8 address[] = {0xC1,0xC2,0xC3,0xC4,0xC5};
  
  NRF24L01_Set_Power(_POWER_UP);
    
  NRF24L01_Disable_All_Pipes();
  NRF24L01_Init(_RX_MODE, 45, _1Mbps, address, sizeof(address), 4);
  NRF24L01_Flush_RX(); 
  NRF24L01_Clear_Interrupts();
  NRF24L01_CE_HIGH;
  
  u8 buff[5];
  buff[5] = 0;
  u16 speed = 0;
  u16 wheel = 0;
    
  while(1)
  {    
    Chip_Staus();
    u8 status =  NRF24L01_ReadReg(FIFO_STATUS);  

    if(status & 0x02)
    {
      speed = 0;
      wheel = 0;
      
      NRF24L01_Read_RX_Buf(buff,sizeof(buff));
      NRF24L01_Flush_RX();
      NRF24L01_Clear_Interrupts(); 
      
      speed |= (u16)buff[0];
      speed <<= 8;
      speed |= (u16)buff[1];
      
      wheel |= (u16)buff[2];
      wheel <<= 8;
      wheel |= (u16)buff[3];
  
      u8 my_str[]="0000";
      IntToString(speed, my_str, 4);
      LCD_FStr(my_str,0,0,0);
      IntToString(wheel, my_str, 4);
      LCD_FStr(my_str,0,1,0);
      LCD_Update();
      LCD_Clear();
    }
    
    lcd_buff[256 + wheel/8]= 0xFF;
    
    NRF24L01_Get_Status();

    delay(5);  
  }  
}
//------------------------------------------------------------------------------
void Chip_Staus(void)
{
    u8  status = NRF24L01_Get_Status();
    LCD_GotoXY(0,3);
    LCD_Chr(Char_arr[(status >> 4) & 0x0F],0); 
    LCD_GotoXY(1,3);
    LCD_Chr(Char_arr[(status & 0x0F)],0);
    
    status = NRF24L01_ReadReg(EN_AA);
    LCD_GotoXY(3,3);
    LCD_Chr(Char_arr[(status >> 4) & 0x0F],0); 
    LCD_GotoXY(4,3);
    LCD_Chr(Char_arr[(status & 0x0F)],0);
    
    status = NRF24L01_ReadReg(CONFIG);
    LCD_GotoXY(6,3);
    LCD_Chr(Char_arr[(status >> 4) & 0x0F],0); 
    LCD_GotoXY(7,3);
    LCD_Chr(Char_arr[(status & 0x0F)],0);
    
    status = NRF24L01_ReadReg(RX_PW_P0);
    LCD_GotoXY(9,3);
    LCD_Chr(Char_arr[(status >> 4) & 0x0F],0); 
    LCD_GotoXY(10,3);
    LCD_Chr(Char_arr[(status & 0x0F)],0);
    
    status = NRF24L01_ReadReg(EN_RXADDR);
    LCD_GotoXY(12,3);
    LCD_Chr(Char_arr[(status >> 4) & 0x0F],0); 
    LCD_GotoXY(13,3);
    LCD_Chr(Char_arr[(status & 0x0F)],0);
    
    status = NRF24L01_ReadReg(RF_SETUP);
    LCD_GotoXY(15,3);
    LCD_Chr(Char_arr[(status >> 4) & 0x0F],0); 
    LCD_GotoXY(16,3);
    LCD_Chr(Char_arr[(status & 0x0F)],0);
    
    status = NRF24L01_ReadReg(FIFO_STATUS);
    LCD_GotoXY(18,3);
    LCD_Chr(Char_arr[(status >> 4) & 0x0F],0); 
    LCD_GotoXY(19,3);
    LCD_Chr(Char_arr[(status & 0x0F)],0);
    
    LCD_Update();
}
//---------------------------------------------
 
//******************************************************************************
//  THE END
//******************************************************************************



