/* Name: main.c
    acrobat code
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "m_general.h"
#include "m_rf.h"
#include "m_bus.h"
#include "m_imu.h" //ADD m_imu.c TO THE PROJECT PATH

int main(void)
{
    //set clock to 16MHz
    m_clockdivide(0);
    
    //call the external setup functions
    wireless_setup();
    timer1setup(1,0,0x00FF); //timer1 is initially OFF

    //set direction and enable pins here
    
    while (1) {
        //do stuff
    }
    return 0;   /* never reached */
}

ISR(INT2_vect){ //interrupt when we receive an incoming wireless signal
    //update proportionality constants
}

ISR(TIMER1_OVF_vect){ //interrupt when timer1 overflows
}

//accel_setup here

void wireless_setup (void) {
    m_bus_init(); //demask the interrupt vector on D2
    sei(); //enable global interupts
    m_rf_open(channel, RXadddress, packet_length); //specify channels from above
    
    //flash the green LED to show successful setup
    m_green(ON)
    m_wait(100);
    m_green(OFF)
    m_wait(100);
}

void timer1setup(int trigger, int time_scale, int max){
    
    //turn clock on and scale by the time_scale input.
    //defaults to scale by 1
    switch (time_scale) {
        case 0:
            clear(TCCR1B, CS10);
            clear(TCCR1B, CS11);
            clear(TCCR1B, CS12);
            break;
        case 1:
            set(TCCR1B, CS10);
            clear(TCCR1B, CS11);
            clear(TCCR1B, CS12);
            break;
        case 8:
            clear(TCCR1B, CS10);
            set(TCCR1B, CS11);
            clear(TCCR1B, CS12);
            break;
        case 64:
            set(TCCR1B, CS10);
            set(TCCR1B, CS11);
            clear(TCCR1B, CS12);
            break;
        case 256:
            clear(TCCR1B, CS10);
            clear(TCCR1B, CS11);
            set(TCCR1B, CS12);
            break;
        case 1024:
            set(TCCR1B, CS10);
            clear(TCCR1B, CS11);
            set(TCCR1B, CS12);
            break;
        default:
            set(TCCR1B, CS10);
            clear(TCCR1B, CS11);
            clear(TCCR1B, CS12);
            break;
    }
    
    //set to:
    //(mode 15) UP to OCR1A, PWM mode
    set(TCCR1A, WGM10);
    set(TCCR1A, WGM11);
    set(TCCR1B, WGM12);
    set(TCCR1B, WGM13);
    
    //set OCR1B to the switch_voltage input after checking to make sure input is valid
    if ((trigger > 0x00FF) || (trigger < 0x0000)) {
        trigger = 127;
    }
    OCR1B = trigger;
    OCR1A = max;
    
    //set B6 for output (PORTB 6 is set and cleared by timer)
    set(DDRB, 6);
    
    //configure timer to:
    //clear at OCR1B
    //this sets and clears output of pin B6
    //sets at rollover
    clear(TCCR1A,COM1B0);
    set(TCCR1A,COM1B1);
    
    //enable the overflow interrupt on timer1
    set(TIMSK1, TOIE1);
    
    //flash the green LED to show successful setup
    m_green(ON)
    m_wait(100);
    m_green(OFF)
    m_wait(100);
}