#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parser.h"

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_character) 
{ 
    static uint32_t state = 0; 
    switch (state){ 
        case 0: 
        { 
            if (current_character == 0x0D){ 
                state = 1; 
            } 
            break; 
        } 

        case 1: 
        { 
            if (current_character == 0x0A){ 
                state = 2; 
            }else { 
                return STATE_MACHINE_READY_WITH_ERROR; 
            } 
            break; 
        } 

        case 2:
        { 
            if (current_character == 'O'){ 
                state = 3; 
            }else if (current_character == 'E'){ 
                state = 7;
            }else if (current_character == '+'){ 
                state = 14;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR; 
            } 
            break;
        } 

        case 3:
        {
            if(current_character == 'K'){
                state = 4;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;            
        }

        case 4:
        {
            if(current_character == 0x0D){
                state = 5;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 5:
        {
            if(current_character == 0x0A){
                state = 6;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 6:
        {
            return STATE_MACHINE_READY_OK;
        }

        case 7:
        {
            if(current_character == 'R'){
                state = 8;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 8:
        {
            if(current_character == 'R'){
                state = 9;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 9:
        {
            if(current_character == 'O'){
                state = 10;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 10:
        {
            if(current_character == 'R'){
                state = 11;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 11:
        {
            if(current_character == 0x0D){
                state = 12;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 12:
        {
            if(current_character == 0x0A){
                state = 12;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 13:
        {
            return STATE_MACHINE_READY_OK;
        }

        case 14:
        {
            if(current_character >=32 && current_character <= 126){
                state = 15;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 15:
        {
            if(current_character >= 32 && current_character <= 126){
                state = 15;
            }else if(current_character == 0x0D){
                state = 16;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 16:
        {
            if(current_character == 0x0A){
                state = 17;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 17:
        {
            if(current_character == '+'){
                state = 14;
            }else if(current_character == 0x0D){
                state = 18;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        case 18:
        {
          if(current_character == 0x0A){
                state = 19;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            }  
        }

        case 19:
        {
           if(current_character == 'E'){
                state = 7;
            }else if(current_character == 'O'){
                state = 3;
            }else{
                return STATE_MACHINE_READY_WITH_ERROR;
            } 
        }

        default:
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
    } 
    return STATE_MACHINE_NOT_READY; 
} 