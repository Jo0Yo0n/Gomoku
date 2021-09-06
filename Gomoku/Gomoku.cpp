//심화프로그래밍 기말 프로젝트
//2019111994 이주윤
//Gomoku.cpp file
//구현부 파일

#include "Gomoku.h"
using namespace std;

/*Game_Board class 구현*/
//바둑판 생성자 실행 시 문장 출력
Game_Board::Game_Board(){
	cout << "바둑판이 생성되었습니다." << endl;
}
//바둑판 출력
void Game_Board::printBoard() {
	//열 정보 출력
	cout << "     ";
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << i << setw(3);
	}
	cout << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << "ㅡㅡ";
	}
	//행 정보 출력 및 격자 무늬 or 바둑돌 출력
	//gameBoard[i][j] 값이 1이면 흑돌, -1 이면 백돌 출력, 0일 때는 격자 무늬 출력
	//gameBoard[][] 배열은 0으로 초기화 되어 있음
	cout << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << setw(3) << i << "│ ";
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] == 0) cout << setw(3) << "+  ";
			else if (gameBoard[i][j] == 1) cout << "○  ";
			else if (gameBoard[i][j] == -1) cout << "●  ";
		}
		cout << endl;
	}
}

//사용자의 입력을 받아 한 점에 착수
void Game_Board::inputUser() {
	int row, col;
	bool flag = false;

	//사용자 입력 유효성 검증
	//사용자가 입력한 값이 바둑판의 범위를 벗어나고, 해당 위치의 가중치가 -1이면
	//제대로 된 값을 입력할 때까지 반복한다.
	//weight[][] 배열은 해당 위치의 가중치를 저장하는데, 만약 돌이 착수되면 -1로 초기화 된다.
	//checkRange()함수는 해당 위치가 바둑판에서 벗어나는지 아닌지를 체크
	do { 
		cout << "착수할 위치를 입력하세요: ";
		cin >> row >> col;
		if (!checkRange(row, col) || (weight[row][col] == -1)) {
			flag = true;
			cin.clear();
			cin.ignore();
		}
		else flag = false;
	} while (flag);

	//사용자가 입력한 위치를 1로 바꾸고, setWeight()함수를 통해 주변 위치의 가중치 설정
	gameBoard[row][col] = 1;
	setWeight(row, col);
}

//AI가 착수할 위치를 찾은 뒤 착수
//닫힌 4목, 띈 4목, 띈 3목, 열린 3목을 모두 한번 씩 확인한다.
//만약 4개의 함수 중 하나가 true를 반환하면 AiOperation()함수를 빠져나온다.
//4개 중 어떠한 함수에서도 확인이 안되면 가중치가 가장 높은 위치에 착수한다.
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
	
	
	//착수 가능한 위치 중 가장 가중치 높은 점에 착수
	//weight[][] 배열 전체를 돌면서 가중치가 가장 높은 위치를 찾는다.
	//만약 가장 큰 값인 max와 weight[i][j]의 값이 같다면 rand() 함수를 통해
	//교체 여부를 결정한다.
	else { 
		int row, col, max = 0;

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (max < weight[i][j]) {
					max = weight[i][j];
					row = i; col = j;
				}
				else if (max == weight[i][j]) { //가중치가 같다면
					if ((rand() % 2) == 1) { //랜덤 값이 1이면 교체
						max = weight[i][j];
						row = i; col = j;
					}
				}
			}
		}
		//AI가 방어할 위치를 -1로 바꾸고, setWeight()함수를 통해 주변 위치의 가중치 설정
		gameBoard[row][col] = -1;
		setWeight(row, col);
		return;
	}
}

