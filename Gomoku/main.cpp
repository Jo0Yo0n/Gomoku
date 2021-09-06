//심화프로그래밍 기말 프로젝트
//2019111994 이주윤
//main.cpp file

#include "Gomoku.h"
using namespace std;

int main() {
	srand((unsigned int)time(NULL)); //rand() 함수를 위한 seed 설정

	//시작 화면
	char start;
	cout << "오목게임" << endl;
	cout << "Press 's' to start" << endl;
	cout << "Press 'q' to quit" << endl << ">> ";
	cin >> start;
	//사용자가 s를 입력할 때까지 반복한다. 만약 q를 입력하면 프로그램을 종료한다.
	while (start != 's') {
		cin.clear();
		cin.ignore();
		cout << ">> ";
		cin >> start;
		if (start == 'q') exit(1);
	}
	
	//Game_Board 객체 생성
	Game_Board game;

	//승리 조건이 참이 될 때까지 바둑판 출력 -> 사용자 입력 -> AI 확인 과정을 반복한다
	while (!game.check_win()) {
		game.printBoard();
		game.inputUser();
		game.AiOperation();

	}
	//마지막으로 바둑판 출력
	game.printBoard();

	//win에 저장된 값이 1이면 사용자 승리, -1이면 패배 문장을 출력한다
	if (game.get_win() == 1) {
		cout << "*********You WIN!*********" << endl;
	}
	else if (game.get_win() == -1) {
		cout << "*********You LOSE*********" << endl;
	}
}