
#include <time.h>

#include <iostream>

using namespace std;

class BingoGame {
private:
	int *map, *aiMap;		// ���� ��, ai ���� ��
	int size = 2;			// ���� or ���� ũ��
	int bingoCount = 0;		// ���� Ƚ��
	int aiBingoCount = 0;	// ai ���� Ƚ��
	int win = 0;			// ���� �̱�� Ƚ��
	int level = 1;			// ai ���̵�
	bool runFlag = false;	// ���� ���� ����

	// �� ������ ����
	void setSize(int size) {
		if (size < 2) {
			return;
		}
		this->size = size;
	}

	// �� �޸� �Ҵ�
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

	// ai ���� Ƚ�� ����
	void setAIBingoCount(int bingo) {
		this->aiBingoCount = bingo;
	}

	// ���� Ƚ�� ����
	void setBingoCount(int bingo) {
		this->bingoCount = bingo;
	}

	// ���� ���� ����
	void setRun(bool runFlag) {
		this->runFlag = runFlag;
	}

	// ���� �¸� Ƚ�� ����
	void setWin(int size) {
		// �ش� ������ 70%
		this->win = (((size * 2 + 2) * 7) / 10);
	}

	// EASY, HARD ���� ����
	int setLevel(int level) {
		if (level == 1) {
			this->level = level;
		}
		else {
			this->level = 2;
		}
	}

	// �� ������ ��ȯ
	int getSize() {
		return this->size;
	}

	// �� ������ ��ȯ
	int *getMap() {
		return this->map;
	}

	// ai �� ������ ��ȯ
	int *getAIMap() {
		return this->aiMap;
	}

	// ���� Ƚ�� ��ȯ
	int getBingoCount() {
		return this->bingoCount;
	}

	// ���� Ƚ�� ��ȯ
	int getAIBingoCount() {
		return this->aiBingoCount;
	}

	// EASY, HARD ���� ��ȯ
	int getLevel() {
		return this->level;
	}

	// ���� ���� ��ȯ
	bool getRun() {
		return this->runFlag;
	}

	// ���� �¸� Ƚ�� ��ȯ
	int getWin() {
		return this->win;
	}

	// �� �ʱ�ȭ
	void initMap(int *map, int *aiMap, int size) {
		int mapSize = size * size;

		// ����� ����
		for (int i = 0; i < mapSize; i++) {
			aiMap[i] = map[i] = i + 1;
		}

		// ����� ����
		int tmp1, tmp2;
		for (int i = 0; i < 10000; i++) {
			tmp1 = rand() % mapSize;
			tmp2 = rand() % mapSize;
			SWAP(map[tmp1], map[tmp2]);
		}

		// ai����� ����
		for (int i = 0; i < 10000; i++) {
			tmp1 = rand() % mapSize;
			tmp2 = rand() % mapSize;
			SWAP(aiMap[tmp1], aiMap[tmp2]);
		}
	}

	// �� ���
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

	// ���� ���
	void printBingo() {
		// 70% �̻� �����ϸ� �¸�
		int win = getWin();
		cout << "Bingo: " << getBingoCount() << " / " << win << endl;
	}

	// ai ���� ���
	void printAIBingo() {
		// 70% �̻� �����ϸ� �¸�
		int win = getWin();
		cout << "AI Bingo: " << getAIBingoCount() << " / " << win << endl;
	}

	// ���� ���
	void printHelp() {
		cout << "���ڸ� �Է��ϼ���(0 : ����) ";
	}

	// ���� ��� ���
	void printGame() {
		int *map = getMap();
		int *aiMap = getAIMap();
		int size = getSize();

		// �� ���� �� ���
		printMap(map, size);
		// �� ���� ���
		printBingo();
		cout << endl;

		// AI ���� �� ���
		printMap(aiMap, size);
		// AI ���� ���
		printAIBingo();
		// ���� ���
		printHelp();
	}

	// ����� ���� �Է�
	int inputGame() {
		int input;			// �Է��� ��
		cin >> input;

		return input;
	}

