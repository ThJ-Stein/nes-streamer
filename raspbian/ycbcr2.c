//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//  Revised: 15-Feb-2013


// Access from ARM Running Linux

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX( a, b ) ( ( (a) > (b) ) ? (a) : (b) )
#define MIN( a, b ) ( ( (a) < (b) ) ? (a) : (b) )


unsigned int loopCount;
unsigned int sameDataCount;
unsigned int notReadyCount;

void calculateRGB(int y, int cr, int cb, unsigned char rgb[3]);

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
  signal(SIGINT, intHandler);

  loopCount = 0;
  char byte;

  active = true;

  char y = 0;
  char cr = 0;
  char cb = 0;

  unsigned char rgb[3];

  int send = 0;

  while (active) {
//	if (loopCount % 720 == 0) {
//		send = 0;
//		printf("%c%c%c", 0, 0, 0);
//	}
	switch(loopCount % 4) {
		case 1:
			y = getchar();
			break;
		case 2:
			cr = getchar();
			if (!send) {
				send = 1;
				break;
			}
			calculateRGB(y, cr, cb, rgb);
			break;
		case 3:
			y = getchar();
			break;
		case 0:
			cb = getchar();
			if (!send) {
				send = 1;
				break;
			}
			calculateRGB(y, cr, cb, rgb);
			break;
		}
		loopCount++;
	}
  return 0;
} // main

double ydiv = 1.164;

double crr = 1.596;

double cbg = -0.392;
double crg = -0.813;

double cbb = 2.017;

void calculateRGB(int y, int cr, int cb, unsigned char rgb[3]) {
//	printf("ycrcb = %d %d %d\n", y, cr, cb);

	double yConstant = (y-16) * ydiv;

	int r = yConstant + crr * (cr-128);
	int g = yConstant + cbg * (cb-128) + crg * (cr-128);
	int b = yConstant + cbb * (cb-128);

	rgb[0] = MAX(0, MIN(r, 255));
	rgb[1] = MAX(0, MIN(g, 255));
	rgb[2] = MAX(0, MIN(b, 255));

	printf("%c%c%c", rgb[0], rgb[1], rgb[2]);
}
