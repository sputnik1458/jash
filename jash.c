/* Jash - Just Another SHell
* Created by: Jacob Frank */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define DELIM " \n\t\r\a"
#define MAXSIZE 256
#define BUFSIZE 64

int main(){
    
    int jash();

    int x = jash();

    return EXIT_SUCCESS;
}

int jash(){

    char* get_line();
    char** split_line(); 
    int execute();

    char* line; 
    char** tok; 
    int status = 1;
    char cwd[1024];
    char *user;
    struct passwd *p;
    char hostname[BUFSIZE];

    p = getpwuid(getuid());
    user = p->pw_name;

    gethostname(hostname, sizeof(hostname));

    while (status != 0){
        printf(ANSI_COLOR_GREEN "%s@%s " ANSI_COLOR_BLUE "%s $ " ANSI_COLOR_RESET, user, hostname, getcwd(cwd, sizeof(cwd))); // prints current directory
        line = get_line(); // gets input
        tok = split_line(line); // tokenizes input
        status = execute(tok); // executes command with args
    }
    return 0;
}

char* get_line(){

    char *str = malloc(MAXSIZE); // allocates memory for dynamic string
    if (str == NULL){
        printf("No memory\n");
    }

    fgets(str, MAXSIZE, stdin);

    return str;
    


}

char** split_line(char* line){
    
    char *tok = strtok(line, DELIM);
    int bufsize = BUFSIZE;
    char **tokarray = malloc(bufsize); // allocates memory for dynamic array
    int pos = 0;

    while (tok != NULL){
        tokarray[pos] = tok; // adds token to token array
        pos++;

        if (pos >= bufsize){
            bufsize += BUFSIZE; // adds memory to buffer
            tokarray = realloc(tokarray, bufsize); // reallocates memory for array
        }
        tok = strtok(NULL, DELIM);
    }

    return tokarray;
}

int execute(char** args){

    int builtin_execute();
    int status;
    const char* builtin[4] = {"cd", "ls", "help", "exit"}; // array of built in commands

    for (int i = 0; i <= 3; i++){
        if (strcmp(args[0], builtin[i]) == 0){ // checks to see if the first string in args is a built in command
            status = builtin_execute(args); 
        }
    }
    return status;
    
}

int builtin_execute(char** args){
    
    int cd();
    int ls();
    int status;

    if (strcmp(args[0], "cd") == 0){
        status = cd(args);
    } else if (strcmp(args[0], "exit") == 0){
        return 0;
    } else if (strcmp(args[0], "ls") == 0){
        status = ls(args);
    } else {
        return 1;
    }
}

int cd(char** args){
    chdir(args[1]); // changes directory 

    return 1;
}

int ls(char** args){
    
    struct dirent **obj_array;
    char* obj;
    int i;

    i = scandir(".", &obj_array, NULL, alphasort);

    while (i--){
        obj = obj_array[i]->d_name;
        if (obj[0] != '.'){
            printf("%s\n", obj);
        }
    }
    return 1;
}
