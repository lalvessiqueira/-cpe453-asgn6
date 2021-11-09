#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process{
   unsigned long startAddress;
   unsigned long endAddress;
   char processID[3];
   struct Process * next;
}Process;

Process * head = NULL;
unsigned long limit = 0;

/*
 * First fit: Allocate the first hole that is big enough. 
 * Searching can start either at the beginning of the set 
 * of holes or at the location where the previous 
 * first-fit search ended. We can stop searching as soon 
 * as we find a free hole that is large enough.
 * 
 * Best fit: Allocate the smallest hole that is big enough. 
 * Search the entire list, unless the list is ordered by size. 
 * This strategy produces the smallest leftover hole.
 * 
 * Worst fit: Allocate the largest hole. 
 * Again, search the entire list, unless it is sorted by size. 
 * This strategy produces the largest leftover hole, which 
 * may be more useful than the smaller leftover hole from 
 * a best-fit approach.
 */
int processExist(char *processID) {
   Process *temp = head;
   while (temp != NULL){
      if (strcmp(temp->processID, processID) == 0){
         return 1;
      }
   }
   return 0;
}

void firstFit(char *processID, unsigned long memSpace, Process * cur){
   Process *temp;
   Process *curr = head;
   // unsigned long left_over_space = 0;
   Process *newProcess = cur;
   strcpy(newProcess->processID, processID);

   //delete this cause its already in the request function
   if (head == NULL) {
      //then add it as the first node
      newProcess->startAddress = 0;
      if (memSpace > limit) {
         fprintf(stderr, "Not enough space!");
         return;
      }
      newProcess->endAddress = memSpace - 1;
      head = newProcess;
      newProcess->next = NULL;
   }
   else {
      // if (processExist){
      //    while (curr != NULL) {
      //       if (strcmp(temp->processID, processID) == 0){
      //          newProcess->endAddress = curr->endAddress + memSpace;
      //          // if (newProcess) 
      //          temp = curr;
      //          temp->next = newProcess;
      //          newProcess->next = curr->next;
      //          return;
      //       }
      //    }
      // }
      // do we need to check if the process exists?
      while (curr->next != NULL) {
         if ((curr->endAddress + 1) - curr->next->startAddress >= memSpace) {
            newProcess->startAddress = curr->startAddress + 1;
            newProcess->endAddress = curr->endAddress + memSpace;
            temp = curr;
            temp->next = newProcess;
            newProcess->next = curr->next;
            return;
         }
         curr = curr->next;
      }
      if (limit - (curr->endAddress + 1) >= memSpace) {
         newProcess->startAddress = curr->endAddress + 1;
         newProcess->endAddress = newProcess->startAddress + memSpace;
         temp = curr;
         temp->next = newProcess;
         newProcess->next = NULL;
         return;
      }
      fprintf(stderr, "Not enough space!");
      return; 
   }
}

