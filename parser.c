#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"

AT_COMMAND_DATA command_data;

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_character) 
{ 
    static uint32_t state = 0;
    static uint32_t line_character_count = 0;

    // printf("\n---STATE: %d w character %c\n", state, current_character);
    switch (state) { 
        case 0: 
        { 
            // initialize command_data struct fields
            command_data.ok = 0;
            memset(command_data.data, 0, sizeof(command_data.data));
            command_data.line_count = 0;

            if (current_character == 0x0D) { 
                state = 1;
            } 
            break; 
        } 

        case 1: 
        { 
            command_data.ok = 0;
            memset(command_data.data, 0, sizeof(command_data.data));
            command_data.line_count = 0;
            if (current_character == 0x0A) { 
                state = 2;
            } else {
                printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s", state, current_character, "<LF>");
                return STATE_MACHINE_READY_WITH_ERROR; 
            } 
            break; 
        } 

        case 2:
        { 
            if (current_character == 'O') { 
                state = 3; 
            } else if (current_character == 'E'){ 
                state = 7;
            } else if (current_character == '+'){ 
                state = 14;
            } else if (current_character!='C' && current_character >= 32 && current_character <= 126){
                if(line_character_count < AT_COMMAND_MAX_LINE_SIZE && command_data.line_count < AT_COMMAND_MAX_LINES) {
                    command_data.data[command_data.line_count][line_character_count++] = current_character;
                }
                state=15;
            } else {
                printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected characters: %c / %c / %c\n", state, current_character, 'O', 'E', '+');
                return STATE_MACHINE_READY_WITH_ERROR; 
            } 
            break;
        } 

        case 3:
        {
            if(current_character == 'K') {
                state = 4;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %c\n", state, current_character, 'K');
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;            
        }

        case 4:
        {
            if(current_character == 0x0D) {
                state = 5;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s\n", state, current_character, "<CR>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 5:
        {
            if(current_character == 0x0A) {
                state = 6;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s\n", state, current_character, "<LF>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 6: 
        {
            command_data.ok = 1;

            if (current_character == 0x0D) { 
                state = 1; 
            } else {
                state = 0;
            }
            return STATE_MACHINE_READY_OK;
        }

        case 7:
        {
            if(current_character == 'R') {
                state = 8;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %c\n", state, current_character,'R');
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 8:
        {
            if(current_character == 'R') {
                state = 9;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %c\n", state, current_character, 'R');
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 9:
        {
            if(current_character == 'O') {
                state = 10;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %c\n", state, current_character, 'O');
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 10:
        {
            if(current_character == 'R') {
                state = 11;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %c\n", state, current_character, 'R');
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 11:
        {
            if(current_character == 0x0D) {
                state = 12;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s\n", state, current_character, "<CR>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 12:
        {
            if(current_character == 0x0A) {
                state = 13;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s\n", state, current_character, "<LF>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 13: 
        {
            command_data.ok = 0;

            if (current_character == 0x0D) { 
                state = 1; 
            } else {
                state = 0;
            }
            return STATE_MACHINE_READY_OK;
        }

        case 14:
        {
            if(current_character=='C') {
                if(line_character_count < AT_COMMAND_MAX_LINE_SIZE && command_data.line_count < AT_COMMAND_MAX_LINES) {
                    command_data.data[command_data.line_count][line_character_count++] = current_character;
                }
                state = 15;
            } else {
                printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected characters: %s - %c\n", state, current_character, "C", 126);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 15:
        {
            if(current_character >= 32 && current_character <= 126) {
                if(line_character_count < AT_COMMAND_MAX_LINE_SIZE && command_data.line_count < AT_COMMAND_MAX_LINES) {
                    command_data.data[command_data.line_count][line_character_count++] = current_character;
                }
                
                state = 15;
            } else if (current_character == 0x0D) {
                state = 16;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s\n", state, current_character, "<CR>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 16:
        { 
             if(current_character == 0x0A) {
                state = 17;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s\n", state, current_character, "<LF>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }

        case 17:
        {
            command_data.data[command_data.line_count][line_character_count] = '\0';
            if(command_data.line_count < AT_COMMAND_MAX_LINES) {
                command_data.line_count ++;
                line_character_count = 0;
            }

            if(current_character == '+') {
                state = 14;
            } else if(current_character == 0x0D) {
                state = 18;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected characters: %c / %s\n", state, current_character, '+', "<CR>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;   
        }

        case 18:
        {
            if(current_character == 0x0A) {
                state = 19;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected character: %s\n", state, current_character, "<LF>");
                return STATE_MACHINE_READY_WITH_ERROR;
            }  
            break;
        }

        case 19:
        {
          if(current_character == 'E') {
                state = 7;
            } else if(current_character == 'O') {
                state = 3;
            } else {
                 printf("STATE MACHINE SYNTAX ERROR INFO:\nstate: %d,\ncurrent_character: %c, expected characters: %c / %c\n", state, current_character, 'O', 'E');
                return STATE_MACHINE_READY_WITH_ERROR;
            } 
            break;  
        }

        default:
        {
             printf("STATE MACHINE SYNTAX ERROR INFO:\nInvalid state; Valid states: 0 - 19; Current state: %d\n", state);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
    }
    return STATE_MACHINE_NOT_READY; 
} 