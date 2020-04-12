/** @mainpage
 @version 0.9
 @author
  Copyright (C) 2012 Luis R. Hilario http://www.luisdigital.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 @section intro Introduction
 This library is intended to be universal and easy to use.
 
 Download source code here: http://www.luisdigital.com/proyectos/nrf24l01/nrf24l01.zip

 You can also provide examples for other micros.
 
 Please report any bug and / or solution you find.
*/
#include "stm8s.h"
#include "nrf24l01.h"
#include "functions.h"

//---- functions prototypes -----------
u8 SPI_Exchange(u8 byte);

//-------------------------------------

u8 SPI_Exchange(u8 byte)
{  
   while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
   SPI_SendData(byte);
   while (SPI_GetFlagStatus(SPI_FLAG_BSY)== SET);
   while (SPI_GetFlagStatus(SPI_FLAG_RXNE)== RESET);
   return SPI_ReceiveData(); 
}
/**
 Read a register

 @param Reg Register to read

 @return Registry Value
*/
char NRF24L01_ReadReg(char Reg) 
{
  char Result;
  
  NRF24L01_CSN_LOW;
  SPI_Exchange(Reg);
  Result = SPI_Exchange(NOP);
  NRF24L01_CSN_HIGH;
  
  return Result;
}

/**
 Returns the STATUS register and then modify a register

 @param Reg Register to change
 @param Value New value

 @return STATUS Register
*/
char NRF24L01_WriteReg(char Reg, char Value) {
  char Result;
  
  NRF24L01_CSN_LOW;
  Result = SPI_Exchange(Reg);
  SPI_Exchange(Value);
  NRF24L01_CSN_HIGH;
  
  return Result;
}

/**
 Returns the STATUS register and then read "n" registers

 @param Reg Register to read
 @param Buf Pointer to a buffer
 @param Size Buffer Size

 @return STATUS Register
*/
char NRF24L01_ReadRegBuf(char Reg, char *Buf, int Size) {
  int i;
  char Result;
  
  NRF24L01_CSN_LOW;

  Result = SPI_Exchange(Reg);
  
  for (i = 0; i < Size; i++) 
  {   
    Buf[i] = SPI_Exchange(NOP);
  }
  
  NRF24L01_CSN_HIGH;
  
  return Result;
}

/**
 Returns the STATUS register and then write "n" registers

 @param Reg Registers to change
 @param Buf Pointer to a buffer
 @param Size Buffer Size

 @return STATUS Register
*/
char NRF24L01_WriteRegBuf(char Reg, char *Buf, int Size) {
  int i;
  char Result;
  
  NRF24L01_CSN_LOW; 
  Result = SPI_Exchange(Reg);
  for (i = 0; i < Size; i++) 
  {
    SPI_Exchange(Buf[i]);
  }
  NRF24L01_CSN_HIGH;
  
  return Result;
}

/**
 Returns the STATUS register

 @return STATUS Register
*/
char NRF24L01_Get_Status(void) {
	char Result;

	NRF24L01_CSN_LOW;
	Result = SPI_Exchange(NOP);
	NRF24L01_CSN_HIGH;

	return Result;
}

/**
 Returns the carrier signal in RX mode (high when detected)

 @return CD
*/
char NRF24L01_Get_CD(void) 
{
	return (NRF24L01_ReadReg(CD) & 1);
}

/**
 Select power mode

 @param Mode = _POWER_DOWN, _POWER_UP

 @see _POWER_DOWN
 @see _POWER_UP

*/
void NRF24L01_Set_Power(char Mode) {
	char Result;

	// 0b01111101 = 7D
        // Read Conf. Reg. AND Clear bit 1 (PWR_UP) and 7 (Reserved)
	Result = NRF24L01_ReadReg(CONFIG) & 0x7D; 
	NRF24L01_WriteReg(W_REGISTER | CONFIG, Result | Mode);
}

/**
 Select the radio channel

 @param CH = 0..125

*/
void NRF24L01_Set_CH(char CH) {
	// 0b01111111 = 0x7F
	NRF24L01_WriteReg(W_REGISTER | RF_CH, (CH & 0x7F)); // Clear bit 8
}

/**
 Select Enhanced ShockBurst ON/OFF

 Disable this functionality to be compatible with nRF2401

 @param Mode = _ShockBurst_ON, _ShockBurst_OFF

 @see _ShockBurst_ON
 @see _ShockBurst_OFF

*/
void NRF24L01_Set_ShockBurst(char Mode) {
	NRF24L01_WriteReg(W_REGISTER | SETUP_RETR, Mode);
	NRF24L01_WriteReg(W_REGISTER | EN_AA, Mode);
}

/**
 Select the address width

 @param Width = 3..5
*/
void NRF24L01_Set_Address_Width(char Width) {
	NRF24L01_WriteReg(W_REGISTER | SETUP_AW, (Width & 3) -2);
}

/**
 Select mode receiver or transmitter

 @param Device_Mode = _TX_MODE, _RX_MODE

 @see _TX_MODE
 @see _RX_MODE
*/
void NRF24L01_Set_Device_Mode(char Device_Mode) {
	char Result;

	// b01111110 = 7E
	Result = NRF24L01_ReadReg(CONFIG) & 0x7E; // Read Conf. Reg. AND Clear bit 0 (PRIM_RX) and 7 (Reserved)
	NRF24L01_WriteReg(W_REGISTER | CONFIG, Result | Device_Mode);
}