void bestFit(char * proc_name, unsigned long proc_size, Process * cur){
    unsigned long small_hole = 0;
      Process * temp = head;
      while(temp->next){
         unsigned long hole = temp->next->startAddress - temp->endAddress;
         if(hole >= proc_size){
            if(!small_hole || small_hole > hole)
              small_hole = hole;
         }
         temp = temp->next;
      }
      if(!small_hole){
         unsigned long new_end = temp->endAddress + proc_size + 1;
         if(new_end >= limit){
            fprintf(stderr, "No memory available\n");
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

void worstFit(char * proc_name, unsigned long proc_size, Process * cur){
   strcpy(cur->processID, proc_name);

   unsigned long biggest_hole = 0;
   Process * temp = head;

   while(temp->next){
      unsigned long hole = temp->next->startAddress- temp->endAddress;
      if(hole >= proc_size) { 
         if(!biggest_hole || biggest_hole <= hole)
            biggest_hole = hole;
      }
      temp = temp->next;
   }
   if(!biggest_hole){
      unsigned long new_end = temp->endAddress + proc_size + 1;
      if(new_end >= limit){
         fprintf(stderr, "No memory available\n");
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
      if(hole == biggest_hole){
         cur->startAddress = temp->endAddress+1;
         cur->endAddress = temp->next->startAddress-1;
         cur->next = temp->next;
         temp->next = cur;
         return; 
      }
   }
}

void request(char * proc_name, unsigned long proc_size, char fit){
   //First fit: Leticia
   //Best fit: Thomas
   Process * cur = (Process *) malloc(sizeof(Process));
   strcpy(cur->processID, proc_name);
   if(!head){
      if(proc_size > limit){
         fprintf(stderr, "No memory available\n");
         return;
      }
      cur->startAddress = 0;
      cur->endAddress = proc_size - 1;
      head = cur;
      head->next = NULL;
   }
   else if(!head->next){
      unsigned long new_end = head->endAddress+ proc_size + 1;
      if(new_end >= limit){
        fprintf(stderr, "No memory available\n");
        return;
     }
     cur->startAddress = head->endAddress+1;
     cur->endAddress = new_end;
     head->next = cur;
   }
   else{
      if(fit == 'F')
         firstFit(proc_name, proc_size, cur);
      else if(fit == 'W')
         worstFit(proc_name, proc_size, cur);
      else if(fit == 'B')
         bestFit(proc_name, proc_size, cur);
      else{
         fprintf(stderr, "Not a valid command\n");
      }
   }
   /*else{
      unsigned long small_hole = 0;
      Process * temp = head;
      while(temp->next){
         unsigned long hole = temp->next->startAddress - temp->endAddress;
         if(hole >= proc_size){
            if(!small_hole || small_hole > hole)
              small_hole = hole;
         }
         temp = temp->next;
      }
      if(!small_hole){
         unsigned long new_end = temp->endAddress + proc_size + 1;
         if(new_end >= limit){
            fprintf(stderr, "No memory available\n");
            return;
         }
         else{
              cur->startAddress = temp->endAddress+1;
              cur->endAddress = new_end;
              temp->next = cur;
              printf("Process added successfully!");
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
              printf("Process added successfully!");
              return; 
          }
      }
   }*/
   
}

void release(char * proc_name){
   Process * temp = head, *cur;
   if(!head){
     printf("No processes to release\n");
   }
   else if(!head->next){
      free(head->processID);
      free(head);
   }
   else{
      if(!strcmp(temp->processID, proc_name)){
          cur = temp;
          head= head->next;
          free(cur->processID);
          free(cur);
          return;
      }
      while(strcmp(temp->next->processID, proc_name)){
          temp=temp->next;
          if(!temp->next){
             break;
          }
      }
      if(!strcmp(temp->next->processID, proc_name)){
          cur = temp->next;
          temp->next = cur->next;
          free(cur->processID);
          free(cur);
      }
      else if(!strcmp(temp->processID, proc_name)){
          free(temp->processID);
          free(temp);
      }
      else{
          fprintf(stderr, "Process Not Found\n");
      }
   }
}

/**
 * If the user enters the C command, your program will compact the set of holes into one larger
 * hole. For example, if you have four separate holes of size 550 KB, 375 KB, 1,900 KB, and
 * 4,500 KB, your program will combine these four holes into one large hole of size 7,325 KB.
 * There are several strategies for implementing compaction, one of which is suggested in
 * Section 9.2.3. Be sure to update the beginning address of any processes that have been
 * affected by compaction.
 */
void compact(){
   Process *curr = head;
   if (head == NULL) {
      fprintf(stderr, "Nothing to compact\n");
      return;
   } 
   // check if first node is zero - shift it!
   if (curr->startAddress != 0) {
      curr->endAddress = curr->endAddress - curr->startAddress;
      curr->startAddress = 0;
   }
   while (curr->next != NULL) {
      // Process *temp = curr;
      unsigned long diff = curr->next->startAddress - curr->endAddress;
      if (diff != 1) {
         curr->next->startAddress = curr->startAddress + 1;
         curr->next->endAddress = curr->next->startAddress + diff;
      }
      curr = curr->next;
   }
}

void statistics(){
      Process * temp = head;
      unsigned long diff = 0;
      if (temp->startAddress != 0) {
         printf("Addresses [%d:%lu] Unused\n", 0, temp->startAddress - 1); 
      }
      while (temp->next != NULL){
         printf("Addresses [%lu:%lu] Process %s\n", temp->startAddress, temp->endAddress, temp->processID);
         diff = temp->next->startAddress - temp->endAddress;
         if (diff != 1) {
           printf("Addresses [%lu:%lu] Unused\n", temp->startAddress, temp->endAddress); 
         }
         temp = temp->next;
      }
// ------------
      // while(temp->next){
      //      if(!temp->processID)
      //        printf("Addresses [%lu:%lu] Unused\n", temp->startAddress, temp->endAddress);
      //      else
      //        printf("Addresses [%lu:%lu] Process %s\n", temp->startAddress, temp->endAddress, temp->processID);
      //      temp = temp->next;
      // }
      // if(temp){
      //    if(!temp->processID)
      //        printf("Addresses [%lu:%lu] Unused\n", temp->startAddress, temp->endAddress);
      //    else
      //        printf("Addresses [%lu:%lu] Process %s\n", temp->startAddress, temp->endAddress, temp->processID);
      //    if(temp->endAddress < limit -1)
      //        printf("Addresses [%lu] . . .\n", temp->endAddress +1);
      // }
      // else{
      //      printf("Addresses [0] . . .\n");
      // }
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
        char fit = command_line[3][0];
        request(command_line[1], proc_size, fit);
     }
     else if(!strcmp(command_line[0], "RL"))
        release(command_line[0]);
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
