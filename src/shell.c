#include "commands/dispatcher.c"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOK_DELIMITADOR " \t\r\n\a"
static int MAX_ARGS_TOKENS_PER_LINE = 100;

/**
 * Lê uma linha inteira do stdin e salva ela em line
 * Também faz o parse dos argumentos da linha e salvando em args
 * 
 * @return o tamanho do vetor de argumentos
 */
int readLine(char** parsedArgs);

int main(int argc, char const *argv[])
{
    char* userName = getenv("USER");

    // tentativa de copiar a inicialização do Ubuntu 22.04 kkkkkkkk
    printf("Welcome to Irius 1.0\n \n");
    printf("  * Documentation: https://github.com/yandv/irius-shell\n");
    printf("  * Management: https://github.com/yandv/irius-shell\n");
    printf("  * Support: https://github.com/yandv/irius-shell\n \n");
    printf("Welcome back %s, use the command 'help' to receive information of the available comamnds!\n", userName);

    char *currentCommand;
    char* parsedArgs[MAX_ARGS_TOKENS_PER_LINE];
    int argsCount = -1;

	char** args;

    do {
        printf("irius@%s# ", userName);
        argsCount = readLine(parsedArgs);

        if (parsedArgs[0] == NULL) continue;

        strcpy(currentCommand, parsedArgs[0]);
        dispatchCommand(currentCommand, parsedArgs, argsCount);
    } while(1);

    return 0;
}


// ADICIONAR PARTICIONADOR DE LINHA PARA CONSEGUIR DIGITAR MAIS DE UM COMANDO POR LINHA

int readLine(char** parsedArgs) {
    ssize_t tam = 0;

    // https://c-for-dummies.com/blog/?p=1112
    if (getline(parsedArgs, &tam, stdin) == -1) {
        perror("readline");
        exit(0);
    }

    char* string = parsedArgs[0];
    int index = 0;

    // particiona a string em tokens
    // https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    char* token = strtok(string, TOK_DELIMITADOR);

    do {
        parsedArgs[index++] = token;
        token = strtok(NULL, TOK_DELIMITADOR);
    } while (token != NULL);

    // ao chamar o execvp, o ultimo argumento deve ser NULL
    parsedArgs[index] = NULL;

    return index;
}