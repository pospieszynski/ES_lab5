#include "AT91SAM9263.h"
#include "FUNCTIONS.h"

#define SET_DS_1 (1<<8)
#define SET_DS_2  (1<<29)
#define SET_BP1 (1<<5)
#define SET_BP2 (1<<4)
#define SET_CLOCK  (1<<4)

#define COUNT_FOR_1_MILISEC 6250

void clearTimerStatusRegister() {
  AT91C_BASE_PITC->PITC_PIVR; 
}

void enablePIT() {
  AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN;
}

void disablePIT() {
   AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITEN;
}

void configurePIT() {
  AT91C_BASE_PITC->PITC_PIMR = COUNT_FOR_1_MILISEC;
  AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITIEN;
  clearTimerStatusRegister();
  disablePIT();
}

void turnOnDS2() {
  *AT91C_PIOC_CODR = SET_DS_2;
}

void turnOffDS2() {
  *AT91C_PIOC_SODR = SET_DS_2;
}

int DS1Status() {
  return (*AT91C_PIOB_PDSR & SET_DS_1);
}

int DS2Status() {
  return (*AT91C_PIOC_PDSR & SET_DS_2);
}

int BP1Status() {
  return (*AT91C_PIOB_PDSR & SET_BP1);
}

int BP2Status() {
  return (*AT91C_PIOC_PDSR & SET_BP2);
}

void turnOnDS1() {
  *AT91C_PIOB_CODR = SET_DS_1;
}

void turnOffDS1() {
  *AT91C_PIOB_SODR = SET_DS_1;
}

void configureLEDs() {
  // Peripherial enable register must be configured to set IO port in IO mode and enable further configuration
  *AT91C_PIOB_PER = SET_DS_1;
  // By defult, IO mode is set to input but we want diods to be used as an output(blinking)
  *AT91C_PIOB_OER = SET_DS_1;
  // Switch off DS1 by default
  turnOffDS1();

// Peripherial enable register must be configured to set IO port in IO mode and enable further configuration
  *AT91C_PIOC_PER = SET_DS_2;
  // By defult, IO mode is set to input but we want diods to be used as an output(blinking)
  *AT91C_PIOC_OER = SET_DS_2;
  //switch off DS2 by default
  turnOffDS2();
}

void configureClock() {
  *AT91C_PMC_PCER = SET_CLOCK;
}

void configurreButtons() {
  *AT91C_PIOC_PER = SET_BP1;
  *AT91C_PIOC_ODR = SET_BP1;
  *AT91C_PIOC_PPUER = SET_BP1;

  *AT91C_PIOC_PER = SET_BP2;
  *AT91C_PIOC_ODR = SET_BP2;
  *AT91C_PIOC_PPUER = SET_BP2;
  //after that on register bit associatd with BP1 we have 0 since we overwritten it but this does not mean that it is disabled. to do that we would have to use PU disable registers
}

void enablePullUpBP1() {
  *AT91C_PIOC_PPUER = SET_BP1;
}

void enablePullUpBP2() {
  *AT91C_PIOC_PPUER = SET_BP2;
}

void disablePullUpBP1() {
  *AT91C_PIOC_PPUDR = SET_BP1;
}

void disablePullUpBP2() {
  *AT91C_PIOC_PPUDR = SET_BP2;
}

int pullUpStatusBP1() {
  return *AT91C_PIOC_PPUSR & SET_BP1;
}

int pullUpStatusBP2() {
  return *AT91C_PIOC_PPUSR & SET_BP2;
}

void configureRegisters() {
  configureClock();
  configureLEDs();
  configurreButtons();
}

void handleButtonPressed() {
  if(checkIfRightButtonPressed()) // BP2
    turnOnDS2();
  else if(checkIfLeftButtonPressed()) {
    turnOffDS2();
  }
}

unsigned int checkIfLeftButtonPressed() {
  if(!(*AT91C_PIOC_PDSR & SET_BP1)) {
    return 1; 
  } else {
    return 0;
  }
}

unsigned int checkIfRightButtonPressed() {
  if(!(*AT91C_PIOC_PDSR & SET_BP2)) {
    return 1;   
  } else {
    return 0;
  }
}

unsigned int checkIfTimerStoppped() {
  if(AT91C_BASE_PITC->PITC_PISR) {
    return 1;
  } else {
    return 0;
  }
}

void delay(unsigned int miliseconds) {
  unsigned int counter;
  
  for(counter = 0; counter < miliseconds; counter++) {
    enablePIT();
    while(!checkIfTimerStoppped()) {
      handleButtonPressed();
    }
    clearTimerStatusRegister();
  }
  disablePIT();
}