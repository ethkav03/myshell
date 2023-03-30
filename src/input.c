#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

#define MAX_ARGS 16
#define SEPARATORS " \t\n"

char* read_input() {
    char *input = NULL;
    size_t buflen = 0;
    getline(&input, &buflen, stdin); // takes line input
    return input;
}
/*
    Tutorial - Write a Shell in C
    https://brennan.io/2015/01/16/write-a-shell-in-c/
*/

char** split_input(char *input) {
    int length = 0;
    int capacity = MAX_ARGS;
    char **args = malloc(capacity * sizeof(char*)); // array of arguments

    char *arg = strtok(input, SEPARATORS); // splits input into arguments

    while (arg != NULL) {
        args[length] = arg; // adds argument to array
        length++;   // increases size of arguments array to accomodate more arguments

        if (length >= capacity) {   // checks if number of arguments is less than max arguments and reallocates memory accordingly
            capacity = (int) (capacity * 1.5);
            args = realloc(args, capacity * sizeof(char*));
        }

        arg = strtok(NULL, SEPARATORS);
    }

    args[length] = NULL;
    free(arg);
    return args;
}
/*
    Tutorial - Write a Shell in C
    https://brennan.io/2015/01/16/write-a-shell-in-c/
*/

void read_file(char *filename) {
    FILE *file = NULL;
    file = fopen(filename, "r");    // open file

    char *line = calloc(MAX_ARGS, sizeof(char*));
    while (file && line && fgets(line, MAX_ARGS * sizeof(char*), file)) { // reads each line
        char ** args = split_input(line);                                 // splits them into arguments
        commands(args);                    //runs each command
        free(args);
    }
    free(line);
    exit(0);
}

/*

Name: Ethan Kavanagh

Student Number: 21344873

Acknowledgement of the DCU Academic Integrity Policy:
I understand that the University regards breaches of academic integrity and plagiarism as grave 
and serious. I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept
the penalties that may be imposed should I engage in practice or practices that breach this policy. 
I have identified and included the source of all facts, ideas, opinions and viewpoints of others
in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books,
journal articles, internet sources, module text, or any other source whatsoever are acknowledged
and the sources cited are identified in the assignment references. I declare that this material,
which I now submit for assessment, is entirely my own work and has not been taken from the work of
others save and to the extent that such work has been cited and acknowledged within the text of my
work. I have used the DCU library referencing guidelines and/or the appropriate referencing system
recommended in the assignment guidelines and/or programme documentation.

*/