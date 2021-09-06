//��ȭ���α׷��� �⸻ ������Ʈ
//2019111994 ������
//main.cpp file

#include "Gomoku.h"
using namespace std;

int main() {
	srand((unsigned int)time(NULL)); //rand() �Լ��� ���� seed ����

	//���� ȭ��
	char start;
	cout << "�������" << endl;
	cout << "Press 's' to start" << endl;
	cout << "Press 'q' to quit" << endl << ">> ";
	cin >> start;
	//����ڰ� s�� �Է��� ������ �ݺ��Ѵ�. ���� q�� �Է��ϸ� ���α׷��� �����Ѵ�.
	while (start != 's') {
		cin.clear();
		cin.ignore();
		cout << ">> ";
		cin >> start;
		if (start == 'q') exit(1);
	}
	
	//Game_Board ��ü ����
	Game_Board game;

	//�¸� ������ ���� �� ������ �ٵ��� ��� -> ����� �Է� -> AI Ȯ�� ������ �ݺ��Ѵ�
	while (!game.check_win()) {
		game.printBoard();
		game.inputUser();
		game.AiOperation();

	}
	//���������� �ٵ��� ���
	game.printBoard();

	//win�� ����� ���� 1�̸� ����� �¸�, -1�̸� �й� ������ ����Ѵ�
	if (game.get_win() == 1) {
		cout << "*********You WIN!*********" << endl;
	}
	else if (game.get_win() == -1) {
		cout << "*********You LOSE*********" << endl;
	}
}