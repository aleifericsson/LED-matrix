/* 
 * File:   main.c
 * Author: ikram
 *
 * Created on May 14, 2024, 7:06 PM
 */
#define F_CPU   16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "bad_apple.h"

/*
 * 
 */

void clock_pulse() {
    PORTD = 0b11111100;
    PORTB = 0b00011111;
    _delay_us(1);
    
    PORTB = 0b00001111;
    
    _delay_us(1);
}

void reset_pulse() {
    PORTD = 0b11111100;
    PORTB = 0b00101111;
    _delay_us(1);
    
    PORTB = 0b00001111;
    
    _delay_us(1);
}
    

void write_columns(uint16_t columns) {
    PORTB =((columns & 0b0000000000000001) << 3)
          |((columns & 0b0000000000000010) << 1)
          |((columns & 0b0000000000000100) >> 1)
          |((columns & 0b0000000000001000) >> 3);
    PORTD =((columns & 0b0000000000010000) << 3)
          |((columns & 0b0000000000100000) << 1)
          |((columns & 0b0000000001000000) >> 1)
          |((columns & 0b0000000010000000) >> 3)
          |((columns & 0b0000000100000000) >> 5)
          |((columns & 0b0000001000000000) >> 7);
          
}

int main(void) {
    DDRD = 0b11111100;
    DDRB = 0b00111111;
    
    while(1) {
        for (int frame = 0; frame < sizeof(video)/sizeof(video[0]); frame++) {
            for (int i = 0; i < 40000/(64*FPS); i++) {
                reset_pulse();
                for (int row = 9; row >= 0; row--) {
                    uint16_t columns;
                    // 11 = full brightness, 10 = 67%, 01 = 33%, 00 = 0%.
                    switch (i % 3) {
                    case 0: // 11 + 10 + 01
                        columns = video[frame][row] | video[frame][2*row + 1];
                        break;
                    case 1: // 11 + 10 
                        columns = video[frame][row] ^ video[frame][2*row + 1];
                        break;
                    case 2: // 11
                        columns = video[frame][row] & video[frame][2*row + 1];
                        break;
                    }
                    write_columns(video[frame][row]);
                    
                    _delay_us(10);
                    clock_pulse();
                }
                
                
            }
        }
    }
    return 0;
}

