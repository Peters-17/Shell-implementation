// Copyright haoyangy your_loginName
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "linkedList.c"

#define MAXSIZE 512
/**
 * create a struct Cmd to store values to conveniently load arguments
 */
typedef struct Cmd {
    int redir;
    int redirErr;
    char* file_name;
    char** commands;
    char newCmd[MAXSIZE];
} Cmd;

void trim(char*);
char** parsing(char*, int*, int*, char**, int*);
void newFree(Cmd);
int checkFileNum(char*, const char*);
char** splitCmd(char*, const char*, int);
int countRedir(char* line);
void execCmd(Cmd, list*, int, FILE*);
void errorBehavior(char*, list*, int, Cmd, FILE*);

int main(int argc, char* argv[]) {
    FILE* fp = NULL;
    FILE* inputType = NULL;
    int flag; // 0 if Interactive and 1 if Batch
    // Interactive
    if (argc == 1) {
        flag = 0;
        inputType = stdin;
        write(1, "mysh> ", strlen("mysh> "));
        // Batch
    } else if (argc == 2) {
        fp = fopen(argv[1], "r");
        // cannot open file
        if (fp == NULL) {
            char error[MAXSIZE];
            snprintf(error, sizeof(error),
                     "Error: Cannot open file %s.\n", argv[1]);
            write(STDERR_FILENO, error, strlen(error));
            exit(1);
        }
        inputType = fp;
        flag = 1;
    } else {
        write(STDERR_FILENO, "Usage: mysh [batch-file]\n", strlen("Usage: mysh [batch-file]\n"));
        exit(1);
    }
    char line[MAXSIZE];
    list* myList = calloc(1, sizeof(list));
    listInit(myList);
    while (fgets(line, MAXSIZE, inputType) != NULL) {
        // echo batch command
        if (flag == 1) {
            write(1, line, strlen(line));
        }
        struct Cmd cmd = {.redir = 0, .redirErr = 0, .file_name = NULL, .commands = NULL, .newCmd= ""};
        int emptyFlag = 0;
        // check list contain this command or not
        if (listGet(myList, line) != NULL) {
            char* new_line;
            new_line = listGet(myList, line);
            strncpy(line, new_line, strlen(new_line)+1);
        }
        cmd.commands = parsing(line, &cmd.redir,
                               &cmd.redirErr, &cmd.file_name, &emptyFlag);
        if (emptyFlag == 1) {
            if (flag == 0) {
                write(1, "mysh> ", strlen("mysh> "));
            }
            newFree(cmd);
            continue;
        }

        if (!cmd.redirErr && strcmp(cmd.commands[0], "exit") == 0) {
            newFree(cmd);
            freeList(myList);
            if (flag == 1) {
                fclose(inputType);
            }
            exit(0);
        }

        // if alias
        if (!cmd.redirErr && ((strcmp(cmd.commands[0], "alias") == 0) ||
                              ((strcmp(cmd.commands[0], "unalias") == 0)))) {
            int commandLen = 0;
            while (cmd.commands[commandLen] != NULL) {
                commandLen++;
            }
            // just alias-- Print all
            if ((strcmp(cmd.commands[0], "alias") == 0)
                && (commandLen == 1)) {
                listPrintAll(myList);
                if (flag == 0) {
                    write(1, "mysh> ", strlen("mysh> "));
                }
                newFree(cmd);
                continue;
            }
            // dangerous case
            if ((strcmp(cmd.commands[1], "alias") == 0)
                || (strcmp(cmd.commands[1], "unalias") == 0)) {
                write(1, "alias: Too dangerous to alias that.\n",
                      strlen("alias: Too dangerous to alias that.\n"));
                if (flag == 0) {
                    write(1, "mysh> ", strlen("mysh> "));
                }
                newFree(cmd);
                continue;
            }
            // alias follow by a word -- Look up
            if (((strcmp(cmd.commands[0], "alias")) == 0) && (commandLen == 2)) {
                listMatch(myList, cmd.commands[1]);
                if (flag == 0) {
                    write(1, "mysh> ", strlen("mysh> "));
                }
                newFree(cmd);
                continue;
            }
            // unalias follow by a word to remove
            if (((strcmp(cmd.commands[0], "unalias")) == 0)) {
                if (commandLen == 2) {
                    listRemove(myList, cmd.commands[1]);
                    if (flag == 0) {
                        write(1, "mysh> ", strlen("mysh> "));
                    }
                    newFree(cmd);
                    continue;
                } else {
                    write(1, "unalias: Incorrect number of arguments.\n",
                          strlen("unalias: Incorrect number of arguments.\n"));
                    if (flag == 0) {
                        write(1, "mysh> ", strlen("mysh> "));
                    }
                    newFree(cmd);
                    continue;
                }
            }
            // add new alias to the list
            if (((strcmp(cmd.commands[0], "alias")) == 0) && (commandLen >= 3)) {
                strncpy(cmd.newCmd, cmd.commands[2], strlen(cmd.commands[2]) + 1);
                for (int i = 3; cmd.commands[i] != NULL; i++) {
                    strncat(cmd.newCmd, " ", 2);
                    strncat(cmd.newCmd, cmd.commands[i],
                            strlen(cmd.commands[i])+1);
                }
                if (listGet(myList, cmd.commands[1]) != NULL) {
                    listRemove(myList, cmd.commands[1]);
                }
                listInsert(myList, cmd.commands[1], cmd.newCmd);
                if (flag == 0) {
                    write(1, "mysh> ", strlen("mysh> "));
                }
                newFree(cmd);
                continue;
            }
        }
        execCmd(cmd, myList, flag, inputType);
        if(!flag) {
            write(1, "mysh> ", strlen("mysh> "));
        }
        newFree(cmd);
    }
    freeList(myList);
    fclose(inputType);
    return 0;
}

