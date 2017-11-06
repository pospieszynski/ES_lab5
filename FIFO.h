#define BUFFER_SIZE 3
#define SUCCESS 1
#define FAILURE -1

typedef struct FIFO {
 char buffer[BUFFER_SIZE];	 
 unsigned int head; 
 unsigned int tail;
};	

void initFIFO (struct FIFO *Fifo);
void emptyFIFO (struct FIFO *Fifo);
int pushToFIFO (struct FIFO *Fifo, char data);
int popFromFIFO (struct FIFO *Fifo, char *data);
