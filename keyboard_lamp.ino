#define RED_POT_PIN		A7	// пин красного рычага
#define GREEN_POT_PIN	A6	// пин зеленого рычага
#define BLUE_POT_PIN	A5	// пин синего рычага

#define RED_LED			5	// пин красных светодиодов
#define GREEN_LED		3	// пин красных светодиодов
#define BLUE_LED		6	// пин красных светодиодов
// Rand
#define DURATION_PIN	A6	// пин рычага, отвечающий за длительность удерживания цвета
#define SHADING_PIN		A5	// пин рычага, отвечающий за длительность перехода

#define K_DURATION		300	// макс длительность удерживания цвета в сек
#define K_SHADING		30	// макс длительность перехода в сек
// Rising / Fall
#define TIME_PARTITION	100	// мин отрезок времени в мс

#define INT_NUM			0	// прерывания с номерами 0 (на digital pin 2) и 1 (на digital pin 3)

long previousMillis = 0;	// здесь будет храниться время последнего изменения состояния

struct Color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} current_color;				// предыдущий цвет

volatile enum {Static, Rand, Rise, Fall} Mode;		// режим работы 

void int_set_mode() {		// прервание: изменение режима работы
	static unsigned long millis_prev;
	if (millis() - 300 > millis_prev) {
		switch (Mode) {
		case Static:	Mode = Rand;	break;
		case Rand:		Mode = Rise;	break;
		case Rise:		Mode = Fall;	break;
		case Fall:		Mode = Static;	break;
		}
		Serial.println(Mode);
		millis_prev = millis();
	}
}

void setup()
{
	Serial.begin(9600);
	attachInterrupt(INT_NUM, int_set_mode, RISING); // привязываем прерывание к функции int_set_mode().
	randomSeed(analogRead(7));
	Mode = Static;
	pinMode(2, INPUT_PULLUP);
	Serial.println("Hello");
}

void loop()
{
	switch (Mode) {
	case Static:
		setColor(analogRead(RED_POT_PIN) / 4, analogRead(GREEN_POT_PIN) / 4, analogRead(BLUE_POT_PIN) / 4);
		delay(TIME_PARTITION);
		break;
	case Rand:
		if (millis() - previousMillis > analogRead(DURATION_PIN) * K_DURATION + 1000) {
			shading(random(255), random(255), random(255));
			previousMillis = millis();
		}
		delay(TIME_PARTITION);
		break;
/*	case Rise:
		setColor(analogRead(DURATION_PIN));
		delay(TIME_PARTITION);
		break;
	case Fall:
		Mode = Static;
		break;*/
	}
}

void setColor(unsigned char red, unsigned char green, unsigned char blue) {
	analogWrite(RED_LED, red);
	analogWrite(BLUE_LED, blue);
	analogWrite(GREEN_LED, green);
	current_color.red = red;
	current_color.blue = blue;
	current_color.green = green;
}

void shading(unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char last_red = current_color.red;
	unsigned char last_blue = current_color.blue;
	unsigned char last_green = current_color.green;
	float r = last_red, g = last_green, b = last_blue;
	while ((current_color.red != red || current_color.blue != blue || current_color.green != green) && Mode == Rand) {
		long time = analogRead(SHADING_PIN) * K_SHADING / TIME_PARTITION;	// длительность перехода / мин отрезок времени
		// скорости
		float vr = (red - last_red + 0.0) / time;		// сумарное расстояние
		float vb = (blue - last_blue + 0.0) / time;		// ------------------- * мин отрезок времени = расстояние проходимое за мин отрезок времени
		float vg = (green - last_green + 0.0) / time;	//	      время

		abs(r - red) < abs(vr) ? r = red : r += vr;
		abs(b - blue) < abs(vb) ? b = blue : b += vb;
		abs(g - green) < abs(vg) ? g = green : g += vg;
		setColor(r, g, b);
		delay(TIME_PARTITION);
	}
}
