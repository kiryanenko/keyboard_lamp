#define RED_POT_PIN		A7	// пин красного рычага
#define GREEN_POT_PIN	A6	// пин зеленого рычага
#define BLUE_POT_PIN	A5	// пин синего рычага

#define RED_LED			5	// пин красных светодиодов
#define GREEN_LED		3	// пин красных светодиодов
#define BLUE_LED		6	// пин красных светодиодов

#define TIME_PARTITION	50	// мин отрезок времени в мс

#define INT_NUM			0	// прерывания с номерами 0 (на digital pin 2) и 1 (на digital pin 3)
// Rand / Shift режим рандомных цветов / режим смены цвета:
#define DURATION_PIN	A6	// пин рычага, отвечающий за длительность удерживания цвета
#define SHADING_PIN		A5	// пин рычага, отвечающий за длительность перехода

#define K_DURATION		100	// макс длительность удерживания цвета в сек
#define K_SHADING		5	// макс длительность перехода в сек
// Rising / Fall / Vector / Flicker режимы возрастания / убывания / по середине рычага 0 / мерцание:
#define MAX_SPEED		256 // макс скорость изменения цвета в цвет / сек
#define EXPONENT		17	// степень для рычагов

long previousMillis = 0;	// здесь будет храниться время последнего изменения состояния

struct Color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} current_color;				// предыдущий цвет

volatile enum { Static, Rand, Rise, Fall, Vector, Flicker, Shift } Mode;		// режим работы 

void int_set_mode() {		// прервание: изменение режима работы
	static unsigned long millis_prev;
	if (millis() - 300 > millis_prev) {
		switch (Mode) {
		case Static:	Mode = Rand;	break;
		case Rand:		Mode = Rise;	break;
		case Rise:		Mode = Fall;	break;
		case Fall:		Mode = Vector;	break;
		case Vector:	Mode = Flicker;	break;
		case Flicker:	Mode = Shift;	break;
		case Shift:		Mode = Static;	break;
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
		break;
	case Rand:
		if (millis() - previousMillis > long(analogRead(DURATION_PIN)) * K_DURATION + 1000) {
			shading(random(256), random(256), random(256));
			previousMillis = millis();
		}
		break;
	case Rise:
		vector(pow(long(analogRead(RED_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000,
			pow(long(analogRead(GREEN_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000,
			pow(long(analogRead(BLUE_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000);	// 1000 примерно равно 1023 (сокращаются)
		break;
	case Fall:
		vector(-pow(long(analogRead(RED_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000,
			-pow(long(analogRead(GREEN_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000,
			-pow(long(analogRead(BLUE_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000);	// 1000 примерно равно 1023 (сокращаются)
		break;
	case Vector:
		vector(pow(long(analogRead(RED_POT_PIN) - 511) / 511.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000,
			pow(long(analogRead(GREEN_POT_PIN) - 511) / 511.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000,
			pow(long(analogRead(BLUE_POT_PIN) - 511) / 511.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000);
		break;
	case Flicker:
		flicker();
		break;
	case Shift:
		if (millis() - previousMillis > long(analogRead(DURATION_PIN)) * K_DURATION + 1000) {
			char rand = random(3);
			switch (rand) {
			case 0: shading(255, 0, 0);	break;
			case 1: shading(0, 255, 0);	break;
			case 2: shading(0, 0, 255);	break;
			}
			previousMillis = millis();
		}
		break;
	}
	delay(TIME_PARTITION);
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
	while ((current_color.red != red || current_color.blue != blue || current_color.green != green) 
		&& (Mode == Rand || Mode == Shift)) {
		float time = long(analogRead(SHADING_PIN) + 1) * K_SHADING / TIME_PARTITION;	// длительность перехода / мин отрезок времени
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

void vector(float red, float green, float blue) {	// функция увеличивающаю текущий цвет на заданное значение
	static float r = random(256), g = random(256), b = random(256);
	r += red;
	g += green;
	b += blue;
	if (r > 255) r = 0;	if (r < 0) r = 255;
	if (g > 255) g = 0;	if (g < 0) g = 255;
	if (b > 255) b = 0;	if (b < 0) b = 255;
	setColor(r, g, b);
}

void flicker() {	// функция мерцания
	static float r = random(256), g = random(256), b = random(256);
	static char vr = 1, vg = 1, vb = 1;

	r += vr * pow(long(analogRead(RED_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000;
	g += vg * pow(long(analogRead(GREEN_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000;
	b += vb * pow(long(analogRead(BLUE_POT_PIN)) / 1023.0, EXPONENT) * MAX_SPEED * TIME_PARTITION / 1000;

	if (r >= 255 || r <= 0) { vr *= -1;	r >= 255 ? r = 255 : r = 0;	}
	if (g >= 255 || g <= 0) { vg *= -1;	g >= 255 ? g = 255 : g = 0; }
	if (b >= 255 || b <= 0) { vb *= -1;	b >= 255 ? b = 255 : b = 0; }
	
	setColor(r, g, b);
}
