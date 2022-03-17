#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"

//Initializing the main, creating the hashmap and default size
int main(int argc, char *argv[]){
  int echo = 0;      
  char cmd[128];
  hashmap_t hashmap;
  char success;
  hashmap_init(&hashmap, HASHMAP_DEFAULT_TABLE_SIZE);
                          
  if(argc > 1 && strcmp("-echo",argv[1])==0) { 
    echo=1;
  }
//print statements in the beginning:

printf("Hashmap Main\n");
printf("Commands:\n");
printf("  hashcode <key>   : prints out the numeric hash code for the given key (does not change the hash map)\n");
printf("  put <key> <val>  : inserts the given key/val into the hash map, overwrites existing values if present\n");
printf("  get <key>        : prints the value associated with the given key or NOT FOUND\n");
printf("  print            : shows contents of the hashmap ordered by how they appear in the table\n");
printf("  structure        : prints detailed structure of the hash map\n");
printf("  clear            : reinitializes hash map to be empty with default size\n");
printf("  save <file>      : writes the contents of the hash map the given file\n");
printf("  load <file>      : clears the current hash map and loads the one in the given file\n");
printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
printf("  expand           : expands memory size of hashmap to reduce its load factor\n");
printf("  quit             : exit the program\n");


while(1){
    printf("HM> ");                 // print prompt
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }
    else if(strcmp("quit", cmd)==0 ){     // check for quit command
      if(echo){
        printf("quit ");
      }
      hashmap_free_table(&hashmap);
      break;                          
    }
    else if(strcmp("hashcode", cmd)==0){ //hashcode
      char key[128];
      fscanf(stdin, "%s", key);
      if(echo){
        printf("hashcode %s\n", key);
      }
      printf("%ld\n", hashcode(key));
      
    }
    else if(strcmp("print", cmd)==0){ //print 
      if(echo){
        printf("print\n");
      }
      hashmap_write_items(&hashmap, stdout);
    }
    else if(strcmp("put", cmd)==0 ){ // put
      char key[128];
      char val[128];
      fscanf(stdin,"%s %s",key, val);
      if(echo){
        printf("put %s %s\n", key, val);
        
        success = hashmap_put(&hashmap, key, val);
      if(!success){                      // check for unsuccess to then print overwriting 
        printf("Overwriting previous key/val\n");
      }
    }
    }
    else if(strcmp("get", cmd)==0 ){//get
      char val[128];
      fscanf(stdin, "%s", val);
      if(echo){
        printf("get %s\n", val);
      }
      char *success1 = hashmap_get(&hashmap, val);
      if(success1 == NULL){
        printf("NOT FOUND\n");
      }
      else{
        printf("FOUND: %s\n", success1);
      }
    }
    else if(strcmp("clear", cmd)==0){//clear
      if(echo){
        printf("clear\n");
      }
      hashmap_free_table(&hashmap);
      hashmap_init(&hashmap, HASHMAP_DEFAULT_TABLE_SIZE);
    }
    else if(strcmp("structure", cmd)==0) {//structure
      if(echo){
        printf("structure\n");
      }
      hashmap_show_structure(&hashmap);
    }
    else if(strcmp("save ", cmd)==0){ //Save
      char theFile[128];
      fscanf(stdin, "%s", theFile);
      if(echo){
        printf("save %s\n", theFile);
      }
      hashmap_save(&hashmap, theFile);
      // printf("save %s\n", theFile);
    }
    else if(strcmp("load <file>", cmd)==0){  //Load: havent finished the method for it in funcs.
      char theFile[128];
      fscanf(stdin, "%s\n", theFile);
      if(echo){
        printf("load %s\n", theFile);
      }
      success = hashmap_load(&hashmap, theFile);
      if(success == 0){
        printf("Load Failed %s\n", theFile);
      }
    }

    //Don't know if this is needed, so I am leaving it out for now:

  //   else{                                 // unknown command
  //     if(echo){
  //       printf("%s\n",cmd);
  //     }
  //     printf("unknown command %s\n",cmd);
  //   }
  
}
  //free the hashmap at the end:

  hashmap_free_table(&hashmap); 
  return 0;
}
