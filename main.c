#include <stdio.h>
#include "parser.h"

int main (int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "rb");
    int ch;
    STATE_MACHINE_RETURN_VALUE ret_val;

    if(f == NULL) {
        printf("Error on opening file attempt.");
    } else {
        while(!feof(f)) {
            ch = fgetc(f);

            ret_val = at_command_parse(ch);
            if(ret_val == STATE_MACHINE_READY_OK) {
                printf("OK\n");
            } else if (ret_val == STATE_MACHINE_READY_WITH_ERROR) {
                printf("ERROR\n");
                break;
            }
        }
    }

    fclose(f);

    return 0;
}