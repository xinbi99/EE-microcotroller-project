/* 
 * File:   project3.c
 * Author: xfb5013
 *
 * Created on November 16, 2019, 8:36 PM
 */



#define FCY 3685000UL
#define INSTRUCTION 0
#define DATA 1
#define pgm_delay_ms 2

#include <p33EP64MC502.h>
#include <libpic30.h>
#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#pragma config OSCIOFNC = ON
#pragma config ICS = PGD2 // Communicate on PGED2 and PGEC3
#pragma config JTAGEN = OFF

void Init_DIO_Ports (void);
typedef enum {
    S0, S1, S2, S3, S4
} STATES;

void Init_DIO_Ports (void) {
    ANSELA = 0; //Initialize all RA pins for digital IO
    TRISA = 0b11111; //All RA pins for input
    ANSELB = 0; // Initialize all RB pins for digital IO
    TRISB = 0xC0E0; //RB pins 8 to 13 for output, the rest for input
}
void Toggle_Enable_line (void) {
    __delay_ms(pgm_delay_ms);   // delay
    PORTBbits.RB13 = 1;         // set E high
    __delay_ms(pgm_delay_ms);   // delay
    PORTBbits.RB13 = 0;         // set E low
    __delay_ms(pgm_delay_ms);   // delay
}

void Write_LCD_Nibble(int data, int cmd) {
 LATBbits.LATB8 = data & 0b1;       //8th bit in 1 bit length
 LATBbits.LATB9 = data >> 1 & 0b1;  //9th bit which is one bit to the right, in 1 bit length
 LATBbits.LATB10 = data >> 2 & 0b1; //10th bit which is two bits to the right, in 1 bit length
 LATBbits.LATB11 = data >> 3 & 0b1; //11th bit which is three bits to the right, in 1 bit length
 
 PORTBbits.RB12 = cmd;   // set RS; cmd = 0 for instruction, 1 for data
 Toggle_Enable_line ();  // strobe data into LCD module
 PORTBbits.RB8 =  0;     // set all RB8 to RB13 to low
 PORTBbits.RB9 =  0;
 PORTBbits.RB10 =  0;
 PORTBbits.RB11 =  0;
 PORTBbits.RB12 =  0;
 PORTBbits.RB13 =  0;
}

void Write_LCD_Byte(int data, int cmd) {
    Write_LCD_Nibble((data & 0x00F0) >> 4, cmd); // write upper nibble
    Write_LCD_Nibble( data & 0x000F, cmd);       // write lower nibble
}

void Init_LCD_Module(void) {
    Write_LCD_Nibble(0b0011, INSTRUCTION);  // Initialize the LCD Module
    Write_LCD_Nibble(0b0011, INSTRUCTION);
    Write_LCD_Nibble(0b0011, INSTRUCTION);
    Write_LCD_Nibble(0b0010, INSTRUCTION);  // invoke 4-bit mode
    Write_LCD_Byte(0b00101000, INSTRUCTION);// 4-bit mode, two-line,5X7 dot
    Write_LCD_Byte(0b00000001, INSTRUCTION);// clear display, cursor at 0x00
    Write_LCD_Byte(0b00001111, INSTRUCTION);// display on,cursor blink/underline
}

