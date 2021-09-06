//��ȭ���α׷��� �⸻ ������Ʈ
//2019111994 ������
//Gomoku.cpp file
//������ ����

#include "Gomoku.h"
using namespace std;

/*Game_Board class ����*/
//�ٵ��� ������ ���� �� ���� ���
Game_Board::Game_Board(){
	cout << "�ٵ����� �����Ǿ����ϴ�." << endl;
}
//�ٵ��� ���
void Game_Board::printBoard() {
	//�� ���� ���
	cout << "     ";
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << i << setw(3);
	}
	cout << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << "�Ѥ�";
	}
	//�� ���� ��� �� ���� ���� or �ٵϵ� ���
	//gameBoard[i][j] ���� 1�̸� �浹, -1 �̸� �鵹 ���, 0�� ���� ���� ���� ���
	//gameBoard[][] �迭�� 0���� �ʱ�ȭ �Ǿ� ����
	cout << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << setw(3) << i << "�� ";
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] == 0) cout << setw(3) << "+  ";
			else if (gameBoard[i][j] == 1) cout << "��  ";
			else if (gameBoard[i][j] == -1) cout << "��  ";
		}
		cout << endl;
	}
}

//������� �Է��� �޾� �� ���� ����
void Game_Board::inputUser() {
	int row, col;
	bool flag = false;

	//����� �Է� ��ȿ�� ����
	//����ڰ� �Է��� ���� �ٵ����� ������ �����, �ش� ��ġ�� ����ġ�� -1�̸�
	//����� �� ���� �Է��� ������ �ݺ��Ѵ�.
	//weight[][] �迭�� �ش� ��ġ�� ����ġ�� �����ϴµ�, ���� ���� �����Ǹ� -1�� �ʱ�ȭ �ȴ�.
	//checkRange()�Լ��� �ش� ��ġ�� �ٵ��ǿ��� ������� �ƴ����� üũ
	do { 
		cout << "������ ��ġ�� �Է��ϼ���: ";
		cin >> row >> col;
		if (!checkRange(row, col) || (weight[row][col] == -1)) {
			flag = true;
			cin.clear();
			cin.ignore();
		}
		else flag = false;
	} while (flag);

	//����ڰ� �Է��� ��ġ�� 1�� �ٲٰ�, setWeight()�Լ��� ���� �ֺ� ��ġ�� ����ġ ����
	gameBoard[row][col] = 1;
	setWeight(row, col);
}

//AI�� ������ ��ġ�� ã�� �� ����
//���� 4��, �� 4��, �� 3��, ���� 3���� ��� �ѹ� �� Ȯ���Ѵ�.
//���� 4���� �Լ� �� �ϳ��� true�� ��ȯ�ϸ� AiOperation()�Լ��� �������´�.
//4�� �� ��� �Լ������� Ȯ���� �ȵǸ� ����ġ�� ���� ���� ��ġ�� �����Ѵ�.
void Game_Board::AiOperation() {
	if (check_closed_4()) {
		return;
	}
	else if(check_blanked_4()) {
		return;
	}
	else if (check_opened_3()) {
		return;
	}
	else if (check_blanked_3()) {
		return;
	}
	
	
	//���� ������ ��ġ �� ���� ����ġ ���� ���� ����
	//weight[][] �迭 ��ü�� ���鼭 ����ġ�� ���� ���� ��ġ�� ã�´�.
	//���� ���� ū ���� max�� weight[i][j]�� ���� ���ٸ� rand() �Լ��� ����
	//��ü ���θ� �����Ѵ�.
	else { 
		int row, col, max = 0;

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (max < weight[i][j]) {
					max = weight[i][j];
					row = i; col = j;
				}
				else if (max == weight[i][j]) { //����ġ�� ���ٸ�
					if ((rand() % 2) == 1) { //���� ���� 1�̸� ��ü
						max = weight[i][j];
						row = i; col = j;
					}
				}
			}
		}
		//AI�� ����� ��ġ�� -1�� �ٲٰ�, setWeight()�Լ��� ���� �ֺ� ��ġ�� ����ġ ����
		gameBoard[row][col] = -1;
		setWeight(row, col);
		return;
	}
}

