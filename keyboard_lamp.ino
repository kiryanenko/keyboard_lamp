#define INT_NUM 1		// ���������� � �������� 0 (�� digital pin 2) � 1 (�� digital pin 3)

volatile bool isRand;	// ���������: ��������� ������ ������

void int_set_mode() {	// ���������: ��������� ������ ������
	isRand = !isRand;
}

void setup()
{
	attachInterrupt(INT_NUM, int_set_mode, RISING); // ����������� ���������� � ������� int_set_mode().

}

void loop()
{

  /* add main program code here */

}
