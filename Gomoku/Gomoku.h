//��ȭ���α׷��� �⸻ ������Ʈ
//2019111994 ������
//Gomoku.h file
//����� ����

#define BOARD_SIZE 19
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

class Game_Board{
	int gameBoard[BOARD_SIZE][BOARD_SIZE] = { 0, };	//�ٵ��� ���� �� �ʱ�ȭ
	int weight[BOARD_SIZE][BOARD_SIZE] = { 0, }; //�� ��ġ�� ���� ����ġ
	int win; //1�̸� ����� �¸�, -1�̸� ai �¸�
	void setWeight(int r, int c); //������ �� ������ ����ġ ����
	bool check_closed_4(); //���� 4�� Ȯ�� �� ������Ʈ
	bool check_blanked_4(); //�� 4��(case 1)Ȯ�� �� ������Ʈ
	bool check_opened_3(); //���� 3�� + �� 4��(case 2)Ȯ�� �� ������Ʈ
	bool check_blanked_3(); //�� 3�� Ȯ�� �� ������Ʈ
	bool checkRange(int row, int col); //���� ��ġ�� ���� ��ȿ�� Ȯ��
public:
	int get_win();
	Game_Board(); //�⺻ ������, �ٵ��� ��ü�� �����ƴٴ� ���� ���
	void printBoard();	//�ٵ��� ���
	void inputUser(); //������� �Է��� �޾� �� ���� ����
	void AiOperation(); //AI�� ������� �Է¿� ���� ����
	bool check_win(); //�¸����� Ȯ��
};