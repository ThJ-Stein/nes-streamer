#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <limits.h>

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

void onRisingEdge();

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

int main() {

	bool active = true;

	wiringPiSetupGpio();

	pinMode(DATA_READY, INPUT);
	pinMode(DATA_PROCESSED, OUTPUT);
	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		pinMode(i, INPUT);
	}

	//wiringPiISR(DATA_READY, INT_EDGE_RISING, &onRisingEdge);

	while (true) {
		int value = digitalRead(DATA_READY);
		digitalWrite(DATA_PROCESSED, value);
	}
}

void onRisingEdge() {
	digitalWrite(DATA_PROCESSED, 0);

	char result = 0;

	for (int i = 0; i < DATA_PIN_COUNT; i++) {
		result << 0;
		result += digitalRead(dataPins[i]);
	}

	printf("%d", result);

	digitalWrite(DATA_PROCESSED, 1);
}
