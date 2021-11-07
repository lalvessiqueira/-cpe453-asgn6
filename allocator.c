#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Process{
   unsigned long startAddress;
   unsigned long addressSize;
   char processID[3];
}Process;

void request(){
   printf("Request\n");
}
void release(){
   printf("Release\n");
}
void compact(){
   printf("Compact\n");
}
void statistics(){
   printf("Statistics\n");  
}
int main(int argc, char ** argv){
   char * temp, * token;
   const char delim[3] = " \n";
   char *command_line[25];
   if(argc != 2){
      fprintf(stderr, "Usage: ./allocator <number of bytes allocated>\n");
      exit(EXIT_FAILURE);
   }

   unsigned long limit = strtol(argv[1], &temp, 0);
   if(*temp != '\0'){
      fprintf(stderr, "Type the number of bytes to allocate\n");
      exit(EXIT_FAILURE);
   }
   char command[50];
   while(1){
     printf("allocator> ");
     fgets(command, 50, stdin);
     token = strtok(command, delim);
     int i = 0, j;
     while(token != NULL){
        printf("%s\n",  token);
        command_line[i] = (char *) malloc(strlen(token)+1);
        command_line[i] = token;
        token = strtok(NULL, delim);
        i++;
     }
     if(!strcmp(command_line[0], "RQ"))
        request();
     else if(!strcmp(command_line[0], "RL"))
        release();
     else if(!strcmp(command_line[0], "C"))
        compact();
     else if (!strcmp(command_line[0], "STAT"))
        statistics();
     else
        fprintf(stderr, "Not a valid command\n");
     memset(command, 0, 50);
     for(j=0; j<i; j++)
        command_line[j] = NULL;  
   } 
   return 0;
}