void trim(char* str) {
    int i, j, len;
    len = (int)strlen(str);
    for (i = 0; i < len; i++) {
        if (str[0] == ' ' || str[0] == '\t') {
            for (i = 0; i < (len - 1); i++)
                str[i] = str[i + 1];
            str[i] = '\0';
            len--;
            i = -1;
            continue;
        }
        if ((str[i] == ' ' || str[i] == '\t')
            && (str[i + 1] == ' ' || str[i + 1] == '\t')) {
            for (j = i; j < (len - 1); j++) {
                str[j] = str[j + 1];
            }
            str[j] = '\0';
            len--;
            i--;
        }
    }
}

int countRedir(char* line) {
    int count = 0;
    char* tempLine = line;
    int len = (int)strlen(line);
    for (int i = 0; i < len; i++) {
        if (*tempLine == '>') {
            count++;
        }
        tempLine++;
    }
    return count;
}

char** splitCmd(char* line, const char* delim, int size) {
    char** wholeLines;
    char* eachLine;
    int position = 0;
    wholeLines = calloc(size, sizeof(char*));
    eachLine = strtok(line, delim);
    while (eachLine != NULL) {
        wholeLines[position] = eachLine;
        position++;
        eachLine = strtok(NULL, delim);
        if (position >= size) {
            size++;
            wholeLines = realloc(wholeLines, size* sizeof(char*));
        }
    }
    wholeLines[position] = NULL;
    return wholeLines;
}

int checkFileNum(char* file, const char* delim) {
    char* tempFile = strdup(file);
    char* partLine;
    int count = 0;
    partLine = strtok(tempFile, delim);
    while (partLine != NULL) {
        count++;
        partLine = strtok(NULL, delim);
    }
    if (count > 1) {
        free(tempFile);
        return 1;
    }
    free(tempFile);
    return 0;
}

char** parsing(char* line, int* redir,
               int* redirErr, char** file_name, int* emptyFlag) {
    char** wholeLines;
    char** commands;
    trim(line);
    line = strtok(line, "\n");
    if (line == NULL) {
        *emptyFlag = 1;
        return NULL;
    }
    int redirec_number = countRedir(line);
    if (redirec_number > 1) {
        *redir = 1;
        *redirErr = 1;
        return NULL;
    } else if (redirec_number == 0) {
        *redir = 0;
        *redirErr = 0;
        commands = splitCmd(line, " ", 1);
        return commands;
    } else {
        if (strncasecmp(line, ">", 1) == 0) {
            *redir = 1;
            *redirErr = 1;
            return NULL;
        }
        wholeLines = splitCmd(line, ">", 1);
        if (wholeLines[1] == NULL || checkFileNum(wholeLines[1], " ")) {
            *redir = 1;
            *redirErr = 1;
            free(wholeLines);
            return NULL;
        }
        *redir = 1;
        *redirErr = 0;
        commands = splitCmd(wholeLines[0], " ", 1);
        wholeLines[1] = strtok(wholeLines[1], " ");
        *file_name = strdup(wholeLines[1]);
        free(wholeLines);
        return commands;
    }
}

void newFree(Cmd cmd) {
    if (cmd.commands != NULL) {
        free(cmd.commands);
    }
    if (cmd.file_name != NULL) {
        free(cmd.file_name);
    }
}
void execCmd(Cmd cmd, list* myList, int flag, FILE* inputType) {
    int pid = 0;
    int states = 0;
    pid = fork();
    if(pid){
        waitpid(pid, &states, WUNTRACED);
    } else {
        if(cmd.redirErr == 0 && cmd.redir) {
            int outStream;
            outStream = open(cmd.file_name, O_WRONLY | O_TRUNC | O_CREAT,
			    S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);
            if(outStream < 0) {
                char tempStr[MAXSIZE];
                snprintf(tempStr, sizeof(tempStr),
                         "Cannot write to file %s.\n", cmd.file_name);
                errorBehavior(tempStr, myList, flag, cmd, inputType);
            }
            if(dup2(outStream, STDOUT_FILENO) == -1){
                exit(1);
            }
            close(outStream);
        }
        else if(cmd.redirErr == 1 && cmd.redir) {
            char tempStr2[MAXSIZE];
            snprintf(tempStr2, sizeof(tempStr2),
                     "Redirection misformatted.\n");
            errorBehavior(tempStr2, myList, flag, cmd, inputType);
        }
        execv(cmd.commands[0], cmd.commands);
        char tempStr3[MAXSIZE];
        snprintf(tempStr3, sizeof(tempStr3),
                 "%s: Command not found.\n", cmd.commands[0]);
        errorBehavior(tempStr3, myList, flag, cmd, inputType);
    }
}

void errorBehavior(char* str, list* myList, int flag, Cmd cmd, FILE* inputType) {
    write(STDERR_FILENO, str,strlen(str));
    newFree(cmd);
    freeList(myList);
    if (flag == 1) {
        fclose(inputType);
    }
    _exit(1);
}
