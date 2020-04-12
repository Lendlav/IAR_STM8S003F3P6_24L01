#include "main.h"


//---------------------------------------------
#define TIM4_PERIOD     124// 124
//---------------------------------------------

//---------------------------------------------
void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
//  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* Enable TIM4 */
  TIM4->CR1 |= TIM4_CR1_CEN;    //TIM4_Cmd(ENABLE);
}
//---------------------------------------------
void TIM2_Config(void)
{ 
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 13000);
  TIM2_OC2Init(TIM2_OCMODE_PWM1, 
               TIM2_OUTPUTSTATE_ENABLE, 
               0,
               TIM2_OCPOLARITY_HIGH);
  TIM2_OC3Init(TIM2_OCMODE_PWM1, 
               TIM2_OUTPUTSTATE_ENABLE, 
               1300,
               TIM2_OCPOLARITY_HIGH);
  TIM2_Cmd(ENABLE);

}
//---------------------------------------------
void TIM1_Config(void)
{
#define Period          ((uint16_t)50)
#define CCR1_Val        ((uint16_t)(20))
#define Prescaler       ((uint16_t)(1600-1))
  
  TIM1_DeInit();
  TIM1_TimeBaseInit(Prescaler, TIM1_COUNTERMODE_UP, Period, 0);
//  TIM1_OC3Init(TIM1_OCMODE_PWM2, 
//               TIM1_OUTPUTSTATE_ENABLE,
//               TIM1_OUTPUTNSTATE_DISABLE,
//               CCR1_Val, 
//               TIM1_OCPOLARITY_LOW,
//               TIM1_OCNPOLARITY_LOW,
//               TIM1_OCIDLESTATE_RESET,
//               TIM1_OCNIDLESTATE_RESET
//                 ); 
//  TIM1_CtrlPWMOutputs(ENABLE);
 
//  TIM1_EncoderInterfaceConfig(TIM1_ENCODERMODE_TI12, 
//                              TIM1_ICPOLARITY_FALLING, //TIM1_ICPOLARITY_RISING,//
//                              TIM1_ICPOLARITY_FALLING);
//  TIM1_UpdateDisableConfig(ENABLE);
  
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
//  TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
  TIM1_Cmd(ENABLE);  
  
}
//---------------------------------------------
void Init_CLK(void)
{
   CLK_DeInit();
   CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);  // 1..8
   CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1); // 1..128   
//   CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, 
//                         CLK_SOURCE_HSI, 
//                         DISABLE, 
//                         CLK_CURRENTCLOCKSTATE_DISABLE);  
}
//---------------------------------------------

void Init_EXTI(void)
{
   //   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW);
   //   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
   //   EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}

//---------------------------------------------
void Init_ITC(void)
{ 
  /* enable interrupts */
  enableInterrupts();
}
//---------------------------------------------
void Init_GPIO(void)
{   
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
   
  //the 24L01+ module pinout descriotion
  //STM8S      -> 24L01
  //------------------------
  //PD2        -> IRQ  //Maskable Interrup pin. Active Low
  //PC3        -> CE   //Chip Enable Activates RX or TX mode
  //PC4        -> CSN
  //PC5        -> SCLK
  //PC6        -> SDI
  //PC7        -> SDO

  GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);      //IRQ active low
  GPIO_Init(GPIOC,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);  //Chip Enable    
  GPIO_Init(GPIOC,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_FAST); //Chip Select   
 
  GPIO_Init(GPIOA,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); 
  GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT); 
  GPIO_Init(GPIOD,GPIO_PIN_5,GPIO_MODE_IN_FL_NO_IT); 
  GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT); 
  
  GPIO_Init(GPIOB,GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_SLOW); 
  GPIO_Init(GPIOB,GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_SLOW); 
}
//---------------------------------------------
void Init_SPI(void)
{
   /* SPI configuration */
   SPI_DeInit();
   /* Initialize SPI in Slave mode  */
   SPI_Init(
            SPI_FIRSTBIT_MSB, 
            SPI_BAUDRATEPRESCALER_2, 
            SPI_MODE_MASTER, 
            SPI_CLOCKPOLARITY_LOW,
            SPI_CLOCKPHASE_1EDGE, 
            SPI_DATADIRECTION_2LINES_FULLDUPLEX, 
            SPI_NSS_SOFT,
            (uint8_t)0x07
               );
   
   /* Enable the SPI*/
   SPI_Cmd(ENABLE);  
}
//---------------------------------------------
void Init_ADC()
{ 
   ADC1_DeInit();
   ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
             ADC1_CHANNEL_5, 
             ADC1_PRESSEL_FCPU_D12, 
             ADC1_EXTTRIG_TIM, DISABLE, 
             ADC1_ALIGN_RIGHT, 
             ADC1_SCHMITTTRIG_CHANNEL5|ADC1_SCHMITTTRIG_CHANNEL6,DISABLE
               );
   ADC1_Cmd(ENABLE);
}
//---------------------------------------------
void Init_I2C(void)
{   
   I2C_DeInit();
   I2C_Init(
            (uint32_t)600000,   //I2C_MAX_FAST_FREQ, 
            0x70,               //OWN_ADDRESS
            I2C_DUTYCYCLE_2, 
            I2C_ACK_CURR, 
            I2C_ADDMODE_7BIT, 
            16 
               );  
}
//---------------------------------------------
void Init_AWU(void)
{
   AWU_Init(AWU_TIMEBASE_500US);
   AWU_Cmd(ENABLE);
}
//---------------------------------------------
void Init_SWIM(void)
{
   CFG->GCR |= CFG_GCR_SWD;   //Disable the SWIM function
}
//---------------------------------------------
void Init_BEEP(void)
{
   BEEP_DeInit();
//   BEEP_LSICalibrationConfig(128000);
   BEEP_Init(BEEP_FREQUENCY_4KHZ);
}
