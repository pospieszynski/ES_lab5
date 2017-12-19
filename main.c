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
        printString("..:GPIO CONSOLE:..");
	while(1) {
    printString("\n\r> ");
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

  if(command != NULL) {
    int index = 0;
    char letter;
    
    while(popFromFIFO(fifo, &letter) == SUCCESS) {
      command[index] = letter;
      index++;
    }
    
    command[index] = '\0';
    printString(command);
    
    if(strcmp(command, "d 1 e") == 0) {
      turnOnDS1();
    } else if(strcmp(command, "d 2 e") == 0) {
      turnOnDS2();
    } else if(strcmp(command, "d 2 d") == 0) {
      turnOffDS2();
    } else if(strcmp(command, "d 1 d") == 0) {
      turnOffDS1();
    } else if(strcmp(command, "d 1 s") == 0) {

    } else if(strcmp(command, "d 2 s") == 0) {

    } else if(strcmp(command, "b 1 p e") == 0) {
      enablePullUpBP1();
    } else if(strcmp(command, "b 2 p e") == 0) {
      enablePullUpBP2();
    } else if(strcmp(command, "b 1 p d") == 0) {
      disablePullUpBP1();
    } else if(strcmp(command, "b 2 p d") == 0) {
      disablePullUpBP2();
    } else if(strcmp(command, "b 1 p s") == 0) {
      pullUpStatusBP1();
    } else if(strcmp(command, "b 2 p s") == 0) {
      pullUpStatusBP1();
    } else if(strcmp(command, "b 1 s") == 0) {
      if(checkIfLeftButtonPressed()) {
        printString("\n\rBP 1 - 0 state\r\n");
      } else {
        printString("\n\rBP 1 - 1 state\r\n");
      }
    } else if(strcmp(command, "b 2 s") == 0) {
      if(checkIfRightButtonPressed() ) {
        printString("\n\r BP 2 - 0 state\r\n");
      } else {
        printString("\n\rBP 2 - 1 state\r\n");
      }
    } else if(strcmp(command, "h") == 0) {
      printString("\n\rd 1 e - diod 1 enable\r\n");
      printString("\n\rd 2 e - diod 2 enable\r\n");
      printString("\n\rd 1 d - diod 1 disable\r\n");
      printString("\n\rd 2 d - diod 2 disable\r\n");
      printString("\n\rb 1 p e - button 1 pullup enable\r\n");
      printString("\n\rb 2 p e - button 2 pullup enable\r\n");
      printString("\n\rb 1 p d - button 1 pullup disable\r\n");
      printString("\n\rb 2 p d - button 2 pullup disable\r\n");
      printString("\n\rb 1 p s - button 1 pullup status\r\n");
      printString("\n\rb 2 p s - button 2 pullup status\r\n");
      printString("\n\rb 1 s - button 1 status\r\n");
      printString("\n\rb 2 s - button 2 status\r\n");
    } else {
      printString("\n\rUnknown command, press h to see available commands\r\n");
    }

    free(command);
  }
}