//�ش� ��ġ �ֺ� 8ĭ�� ����ġ ����
//�Ű������� ��, �� ���� �޴´�. addW�� �Ű������� ���� ��ġ�� �鵹����, �浹���� �ľ��ؼ�
//�鵹�̸� -2�� �ʱ�ȭ, �浹�̸� 2�� �ʱ�ȭ �ȴ�.
//weight[][]�� ���� -1���� �ƴ����� �ش� ��ġ�� ���� �ִ��� ���θ� �� �� �ִ�.
void Game_Board::setWeight(int r, int c) {
	int addW;
	//�浹�̸� addW�� 2 ����, �鵹�̸� -2 ����
	if (gameBoard[r][c] == 1) addW = 2;
	else if(gameBoard[r][c] == -1) addW = -2;

	//������ ��ġ�� ����ġ ���� (-1�� �ʱ�ȭ)
	weight[r][c] = -1;
	
	//�ش� ��ġ�� �ٵ����� ������ ���� �ʰ�, ���� ���� �� ����ġ ����
	if (checkRange(r - 1, c - 1) && weight[r - 1][c - 1] != -1) weight[r - 1][c - 1] += addW;
	if (checkRange(r, c - 1) && weight[r][c - 1] != -1) weight[r][c - 1] += addW;
	if (checkRange(r + 1, c - 1) && weight[r + 1][c - 1] != -1) weight[r + 1][c - 1] += addW;
	if (checkRange(r + 1, c) && weight[r + 1][c] != -1) weight[r + 1][c] += addW;
	if (checkRange(r + 1, c + 1) && weight[r + 1][c + 1] != -1) weight[r + 1][c + 1] += addW;
	if (checkRange(r, c + 1) && weight[r][c + 1] != -1) weight[r][c + 1] += addW;
	if (checkRange(r - 1, c + 1) && weight[r - 1][c + 1] != -1) weight[r - 1][c + 1] += addW;
	if (checkRange(r - 1, c) && weight[r - 1][c] != -1) weight[r - 1][c] += addW;
}


/*check_ �Լ� ���� �κ�: �� 4�� ���. ��� �� 4���� ��� üũ*/
// '��' ���, '��' ���, '/' ���, '\' ����� �ٵ����� (0, 0) ���� (BOARD_SIZE, BOARD_SIZE)���� Ȯ���Ѵ�.
//���� 4���� ��� �� �ϳ��� ���̸� AI�� ����� ��ġ�� -1�� �ٲٰ�, setWeight()�Լ��� ���� �ֺ� ��ġ�� ����ġ ������ �� true ��ȯ
// �ƹ� ��絵 �߰� ���ϸ� false�� ��ȯ

