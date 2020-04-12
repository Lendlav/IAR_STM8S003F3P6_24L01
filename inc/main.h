#include "stm8s.h"
#include "string.h"
//---------------------------------------------
extern const unsigned char aftograf_128x32[512];
//---------------------------------------------

typedef union{
  struct{
    unsigned b1         :1;
    unsigned b2         :1;
    unsigned flip       :1;
    unsigned res4       :1;
    unsigned res5       :1;
    unsigned res6       :1;
    unsigned res7       :1;
    unsigned res8       :1;
  }bits;
  u8 all;
}flags;

typedef union {
    struct {
        u8 LB;
        u8 HB;
    };
    u16 all;
} word;

//---------------------------------------------
void Init_CLK                   (void);
void Init_GPIO                  (void);
void Init_SPI                   (void);
void Init_ADC                   (void);
void Init_I2C                   (void);
void Init_ITC                   (void);
void Init_EXTI                  (void);
void TIM4_Config                (void);
void TIM2_Config                (void);
void TIM1_Config                (void);
void Init_AWU                   (void);
void Init_SWIM                  (void);
void Init_BEEP                  (void);



