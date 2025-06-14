#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 5;

char board[SIZE][SIZE];

// ������������� �������� ����
void initBoard() {
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			board[i][j] = ' ';
		}
	}
}


// ����� �������� ����
void printBoard() {
	cout << "  ";
	for (int i = 0; i < SIZE; ++i) {
		cout << i + 1 << " ";
	}
	cout << endl;

	for (int i = 0; i < SIZE; ++i) {
		cout << i + 1 << " ";
		for (int j = 0; j < SIZE; ++j) {
			cout << (board[i][j] == ' ' ? '.' : board[i][j]) << " "; // ��������� �����, ���� ������ �� ���������
		}
		cout << endl;
	}
}


// �������� �� ���������� ��������� � �������� ����
bool isWithinBoard(int x, int y) {
	return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}


// �������� �� ������� ���������� �������� ������ � ����� �����������
bool checkDirection(int x, int y, int dx, int dy, char player, int length) {
	int count = 0;
	for (int i = 0; i < length; ++i) {
		int nx = x + i * dx;
		int ny = y + i * dy;
		if (!isWithinBoard(nx, ny) || board[nx][ny] != player) {
			return false;
		}
	}
	return true;
}


// �������� �� ������ (5 �������� ������)
bool isWin(char player) {
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == player) {
				if (checkDirection(i, j, 0, 1, player, 5) || //�����������
					checkDirection(i, j, 1, 0, player, 5) || //���������
					checkDirection(i, j, 1, 1, player, 5) || //���������
					checkDirection(i, j, 1, -1, player, 5))  //���������
					return true;
			}
		}
	}
	return false;
}

// �������� �� �����
bool isDraw() {
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == ' ') {
				return false;
			}
		}
	}
	return true;
}

// ��� ������
void playerMove(char player) {
	int row, col;
	while (true) {
		cout << "����� " << player << ", ������� ������ � ������� (�� 1 �� " << SIZE << "): ";
		cin >> row >> col;
		row--;
		col--;
		if (!isWithinBoard(row, col)) {
			cout << "��������� ������ �� ��������� �������� ����." << endl;
		}
		else if (board[row][col] != ' ') {
			cout << "������� ��� ������." << endl;
		}
		else {
			board[row][col] = player;
			break;
		}
	}
}
// ���������� ���� ����������
// 1000 - ������, 500 - ���������� ���� ������
int winOrBlock(int row, int col, char compChar, char playerChar) {
	int score = 0;

	// �������� �� ������
	board[row][col] = compChar;
	if (isWin(compChar)) {
		board[row][col] = ' ';
		return 1000;
	}
	board[row][col] = ' ';


	// �������� �� ���������� ���� ������
	board[row][col] = playerChar;
	if (isWin(playerChar)) {
		board[row][col] = ' ';
		return 500;
	}
	board[row][col] = ' ';

	// ������� ����� �� ������ ���������� �������� ���������� �����
	const int directions[4][2] = { {0,1},{1,0},{1,1},{1,-1} };

	for (auto& dir : directions) {
		int dx = dir[0];
		int dy = dir[1];

		int count = 0;
		for (int i = 1; i <= 2; ++i) {
			int nx = row + dx * i;
			int ny = col + dy * i;

			if (isWithinBoard(nx, ny) && board[nx][ny] == compChar)
				count++;
		}
		if (count == 2)
			score += 50;
		else if (count == 1)
			score += 10;
	}
	return score;
}
// ��� ���������, ����� �������� �������� ������ ��� ����
void compMove(char compChar) {
	cout << "��� ����������: " << compChar << endl;

	char playerChar = (compChar == 'X') ? 'O' : 'X';

	int bestScore = -1;
	int bestRow = -1;
	int bestCol = -1;

	// ������� � ������ ������ �����
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == ' ') {
				int score = winOrBlock(i, j, compChar, playerChar);
				if (score > bestScore) {
					bestScore = score;
					bestRow = i;
					bestCol = j;
				}
			}
		}
	}

	// ��������� ���
	if (bestRow == -1 || bestCol == -1) {
		do {
			bestRow = rand() % SIZE;
			bestCol = rand() % SIZE;
		} while (board[bestRow][bestCol] != ' ');
	}
	board[bestRow][bestCol] = compChar;
}

// �������� ������� ����
void playGame(bool vsComp) {
	initBoard(); // ������� ����
	char player1 = 'X';
	char player2 = 'O';
	char current = player1;

	while (true) {
		printBoard();

		// ��� ������ ��� ����������
		if (current == player1 || !vsComp) {
			playerMove(current);
		}
		else {
			compMove(current);
		}
		
		// �������� ������
		if (isWin(current)) {
			printBoard();
			cout << "������� ����� " << current << "!" << endl;
			break;
		}
		else if (isDraw()) {
			printBoard();
			cout << "�����!" << endl;
			break;
		}
		// ����� ������
		current = (current == player1) ? player2 : player1;
	}
}

int main() {
	setlocale(LC_ALL, "rus");

	int choice;
	// ����� ����
	do {
		cout << "��������-������ 5x5. ����:" << endl;
		cout << "1. ��� ������" << endl;
		cout << "2. ����� � ���������" << endl;
		cout << "3. �����" << endl;
		cout << "������� ����� �� 1 �� 3: ";
		cin >> choice;

		switch (choice) {
		case 1:
			playGame(false); // ��� ������
			break;

		case 2:
			playGame(true); // ������ ����������
			break;

		case 3:
			cout << "����� �� ����.." << endl;
			break;

		default:
			cout << "�� ����� �������� �����. ��������� �������." << endl;
			break;
		}
	}
	while (choice != 3);

	return 0;
}