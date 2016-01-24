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
}
