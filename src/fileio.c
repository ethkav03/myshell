#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include "commands.h"
#include "input.h"

void file_output(char ** args, char * filename, char * symbol) {
    FILE *fp;

    if(!strcmp(symbol, ">")) {
        fp = freopen(filename, "w", stdout);    // opens file to write and redirects stdout to that file
    } else {
        fp = freopen(filename, "a", stdout);    // opens file to append and redirects stdout to that file
    }
    if (!fp) {
        printf("Output file does not exist.\n");
        exit(0);
    }
    commands(args);

    fclose(fp);

    freopen ("/dev/tty", "a", stdout);
    // redirect stdout back to stdout
    // https://stackoverflow.com/questions/1908687/how-to-redirect-the-output-back-to-the-screen-after-freopenout-txt-a-stdo
    return;
}

void file_input(char ** args, char * infilename, bool output, char * outfilename, char * symbol) {
    FILE *fpin;

    fpin = freopen(infilename, "r", stdin); // opens file to read and redirects file contents to stdin
    if (!fpin) {
        printf("Input file does not exist.\n");
        exit(0);
    } else {
        if (output) {
            file_output(args, outfilename, symbol);
        }

        commands(args);

        freopen ("/dev/tty", "r", stdin);       // redirects stdin back to stdin
    }

    return;
}

void check_input_output(char *line) {
    int stop;
    char **args2;
    int in = 0;
    int out = 0;
    char *outputfile = NULL;
    char *inputfile = NULL;
    char* symbol = NULL;
    bool output = false;
    bool input = false;

    if (strchr(line, '>') != NULL || strstr(line, ">>") != NULL) {      // checks if we need to truncate or append
        output = true;
        if (strstr(line, ">>") != NULL) {
            symbol = ">>";
        } else {
            symbol = ">";
        }
    }
    if (strchr(line, '<') != NULL) { input = true; }
    char **args = split_input(line);   //split input into arguments

    if (input) {
        for (int i = 0; args[i]; ++i) {     // finds the input file
            if (!strcmp(args[i], "<")) {
                in = i;
                inputfile = args[i + 1];
                break;
            }
        }
    }
    if (output) {
        for (int i = 0; args[i]; ++i) {
            if (!strcmp(args[i], ">") || !strcmp(args[i], ">>")) {  // finds the output file
                out = i;
                outputfile = args[i + 1];
                break;
            }
        }
    }

    if (input || output) {
        if (in < out && in != 0) {
            stop = in;
        } else if (in < out) {
            stop = out;
        } else {
            stop = in;
        }
        args2 = calloc(stop + 1, sizeof(char*));
        args2[stop] = NULL;
        for (int i = 0; i < stop; ++i) {       // removes '< > >>' and filenames from command arguments
            args2[i] = args[i];
        }
    }
    

    if (input) {    // checks if command takes file input
        if (output) {   // checks if commands provides file output as well
            file_input(args2, inputfile, output, outputfile, symbol);
            output = false;
        } else {
            file_input(args2, inputfile, output, NULL, NULL);
        }
    }
    if (output) { // checks if commands provides file output
        file_output(args2, outputfile, symbol);
        free(args2);
    }
    if (!input && !output) {
        commands(args);                 // run specified command with no file input or file output
    }
    free(args);
}