//���� 4�� Ȯ�� �� ���
//2019111994 ������
bool Game_Board::check_closed_4(){
	// '��' ���
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//���� 4���� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c] == 1)\
				&& (gameBoard[r + 2][c] == 1) && (gameBoard[r + 3][c] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r - 1, c) && weight[r - 1][c] != -1) {
					gameBoard[r - 1][c] = -1;
					setWeight(r - 1, c);
					return true;
				}
				else if (checkRange(r + 4, c) && weight[r + 4][c] != -1) {
					gameBoard[r + 4][c] = -1;
					setWeight(r + 4, c);
					return true;
				}
			}
		}
	}
	// '��' ���
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//���� 4���� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r][c + 1] == 1)\
				&& (gameBoard[r][c + 2] == 1) && (gameBoard[r][c + 3] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r, c - 1) && weight[r][c - 1] != -1) {
					gameBoard[r][c - 1] = -1;
					setWeight(r, c - 1);
					return true;
				}
				else if (checkRange(r, c + 4) && weight[r][c + 4] != -1) {
					gameBoard[r][c + 4] = -1;
					setWeight(r, c + 4);
					return true;
				}	
			}
		}
	}
	// '/' ���
	for (int r = BOARD_SIZE - 1; r > 2; r--) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//���� 4���� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r - 1][c + 1] == 1)\
				&& (gameBoard[r - 2][c + 2] == 1) && (gameBoard[r - 3][c + 3] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r + 1, c - 1) && weight[r + 1][c - 1] != -1) {
					gameBoard[r + 1][c - 1] = -1;
					setWeight(r + 1, c - 1);
					return true;
				}
				else if (checkRange(r - 4, c + 4) && weight[r - 4][c + 4] != -1) {
					gameBoard[r - 4][c + 4] = -1;
					setWeight(r - 4, c + 4);
					return true;
				}
			}
		}
	}
	// '\' ���
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//���� 4���� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c + 1] == 1)\
				&& (gameBoard[r + 2][c + 2] == 1) && (gameBoard[r + 3][c + 3] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r - 1, c - 1) && weight[r - 1][c - 1] != -1) {
					gameBoard[r - 1][c - 1] = -1;
					setWeight(r - 1, c - 1);
					return true;
				}
				else if (checkRange(r + 4, c + 4) && weight[r + 4][c + 4] != -1) {
					gameBoard[r + 4][c + 4] = -1;
					setWeight(r + 4, c + 4);
					return true;
				}
			}
		}
	}
	return false;
}

//�� 4�� case1 Ȯ�� �� ��� (case1: 1 1 0 1 1 ��)
//�� 4���� �����ؾ� �� ��ġ�� �� �� �ۿ� ���� ���� ��ȿ���� Ȯ������ �ʾƵ� ��
//2019111994 ������
bool Game_Board::check_blanked_4() {
	// '��' ���
	for (int r = 0; r < BOARD_SIZE - 4; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//�� 4�� case1 �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c] == 1) && (gameBoard[r + 2][c] == 0)\
				&& (gameBoard[r + 3][c] == 1) && (gameBoard[r + 4][c] == 1)) {
				gameBoard[r + 2][c] = -1;
				setWeight(r + 2, c);
				return true;
			}
		}
	}
	// '��' ���
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 4; c++) {
			//�� 4�� case1 �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r][c + 1] == 1) && (gameBoard[r][c + 2] == 0)\
				&& (gameBoard[r][c + 3] == 1) && (gameBoard[r][c + 4] == 1)) {
				gameBoard[r][c + 2] = -1;
				setWeight(r, c + 2);
				return true;
			}
		}
	}
	// '/' ���
	for (int r = BOARD_SIZE - 1; r > 3; r--) {
		for (int c = 0; c < BOARD_SIZE - 4; c++) {
			//�� 4�� case1 �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r - 1][c + 1] == 1) && (gameBoard[r - 2][c + 2] == 0)\
				&& (gameBoard[r - 3][c + 3] == 1) && (gameBoard[r - 4][c + 4] == 1)) {
				gameBoard[r - 2][c + 2] = -1;
				setWeight(r - 2, c + 2);
				return true;
			}
		}
	}
	// '\' ���
	for (int r = 0; r < BOARD_SIZE - 4; r++) {
		for (int c = 0; c < BOARD_SIZE - 4; c++) {
			//�� 4�� case1 �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c + 1] == 1) && (gameBoard[r + 2][c + 2] == 0)\
				&& (gameBoard[r + 3][c + 3] == 1) && (gameBoard[r + 4][c + 4] == 1)) {
				gameBoard[r + 2][c + 2] = -1;
				setWeight(r + 2, c + 2);
				return true;
			}
		}
	}
	return false;
}

