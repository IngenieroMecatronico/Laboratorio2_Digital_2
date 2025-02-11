/*
 * POT.h
 *
 * Created: 5/02/2025 17:54:38
 *  Author: ferma
 */ 


#ifndef POT_H_
#define POT_H_

#include <avr/io.h>
#include <stdint.h>
//Función para enviar un caracter a la pantalla.
void caracter(char *a);
//Función para inicializar la pantalla LCD.
void initLDC8bit(void);
//Función para colocar en el puerto, un valor.
void LCD_Port(char a);
//Función para enviar un comando.
void LCD_CMD(char a);
//Función para escribir un caracter.
void LCD_Write_Char(char c);
//Función para mandar una palabra.
void LCD_write_String(char *a);
//Desplazamiento hacia la derecha.
void LCD_Shift_Right(void);
//Desplazamiento hacia la izquierda.
void LCD_Shift_Left(void);
//Establecer un cursor.
void LCD_Set_Cursor(char x, char y);
//Inicialización del UART.
void initUart9600(void);


#endif /* POT_H_ */