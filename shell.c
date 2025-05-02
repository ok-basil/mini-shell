#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

void shell_loop()
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        printf("mini-shell> ");

        // Get input
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) 
        {
            perror("fgets failed");
            continue;
        }

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        // Exit command
        if (strcmp(input, "exit") == 0) 
        {
            break;
        }

        // Tokenize input
        char *args[MAX_ARGS];
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Fork and exec
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            if (execvp(args[0], args) == -1)
            {
                perror("Command failed");
            }
            exit(EXIT_FAILURE);
        } else if (pid > 0)
        {
            // Parent waits
            wait(NULL);
        } else
        {
            perror("Fork failed");
        }
    }
}