/**
 Enables and configures the pipe receiving the data

 @param PipeNum Number of pipe
 @param Address Address
 @param AddressSize Address size
 @param PayloadSize Buffer size, data receiver

*/
void NRF24L01_Set_RX_Pipe(char PipeNum, char *Address, int AddressSize, char PayloadSize) {
	char Result;

	Result = NRF24L01_ReadReg(EN_RXADDR);
	NRF24L01_WriteReg(W_REGISTER | EN_RXADDR, Result | (1 << PipeNum));

	NRF24L01_WriteReg(W_REGISTER | (RX_PW_P0 + PipeNum), PayloadSize);
	NRF24L01_WriteRegBuf(W_REGISTER | (RX_ADDR_P0 + PipeNum), Address, AddressSize);
}

/**
 Disable all pipes
*/
void NRF24L01_Disable_All_Pipes(void) {
	NRF24L01_WriteReg(W_REGISTER | EN_RXADDR, 0);
}


/** Returns the STATUS register and then clear all interrupts
 *
 * @return STATUS Register
 */
char NRF24L01_Clear_Interrupts(void) {
	return NRF24L01_WriteReg(W_REGISTER | STATUS, _RX_DR | _TX_DS | _MAX_RT);
}

/**
 Sets the direction of transmission

 @param Address Address
 @param Size Address size 3..5

*/
void NRF24L01_Set_TX_Address(char *Address, int Size) {
	NRF24L01_WriteRegBuf(W_REGISTER | TX_ADDR, Address, Size);
}

/**
 Empty the transmit buffer

*/
void NRF24L01_Flush_TX(void) {
	NRF24L01_CSN_LOW;
	SPI_SendData(FLUSH_TX);
	NRF24L01_CSN_HIGH;
}

/**
 Empty the receive buffer
*/
void NRF24L01_Flush_RX(void) {
	NRF24L01_CSN_LOW;
	SPI_SendData(FLUSH_RX);
	NRF24L01_CSN_HIGH;
}

/**
 Initializes the device
 @param Device_Mode = _TX_MODE, _RX_MODE
 @param CH = 0..125
 @param DataRate = _1Mbps, _2Mbps
 @param Address Address
 @param Address_Width Width direction: 3..5
 @param Size_Payload Data buffer size

 @see _TX_MODE
 @see _RX_MODE
 @see _1Mbps
 @see _2Mbps
*/
void NRF24L01_Init(char Device_Mode, char CH, char DataRate,
		char *Address, char Address_Width, char Size_Payload) {

	NRF24L01_CE_OUT; // Set Port DIR out

	// Disable Enhanced ShockBurst
	NRF24L01_Set_ShockBurst(_ShockBurst_OFF);

	// RF output power in TX mode = 0dBm (Max.)
	// Set LNA gain
	// 0b00000111 = 07
	NRF24L01_WriteReg(W_REGISTER | RF_SETUP, 0x07 | DataRate);

	NRF24L01_Set_Address_Width(Address_Width);

	NRF24L01_Set_RX_Pipe(0, Address, Address_Width, Size_Payload);

	NRF24L01_Set_CH(CH);

	NRF24L01_Set_TX_Address(Address, Address_Width); // Set Transmit address

	// Bits 4..6: Reflect interrupts as active low on the IRQ pin
	// Bit 3: Enable CRC
	// Bit 2: CRC 1 Byte  (0 - 1Byte, 1 - 2Byte)
	// Bit 1: Power Up
        // Bit 0: Rx/Tx mode
	// b00001010 = 0A
	NRF24L01_WriteReg(W_REGISTER | CONFIG, 0x0A | Device_Mode);

	delay(15);
}

/**
 Turn on transmitter, and transmits the data loaded into the buffer
*/
void NRF24L01_RF_TX(void) {
	NRF24L01_CE_HIGH;
	for(u8 i=0; i<200; i++)
        {
          nop();
          continue;
        }
	NRF24L01_CE_LOW;
}

/**
 Writes the buffer of data transmission

 @param Buf Buffer with data to send
 @param Size Buffer size

*/
void NRF24L01_Write_TX_Buf(char *Buf, int Size) {
	NRF24L01_WriteRegBuf(W_REGISTER | W_TX_PAYLOAD, Buf, Size);
}

/**
 Read the data reception buffer

 @param Buf Buffer with data received
 @param Size Buffer size

*/
void NRF24L01_Read_RX_Buf(char *Buf, int Size) {
	NRF24L01_ReadRegBuf(R_RX_PAYLOAD, Buf, Size);
}

//continuous carrier
void NRF24L01_Cont_Wave_ON(u8 channel, u8 time) {

  NRF24L01_WriteReg(W_REGISTER | CONFIG,0x7A);  //PWR_UP=1, PRIM_RX=0
  delay(10);
  NRF24L01_WriteReg(W_REGISTER | RF_SETUP,0x96); //CONT_WAVE=1, PLL_LOCK=1, RF_PWR=0dbm
  NRF24L01_Set_CH(channel);
  NRF24L01_CE_HIGH;
  for(u8 i=0; i<time; i++)
  {
      delay(1000);
  }
  NRF24L01_CE_LOW;

}

