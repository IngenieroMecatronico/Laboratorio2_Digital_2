/*
 * Prelaboratorio.c
 *
 * Created: 4/02/2025 22:22:16
 * Author : Juan Fernando Maldonado Menéndez - 22449
 * Catedrático: Katha Senn
 * Descripción: VOy a tener una pantalla LCD a la cual se conectan 2 potenciómetros, voy a mostrar los valores del ADC de los mismos y
 * voy a tener un contador que se administra con la hiperterminal de UART en la PC.
 */ 


//*********************************************************************************************
//Librerias
//*********************************************************************************************

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "POT/POT.h"
//*********************************************************************************************
//Prototipos de función
//*********************************************************************************************

void InitADC1(void);
void InitADC2(void);
//Función para iniciar enviar el valor de los potenciómetros.
void valorpot(uint8_t numpot, uint8_t valoradc);
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
//Escribir un str en el puerto UART.
void WriteTextUART(char * Texto);
//Caracter
void writeUART(char Caracter);
//Lectura ASCII
void LecturaAscii(void);


//*********************************************************************************************
//Variables globales
//*********************************************************************************************

volatile uint8_t valoradc = 0; //Variable donde guardare el valor ADC.
volatile uint8_t contador = 0;	  //Variable donde guardo el contador a desplegar por la hiperterminal.
volatile uint8_t BufferTX;		  //Variable doonde se guarda el valor leído.
volatile uint8_t datorecibido;	  //Variable para mantener el dato recibido.

int main(void)
{
	initUart9600();   //Inicializo mi TX y RX pero lo usaré a 115200
	sei();			  //Habilito las interrupciones globales
	InitADC1();		  //Inicio mi ADC.
	InitADC2();		  //Inicio mi ADC.
	initLDC8bit();	  //Inicio mi Pantalla.
	LCD_CMD(0x01);	  //Configuración para encendido.
	LCD_CMD(0x06);
	LCD_Set_Cursor(0,1);	//Posicionamiento en la columna 0, fila 1.
	LCD_write_String("S1:  S2:  S3:"); //Escribo el encabezado de mis valores.
	BufferTX = 0;						//Limpio mi variable donde recibo los datos de UART.
	contador = 0;						//Inicializo mi contador.
    while (1) 
    {
		ADCSRA |= (1<<ADSC);   //Habilito la interrupcion de ADC.
		LCD_Set_Cursor(0,2);   //Posicionamiento en la columna 0, fila 2.
		valorpot(2, valoradc); //Llamo a mi función para enviar mi valor ADC y que imprima lo del pot del ADC2.
		_delay_ms(100);		   //Retraso de 100 ms.
		LCD_Set_Cursor(5,2);   //Posicionamiento en la columna 5, fila 2.
		valorpot(4, valoradc); //Llamo a mi función para enviar mi valor ADC y que imprima lo del pot del ADC4.
		_delay_ms(100);
		LCD_Set_Cursor(11,2);  //Posicionamiento en la columna 11, fila 2.
		datorecibido = BufferTX;//Recibo el BUFFER o lo leído del UART y lo almaceno.
		//writeUART(BufferTX);
		switch(datorecibido){
				case 43:		//Si recibí un más, imprimo en la hiperterminal el contador en ASCII +1.
				LCD_Set_Cursor(11,2);//Seteo posición de nuevo.
				contador++;
				//LCD_Write_Char(contador);
				WriteTextUART("Entro_a_suma");
				WriteTextUART("\n\r");
				writeUART(contador+48);
				//_delay_ms(100);
				//LCD_Write_Char(contador+48);
				_delay_ms(100);
				BufferTX = 0;
				break;
				case 45://Si recibí un menos, imprimo en la hiperterminal el contador en ASCII -1.
				LCD_Set_Cursor(11,2);
				WriteTextUART("Entro_a_resta");
				WriteTextUART("\n\r");
				writeUART(contador+48);
				contador--;
				_delay_ms(100);
				BufferTX = 0;
				break;
		}
		LCD_Set_Cursor(11,2); //Seteo el punto.
		LCD_Write_Char(contador+48); //Imprimo en la LCD el ASCII del contador +1.
		_delay_ms(100);
		
			
	}		
}
//*********************************************************************************************
//Subrutinas
//*********************************************************************************************


