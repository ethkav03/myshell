#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "execute.h"
#include "fileio.h"
#include "input.h"

#define MAX_LEN 128

int main(int argc, char* argv[]) {
    char* pwd;
    char *line;

    char pathShell[MAX_LEN];
    getcwd(pathShell, sizeof(pathShell));
    setenv("SHELL_ROOT", pathShell, 1);
    setenv("MAN_README", strcat(pathShell, "/../manual/readme"), 1);

    signal(SIGINT, signal_handler);
    if (argc > 1) {
        read_file(argv[1]); // take input from file
    } else {
        while (true) {
            pwd = getenv("PWD");
            fprintf(stdout, "~%s$ ", pwd);  // display current directory
            line = read_input();     // take input from command line
            if (*line == '\n') { continue; }
            check_input_output(line);
        }
    }
    free(pwd);
    free(line);
    return 0;
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