//해당 위치 주변 8칸의 가중치 설정
//매개변수로 행, 열 값을 받는다. addW는 매개변수로 들어온 위치가 백돌인지, 흑돌인지 파악해서
//백돌이면 -2로 초기화, 흑돌이면 2로 초기화 된다.
//weight[][]의 값이 -1인지 아닌지로 해당 위치에 돌이 있는지 여부를 알 수 있다.
void Game_Board::setWeight(int r, int c) {
	int addW;
	//흑돌이면 addW에 2 더함, 백돌이면 -2 더함
	if (gameBoard[r][c] == 1) addW = 2;
	else if(gameBoard[r][c] == -1) addW = -2;

	//착수한 위치의 가중치 삭제 (-1로 초기화)
	weight[r][c] = -1;
	
	//해당 위치가 바둑판의 범위를 넘지 않고, 돌이 없을 때 가중치 더함
	if (checkRange(r - 1, c - 1) && weight[r - 1][c - 1] != -1) weight[r - 1][c - 1] += addW;
	if (checkRange(r, c - 1) && weight[r][c - 1] != -1) weight[r][c - 1] += addW;
	if (checkRange(r + 1, c - 1) && weight[r + 1][c - 1] != -1) weight[r + 1][c - 1] += addW;
	if (checkRange(r + 1, c) && weight[r + 1][c] != -1) weight[r + 1][c] += addW;
	if (checkRange(r + 1, c + 1) && weight[r + 1][c + 1] != -1) weight[r + 1][c + 1] += addW;
	if (checkRange(r, c + 1) && weight[r][c + 1] != -1) weight[r][c + 1] += addW;
	if (checkRange(r - 1, c + 1) && weight[r - 1][c + 1] != -1) weight[r - 1][c + 1] += addW;
	if (checkRange(r - 1, c) && weight[r - 1][c] != -1) weight[r - 1][c] += addW;
}


/*check_ 함수 구현 부분: 총 4개 경우. 경우 당 4개의 모양 체크*/
// '│' 모양, '─' 모양, '/' 모양, '\' 모양을 바둑판의 (0, 0) 부터 (BOARD_SIZE, BOARD_SIZE)까지 확인한다.
//만약 4개의 모양 중 하나라도 참이면 AI가 방어할 위치를 -1로 바꾸고, setWeight()함수를 통해 주변 위치의 가중치 설정한 뒤 true 반환
// 아무 모양도 발견 못하면 false를 반환

