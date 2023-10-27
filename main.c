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
                printf("STRUCT FIELDS: {\nok = %d,\ndata =\n", command_data.ok);
                for (int i = 0; i < command_data.line_count; i++) {
                    for (int j = 0; command_data.data[i][j] != '\0'; j++) {
                        printf("%c", command_data.data[i][j]);
                    }
                    if(i != command_data.line_count -1 ){
                        printf("\n");
                    } else {
                        printf(",\n");
                    }
                }
                printf("line_count = %d\n}\n", command_data.line_count);
            } else if (ret_val == STATE_MACHINE_READY_WITH_ERROR) {
                break;
            }
        }
    }

    fclose(f);

    return 0;
}