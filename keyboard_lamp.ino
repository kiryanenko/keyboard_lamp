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

#define INT_NUM			1	// ���������� � �������� 0 (�� digital pin 2) � 1 (�� digital pin 3)

volatile bool isRand;		// ���������: ��������� ������ ������

void int_set_mode() {		// ���������: ��������� ������ ������
	isRand = !isRand;
}

void setup()
{
	attachInterrupt(INT_NUM, int_set_mode, RISING); // ����������� ���������� � ������� int_set_mode().
}

void loop()
{
	if (isRand) {

	} else {
		unsigned char red   = analogRead(RED_POT_PIN);
		unsigned char blue  = analogRead(BLUE_POT_PIN);
		unsigned char green = analogRead(GREEN_POT_PIN);
		digitalWrite(RED_LED,   red);
		digitalWrite(BLUE_LED,  blue);
		digitalWrite(GREEN_LED, green);
		delay(100);
	}
}
