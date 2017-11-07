//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//  Revised: 15-Feb-2013


// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x3F000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

unsigned int loopCount;
unsigned int sameDataCount;
unsigned int notReadyCount;

unsigned int processed;
const unsigned int processedMask = 13;

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock
#define GPIO_DATA_BYTE (*(gpio+13)&(0b1111111100))>>2 // bitmask to get the value of pin 2 to 9.
#define GPIO_DATA_READY (*(gpio+13)&(1<<10)) // bitmask to get the data ready value at pin 10. 
#define GPIO_FRAME_READY (*(gpio+13)&(1<<11)) // bitmask to get the frame ready value at pin 11.

#define GPIO_DATA_PROCESSED 12
#define GPIO_DATA_PROCESSED_MASK 1<<12

#define FRAME_SIZE 240

void setup_io();

bool active;

void busyWait(int loops) {
        while (loops > 0) {
                loops--;
        }
}

void intHandler(int dummy) {
  active = false;
  printf("lc: %d, nrc: %d, sdc: %d \n",  loopCount, notReadyCount, sameDataCount);
}

int main(int argc, char **argv)
{
  int g,rep;

  // Set up gpi pointer for direct register access
  setup_io();
  signal(SIGINT, intHandler);
  // Switch GPIO (BCM) 0..9 to input mode

 /************************************************************************\
  * You are about to change the GPIO settings of your computer.          *
  * Mess this up and it will stop working!                               *
  * It might be a good idea to 'sync' before running this program        *
  * so at least you still have your code changes written to the SD-card! *
 \************************************************************************/

  // Set GPIO pins 2-12 to input
  for (g=2; g<=12; g++)
  {
    INP_GPIO(g);
  }
  // Set GPIO pin 12 to output
  OUT_GPIO(12);
  loopCount = 0;
  sameDataCount = 0;
  notReadyCount = 0;
  unsigned int ready = 0;
  unsigned int oldReady = 0;
  char byte;

  char frame[FRAME_SIZE] = {0};
  int frameIndex = 0;

  active = true;

  processed = 10;


	//unsigned int mask = (((unsigned int) gpio) + 7) ^ (((unsigned int) gpio) + 10);

	//printf("%d %d %d %d\n", gpio + 7, gpio + 10);
  int fd;
  int ret;

  char y = 0;
  char cr = 0;
  char cb = 0;

  char rgb[3];

  int send = 0;

  GPIO_CLR = GPIO_DATA_PROCESSED_MASK;

  while (active) {
//    printf("%d\n", loopCount);
    ready = GPIO_DATA_READY;
    if (oldReady != ready) {
        oldReady = ready;

	frame[frameIndex] = GPIO_DATA_BYTE;
	frameIndex = (frameIndex + 1) % FRAME_SIZE;
	if (!frameIndex) {
		write(1, frame, FRAME_SIZE);
	}

	processed = processed ^ processedMask;
	*(gpio + processed) = GPIO_DATA_PROCESSED_MASK;
    } else {
//      notReadyCount++;
    }
  }

  close(fd);
//  printf("fifo closed\n");
//  unlink("myfifo");
//  printf("fifo unlinked");



  GPIO_CLR = GPIO_DATA_PROCESSED_MASK;
  return 0;
} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);//errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io
