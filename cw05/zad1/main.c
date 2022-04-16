//
// Created by Damian Tworek on 14/04/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define P_END 0
#define P_BEG 1

typedef struct Command {
    char **args;  // {{"grep" "2}}
    int no_args;  // 2
} Command;

typedef struct Component{
    char *name;
    Command *commands;
    int no_commands;
} Component;


void too_long(char* str){
    printf("No allocated space for: %s", str);
    exit(2);
}



void test_split_to_command();
void test_execute_commands();
void parse_line(char *str, Component* line);
void parse_file(char *filename);
void test_parse_line();


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s", argv[0]);
        return 1;
    }
    parse_file(argv[1]);

//    test_execute_commands();
//    test_split_to_command();
//    test_parse_line();
}

Command split_to_command(char *str) {
    Command cmd;
    cmd.args= calloc(1, sizeof(char*));
    if (cmd.args == NULL) {
        printf("error:     cmd.args= calloc(1, sizeof(char*));");
        exit(6);
    }

    cmd.no_args = 0;

    char *tok;
    char sep[] = " \n\t";  // delimit if encounter whitespace, new line char, tabular char,
    tok = strtok(str, sep);  // tokenize string

    while (tok != NULL) {
        cmd.no_args++;
        cmd.args = realloc(cmd.args, cmd.no_args * sizeof(char *));
        if (cmd.args == NULL) {
            printf("error     cmd.args = realloc(cmd.args, cmd.no_args * sizeof(char *));");
            exit(8);
        }

        cmd.args[cmd.no_args - 1] = calloc(strlen(tok) + 1 , sizeof(char));
        if (cmd.args[cmd.no_args - 1] == NULL) {
            printf("error   cmd.args[cmd.no_args - 1] = calloc(sizeof(tok) , sizeof(char));");
            exit(9);
        }

        if (strlcpy(cmd.args[cmd.no_args - 1], tok, (strlen(tok) + 1) * sizeof(char)) >= (strlen(tok) + 1) * sizeof(char)) {
            too_long(tok);
            exit(2);
        }
        tok = strtok(NULL, sep);  // get next token
    }

    cmd.args = realloc(cmd.args, (cmd.no_args + 1) * sizeof (char *));
    cmd.args[cmd.no_args] = NULL;

    return cmd;
}


void parse_line(char *str, Component* line) {
    line->no_commands = 0;
    char* tok;
    tok = strtok(str, "=");  // tok contains : "skladnik "
    if (tok == NULL) {
        printf("parse_line error");
        return;
    }

    line->name = calloc(strlen(tok), sizeof(char)); // strlen(tok) because I don't want last whitespace
    if (line->name == NULL) {
        printf("error  line->name = calloc(strlen(tok) , sizeof(char));");
        exit(10);
    }

    tok[strlen(tok) - 1] = '\0';

    if (strlcpy(line->name, tok, (strlen(tok) + 1 )*sizeof(char)) >= (strlen(tok) + 1) * sizeof(char)) {
        too_long(tok);
    }

    char **commands_to_parse = calloc(1, sizeof (char*));
    if (commands_to_parse == NULL) {
        printf("error char **commands_to_parse = calloc(1, sizeof (char*));");
        exit(11);
    }

    tok = strtok(NULL, "|");

    int* i = &(line->no_commands);  // i is pointing to line->no_commands
    while (tok != NULL) {
        (*i)++;

        commands_to_parse = realloc(commands_to_parse, sizeof(char *) * *i);
        if (commands_to_parse == NULL) {
            printf("error commands_to_parse = realloc(commands_to_parse, sizeof(char *) * *i);");
            exit(12);
        }

        //for some reason sizeof(tok) didnt count whitespaces on token and only on token
        size_t cmd_2parse_size = (strlen(tok)) * sizeof(char); // strlen  no +1 because im doing tok++ later.

        commands_to_parse[*i - 1] = calloc(strlen(tok) , sizeof(char) );
        if (commands_to_parse[*i - 1] == NULL) {
            printf("error     commands_to_parse[*i - 1] = calloc(strlen(tok) + 1 , sizeof(char) );;");
            exit(13);
        }

        if (tok[strlen(tok) - 1] == ' ')
            tok[strlen(tok) - 1] = '\0';

        tok++;

        if (strlcpy(commands_to_parse[*i - 1], tok, cmd_2parse_size) >= cmd_2parse_size) {
            printf("strlcpy(commands_to_parse[*i - 1], tok, sizeof(tok) * sizeof(char)) >= sizeof(tok) * sizeof(char)");
            exit(14);
        }

        tok = strtok(NULL, "|");
    }

    line->commands = calloc(*i, sizeof (Command));
    if (line->commands == NULL) {
        printf("error line->commands = calloc(*i, sizeof (Command));");
        exit(15);
    }

    for (int j = 0; j < line->no_commands ; j++) {
        line->commands[j] = split_to_command(commands_to_parse[j]);
    }

    for (int j = 0; j < line->no_commands; j++) {
        free(commands_to_parse[j]);
    }

    free(commands_to_parse);
}


