#define RED_POT_PIN		A7	// пин красного рычага
#define GREEN_POT_PIN	A6	// пин зеленого рычага
#define BLUE_POT_PIN	A5	// пин синего рычага

#define RED_LED			6	// пин красных светодиодов
#define GREEN_LED		2	// пин красных светодиодов
#define BLUE_LED		4	// пин красных светодиодов

#define DURATION_PIN	A6	// пин рычага, отвечающий за длительность удерживания цвета
#define SHADING_PIN		A5	// пин рычага, отвечающий за длительность перехода

#define K_DURATION		300	// макс длительность удерживания цвета в сек
#define K_SHADING		30	// макс длительность перехода в сек

#define INT_NUM			1	// прерывания с номерами 0 (на digital pin 2) и 1 (на digital pin 3)

volatile bool isRand;		// прервание: изменение режима работы

void int_set_mode() {		// прервание: изменение режима работы
	isRand = !isRand;
}

void setup()
{
	attachInterrupt(INT_NUM, int_set_mode, RISING); // привязываем прерывание к функции int_set_mode().

}

void loop()
{
}
