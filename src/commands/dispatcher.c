#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* name;
    char* description;
    char* usage;
} Command;

static Command commands[] = {
    {"help",
    "Command to help you to know all the things Irius-Shell can do for you.",
    "Use help <command name> to know more about a specific command."},

    {"exit",
    "Command to exit Irius-Shell.",
    "Use exit to exit Irius-Shell."}
};

static int COMMAND_COUNT = sizeof(commands) / sizeof(Command);

/** 
* Dispara o comando para a função que irá executá-lo
*/
void dispatchCommand(char* command, char** args, int argCount);

int getCommandIndex(char* command);

/**
 * Executa o comando help
*/
void handleHelpCommand(char** args, int argCount) {
    if (argCount == 1) {
        printf("Available commands:\n");

        for (int i = 0; i < COMMAND_COUNT; i++) {
            printf("  * %s: %s\n", commands[i].name, commands[i].usage);
        }

        return;
    }

    int index = getCommandIndex(args[1]);

    if (index == -1) {
        printf("I cannot found the command '%s'.\n",
                                                    args[1]);
        return;
    }

    printf("Command '%s': %s\n \n%s\n \n", commands[index].name, commands[index].description, commands[index].usage);
}

/**
 * Executa o comando exit
*/
void handleExitCommand(char** args, int argCount) {
    printf("Bye bye!\n");
    exit(0);
}

void handleForkCommand(char* command, char** args, int argCount) {
    pid_t pid, wpid;
    int status;
    //char* argv[]={"test", "test", "test", (char*)0};
    pid = fork();

    if (pid < 0) {
        printf("Could not fork process.\n");
        return;
    }

    if (pid == 0) { // quando é o processo filho
        if (execvp(command, args) == -1) {
            printf("Command '%s' not found.\n", command);
        }
        exit(EXIT_FAILURE);
        return;
    }

    // quando é o processo pai
    
    // espera o processo filho terminar para continuar a execução, além disso
    // sem isso fica dando um problema que ao fazer a fork o console buga a linha

    do {
        wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));   
}

int getCommandIndex(char* command) {
    int i;

    for (i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(command, commands[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

void dispatchCommand(char* command, char** args, int argCount) {
    int index = getCommandIndex(command);

    switch (index) {
    case 0:
        handleHelpCommand(args, argCount);
        break;
    case 1: 
        handleExitCommand(args, argCount);
        break;
    default:
        handleForkCommand(command, args, argCount);
	    break;
    }
}