//닫힌 4목 확인 후 방어
//2019111994 이주윤
bool Game_Board::check_closed_4(){
	// '│' 모양
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//닫힌 4목을 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c] == 1)\
				&& (gameBoard[r + 2][c] == 1) && (gameBoard[r + 3][c] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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
	// '─' 모양
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//닫힌 4목을 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r][c + 1] == 1)\
				&& (gameBoard[r][c + 2] == 1) && (gameBoard[r][c + 3] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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
	// '/' 모양
	for (int r = BOARD_SIZE - 1; r > 2; r--) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//닫힌 4목을 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r - 1][c + 1] == 1)\
				&& (gameBoard[r - 2][c + 2] == 1) && (gameBoard[r - 3][c + 3] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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
	// '\' 모양
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//닫힌 4목을 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c + 1] == 1)\
				&& (gameBoard[r + 2][c + 2] == 1) && (gameBoard[r + 3][c + 3] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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

//띈 4목 case1 확인 후 방어 (case1: 1 1 0 1 1 꼴)
//띈 4목은 착수해야 할 위치가 한 곳 밖에 없고 범위 유효성을 확인하지 않아도 됨
//2019111994 이주윤
bool Game_Board::check_blanked_4() {
	// '│' 모양
	for (int r = 0; r < BOARD_SIZE - 4; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//띈 4목 case1 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c] == 1) && (gameBoard[r + 2][c] == 0)\
				&& (gameBoard[r + 3][c] == 1) && (gameBoard[r + 4][c] == 1)) {
				gameBoard[r + 2][c] = -1;
				setWeight(r + 2, c);
				return true;
			}
		}
	}
	// '─' 모양
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 4; c++) {
			//띈 4목 case1 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r][c + 1] == 1) && (gameBoard[r][c + 2] == 0)\
				&& (gameBoard[r][c + 3] == 1) && (gameBoard[r][c + 4] == 1)) {
				gameBoard[r][c + 2] = -1;
				setWeight(r, c + 2);
				return true;
			}
		}
	}
	// '/' 모양
	for (int r = BOARD_SIZE - 1; r > 3; r--) {
		for (int c = 0; c < BOARD_SIZE - 4; c++) {
			//띈 4목 case1 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r - 1][c + 1] == 1) && (gameBoard[r - 2][c + 2] == 0)\
				&& (gameBoard[r - 3][c + 3] == 1) && (gameBoard[r - 4][c + 4] == 1)) {
				gameBoard[r - 2][c + 2] = -1;
				setWeight(r - 2, c + 2);
				return true;
			}
		}
	}
	// '\' 모양
	for (int r = 0; r < BOARD_SIZE - 4; r++) {
		for (int c = 0; c < BOARD_SIZE - 4; c++) {
			//띈 4목 case1 발견
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

//열린 3목 + 띈 4목 case2 확인 후 방어
//띈 4목은 열린 3목의 확인 과정에서 한 과정만 더하면 확인할 수 있기 때문에
//두 경우를 함께 묶어서 구현함
//만약 열린 3목이 발견되면 3목에서 한 칸 띈 돌이 있는지 확인하고 있으면 그 사이을 막는다.
//2019111994 이주윤
bool Game_Board::check_opened_3() {
	// '│' 모양
	for (int r = 0; r < BOARD_SIZE - 2; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//열린 3목을 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c] == 1)\
				&& (gameBoard[r + 2][c] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//띈 4목 발견 & 방어
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
				//양 쪽으로 돌이 없을 경우
				//열린 3목 방어
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

	// '─' 모양
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 2; c++) {
			//열린 3목 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r][c + 1] == 1)\
				&& (gameBoard[r][c + 2] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//띈 4목 발견 & 방어
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
				//양 쪽으로 돌이 없을 경우
				//열린 3목 방어
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
	// '/' 모양
	for (int r = BOARD_SIZE - 1; r > 1; r--) {
		for (int c = 0; c < BOARD_SIZE - 2; c++) {
			//열린 3목 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r - 1][c + 1] == 1)\
				&& (gameBoard[r - 2][c + 2] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//띈 4목 발견 & 방어
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
				//양 쪽으로 돌이 없을 경우
				//열린 3목 방어
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

	// '\' 모양
	for (int r = 0; r < BOARD_SIZE - 2; r++) {
		for (int c = 0; c < BOARD_SIZE - 2; c++) {
			//열린 3목 발견
			if ((gameBoard[r][c] == 1) && (gameBoard[r + 1][c + 1] == 1)\
				&& (gameBoard[r + 2][c + 2] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//띈 4목 발견 & 방어
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
				//양 쪽으로 돌이 없을 경우
				//열린 3목 방어
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

//띈 3목 확인 후 방어
//띈 3목이 있을 때 가운데 두 위치 중 하나만 존재하는 경우를 확인하기 위해 비트단위 연산자 '^'를 사용
//2019111994 이주윤
bool Game_Board::check_blanked_3() {
	// '│' 모양
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			//띈 3목을 발견
			if ((gameBoard[r][c] == 1) && ((gameBoard[r + 1][c] == 1) ^ (gameBoard[r + 2][c] == 1))\
				&& (gameBoard[r + 3][c] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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
	// '─' 모양
	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//띈 3목을 발견
			if ((gameBoard[r][c] == 1) && ((gameBoard[r][c + 1] == 1) ^ (gameBoard[r][c + 2] == 1))\
				&& (gameBoard[r][c + 3] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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
	// '/' 모양
	for (int r = BOARD_SIZE - 1; r > 2; r--) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//띈 3목을 발견
			if ((gameBoard[r][c] == 1) && ((gameBoard[r - 1][c + 1] == 1) ^ (gameBoard[r - 2][c + 2] == 1))\
				&& (gameBoard[r - 3][c + 3] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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
	// '\' 모양
	for (int r = 0; r < BOARD_SIZE - 3; r++) {
		for (int c = 0; c < BOARD_SIZE - 3; c++) {
			//띈 3목을 발견
			if ((gameBoard[r][c] == 1) && ((gameBoard[r + 1][c + 1] == 1) ^ (gameBoard[r + 2][c + 2] == 1))\
				&& (gameBoard[r + 3][c + 3] == 1)) {
				//두 끝점 중 막을 위치를 결정
				//바둑판의 위치에서 벗어나는지, 돌이 있는지를 확인한 후 착수
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

//승리조건 확인
//4가지 모양을 바둑판 전체를 돌면서 확인한다.
//i = 1, i = -1 일 때로 두 번 확인한다.
//만약 i = 1인 상태에서 승리 조건이 만족하면 win 변수에 1 대입한다
//만약 i = -1인 상태에서 승리 조건이 만족하면 win 변수에 -1 대입한다
//main() 함수에서 win이 1이면 사용자 승리, -1이면 AI 승리로 판단한다
//i가 1이든 -1이든 승리 조건이 확인되면 true 값을 반환한다.
bool Game_Board::check_win() {
	for (int i = 1; i >= -1; i -= 2) {
		// '│' 모양 승리
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

		// '─' 모양 승리
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

		// '/' 모양 승리
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

		// '\' 모양 승리
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

//매개변수로 들어온 위치가 정해진 바둑판의 크기에서 벗어나는 지 여부 확인
//바둑판의 영역에 있으면 true, 벗어나면 false 반환
bool Game_Board::checkRange(int row, int col) {
	if ((row >= 0 && row < BOARD_SIZE) && (col >= 0 && col < BOARD_SIZE))
		return true;
	else return false;
}

int Game_Board::get_win() {
	return win;
}