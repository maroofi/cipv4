#include <cipv4.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <util_string.h>

/**
 * The demo accepts one IP address from the command line
 * and prints the IP range for the input IP address
 * by looking into the example.db file.
 * 
 * The same program written in python in pytest_1.py file.
 */




/**
 * reads the file line by line
 * returns a pointer to the line (must be freed by user)
 * or NULL in case of end-of-file
 */
char * readline(FILE * fp){
    const int MAX_LINE_SIZE = 255;
    char * line = (char*) calloc(MAX_LINE_SIZE + 1, sizeof(char));
    int ch = -1;
    char * new_memory = NULL;
    unsigned long int count = 0;
    while ((ch = fgetc(fp)) != EOF){
        line[count++] = ch;
        if (ch == '\n'){
            return line;
        }
        if (count % MAX_LINE_SIZE == 0){
            // we need to reallocate
            new_memory = (char*)realloc(line, cstr_len(line) + MAX_LINE_SIZE);
            if (new_memory == NULL){
                printf("ERROR: Can not reallocate memory for reading line...");
                free(line);
                return NULL;
            }
            line = new_memory;
        }
    }
    if (count == 0){
        // we are at the end of file
        free(line);
        return NULL;
    }
    line[count] = '\0';
    return line;
}

// main driver
int main(int argc, char ** argv){
    if (argc != 2){
        fprintf(stdout, "Usage %s <ip-address>\n", argv[0]);
        return 1;
    }
    FILE * f = fopen("example.db", "r");
    if (!f){
        fprintf(stderr, "Can not open the input file....\n");
        return 1;
    }
    char * line = NULL;
    cipv4_ctx * ctx = NULL;
    char buffer[50];
    while ((line = readline(f)) != NULL){
        if (cstr_rtrim(line, buffer) == NULL){
            fprintf(stdout, "Error. in trimming buffer\n");
            continue;
        }
        free(line);
        ctx = cipv4_parse_ip(buffer);
        if (!ctx || ctx->error != 0){
            fprintf(stderr, "%s\n", ctx->err_msg);
            continue;
        }
        if (cipv4_is_address_in(ctx, argv[1]) == 1){
            fprintf(stdout, "%s\n", ctx->raw);
        }
        cipv4_free(ctx);
    }
    fclose(f);
    return 0;
}
