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
	switch(loopCount % 4) {
		case 1:
			y = getchar();
			break;
		case 2:
			cr = getchar();
			calculateRGB(y, cr, cb, rgb);
			send = 3;
			break;
		case 3:
			y = getchar();
			break;
		case 0:
			cb = getchar();
			calculateRGB(y, cr, cb, rgb);
			break;
		}
		loopCount++;
	}
  return 0;
} // main

double ydiv = 298.082 / 256;

double crr = 408.583 / 256;
double rConstant = 222.921;

double cbg = 100.291 / 256;
double crg = 208.120 / 256;
double gConstant = 135.576;

double cbb = 516.412 / 256;
double bConstant = 276.836;

void calculateRGB(int y, int cr, int cb, unsigned char rgb[3]) {
	printf("ycrcb = %d %d %d\n", y, cr, cb);

	double yConstant = y * ydiv;

	int r = yConstant + rConstant + crr * cr;
	int g = yConstant + gConstant + cbg * cb + crg * cr;
	int b = yConstant + bConstant + cbb * cb;

	rgb[0] = r > 0 ? r : 0;
	rgb[1] = g > 0 ? g : 0;
	rgb[2] = b > 0 ? b : 0;

	printf("rgb = %d %d %d\n", rgb[0], rgb[1], rgb[2]);
}