void execute_commands(Command **commands, int no_cmds){
    int **fds = calloc(no_cmds, sizeof (int *));

    for(int i = 0; i < no_cmds ; i++) {
        fds[i] = calloc(2, sizeof (int));
        if (fds[i] == NULL) {
            printf("error  fds[i] = calloc(2, sizeof (int))");
            exit(15);
        }
        if (pipe(fds[i]) < 0) {  // creating pipe
            printf("error 185 if (pipe[fds[i]] < 0) {");
            exit(16);
        }
    }

    for(int i = 0; i < no_cmds ; i++) {
        if (fork() == 0) {
            if( i > 0) {  //first doesnt need in
                dup2(fds[i - 1][P_END], STDIN_FILENO); // takes end of prev child
            }
            if(i < no_cmds - 1) { // last doesnt need out
                dup2(fds[i][P_BEG], STDOUT_FILENO);  // takes start of prev child
            }
            for(int j = 0; j < no_cmds; j++) {
                close(fds[j][P_BEG]);  // close coz dup duplicates
                close(fds[j][P_END]);
            };

            puts("\n");
            execvp(commands[i]->args[0], commands[i]->args);
        }
    }

    // close in parent
    for(int j = 0; j < no_cmds; j++) {
        close(fds[j][P_BEG]);
        close(fds[j][P_END]);
    }

    for (int i = 0; i < no_cmds; i++)
        wait(NULL);

    for(int i = 0; i < no_cmds ; i++) {
        free(fds[i]);
    }
    free(fds);
}


void parse_line_to_execute(char *line, Component *components, int no_components) {
    Command **commands_to_run = calloc(1, sizeof (Command *));
    int ix = -1;
    char *exp = strtok(line, "| \n\t");


    while(exp != NULL) {
        for(int i = 0; i < no_components; i++) {
            if(strcmp(exp, components[i].name) == 0) {
                for(int j = 0; j < components[i].no_commands; j++) {
                    ix++;
                    commands_to_run = realloc(commands_to_run, (ix + 1) * sizeof (Command *));
                    commands_to_run[ix] = &components[i].commands[j];
                }
            }
        }
        exp = strtok(NULL, "| \n\t");
    }

    execute_commands(commands_to_run, ix + 1);
    free(commands_to_run);
}


void parse_file(char *filename) {
    Component* components = calloc(1, sizeof(Component));
    int no_components = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("error  FILE *fp = fopen(filename, \"r\");");
        exit(20);
    }

    char *line = calloc(250, sizeof(char));
    size_t len = 0, nread;
    while ((nread = getline(&line, &len, fp)) != -1) {
        if (nread == 0)
            continue;


        char *is_component = strchr(line, '=');
        if (is_component != NULL) {
            no_components++;
            components = realloc(components, no_components * sizeof (Component));
            parse_line(line, &components[no_components - 1]);
        } else {
            parse_line_to_execute(line, components, no_components);
        }
    }

    free(line);
    for(int i = 0; i < no_components; i++) {
        for(int j = 0; j < components[i].no_commands; j++) {
            for(int k = 0; k < components[i].commands[j].no_args; k++)
                free(components[i].commands[j].args[k]);
            free(components[i].commands[j].args);
        }
        free(components[i].commands);
        free(components[i].name);
    }
    free(components);
    fclose(fp);
    return;
}








void test_split_to_command() {
    char* p = calloc(100, sizeof( char ));
    char x[] = "grep 1 2";
    strcpy(p, x);
    Command tmp = split_to_command(p);
    free(p);
}

void test_parse_line(){

    Component* p = malloc(sizeof(Component));
    if (p == NULL) {
        printf("error    Line* p = malloc(sizeof(Line));");
        exit(4);
    }

    char my_str[] = "skladnik = grep 1 2 | calc 2\n";

    char* str = calloc(sizeof(my_str), sizeof(char));
    if (str == NULL) {
        printf("error     char* str = calloc(sizeof(my_str), sizeof(char));");
        exit(5);
    }

    if (strlcpy(str, my_str, sizeof(my_str) * sizeof(char)) >= sizeof(my_str) * sizeof(char)) {
        too_long(my_str);
    }
    parse_line(str, p);

    printf("%s", p->name);
    for (int i = 0; i < p->no_commands; i++) {
        for (int j =0; j < p->commands[i].no_args; j++) {
            printf("%s", p->commands[i].args[j]);
        }
    }
    free(p);
}

void test_execute_commands() {
    char* p = calloc(100, sizeof( char ));
    char x[] = "grep happy";
    strcpy(p, x);
    Command tmp1 = split_to_command(p);

    char* p2 = calloc(100, sizeof( char ));
    char x2[] = "cat t.txt";
    strcpy(p2, x2);
    Command tmp2 = split_to_command(p2);


    Command** ps = calloc(2, sizeof(Command*));

    ps[0] = &tmp2;
    ps[1] = &tmp1;

    execute_commands(ps, 2);
    free(p);
    free(p2);
    free(ps);
}