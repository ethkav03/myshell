#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 16

int execute(char **inp) // This function takes an array of strings as input and executes the command
{
    pid_t pid;
    pid_t wpid;
    int status;
    bool bg = false; // variable to track if the process should be run in the background
    int i = 0;
    // check if the command should be run in the background by checking if it has '&' in the argument list
    while (*(inp + i)) {
        if (!strcmp(*(inp + i), "&")) {
            bg = true;
            break;
        }
        i++;
    }

    pid = fork();
    if (pid == 0) { // child process
        if (execvp(inp[0], inp) == -1) { // execute the command using execvp system call
            perror("Child process could not be created.\n"); // if there is an error, print error message
        }
        exit(EXIT_FAILURE); // exit child process with failure status if the command is not executed
    } else if (pid < 0) {
        perror("Error forking the process.\n"); // if there is an error forking the process, print error message
    } else { // parent process
        // if the command is to be run in the background, print message and return
        if (inp[1] != NULL && bg) {
            printf("Process running in background.\n");
            return 1;
        } else {
            // wait for child process to complete
            do {
                wpid = waitpid(pid, &status, WUNTRACED); // wait for child process to complete or be stopped
            } while (!WIFEXITED(status) && !WIFSIGNALED(status)); // continue waiting while the child process is not exited normally or not terminated by a signal
        }
    }
}
/*
    fork() and exec()
    https://ece.uwaterloo.ca/~dwharder/icsrts/Tutorials/fork_exec/
*/

void signal_handler(int signal_number) { // Signal handler function to handle the interrupt signal
    printf("\n");
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