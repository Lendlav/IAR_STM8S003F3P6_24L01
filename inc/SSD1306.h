//SSD1306 library for STM8S
#include "stm8s.h"
//----------------------------------------------------------------------------------
#define OWN_ADDRESS     0x70
#define SLAVE_ADDRESS   0x78

#define DATA_MODE       0x40
#define COMMAND_MODE    0x00   // bit Co = 0, D/C# = 0

//----------------------------------------------------------------------------------
void LCD_Init   (void);
void LCD_Mode   (u8 mode);
void LCD_Flip   (u8 flip);
void LCD_Contrast(u8 contrast);
void LCD_Off    (void);
void LCD_On     (void);
void LCD_Clear  (void);
void LCD_Update (void);
void LCD_Batt   (int val);
void LCD_Chr    (u8 ch, u8 invers);
void LCD_2xChr  (u8 ch);
void LCD_FStr   (const u8 *dataPtr,u8 x,u8 y, u8 invers);
void LCD_2xFStr (const u8 *dataPtr,u8 x,u8 y);
void LCD_GotoXY (u8 x,u8 y);
void LCD_Picture(const u8 *Picture);
void LCD_Logo   (const u8 *picture, u16 picture_size, u8 Inversion);
void LCD_command(u8 cmd);

//----------------------------------------------------------------------------------
extern unsigned char            lcd_buff[];
extern const unsigned char      font [][5];

///SSD1306 library for STM8S
////----------------------------------------------------------------------------------
//#define OWN_ADDRESS     0x70
//#define SLAVE_ADDRESS   0x78
//
//
////----------------------------------------------------------------------------------
//void LCD_Init   (void);
//void LCD_Off    (void);
//void LCD_On     (void);
//void LCD_Clear  (void);
//void LCD_Update (void);
//void LCD_Chr    (u8 ch);
//void LCD_2xChr  (u8 ch);
//void LCD_FStr   (const u8 *String);
//void LCD_2xFStr (const u8 *String);
//void LCD_User   (u8 *udg);
//void LCD_GotoXY (u8 x,u8 y);
//void LCD_Picture(const u8 *Picture);
//void LCD_Logo   (const u8 *picture, u16 picture_size, u8 Inversion);
//void LCD_command(u8 cmd);
//
//
////----------------------------------------------------------------------------------
//
//extern unsigned char            lcd_buff[];
//extern const unsigned char      font [][5];
//extern unsigned long            *point;