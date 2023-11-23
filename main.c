#include <stdio.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "radio.h"
#include "time.h"
#include "leds.h"
#include "button.h"
#include "serial.h"
#include "timer.h"

#define NODE 0x0001
#define DESTINATION 0x0002

static unsigned char buffer[128];
static unsigned char counter=0;

volatile unsigned char state=0;

//interrupt test
ISR(INT0_vect) {
}

void init(void) {
	
	//Set and enable sleep mode
	//set_sleep_mode(SLEEP_MODE_IDLE); // select idle mode
	//set_sleep_mode(SLEEP_MODE_ADC); // select ADC noise reduction mode
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN); //Power Down
	//set_sleep_mode(SLEEP_MODE_PWR_SAVE); // select power-save mode
	set_sleep_mode(SLEEP_MODE_STANDBY); //Standby
	//set_sleep_mode(SLEEP_MODE_EXT_STANDBY); // select extended standby mode
	//sleep_enable();
	
	//serial_init(9600);
	leds_init();
	radio_init(NODE);
	
	//enable interrupts
	EIMSK |= 1<<INT0;
	EICRA_struct.isc0 = 0x03;
	
	sei();
	shutdown();
	powerdown();
}

void loop(void) {
	
	//Sense
	leds_on(1);
	leds_on(2);
	leds_on(3);
	timer_wait_milli(60);
	leds_off(1);
	leds_off(2);
	leds_off(3);
	
	//Transmit startup
	radio_startup();
	
	//Transmit begin
	buffer[0] = 0x00;
	buffer[1] = 'X';
	buffer[2] = 'Y';
	buffer[3] = counter++;
	radio_send(buffer,4,DESTINATION);
	//printf("TX at %lu to %d: %02x %02x %02x %02x\n\r", time_millis(), DESTINATION, buffer[0], buffer[1], buffer[2], buffer[3]);
	radio_tx_done();
	//Transmit end
	
	//Transmit shutdown
	radio_shutdown();
	//shutdown();
	
	sleep_enable();
	sleep_cpu();
	//printf("Wakeup %d \n\r", x++);
}

void radio_shutdown() // shut down the radio
{
	// reset
	TRXPR = 0;
	timer_wait_milli(6);
	TRXPR = (1<<TRXRST);

	// go to sleep
	TRXPR |= (1<<SLPTR);
}

void radio_startup() // starts up the radio
{
	// reset
	TRXPR = 0;
	timer_wait_milli(6);
	TRXPR = (1<<TRXRST);

	// go to sleep
	TRXPR |= (0<<SLPTR);
}

void ports_shutdown() // set I/O ports into their lowest current state
{
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0x00; PORTC = 0x00;
	DDRD = 0x00; PORTD = 0x00;
	//DDRE = 0x00; //PORTE = 0x00;
	DDRF = 0x00; PORTF = 0x00;
	DDRG = 0x00; PORTG = 0x00;
}

void serial_shutdown() //shut down the serial line
{
	UCSR0B = 0x00;        // disable UART0
}

void shutdown()
{
	//clear_jtd();
	radio_shutdown();
	serial_shutdown();
	ports_shutdown();
}

void powerup()
{
	//enable everything
	PRR0 = 0x00;
	PRR1 = 0x00;
}

void powerdown()
{
	//disabel everything
	PRR0 = 0x01; //Power Reduction Register
	PRR1 = 0x01;
}

void clear_jtd() //disable the on-chip hardware support for debugging
{
	unsigned char value = MCUCR;
	value |= (1<<JTD) | (1<<PUD);
	MCUCR = value;
	MCUCR = value;
}