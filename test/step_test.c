#include <wiringPi.h>
#include <stdio.h>

#define Z_M1 12
#define Z_M2 13
#define Z_M3 14

#define Z_DIR 6
#define Z_RESET 22
#define Z_ENABLE 26
#define Z_STEP 1

int main(int argc, char *argv[])
{
	int speed = atoi(argv[1]);
	int direction = atoi(argv[2]);

	wiringPiSetup();

	pinMode(Z_ENABLE, OUTPUT);
	pinMode(Z_DIR, OUTPUT);
	pinMode(Z_M1, OUTPUT);
	pinMode(Z_M2, OUTPUT);
	pinMode(Z_M3, OUTPUT);

	pinMode(Z_STEP, OUTPUT);
	pinMode(Z_RESET, OUTPUT);
	
	
	digitalWrite(Z_ENABLE, HIGH);
	digitalWrite(Z_RESET, HIGH);	

	digitalWrite(Z_M1, LOW);
	digitalWrite(Z_M2, LOW);
	digitalWrite(Z_M3, HIGH);
	
	
	digitalWrite(Z_DIR, direction);
	while(1)
	{
		digitalWrite(Z_STEP, !digitalRead(Z_STEP));
		delayMicroseconds(speed);
	}
	
	digitalWrite(Z_M1, LOW);
	digitalWrite(Z_M2, LOW);
	digitalWrite(Z_M3, LOW);
	
	
	//digitalWrite(Z_ENABLE, LOW);
	//digitalWrite(Z_RESET, LOW);
	return 0;

}