int main(void) {
    STATES  state = S0;
    Init_DIO_Ports(); //Call initialize functions
    Init_LCD_Module();
    int input_0, input_1, input_2, input_3, input, reset, counter, idx;
    while (1){
        input_0 = PORTAbits.RA3 ^ 1; // read inputs and XOR with one
        input_1 = PORTAbits.RA2 ^ 1; // to have high = pressed
        input_2 = PORTAbits.RA1 ^ 1; // low = not pressed
        input_3 = PORTAbits.RA0 ^ 1;
        input = 8 * input_3 + 4 * input_2 + 2 * input_1 + input_0; // add the buttons to make case numbers
        switch (state) {
            case S0 :
                switch (input) {
                    case 0: state = S0; break;
                    case 1: state = S0; break;
                    case 2: state = S0; break;
                    case 3: state = S0; break;
                    case 4: state = S0; break;
                    case 5: state = S1; break; // first digit
                    case 6: state = S0; break;
                    case 7: state = S0; break;
                    case 8: state = S0; break;
                    case 9: state = S0; break;
                    case 10: state = S0; break;
                    case 11: state = S0; break;
                    case 12: state = S0; break;
                    case 13: state = S0; break;
                    case 14: state = S0; break;
                    case 15: state = S0; break;
                }; break;
            case S1 :
                switch (input) {
                    case 0: state = S1; break;
                    case 1: state = S0; break;
                    case 2: state = S0; break;
                    case 3: state = S0; break;
                    case 4: state = S0; break;
                    case 5: state = S0; break;
                    case 6: state = S0; break;
                    case 7: state = S0; break;
                    case 8: state = S0; break;
                    case 9: state = S2; break; // second digit
                    case 10: state = S0; break;
                    case 11: state = S0; break;
                    case 12: state = S0; break;
                    case 13: state = S0; break;
                    case 14: state = S0; break;
                    case 15: state = S0; break;
                }; break;
            case S2 :
                switch (input) {
                    case 0: state = S2; break;
                    case 1: state = S3; break; // third digit
                    case 2: state = S0; break;
                    case 3: state = S0; break;
                    case 4: state = S0; break;
                    case 5: state = S0; break;
                    case 6: state = S0; break;
                    case 7: state = S0; break;
                    case 8: state = S0; break;
                    case 9: state = S0; break;
                    case 10: state = S0; break;
                    case 11: state = S0; break;
                    case 12: state = S0; break;
                    case 13: state = S0; break;
                    case 14: state = S0; break;
                    case 15: state = S0; break;
                }; break;
            case S3 :
                switch (input) {
                    case 0: state = S3; break;
                    case 1: state = S0; break;
                    case 2: state = S0; break;
                    case 3: state = S4; break; // 4th digit
                    case 4: state = S0; break;
                    case 5: state = S0; break;
                    case 6: state = S0; break;
                    case 7: state = S0; break;
                    case 8: state = S0; break;
                    case 9: state = S0; break;
                    case 10: state = S0; break;
                    case 11: state = S0; break;
                    case 12: state = S0; break;
                    case 13: state = S0; break;
                    case 14: state = S0; break;
                    case 15: state = S0; break;
                }; break;
            case S4 :
                switch (input) {
                    case 0: state = S4; break; // Stay at S4 unless a button is  pressed
                    case 1: state = S0; break;
                    case 2: state = S0; break;
                    case 3: state = S0; break;
                    case 4: state = S0; break;
                    case 5: state = S0; break;
                    case 6: state = S0; break;
                    case 7: state = S0; break;
                    case 8: state = S0; break;
                    case 9: state = S0; break;
                    case 10: state = S0; break;
                    case 11: state = S0; break;
                    case 12: state = S0; break;
                    case 13: state = S0; break;
                    case 14: state = S0; break;
                    case 15: state = S0; break;
                }; break;
        }
        switch (state){ // outputs
            case S0: {
                Write_LCD_Byte(0b10000000, INSTRUCTION);
                for (idx = 0; idx < strlen("STATE = S0"); idx++)
                Write_LCD_Byte("STATE = S0"[idx], DATA); // S0 outputs
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0; 
                break;
            }
            case S1: {
                Write_LCD_Byte(0b10000000, INSTRUCTION);
                for (idx = 0; idx < strlen("STATE = S1"); idx++)
                Write_LCD_Byte("STATE = S1"[idx], DATA); // S1 outputs
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 1; 
                break;
            }
            case S2: {
                Write_LCD_Byte(0b10000000, INSTRUCTION);
                for (idx = 0; idx < strlen("STATE = S2"); idx++)
                Write_LCD_Byte("STATE = S2"[idx], DATA); // S2 outputs
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0; 
                break ;
            }
            case S3: {
                Write_LCD_Byte(0b10000000, INSTRUCTION);
                for (idx = 0; idx < strlen("STATE = S3"); idx++)
                Write_LCD_Byte("STATE = S3"[idx], DATA); // S3 outputs
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 1; 
                break;
            }
            case S4: {
                Write_LCD_Byte(0b10000000, INSTRUCTION);
                for (idx = 0; idx < strlen("STATE = S4"); idx++)
                Write_LCD_Byte("STATE = S4"[idx], DATA); // S4 outputs
                LATBbits.LATB0 = 0;
                LATBbits.LATB1 = 1;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0; 
                break;
            }
        }
        for (counter = 1; counter <= 1000; counter ++){
            reset = PORTAbits.RA4 ^ 1; //read reset button
            if (reset == 1) { // reset button functionality
                state = S0;
                Write_LCD_Byte(0b10000000, INSTRUCTION);
                for (idx = 0; idx < strlen("STATE = S0"); idx++)
                Write_LCD_Byte("STATE = S0"[idx], DATA); // Reset outputs to S0
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
            }
            LATBbits.LATB4 = 1;
            __delay_ms(2);
        }
        for (counter = 1; counter <= 1000; counter ++){
            reset = PORTAbits.RA4 ^ 1; // read reset button
            if (reset == 1) { // reset button functionality
                state = S0;
                Write_LCD_Byte(0b10000000, INSTRUCTION);
                for (idx = 0; idx < strlen("STATE = S0"); idx++)
                Write_LCD_Byte("STATE = S0"[idx], DATA); // Reset outputs to S0
                LATBbits.LATB0 = 1; 
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
            }
            LATBbits.LATB4 = 0;
            __delay_ms(2);
        }
        ClrWdt();
    }
}

