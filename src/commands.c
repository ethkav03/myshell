#include "commands.h"
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "execute.h"

#define MAX_LEN 256

// function calls
void commands(char ** args) {
    if (!strcmp(args[0], "cd")) {
        change_directory(args);
    } else if (!strcmp(args[0], "clr")) {
        clear_screen();
    } else if (!strcmp(args[0], "dir")) {
        directory_list(args);
    } else if (!strcmp(args[0], "echo")) {
        echo(args);
    } else if (!strcmp(args[0], "environ")) {
        environment_variables();
    } else if (!strcmp(args[0], "help")) {
        help(args);
    } else if (!strcmp(args[0], "pause")) {
        pause_shell();
    } else if (!strcmp(args[0], "quit")) {
        quit_shell(args);
    } else {
        execute(args);
    }
    return;
}

void change_directory(char ** args) {
    if (!args[1]) {
		fprintf(stdout, "%s\n", getenv("PWD")); //changes to specified directpry
	}
	else if (chdir(args[1]) != 0) {
		perror("directory does not exist"); // error if directory does not exist
	}
	else {
		char c[MAX_LEN];    // displays current directory if none is provided
		getcwd(c, sizeof(c));
		setenv("PWD", c, 1);
	}
    return;
}
/*
    chdir() in C language with Examples
    https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/

    How to get the current directory in a C program?
    https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
*/

void clear_screen() {
    system("clear");    // clears screen
    return;
}

void directory_list(char **args) {
    DIR *directory;
	struct dirent *dir;

	directory = (args[1] == NULL) ? opendir(".") : opendir(args[1]);    // if no directory provided list current directory
                                                                        // otherwise prints provided directory
	if (directory) {
        int i = 0;
		while ((dir = readdir(directory)) != NULL)  // read contents of directory and print them
			printf("%s\n", dir->d_name);
            i++;
		closedir(directory);
	} else {
        perror("directory does not exist");
    }
    return;
}
/*
    dirent.h man page
    https://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html
*/

void environment_variables() {
    extern char **environ;  // stores environment variables

    for (char **env = environ; *env; ++env) {   // prints environment variables
        fprintf(stdout, "%s\n", *env);
    }
    return;
}
/*
    List environment variables with C in UNIX
    https://stackoverflow.com/questions/3473692/list-environment-variables-with-c-in-unix
*/

void echo(char ** args) {
    if (!args[1]) {
        fprintf(stdout, "Invalid Usage: echo <message>\n"); // show use of echo if no message provided
    } else {
        int i = 1;
        while (args[i]) {
            fprintf(stdout, "%s ", args[i]);    // prints full message
            ++i;
        }
        fprintf(stdout, "\n");
    }
    return;
}

void help(char ** args) {
    FILE *f;
    char c;
    char *man = getenv("MAN_README");
    printf("%s\n", man);
    
    f = fopen(man, "r");
    if (f == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    c = fgetc(f);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(f);
    }
    fclose(f);
    return;
}

void pause_shell() {
    fprintf(stdout, "Press enter key to continue...");  // pauses shell and wait for enter key press ('\n' entry) 
    char c = getchar();
    while (c != '\n') { c = getchar(); }
    return;
}

void quit_shell(char ** args) {
    fprintf(stdout, "Exiting shell...\n");  // quit shell
    free(args);
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