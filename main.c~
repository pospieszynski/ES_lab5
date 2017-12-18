#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dbgu_print_ascii(const char *buffer) {}

#include "FIFO.h"
#include "DBGU.h"
#define DBGU_INIT_SUCCESS 0
#define SUCCESS 1
#define FAILURE -1
#define TASK FIFO

int populateFifo(struct FIFO* fifo);
void readFromFifo(struct FIFO* fifo);

int main() {
  configureRegisters(); // LED !!
  
  if( initializeDGBU() == DBGU_INIT_SUCCESS ) {
    
#if TASK == 1
      printAlphabet();
#endif
      
#if TASK == 2
      printString("\n\rHello world!\r\n");
#endif
      
#if TASK == 3
      while(1)
	reverseLetterCase();
#endif
	
#if TASK == FIFO
      struct FIFO fifo;
      
      if( initFIFO(&fifo) == SUCCESS ) {
	while(1) {
	  populateFifo(&fifo);
	  readFromFifo(&fifo);
	}
      }
#endif 
  }
  while(1);
}

int populateFifo(struct FIFO* fifo) {
  char character;
  
  while(1) {
    readCharacter(&character);
    if( character != '\r') {
      if(pushToFIFO(fifo, character) == FAILURE) {
      	printString("\n\rBuffer overflow error.\r\n");
      	return FAILURE;
      }
    } else {
      return SUCCESS; 
    }
  }
}

void readFromFifo(struct FIFO* fifo) {
    int size = getFIFOSize(fifo);
    char* command = (char*) malloc((size+1)*sizeof(char));
    int index = 0;
    char letter;
    
    while(popFromFIFO(fifo, &letter) == SUCCESS) {
      command[index] = letter;
      index++;
    }
    
    command[index] = '\0';
    printString(command);
    
    if(strcmp(command, "de1") == 0) {
      printString("\n\r Turning on diod 1.\r\n");
      turnOnDS1();
    } else if(strcmp(command, "de2") == 0) {
      printString("\n\r Turning on diod 2.\r\n");
      turnOnDS2();
    } else if(strcmp(command, "dd2") == 0) {
      printString("\n\r Turning off diod 2.\r\n");
      turnOffDS2();
    } else if(strcmp(command, "dd1") == 0) {
      printString("\n\r Turning off diod 1.\r\n");
      turnOffDS1();
    }
}
