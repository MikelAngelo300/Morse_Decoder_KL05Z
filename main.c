/**
 * @file main.c
 * @author Ryszard Mleczko
 * @date January 2025
 * @brief File containing Morse_Decoder code
 */

#include "MKL05Z4.h"
#include "lcd1602.h"
#include "frdm_bsp.h"
#include "klaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DOT  '.'   
#define DASH '-'   
#define DOT_THRESHOLD 300  
#define NEXT_CHAR_THRESHOLD 1000 
#define SPACE_THRESHOLD 3000

const char *morse_codes[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-",
    ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-",
    ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----.", "-----", ".-.-.-", "--..--", ".----.", ".-..-.",
    "..--.-", "---...", "-.-.-.", "..--..", "-.-.--", "-....-", ".-.-.", "-..-.", "-.--.",
    "-.--.-", "-...-", ".--.-."
};
const char morse_letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,'\"_:;?!-+/()=@";
static char decoded_message[18] = ""; 
static char morse_input[18] = ""; 

volatile uint8_t morse_index = 0;
volatile uint8_t len = 0;   
volatile uint8_t S2_press = 0;         
volatile uint8_t S3_press = 0;   
volatile uint8_t flag_measure = 0;    
volatile uint8_t debounce_time = 0; 
volatile uint32_t press_time = 0;     
volatile uint32_t no_press_time = 0;

char decode_morse(const char*);
void PORTA_IRQHandler(void);
void SysTick_Handler(void);

/**
 * @brief Function to decode morse code
 * @param code - morse code to decode
 * @return decoded character or '*' if the code is unknown
 */
char decode_morse(const char *code) {
    for (int i = 0; i < 52; i++) {
        if (strcmp(code, morse_codes[i]) == 0) {
            return morse_letters[i];
        }
    }
    return '*'; 
}

/**
 * @brief Function to handle PORTA interrupts
 */
void PORTA_IRQHandler(void) {
    uint32_t flags = PORTA->ISFR; 

    if (flags & S2_MASK && debounce_time == 0) {
        if (!(PTA->PDIR & S2_MASK)) { 
            flag_measure = 1;  
            press_time = 0;   
        } else { 
            flag_measure = 0; 
            S2_press = 1;    
        }
        no_press_time = 0;
    }

    if (flags & S3_MASK && debounce_time == 0) {
        S3_press = 1;
    }

    debounce_time = 50;      
    PORTA->ISFR = flags; 
}

/**
 * @brief Function to handle SysTick interrupts
 */
void SysTick_Handler(void) {
    if (flag_measure) {
        press_time++; 
    }
    if (!flag_measure) {
        no_press_time++; 
    }
    if (debounce_time > 0) {
        debounce_time--;
    }
}

int main(void) {
    Klaw_Init();       
    Klaw_S2_3_Int();     
           
    LCD1602_Init();    
    LCD1602_Backlight(TRUE); 

    LCD1602_ClearAll();  
    LCD1602_Print("Morse Decoder");

    SysTick_Config(SystemCoreClock / 1000);  

    while (1) {
        if (S2_press) { 
            S2_press = 0; 

            // Check if the time between button presses is less than the threshold for a dot
            if (press_time < DOT_THRESHOLD) {
                if (morse_index < sizeof(morse_input) - 1) {
                    morse_input[morse_index++] = DOT;
                }
            } else { // Check if the time between button presses is greater than the threshold for a dash
                if (morse_index < sizeof(morse_input) - 1) {
                    morse_input[morse_index++] = DASH;
                }
            }

            LCD1602_SetCursor(0, 0);
            LCD1602_Print("                   "); 
            LCD1602_SetCursor(0, 0);
            LCD1602_Print(morse_input);
        }

        // Check if the time between button presses is greater than the threshold for the next character
        if (no_press_time > NEXT_CHAR_THRESHOLD && no_press_time < SPACE_THRESHOLD) { 
            if (morse_index > 0) {
                char decoded_char = decode_morse(morse_input);
                
                LCD1602_SetCursor(0, 0);
                LCD1602_Print("                   "); 
                if (decoded_char != '*') {
                    len = (uint8_t)strlen(decoded_message);
                    decoded_message[len] = decoded_char;
                    
                    LCD1602_ClearAll();
										LCD1602_SetCursor(0, 1);
                    LCD1602_Print(decoded_message);
                    }
                    // For loop to reset the morse_input array
                    for (uint32_t i = 0; i < sizeof(morse_input) / sizeof(char); i++) {
                        morse_input[i] = '\0';
                    }
                    morse_index = 0;
            } else { // Check if the time between button presses is greater than the threshold for the space between words
                len = (uint8_t)strlen(decoded_message);
                if (len > 0 && decoded_message[len - 1] != ' ' && len < sizeof(decoded_message) - 1) {
                    decoded_message[len] = ' ';
                }
            }
            no_press_time = 0;
        }

        // S3 button press resets the decoded message
        if (S3_press) {
            S3_press = 0;
            LCD1602_ClearAll();
            LCD1602_SetCursor(0, 0);
            LCD1602_Print("Morse Decoder");
            
            // For loop to reset the decoded_message array
            for (int i = 0; i < sizeof(decoded_message) / sizeof(char); i++) {
                decoded_message[i] = '\0';
            }

            // For loop to reset the morse_input array
            for (int i = 0; i < sizeof(morse_input) / sizeof(char); i++) {
                morse_input[i] = '\0';
            }
            morse_index = 0;
        }
    }
}
