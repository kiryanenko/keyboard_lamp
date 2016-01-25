#define RED_POT_PIN		A7	// ��� �������� ������
#define GREEN_POT_PIN	A6	// ��� �������� ������
#define BLUE_POT_PIN	A5	// ��� ������ ������

#define RED_LED			6	// ��� ������� �����������
#define GREEN_LED		2	// ��� ������� �����������
#define BLUE_LED		4	// ��� ������� �����������

#define DURATION_PIN	A6	// ��� ������, ���������� �� ������������ ����������� �����
#define SHADING_PIN		A5	// ��� ������, ���������� �� ������������ ��������

#define K_DURATION		300	// ���� ������������ ����������� ����� � ���
#define K_SHADING		30	// ���� ������������ �������� � ���

#define TIME_PARTITION	100	// ��� ������� ������� � ��

#define INT_NUM			1	// ���������� � �������� 0 (�� digital pin 2) � 1 (�� digital pin 3)

long previousMillis = 0;	// ����� ����� ��������� ����� ���������� ��������� ���������

struct Color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} current_color;				// ���������� ����

volatile enum {Static, Rand, Rise, Fall} Mode;		// ����� ������ 

void int_set_mode() {		// ���������: ��������� ������ ������
	switch (Mode) {
	case Static:
		Mode = Rand;
		break;
	case Rand:
		Mode = Rise;
		break;
	case Rise:
		Mode = Fall;
		break;
	case Fall:
		Mode = Static;
		break;
	}
}

void setup()
{
	attachInterrupt(INT_NUM, int_set_mode, RISING); // ����������� ���������� � ������� int_set_mode().
	randomSeed(analogRead(0));
	Mode = Static;
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
	case Rise:
		Mode = Fall;
		break;
	case Fall:
		Mode = Static;
		break;
	}
}

void setColor(unsigned char red, unsigned char green, unsigned char blue) {
	digitalWrite(RED_LED, red);
	digitalWrite(BLUE_LED, blue);
	digitalWrite(GREEN_LED, green);
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
		long time = analogRead(SHADING_PIN) * K_SHADING / TIME_PARTITION;	// ������������ �������� / ��� ������� �������
		// ��������
		float vr = (red - last_red + 0.0) / time;		// �������� ����������
		float vb = (blue - last_blue + 0.0) / time;		// ------------------- * ��� ������� ������� = ���������� ���������� �� ��� ������� �������
		float vg = (green - last_green + 0.0) / time;	//	      �����

		abs(r - red) < abs(vr) ? r = red : r += vr;
		abs(b - blue) < abs(vb) ? b = blue : b += vb;
		abs(g - green) < abs(vg) ? g = green : g += vg;
		setColor(r, g, b);
		delay(TIME_PARTITION);
	}
}