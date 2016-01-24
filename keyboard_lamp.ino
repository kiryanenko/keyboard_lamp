#define INT_NUM 1		// прерывания с номерами 0 (на digital pin 2) и 1 (на digital pin 3)

volatile bool isRand;	// прервание: изменение режима работы

void int_set_mode() {	// прервание: изменение режима работы
	isRand = !isRand;
}

void setup()
{
	attachInterrupt(INT_NUM, int_set_mode, RISING); // привязываем прерывание к функции int_set_mode().

}

void loop()
{

  /* add main program code here */

}