//���� 3�� + �� 4�� case2 Ȯ�� �� ���
//�� 4���� ���� 3���� Ȯ�� �������� �� ������ ���ϸ� Ȯ���� �� �ֱ� ������
//�� ��츦 �Բ� ��� ������
//���� ���� 3���� �߰ߵǸ� 3�񿡼� �� ĭ �� ���� �ִ��� Ȯ���ϰ� ������ �� ������ ���´�.
//2019111994 ������
bool Game_Board::check_opened_3() {
	// '��' ���
	for (int r = 0; r < BOARD_SIZE - 2; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//���� 3���� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c] == 1)\
				&& (gameBoard[r + 2][c] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�� 4�� �߰� & ���
				if (checkRange(r - 2, c) && weight[r - 2][c] == -1) {
					if (checkRange(r - 1, c) && weight[r - 1][c] != -1) {
						gameBoard[r - 1][c] = -1;
						setWeight(r - 1, c);
						return true;
					}
				}
				else if (checkRange(r + 4, c) && weight[r + 4][c] == -1) {
					if (checkRange(r + 3, c) && weight[r + 3][c] != -1) {
						gameBoard[r + 3][c] = -1;
						setWeight(r + 3, c);
						return true;
					}
				}
				//�� ������ ���� ���� ���
				//���� 3�� ���
				else if (checkRange(r - 1, c) && weight[r - 1][c] != -1) {
					gameBoard[r - 1][c] = -1;
					setWeight(r - 1, c);
					return true;
				}
				else if (checkRange(r + 3, c) && weight[r + 3][c] != -1) {
					gameBoard[r + 3][c] = -1;
					setWeight(r + 3, c);
					return true;
				}
			}
		}
	}

	// '��' ���
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 2; c++) {
			//���� 3�� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r][c + 1] == 1)\
				&& (gameBoard[r][c + 2] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�� 4�� �߰� & ���
				if (checkRange(r, c - 2) && weight[r][c - 2] == -1) {
					if (checkRange(r, c - 1) && weight[r][c - 1] != -1) {
						gameBoard[r][c - 1] = -1;
						setWeight(r, c - 1);
						return true;
					}
				}
				else if (checkRange(r, c + 4) && weight[r][c + 4] == -1) {
					if (checkRange(r, c + 3) && weight[r][c + 3] != -1) {
						gameBoard[r][c + 3] = -1;
						setWeight(r, c + 3);
						return true;
					}
				}
				//�� ������ ���� ���� ���
				//���� 3�� ���
				else if (checkRange(r, c - 1) && weight[r][c - 1] != -1) {
					gameBoard[r][c - 1] = -1;
					setWeight(r, c - 1);
					return true;
				}
				else if (checkRange(r, c + 3) && weight[r][c + 3] != -1) {
					gameBoard[r][c + 3] = -1;
					setWeight(r, c + 3);
					return true;
				}
			}
		}
	}
	// '/' ���
	for (int r = BOARD_SIZE - 1; r > 1; r--) {
		for (int c = 0; c < BOARD_SIZE - 2; c++) {
			//���� 3�� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r - 1][c + 1] == 1)\
				&& (gameBoard[r - 2][c + 2] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�� 4�� �߰� & ���
				if (checkRange(r + 2, c - 2) && weight[r + 2][c - 2] == -1) {
					if (checkRange(r + 1, c - 1) && weight[r + 1][c - 1] != -1) {
						gameBoard[r + 1][c - 1] = -1;
						setWeight(r + 1, c - 1);
						return true;
					}
				}
				else if (checkRange(r - 4, c + 4) && weight[r - 4][c + 4] == -1) {
					if (checkRange(r - 3, c + 3) && weight[r - 3][c + 3] != -1) {
						gameBoard[r - 3][c + 3] = -1;
						setWeight(r - 3, c + 3);
						return true;
					}
				}
				//�� ������ ���� ���� ���
				//���� 3�� ���
				else if (checkRange(r + 1, c - 1) && weight[r + 1][c - 1] != -1) {
					gameBoard[r + 1][c - 1] = -1;
					setWeight(r + 1, c - 1);
					return true;
				}
				else if (checkRange(r - 3, c + 3) && weight[r - 3][c + 3] != -1) {
					gameBoard[r - 3][c + 3] = -1;
					setWeight(r - 3, c + 3);
					return true;
				}
			}
		}
	}

	// '\' ���
	for (int r = 0; r < BOARD_SIZE - 2; r++) {
		for (int c = 0; c < BOARD_SIZE - 2; c++) {
			//���� 3�� �߰�
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c + 1] == 1)\
				&& (gameBoard[r + 2][c + 2] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�� 4�� �߰� & ���
				if (checkRange(r - 2, c - 2) && weight[r - 2][c - 2] == -1) {
					if (checkRange(r - 1, c - 1) && weight[r - 1][c - 1] != -1) {
						gameBoard[r - 1][c - 1] = -1;
						setWeight(r - 1, c - 1);
						return true;
					}
				}
				else if (checkRange(r + 4, c + 4) && weight[r + 4][c + 4] == -1) {
					if (checkRange(r + 3, c + 3) && weight[r + 3][c + 3] != -1) {
						gameBoard[r + 3][c + 3] = -1;
						setWeight(r + 3, c + 3);
						return true;
					}
				}
				//�� ������ ���� ���� ���
				//���� 3�� ���
				else if (checkRange(r - 1, c - 1) && weight[r - 1][c - 1] != -1) {
					gameBoard[r - 1][c - 1] = -1;
					setWeight(r - 1, c - 1);
					return true;
				}
				else if (checkRange(r + 3, c + 3) && weight[r + 3][c + 3] != -1) {
					gameBoard[r + 3][c + 3] = -1;
					setWeight(r + 3, c + 3);
					return true;
				}
			}
		}
	}
	return false;

}

