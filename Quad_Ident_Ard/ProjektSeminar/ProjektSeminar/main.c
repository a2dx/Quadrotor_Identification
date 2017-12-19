#define F_CPU 16000000L // Clock Speed
#include <avr/io.h>
#include <util/delay.h>
#define BAUD 115200	// Baudrate
// Calculations for the Baudrate
#define MYUBRR F_CPU/8/BAUD-1

// variable to store incomming bytes
unsigned char incomming;

// send data
void USART_Transmit(unsigned char data){ // unsigned char: 0 - 255
	
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

// receive data
unsigned char USART_Receive( void ){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

// init PWM
void initPWM(){
	// Set Pin 9 (Arduino) to Output. Atmega 328p Pin: (OC1)PB1
	DDRB = (1 << PORTB1);
	
	// Set non-inverted mode with COM1A1 and set Waveform Generation Mode with WGM11
	TCCR1A |= 1<<WGM11 | 1<<COM1A1;
	
	// Set Waveform Generation Mode with WGM12 and WGM13 and set prescaler value to 1 with CS10
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	
	// Set TOP = maxvalue of Timer1 to 4000
	ICR1 = 3999; // 4000 - 1
	
	// Set output compare match value
	OCR1A = 1900; // 1999 = 50 % duty cycle
}

void initUSART( unsigned int ubrr){
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0A = (1<<UDRE0)|(1<<U2X0);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR0C =  (1 << UCSZ01) | (1 << UCSZ00);
}

int main(void){
	
	initUSART(MYUBRR);
	initPWM();
	
	while (1){
		
		// check if data is available
		if(UCSR0A & (1<<RXC0)){
			// receive data
			incomming = USART_Receive();
			// send data to confirm receipt
			USART_Transmit(incomming);
			// set PWM
			OCR1A = 1950 + incomming * 8;
		}
	}
}


