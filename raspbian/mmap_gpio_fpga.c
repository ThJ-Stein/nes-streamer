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
 
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

unsigned int loopCount;
unsigned int sameDataCount;
unsigned int notReadyCount;
 
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
 
  // Set GPIO pins 0-10 to input
  for (g=2; g<=12; g++)
  {
    INP_GPIO(g);
  }
  // Set GPIO pin 10 to output
  OUT_GPIO(12);
  loopCount = 0;
  sameDataCount = 0;
  notReadyCount = 0;
  unsigned int ready = 0;
  char byte;
  active = true;
  GPIO_CLR = GPIO_DATA_PROCESSED_MASK;

  while (active) {
    if (GPIO_DATA_READY != ready) {
//       printf("1: %d, %d \n", ready, GPIO_DATA_READY); // debug statement to see if data ready altnernates between 0 and 256.

     loopCount++;
//      if (byte == GPIO_DATA_BYTE) { // branch is relatively slow
//        sameDataCount++;
      byte = GPIO_DATA_BYTE;
      ready = GPIO_DATA_READY;

//       printf("2: %d, %d \n", ready, GPIO_DATA_READY); // debug statement to see if data ready altnernates between 0 and 256.


//      printf("%X\n", byte); // disable this to test speed, a char is a byte, this prints the char corresponding to GPIO_DATA_BYTE

      if(GET_GPIO(GPIO_DATA_PROCESSED)) { // branch is relatively slow, there is a way to eliminate this one I think
//        printf("setting processed to 0\n");
	GPIO_CLR = GPIO_DATA_PROCESSED_MASK; // sets data processed to 0
      } else {
//      printf("setting processed to 1\n");
        GPIO_SET = GPIO_DATA_PROCESSED_MASK; // sets data processed to 1
      }
//	printf("%d\n", GET_GPIO(GPIO_DATA_PROCESSED));
      busyWait(10);

    } else {
      notReadyCount++;
    }
  }

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