//�� 3�� Ȯ�� �� ���
//�� 3���� ���� �� ��� �� ��ġ �� �ϳ��� �����ϴ� ��츦 Ȯ���ϱ� ���� ��Ʈ���� ������ '^'�� ���
//2019111994 ������
bool Game_Board::check_blanked_3() {
	// '��' ���
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//�� 3���� �߰�
			if ((gameBoard[r][c] == 1) && ((gameBoard[r + 1][c] == 1) ^ (gameBoard[r + 2][c] == 1))\
				&& (gameBoard[r + 3][c] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r - 1, c) && weight[r - 1][c] != -1) {
					gameBoard[r - 1][c] = -1;
					setWeight(r - 1, c);
					return true;
				}
				else if (checkRange(r + 4, c) && weight[r + 4][c] != -1) {
					gameBoard[r + 4][c] = -1;
					setWeight(r + 4, c);
					return true;
				}
			}
		}
	}
	// '��' ���
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//�� 3���� �߰�
			if ((gameBoard[r][c] == 1) && ((gameBoard[r][c + 1] == 1) ^ (gameBoard[r][c + 2] == 1))\
				&& (gameBoard[r][c + 3] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r, c - 1) && weight[r][c - 1] != -1) {
					gameBoard[r][c - 1] = -1;
					setWeight(r, c - 1);
					return true;
				}
				else if (checkRange(r, c + 4) && weight[r][c + 4] != -1) {
					gameBoard[r][c + 4] = -1;
					setWeight(r, c + 4);
					return true;
				}
			}
		}
	}
	// '/' ���
	for (int r = BOARD_SIZE - 1; r > 2; r--) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//�� 3���� �߰�
			if ((gameBoard[r][c] == 1) && ((gameBoard[r - 1][c + 1] == 1) ^ (gameBoard[r - 2][c + 2] == 1))\
				&& (gameBoard[r - 3][c + 3] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r + 1, c - 1) && weight[r + 1][c - 1] != -1) {
					gameBoard[r + 1][c - 1] = -1;
					setWeight(r + 1, c - 1);
					return true;
				}
				else if (checkRange(r - 4, c + 4) && weight[r - 4][c + 4] != -1) {
					gameBoard[r - 4][c + 4] = -1;
					setWeight(r - 4, c + 4);
					return true;
				}
			}
		}
	}
	// '\' ���
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//�� 3���� �߰�
			if ((gameBoard[r][c] == 1) && ((gameBoard[r + 1][c + 1] == 1) ^ (gameBoard[r + 2][c + 2] == 1))\
				&& (gameBoard[r + 3][c + 3] == 1)) {
				//�� ���� �� ���� ��ġ�� ����
				//�ٵ����� ��ġ���� �������, ���� �ִ����� Ȯ���� �� ����
				if (checkRange(r - 1, c - 1) && weight[r - 1][c - 1] != -1) {
					gameBoard[r - 1][c - 1] = -1;
					setWeight(r - 1, c - 1);
					return true;
				}
				else if (checkRange(r + 4, c + 4) && weight[r + 4][c + 4] != -1) {
					gameBoard[r + 4][c + 4] = -1;
					setWeight(r + 4, c + 4);
					return true;
				}
			}
		}
	}
	return false;
}

