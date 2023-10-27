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
            if (current_character == 0x0A) { 
                state = 2;
            } else { 
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
            } else {
                return STATE_MACHINE_READY_WITH_ERROR; 
            } 
            break;
        } 

        case 3:
        {
            if(current_character == 'K') {
                state = 4;
            } else {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;            
        }

        case 4:
        {
            if(current_character == 0x0D) {
                state = 5;
            } else {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 5:
        {
            if(current_character == 0x0A) {
                state = 6;
            } else {
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
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 8:
        {
            if(current_character == 'R') {
                state = 9;
            } else {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 9:
        {
            if(current_character == 'O') {
                state = 10;
            } else {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 10:
        {
            if(current_character == 'R') {
                state = 11;
            } else {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 11:
        {
            if(current_character == 0x0D) {
                state = 12;
            } else {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 12:
        {
            if(current_character == 0x0A) {
                state = 13;
            } else {
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
            if(current_character >= 32 && current_character <= 126) {
                if(line_character_count < AT_COMMAND_MAX_LINE_SIZE && command_data.line_count < AT_COMMAND_MAX_LINES) {
                    command_data.data[command_data.line_count][line_character_count++] = current_character;
                }
                state = 15;
            } else {
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
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 16:
        {
            if(current_character == 0x0A) {
                state = 17;
            } else {
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
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;  
        }

        case 18:
        {
          if(current_character == 0x0A) {
                state = 19;
            } else {
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
                return STATE_MACHINE_READY_WITH_ERROR;
            } 
            break;  
        }

        default:
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
    }
    return STATE_MACHINE_NOT_READY; 
} 