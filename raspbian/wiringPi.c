#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define DATA_READY 17		//physical pin 11, D11
#define DATA_PROCESSED 18	//physical pin 12, D10
#define FRAME_READY 5		//physical pin 29, D20

#define DATA_PIN_1 6		//physical pin 31, D19, most significant bit
#define DATA_PIN_2 12		//physical pin 32, D18
#define DATA_PIN_3 13		//physical pin 33, D17
#define DATA_PIN_4 19		//physical pin 35, D27
#define DATA_PIN_5 16		//physical pin 36, D26
#define DATA_PIN_6 26		//physical pin 37, D25
#define DATA_PIN_7 20		//physical pin 20, D24
#define DATA_PIN_8 21		//phyiscal pin 21, D23, least significant bit

#define DATA_PIN_COUNT 8

void onReady();

void readPins(int *pins);
void copyPins(int *oldPins, int *newPins);
int comparePins(int *oldPins, int *newPins);
void busyWait(int loops);

void printAllPins();
void printPins(int *pins);

const int dataPins[] = {
	DATA_PIN_1,
	DATA_PIN_2,
	DATA_PIN_3,
	DATA_PIN_4,
	DATA_PIN_5,
	DATA_PIN_6,
	DATA_PIN_7,
	DATA_PIN_8
};

bool active;

void intHandler(int dummy) {
//	printAllPins();

	active = false;
}

int main(int argc, char **argv) {
	active = true;



	int oldReady = 0;
	int oldProcessed = 0;

	int newPins[8] = {0};
	int oldPins[8] = {0};

	int dummyPins[8] = {0};

	int loopCount = 0;
	int sameDataCount = 0;
	int notReadyCount = 0;

	signal(SIGINT, intHandler);

	wiringPiSetupGpio();

	pinMode(DATA_READY, INPUT);
	pinMode(DATA_PROCESSED, OUTPUT);
	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		pinMode(i, INPUT);
	}


	digitalWrite(DATA_PROCESSED, oldProcessed);

	while (active) {
		int ready = digitalRead(DATA_READY);

		if (ready != oldReady) {
			oldReady = ready;

			loopCount++;
			copyPins(oldPins, newPins);

			readPins(newPins);
//			printPins(newPins);

			oldProcessed = oldProcessed + 1 % 2;
			digitalWrite(DATA_PROCESSED, oldProcessed);

			busyWait(100);

		} else {
			notReadyCount++;
		}
	}

	digitalWrite(DATA_PROCESSED, 0);

	printf("%d %d %d\n", loopCount, sameDataCount, notReadyCount);
}

void readPins(int *pins) {
	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		pins[i] = digitalRead(dataPins[i]);
	}
}

void copyPins(int *oldPins, int *newPins) {
	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		oldPins[i] = newPins[i];
	}
}

int comparePins(int *oldPins, int *newPins) {
	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		if (oldPins[i] != newPins[i]) {
			return 0;
		}
	}
	return 1;

}

//this method is bad
void printAllPins() {

	int pins[8];

	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		pins[i] = digitalRead(dataPins[i]);
	}

	int ready = digitalRead(DATA_READY);
	int processed = digitalRead(DATA_PROCESSED);

	printf("R=%d	P=%d\n", ready, processed);
	printPins(pins);
}

void printPins(int *pins) {
	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		printf("%d", pins[i]);
	}
	printf("\n");
}

void busyWait(int loops) {
	while (loops > 0) {
		loops--;
	}
}
