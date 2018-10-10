
#include <time.h>

#include <iostream>

using namespace std;

class BingoGame {
private:
	int *map, *aiMap;		// 게임 맵, ai 게임 맵
	int size = 2;			// 가로 or 세로 크기
	int bingoCount = 0;		// 빙고 횟수
	int aiBingoCount = 0;	// ai 빙고 횟수
	int win = 0;			// 빙고 이기는 횟수
	int level = 1;			// ai 난이도
	bool runFlag = false;	// 게임 실행 여부

	// 맵 사이즈 설정
	void setSize(int size) {
		if (size < 2) {
			return;
		}
		this->size = size;
	}

	// 맵 메모리 할당
	bool newMap(int size) {
		map = new int[size*size];
		if (map == nullptr) {
			return false;
		}

		aiMap = new int[size*size];
		if (aiMap == nullptr) {
			delete map;
			return false;
		}
		return true;
	}

	// ai 빙고 횟수 설정
	void setAIBingoCount(int bingo) {
		this->aiBingoCount = bingo;
	}

	// 빙고 횟수 설정
	void setBingoCount(int bingo) {
		this->bingoCount = bingo;
	}

	// 실행 여부 설정
	void setRun(bool runFlag) {
		this->runFlag = runFlag;
	}

	// 빙고 승리 횟수 설정
	void setWin(int size) {
		// 해당 빙고의 70%
		this->win = (((size * 2 + 2) * 7) / 10);
	}

	// EASY, HARD 여부 설정
	int setLevel(int level) {
		if (level == 1) {
			this->level = level;
		}
		else {
			this->level = 2;
		}
	}

	// 맵 사이즈 반환
	int getSize() {
		return this->size;
	}

	// 맵 데이터 반환
	int *getMap() {
		return this->map;
	}

	// ai 맵 데이터 반환
	int *getAIMap() {
		return this->aiMap;
	}

	// 빙고 횟수 반환
	int getBingoCount() {
		return this->bingoCount;
	}

	// 빙고 횟수 반환
	int getAIBingoCount() {
		return this->aiBingoCount;
	}

	// EASY, HARD 여부 반환
	int getLevel() {
		return this->level;
	}

	// 실행 여부 반환
	bool getRun() {
		return this->runFlag;
	}

	// 빙고 승리 횟수 반환
	int getWin() {
		return this->win;
	}

	// 맵 초기화
	void initMap(int *map, int *aiMap, int size) {
		int mapSize = size * size;

		// 빙고맵 설정
		for (int i = 0; i < mapSize; i++) {
			aiMap[i] = map[i] = i + 1;
		}

		// 빙고맵 섞기
		int tmp1, tmp2;
		for (int i = 0; i < 10000; i++) {
			tmp1 = rand() % mapSize;
			tmp2 = rand() % mapSize;
			SWAP(map[tmp1], map[tmp2]);
		}

		// ai빙고맵 섞기
		for (int i = 0; i < 10000; i++) {
			tmp1 = rand() % mapSize;
			tmp2 = rand() % mapSize;
			SWAP(aiMap[tmp1], aiMap[tmp2]);
		}
	}

	// 맵 출력
	void printMap(int *map, int size) {
		int mapSize = size * size;

		for (int i = 0; i < mapSize; i++) {
			map[i] == INT_MAX ? 
				cout << "*\t" : cout << map[i] << "\t";
			if (((i + 1) % size) == 0) {
				cout << endl;
			}
		}
	}

	// 빙고 출력
	void printBingo() {
		// 70% 이상 빙고하면 승리
		int win = getWin();
		cout << "Bingo: " << getBingoCount() << " / " << win << endl;
	}

	// ai 빙고 출력
	void printAIBingo() {
		// 70% 이상 빙고하면 승리
		int win = getWin();
		cout << "AI Bingo: " << getAIBingoCount() << " / " << win << endl;
	}

	// 설명 출력
	void printHelp() {
		cout << "숫자를 입력하세요(0 : 종료) ";
	}

	// 게임 출력 담당
	void printGame() {
		int *map = getMap();
		int *aiMap = getAIMap();
		int size = getSize();

		// 내 빙고 맵 출력
		printMap(map, size);
		// 내 빙고 출력
		printBingo();
		cout << endl;

		// AI 빙고 맵 출력
		printMap(aiMap, size);
		// AI 빙고 출력
		printAIBingo();
		// 설명 출력
		printHelp();
	}

	// 사용자 빙고 입력
	int inputGame() {
		int input;			// 입력한 수
		cin >> input;

		return input;
	}

	// AI 빙고 입력
	int inputAIGame() {
		int size = getSize();
		int mapSize = size * size;
		int num;

		// 임의의 수 입력
		if (getLevel() == 1) {
			// EASY
			while (true) {
				num = rand() % mapSize;
				if (aiMap[num] != INT_MAX) {
					break;
				}
			}
		}
		else {
			// HARD
		}

		return num;
	}

