#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process{
   unsigned long startAddress;
   unsigned long endAddress;
   char processID[3];
   struct Process * next;
}Process;
Process * head=NULL;
unsigned long limit;

void request(char * proc_name, unsigned long proc_size){
   //First fit: Leticia
   //Best fit: Thomas
   Process * cur = (Process *) malloc(sizeof(Process));
   strcpy(cur->processID, proc_name);
   if(!head){
      if(proc_size > limit){
         fprintf(stderr, "No holes available\n");
         return;
      }
      cur->startAddress = 0;
      cur->endAddress = proc_size;
      head = cur;
      head->next = NULL;
   }
   else if(!head->next){
      unsigned long new_end = head->endAddress+ proc_size + 1;
      if(new_end >= limit){
        fprintf(stderr, "No holes available\n");
        return;
     }
     cur->startAddress = head->endAddress+1;
     cur->endAddress = new_end;
     head->next = cur;
   }
   else{
      //Best fit
      unsigned long small_hole = 0;
      Process * temp = head;
      while(temp->next){
         unsigned long hole = temp->next->startAddress- temp->endAddress;
         if(hole >= proc_size){
            if(!small_hole || small_hole > hole)
              small_hole = hole;
         }
         temp = temp->next;
      }
      if(!small_hole){
         unsigned long new_end = temp->endAddress+ proc_size + 1;
         if(new_end >= limit){
            fprintf(stderr, "No holes available\n");
            return;
         }
         else{
              cur->startAddress = temp->endAddress+1;
              cur->endAddress = new_end;
              temp->next = cur;
              return;
         }
      }
      while(temp->next){
          unsigned long hole = temp->next->startAddress - temp->endAddress;
          if(hole == small_hole){
              cur->startAddress = temp->endAddress+1;
              cur->endAddress = temp->next->startAddress-1;
              cur->next = temp->next;
              temp->next = cur;
              return; 
          }
      }
   }
   
}
void release(){
   printf("Release\n");
}
void compact(){
   printf("Compact\n");
}
void statistics(){
   Process * temp = head;
   while(temp){
       
   } 
}
int main(int argc, char ** argv){
   char * temp, * token;
   const char delim[3] = " \n";
   char *command_line[25];
   if(argc != 2){
      fprintf(stderr, "Usage: ./allocator <number of bytes allocated>\n");
      exit(EXIT_FAILURE);
   }

   limit = strtol(argv[1], &temp, 0);
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
     if(!strcmp(command_line[0], "RQ")){
        unsigned long proc_size = strtol(command_line[2], &temp, 0);
        if(*temp != '\0'){
           fprintf(stderr, "Unsigned number for bytes to allocate\n");
        }
        request(command_line[1], proc_size);
     }
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
