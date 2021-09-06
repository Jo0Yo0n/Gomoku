//심화프로그래밍 기말 프로젝트
//2019111994 이주윤
//Gomoku.h file
//선언부 파일

#define BOARD_SIZE 19
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

class Game_Board{
	int gameBoard[BOARD_SIZE][BOARD_SIZE] = { 0, };	//바둑판 선언 및 초기화
	int weight[BOARD_SIZE][BOARD_SIZE] = { 0, }; //각 위치에 대한 가중치
	int win; //1이면 사용자 승리, -1이면 ai 승리
	void setWeight(int r, int c); //착수한 점 주위의 가중치 설정
	bool check_closed_4(); //닫힌 4목 확인 후 업데이트
	bool check_blanked_4(); //띈 4목(case 1)확인 후 업데이트
	bool check_opened_3(); //열린 3목 + 띈 4목(case 2)확인 후 업데이트
	bool check_blanked_3(); //띈 3목 확인 후 업데이트
	bool checkRange(int row, int col); //착수 위치의 범위 유효성 확인
public:
	int get_win();
	Game_Board(); //기본 생성자, 바둑판 객체가 생성됐다는 문장 출력
	void printBoard();	//바둑판 출력
	void inputUser(); //사용자의 입력을 받아 한 점에 착수
	void AiOperation(); //AI가 사용자의 입력에 따라 착수
	bool check_win(); //승리조건 확인
};