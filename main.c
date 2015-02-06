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
    //set clock to 4MHz so the accelerometer reading rate works out
    m_clockdivide(2); // = 16Mhz / 2^2 = 4Mhz
    
    //call the external setup functions
    wireless_setup();
    timer1setup(1,1,0x00FF); //timer1 using this for PWM (set OCR1B / OCR1A for duty cycle)
    timer3setup(64); //timer3 start for the sampling rate of acc. Clock speed = 62500 if m_clockdivide(2)
    
    set(DDRC, 6); //C6 for motor direction
    //PORTB 6 is the enable line if we're using PWM
    
    //declare the constants
    double Kp = 0;
    double Ki = 0;
    double Kd = 0;
    
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

ISR(TIMER3_OVF_vect){

}

void accel_setup{
     accel_scale = 1;
     gyro_scale = 1;
    
//    //setup functions from wiki
//    unsigned char = m_imu_init(unsigned char accel_scale, unsigned char gyro_scale)
//    unsigned char = m_imu_raw(int* data)
    
    }
    
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

void timer3setup(int scale){
    switch (scale) {
        case 0:
            clear(TCCR3B, CS32);
            clear(TCCR3B, CS31);
            clear(TCCR3B, CS30);
            break;
        case 1:
            clear(TCCR3B, CS32);
            clear(TCCR3B, CS31);
            set(TCCR3B, CS30);
            break;
        case 1024
            clear(TCCR3B, CS32);
            clear(TCCR3B, CS31);
            set(TCCR3B, CS30);
            break;
        default:
            set(TCCR3B, CS32);
            clear(TCCR3B, CS31);
            set(TCCR3B, CS30);
            break;
    }
    
    //set to mode 14: UP to ICR3 in PWM mode (action at OCR3A)
    set(TCCR3B, WGM33);
    set(TCCR3B, WGM32);
    set(TCCR3A, WGM31);
    clear(TCCR3A, WGM30);
    
    //enable overflow interrupts for Timer 3
    set(TIMSK3, TOIE3)
    
    ICR3 = ;//something to count up to
    OCR3A = ;//when it matches this do... something
    m_green(ON);
    m_wait(100);
    m_green(OFF)
    m_wait(100);
}