#ifndef __MATRIX_LED_H__
#define __MATRIX_LED_H__
void _74HC595_WriteByte(unsigned char Byte);
void MatrixLED_Show(unsigned char Column,unsigned char Line);
void MatrixLED_Init();

#endif