	// AI ���� �Է�
	int inputAIGame() {
		int size = getSize();
		int mapSize = size * size;
		int num;

		// ������ �� �Է�
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

	// ���� Ƚ�� ó��
	void bingoProcess(bool isAI, int *map, int size, int posX, int posY) {
		int vertical = 0;	// ���� ���� ����
		int horizontal = 0;	// ���� ���� ����
		int diagonal = 0;	// �밢�� ���� ����
		int bingo = 0;		// ���� Ƚ��
		int i;

		// ���� �󸶳� ��°�?
		if (posX == posY) {
			// �밢��
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
				// �밢�� üũ
				if (map[i*size + i] == INT_MAX) {
					diagonal++;
				}
				// ���� üũ
				if (map[posY*size + i] == INT_MAX) {
					horizontal++;
				}
				// ���� üũ
				if (map[i*size + posX] == INT_MAX) {
					vertical++;
				}
			}

			// �������� ���� ����
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
				// Ȧ���鼭 �� ��� ���� ���밢���� �����ؾ���
				// ¦���� ��� ����

				// 3*3
				// (Y��ǥ)	 + X��ǥ
				// (2-0) * 3 + 0	(0, 2)
				// (2-1) * 3 + 1	(1, 1)
				// (2-2) * 3 + 2	(2, 0)
				for (i = 0, diagonal = 0; i < size; i++) {
					if (map[((size - 1) - i) * size + i] == INT_MAX) {
						diagonal++;
					}
				}

				// �������� ���� ����
				if (diagonal == size) {
					bingo++;
				}
			}
		}
		else if ((posX + posY) == (size - 1)) {
			// ���밢��

			for (i = 0; i < size; i++) {
				// ���밢�� üũ
				if (map[((size - 1) - i) * size + i] == INT_MAX) {
					diagonal++;
				}
				// ���� üũ
				if (map[posY*size + i] == INT_MAX) {
					horizontal++;
				}
				// ���� üũ
				if (map[i*size + posX] == INT_MAX) {
					vertical++;
				}
			}

			// �������� ���� ����
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
			// ����, ����

			for (i = 0; i < size; i++) {
				// ���� üũ
				if (map[posY*size + i] == INT_MAX) {
					horizontal++;
				}
				// ���� üũ
				if (map[i*size + posX] == INT_MAX) {
					vertical++;
				}
			}

			// �������� ���� ����
			if (horizontal == size) {
				bingo++;
			}
			if (vertical == size) {
				bingo++;
			}
		}

		// ���� Ƚ�� ����
		if (isAI) {
			setAIBingoCount(getAIBingoCount() + bingo);
		}
		else {
			setBingoCount(getBingoCount() + bingo);
		}
	}

	// ���� ���� ó��
	void updateGame(int num) {

		int *map = getMap();
		int *aiMap = getAIMap();
		int size = getSize();
		int mapSize = size * size;

		int posX=-1, posY=-1;		// ���� ��ǥ

		// �Է��� ���� ������ ���� �ϴ���?
		for (int i = 0; i < mapSize; i++) {
			if (map[i] == num) {
				// ���� ó��
				map[i] = INT_MAX;

				// ���� ��ǥ ��������
				posX = i % size;
				posY = i / size;
				break;
			}
		}

		// ���� Ƚ�� �� �ʿ� ����
		if (posX != -1) {
			bingoProcess(false, map, size, posX, posY);
		}

		int aiPosX = -1, aiPosY = -1;	// ���� ai ��ǥ

		// �Է��� ���� ai���� ���� �ϴ���?
		for (int i = 0; i < mapSize; i++) {
			if (aiMap[i] == num) {
				// ���� ó��
				aiMap[i] = INT_MAX;

				// ���� ��ǥ ��������
				aiPosX = i % size;
				aiPosY = i / size;
				break;
			}
		}

		// ���� Ƚ�� �� �ʿ� ����
		if (aiPosX != -1) {
			bingoProcess(true, aiMap, size, aiPosX, aiPosY);
		}

		// 70% �̻� ���� �ϸ� �̱�
		int win = getWin();
		if (win <= getBingoCount()) {
			// ���� ����
			setRun(false);
		} else if (win <= getAIBingoCount()) {
			// ���� ����
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
		// �� ������ ����
		setSize(size);
		setWin(size);

		// �� ����
		if (newMap(size) == false) {
			// �޸� �Ҵ� ����
			exit(1);
		}

		// �� �����ͼ� �ʱ�ȭ
		int *map = getMap();
		int *aiMap = getAIMap();
		initMap(map, aiMap, size);

		// ���� ����
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