	// 빙고 횟수 처리
	void bingoProcess(bool isAI, int *map, int size, int posX, int posY) {
		int vertical = 0;	// 세로 빙고 정보
		int horizontal = 0;	// 가로 빙고 정보
		int diagonal = 0;	// 대각선 빙고 정보
		int bingo = 0;		// 빙고 횟수
		int i;

		// 빙고가 얼마나 됬는가?
		if (posX == posY) {
			// 대각선
			// [00] 10 [20]
			//  01 [11] 21
			// [02] 12 [22]

			// [00] 10  20 [30]
			//  01 [11][21] 31
			//  02 [12][22] 32
			// [03] 13  23 [33]

			// [00] 10  20  30 [40]
			//  01 [11] 21 [31] 41
			//  02  12 [22] 32  42
			//  03 [13] 23 [33] 43
			// [04] 14  24  34 [44]

			for (i = 0; i < size; i++) {
				// 대각선 체크
				if (map[i*size + i] == INT_MAX) {
					diagonal++;
				}
				// 가로 체크
				if (map[posY*size + i] == INT_MAX) {
					horizontal++;
				}
				// 세로 체크
				if (map[i*size + posX] == INT_MAX) {
					vertical++;
				}
			}

			// 실질적인 빙고 갯수
			if (diagonal == size) {
				bingo++;
			}
			if (horizontal == size) {
				bingo++;
			}
			if (vertical == size) {
				bingo++;
			}

			if ((size / 2) == posX) {
				// 홀수면서 정 가운데 수는 역대각선도 생각해야함
				// 짝수는 상관 없음

				// 3*3
				// (Y좌표)	 + X좌표
				// (2-0) * 3 + 0	(0, 2)
				// (2-1) * 3 + 1	(1, 1)
				// (2-2) * 3 + 2	(2, 0)
				for (i = 0, diagonal = 0; i < size; i++) {
					if (map[((size - 1) - i) * size + i] == INT_MAX) {
						diagonal++;
					}
				}

				// 실질적인 빙고 갯수
				if (diagonal == size) {
					bingo++;
				}
			}
		}
		else if ((posX + posY) == (size - 1)) {
			// 역대각선

			for (i = 0; i < size; i++) {
				// 역대각선 체크
				if (map[((size - 1) - i) * size + i] == INT_MAX) {
					diagonal++;
				}
				// 가로 체크
				if (map[posY*size + i] == INT_MAX) {
					horizontal++;
				}
				// 세로 체크
				if (map[i*size + posX] == INT_MAX) {
					vertical++;
				}
			}

			// 실질적인 빙고 갯수
			if (diagonal == size) {
				bingo++;
			}
			if (horizontal == size) {
				bingo++;
			}
			if (vertical == size) {
				bingo++;
			}
		}
		else {
			// 가로, 세로

			for (i = 0; i < size; i++) {
				// 가로 체크
				if (map[posY*size + i] == INT_MAX) {
					horizontal++;
				}
				// 세로 체크
				if (map[i*size + posX] == INT_MAX) {
					vertical++;
				}
			}

			// 실질적인 빙고 갯수
			if (horizontal == size) {
				bingo++;
			}
			if (vertical == size) {
				bingo++;
			}
		}

		// 빙고 횟수 저장
		if (isAI) {
			setAIBingoCount(getAIBingoCount() + bingo);
		}
		else {
			setBingoCount(getBingoCount() + bingo);
		}
	}

	// 게임 로직 처리
	void updateGame(int num) {

		int *map = getMap();
		int *aiMap = getAIMap();
		int size = getSize();
		int mapSize = size * size;

		int posX=-1, posY=-1;		// 현재 좌표

		// 입력한 수가 나한테 존재 하는지?
		for (int i = 0; i < mapSize; i++) {
			if (map[i] == num) {
				// 빙고 처리
				map[i] = INT_MAX;

				// 현재 좌표 가져오기
				posX = i % size;
				posY = i / size;
				break;
			}
		}

		// 빙고 횟수 알 필요 있음
		if (posX != -1) {
			bingoProcess(false, map, size, posX, posY);
		}

		int aiPosX = -1, aiPosY = -1;	// 현재 ai 좌표

		// 입력한 수가 ai한테 존재 하는지?
		for (int i = 0; i < mapSize; i++) {
			if (aiMap[i] == num) {
				// 빙고 처리
				aiMap[i] = INT_MAX;

				// 현재 좌표 가져오기
				aiPosX = i % size;
				aiPosY = i / size;
				break;
			}
		}

		// 빙고 횟수 알 필요 있음
		if (aiPosX != -1) {
			bingoProcess(true, aiMap, size, aiPosX, aiPosY);
		}

		// 70% 이상 빙고를 하면 이김
		int win = getWin();
		if (win <= getBingoCount()) {
			// 게임 끄기
			setRun(false);
		} else if (win <= getAIBingoCount()) {
			// 게임 끄기
			setRun(false);
		}
	}

public:
	void SWAP(int &a, int &b) {
		int tmp = a;
		a = b;
		b = tmp;
	}

	BingoGame(int size) {
		// 맵 사이즈 설정
		setSize(size);
		setWin(size);

		// 맵 생성
		if (newMap(size) == false) {
			// 메모리 할당 에러
			exit(1);
		}

		// 맵 가져와서 초기화
		int *map = getMap();
		int *aiMap = getAIMap();
		initMap(map, aiMap, size);

		// 게임 실행
		setRun(true);
	}

	~BingoGame() {
		delete map;
	}

	void playGame() {
		int num;

		while (getRun()) {
			printGame();
			num = inputGame();
			updateGame(num);
			num = inputAIGame();
			updateGame(num);
			system("cls");
		}

		int win = getWin();
		int user = getBingoCount();
		int ai = getAIBingoCount();
		if (win <= user && win <= ai) {
			cout << "Draw." << endl;
		}
		else if (win <= user) {
			cout << "Congratulation! You Win." << endl;
		}
		else {
			if (rand() % 2) {
				cout << "That's such a bummer! You Lose." << endl;
			}
			else {
				cout << "That's a shame! You Lose." << endl;
			}
		}
	}
};

int main(void) {
	srand((unsigned int)time(NULL));
	BingoGame game = BingoGame(5);
	game.playGame();
	return 0;
}