void InitADC1(void){
	ADMUX |= (1 << REFS0);      //Estoy configurando a que funcione con AVCC.
	ADMUX &= ~(1 << REFS1);
	//Justificacion a la izquierda.
	ADMUX |= (1 << ADLAR);
	//Apagamos la salida digital de PINC2
	DIDR0 |= (1 << ADC2D);
	//Configuración del canal 0 ya que allí esta mi potenciómetro.
	ADMUX |= (1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADCSRA = 0;
	//Habilitamos la bandera del ADC
	ADCSRA |= (1 << ADIE);
	//Habilitamos el prescaler a 128.
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
	//Habilitamos el ADC
	ADCSRA |= (1 << ADEN);
	
	
}


void InitADC2(void){
	ADMUX |= (1 << REFS0);      //Estoy configurando a que funcione con AVCC.
	ADMUX &= ~(1 << REFS1);
	//Justificacion a la izquierda.
	ADMUX |= (1 << ADLAR);
	//Apagamos la salida digital de PINC3
	DIDR0 |= (1 << ADC4D);
	//Configuración del canal 3 ya que allí esta mi potenciómetro.
	ADMUX |= (1 << MUX2);
	ADMUX &= ~(1 << MUX1);
	ADCSRA = 0;
	//Habilitamos la bandera del ADC
	ADCSRA |= (1 << ADIE);
	//Habilitamos el prescaler a 128.
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
	//Habilitamos el ADC
	ADCSRA |= (1 << ADEN);
	
	
}

void valorpot(uint8_t numpot, uint8_t valoradc){
	uint8_t entero, primdecimal, segdecimal, prueba;
	uint16_t numero;
	prueba = ADCH;
	ADMUX = (ADMUX & 0xF0);
	if (numpot == 2)
	{
		//Este potenciómetro controla solo la primera casilla de la pantalla.
		ADMUX = (ADMUX & 0xF0) | numpot; //0b0000 0010
		ADCSRA |= (1<<ADSC);
		//Conversión de 255-0 y 500-0
		numero = 1.96*prueba;
		//Clasificación de los valores:
		entero = (numero)/100;
		primdecimal = (numero - (entero)*100)/10;
		segdecimal = (numero - (entero*100)-(primdecimal*10));
		//Está llegando hasta 2.43
		LCD_Write_Char(entero+48);
		//LCD_Write_Char(entero+48);
		LCD_Write_Char('.');
		LCD_Write_Char(primdecimal+48);
		LCD_Write_Char(segdecimal+48);
		_delay_ms(100);	
	}
	if (numpot == 4)
	{
		ADMUX = (ADMUX & 0xF0) | numpot; //0b0000 0100;
		ADCSRA |= (1<<ADSC);
		numero = 1.96*prueba;
		//Clasificación de los valores:
		entero = (numero)/100;
		primdecimal = (numero - (entero)*100)/10;
		segdecimal = (numero - (entero*100)-(primdecimal*10));
		
		LCD_Write_Char(entero+48);
		LCD_Write_Char('.');
		LCD_Write_Char(primdecimal+48);
		LCD_Write_Char(segdecimal+48);
		_delay_ms(100);
	}
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void WriteTextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)) );
		UDR0 = Texto[i];
	}
}

void writeUART(char Caracter){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
	//PORTD = Caracter;
}



//Revisar ya que me tira valores raros, ver si es por que no requiere la conversión del mapeo diferente.
//*****************************************************************************************************************************************
//ISR interrupciones:
//*****************************************************************************************************************************************
ISR(ADC_vect){
	valoradc = ADCH;
	ADCSRA |= (1<<ADIF); // Apaga la bandera de interrupción del ADC
}

ISR(USART_RX_vect){  //uint8_t bit2; //Interrupción para obtener los valores de UDR0 en BUFFERTX.
	BufferTX = UDR0;
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = BufferTX;
}
