#include <wiringPi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int pin = atoi(argv[1]);
	int value = atoi(argv[2]);
	printf("Setting pin %d to %d\n", pin, value);
	wiringPiSetup();
	pinMode(pin, OUTPUT);
	digitalWrite(pin, value);
	return 0;

}
