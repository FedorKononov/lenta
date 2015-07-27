#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

void USART_Init();
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);

int main(void)
{
	// порты работают наружу
	DDRD = 0b11111111;

	// посигналим что мы включились
	PORTD = 0b01000000;

	// канал которым дергаем
	unsigned char channel = 0b00100000;

	// uart на 9600
	USART_Init();

	USART_Transmit('=');

	_delay_ms(500);

	// на всех портах 0
	PORTD = 0b00000000;

	for(;;)
	{
		// ловим последовательность
		if ( (USART_Receive() == 'j') &&
			(USART_Receive() == 'o') &&
			(USART_Receive() == 'p') &&
			(USART_Receive() == 'a') &&
			(USART_Receive() == 'i') &&
			(USART_Receive() == 'x') &&
			(USART_Receive() == 'e') &&
			(USART_Receive() == 'p') &&
			(USART_Receive() == '\n'))
		{
			// переключаем
			PORTD ^= channel;

			// посылаем статус состояния
			USART_Transmit(PORTD == 0 ? '+' : '-');
		}
	}

	return 0;
}

void USART_Init()
{
	// baud rate 9600
	UBRRH = 0x00;
	UBRRL = 0x33;
	// Разрешение на прием и н апередачу через USART
	UCSRB = ( ( 1 << RXEN ) | ( 1 << TXEN ) );
	UCSRC = (1<<USBS) | (3<<UCSZ0);
}
 
 
unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}
 
void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}