/*
 * POT.c
 *
 * Created: 5/02/2025 17:54:24
 *  Author: ferma
 */ 
#define F_CPU 16000000UL
#include "POT.h"
#include <util/delay.h>

volatile uint8_t BufferTX;

void initLDC8bit(void){
	//Establezco que mi puerto D será salidas.
	DDRD = 0xFF;
	//Apago mi puerto D.
	PORTD = 0;
	//Establezco que mi puerto B serán entradas excepto PB0 y PB5.
	DDRB = 0;
	DDRB |= (1<<PORTB0)|(1<<PORTB5);
	PORTB = 0;
	//Establezco el puerto C como 2 salidas y el resto entradas.
	DDRC = 0x03;
	//Apago mi puerto C.
	PORTC = 0;
	//Función de encendido.
	LCD_Port(0x00); //Inicializo mi pantalla.
	//Pasos para otra posible pantalla.
	/*
	LCD_Port(0x00); //Inicializo mi pantalla.
	_delay_ms(20);  //Se esperan los 20 ms para empezar a trabajar.
	LCD_Port(0x30); //Se manda un uno a PD4 y PD5.
	_delay_ms(5);	//Se esperan 5 ms para continuar.
	LCD_Port(0x30); //Se manda un uno a PD4 y PD5.
	_delay_ms(1);	//Se espera 100us, pero yo doy 1ms.
	LCD_Port(0x30); //Se manda un uno a PD4 y PD5.
	_delay_ms(20);  //Espero luego de haber encendido a pantalla.
	*/
	_delay_ms(20);
	//Funcion set:
	LCD_Port(0x38); //Se manda un uno a PD3-PD5.
	_delay_us(1);
	//Display on/off
	LCD_Port(0b00001100);
	_delay_us(1);
	//Entry mode:
	LCD_Port(0x06);
	_delay_us(1);
	//Clear Display:
	LCD_Port(0x01);
	_delay_ms(5);
	//Ya inicializamos la pantalla.
}

void LCD_CMD(char a){
	PORTB &= ~(1<<PORTB0);  // configuro mi valor de RS = 0, para que lo interprete como comando.
	LCD_Port(a);
	//Enable en 1.
	PORTB |= (1<<PORTB5);
	_delay_ms(4);
	//Vuelvo a apagar el enable.
	PORTB &= ~(1<<PORTB5);
	_delay_ms(50);
}

void LCD_Port(char a){
	if(a & 1){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTC |= (1<<PORTC0);
	}
	else{
		PORTC &= ~(1<<PORTC0);
	}
	if(a & 2){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTC |= (1<<PORTC1);
	}
	else{
		PORTC &= ~(1<<PORTC1);
	}
	if(a & 4){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTD |= (1<<PORTD2);
	}
	else{
		PORTD &= ~(1<<PORTD2);
	}
	if(a & 8){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTD |= (1<<PORTD3);
	}
	else {
		PORTD &= ~(1<<PORTD3);
	}
	if(a & 16){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTD |= (1<<PORTD4);
	}
	else {
		PORTD &= ~(1<<PORTD4);
	}
	if(a & 32){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTD |= (1<<PORTD5);
	}
	else{
		PORTD &= ~(1<<PORTD5);
	}
	if(a & 64){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTD |= (1<<PORTD6);
	}
	else{
		PORTD &= ~(1<<PORTD6);
	}
	if(a & 128){ //Tengo que ir asignando los valores del contador binario a cada bit de la LCD.
		//PD0 = 1;
		PORTD |= (1<<PORTD7);
	}
	else {
		PORTD &= ~(1<<PORTD7);
	}
}
//Función para escribir un caracter.
void LCD_Write_Char(char c){
	char Ccaracter;
	Ccaracter = c;
	
	//Establezco mi RS en 1.
	PORTB |= (1<<PORTB0);
	LCD_Port(Ccaracter);
	//Enciendo mi Enable.
	PORTB |= (1<<PORTB5);
	_delay_ms(4);
	//Apago mi Enable.
	PORTB &= ~(1<<PORTB5);
	_delay_ms(50);
	
}

//Función para enviar una cadena.
void LCD_write_String(char *a){
	int i;
	for (i = 0; a[i] != '\0'; i++)
	{
		LCD_Write_Char(a[i]);
	}
}

//Función para colocar el cursor:
void LCD_Set_Cursor(char x, char y){
	char pos;
	if (y == 1)
	{
		pos = (0x80 + x);
		LCD_CMD(pos);
	}
	else {
		pos = (0xC0 + x);
		LCD_CMD(pos);
	}
}
//Inicialización del uart.
void initUart9600(void){
	//Paso 1: RX como entrada y TX como salida.
	DDRD |= (1<<DDD1);
	DDRD &=	~(1<<DDD0);
	//Paso 2: Configurar UCSR0A
	UCSR0A = 0;
	//Paso 3: Configurar UCSR0B: habilitamos ISR de recepción y RX Y TX.
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0); //Estoy activando la bandera que me indica que se recibió el mensaje.
	//Paso 4: COnfigurar UCSR0C: Primero asíncrono, sin pariedad, bit 1 de stop, Data bits/ 8 bits,
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	//Paso 5: Configurar velocidad de Baudrate: 115200
	UBRR0 = 8;
}