//�¸����� Ȯ��
//4���� ����� �ٵ��� ��ü�� ���鼭 Ȯ���Ѵ�.
//i = 1, i = -1 �� ���� �� �� Ȯ���Ѵ�.
//���� i = 1�� ���¿��� �¸� ������ �����ϸ� win ������ 1 �����Ѵ�
//���� i = -1�� ���¿��� �¸� ������ �����ϸ� win ������ -1 �����Ѵ�
//main() �Լ����� win�� 1�̸� ����� �¸�, -1�̸� AI �¸��� �Ǵ��Ѵ�
//i�� 1�̵� -1�̵� �¸� ������ Ȯ�εǸ� true ���� ��ȯ�Ѵ�.
bool Game_Board::check_win() {
	for (int i = 1; i >= -1; i -= 2) {
		// '��' ��� �¸�
		for (int r = 0; r < BOARD_SIZE - 4; r++) {
			for (int c = 0; c < BOARD_SIZE; c++) {
				if ((gameBoard[r][c] == i) && (gameBoard[r + 1][c] == i) && (gameBoard[r + 2][c] == i)\
					&& (gameBoard[r + 3][c] == i) && (gameBoard[r + 4][c] == i)) {
					if (i == 1) win = 1;
					else if (i == -1) win = -1;
					return true;
				}
			}
		}

		// '��' ��� �¸�
		for (int r = 0; r < BOARD_SIZE; r++) {
			for (int c = 0; c < BOARD_SIZE - 4; c++) {
				if ((gameBoard[r][c] == i) && (gameBoard[r][c + 1] == i) && (gameBoard[r][c + 2] == i)\
					&& (gameBoard[r][c + 3] == i) && (gameBoard[r][c + 4] == i)) {
					if (i == 1) win = 1;
					else if (i == -1) win = -1;
					return true;
				}
			}
		}

		// '/' ��� �¸�
		for (int r = BOARD_SIZE - 1; r > 3; r--) {
			for (int c = 0; c < BOARD_SIZE - 4; c++) {
				if ((gameBoard[r][c] == i) && (gameBoard[r - 1][c + 1] == i) && (gameBoard[r - 2][c + 1] == 1)\
				&& (gameBoard[r - 3][c + 3] == i) && (gameBoard[r - 4][c + 4] == i)) {
					if (i == 1) win = 1;
					else if (i == -1) win = -1;
					return true;
				}
			}
		}

		// '\' ��� �¸�
		for (int r = 0; r < BOARD_SIZE - 4; r++) {
			for (int c = 0; c < BOARD_SIZE - 4; c++) {
				if ((gameBoard[r][c] == i) && (gameBoard[r + 1][c + 1] == i) && (gameBoard[r + 2][c + 2] == i)\
					&& (gameBoard[r + 3][c + 3] == i) && (gameBoard[r + 4][c + 4] == i)) {
					if (i == 1) win = 1;
					else if (i == -1) win = -1;
					return true;
				}
			}
		}
	}
	return false;
}

//�Ű������� ���� ��ġ�� ������ �ٵ����� ũ�⿡�� ����� �� ���� Ȯ��
//�ٵ����� ������ ������ true, ����� false ��ȯ
bool Game_Board::checkRange(int row, int col) {
	if ((row >= 0 && row < BOARD_SIZE) && (col >= 0 && col < BOARD_SIZE))
		return true;
	else return false;
}

int Game_Board::get_win() {
	return win;
}