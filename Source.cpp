#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 5;

char board[SIZE][SIZE];

// инициализация игрового поля
void initBoard() {
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			board[i][j] = ' ';
		}
	}
}


// вывод игрового поля
void printBoard() {
	cout << "  ";
	for (int i = 0; i < SIZE; ++i) {
		cout << i + 1 << " ";
	}
	cout << endl;

	for (int i = 0; i < SIZE; ++i) {
		cout << i + 1 << " ";
		for (int j = 0; j < SIZE; ++j) {
			cout << (board[i][j] == ' ' ? '.' : board[i][j]) << " "; // выводится точка, если ячейка не заполнена
		}
		cout << endl;
	}
}


// проверка на нахождение координат в пределах поля
bool isWithinBoard(int x, int y) {
	return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}


// проверка на наличие одинаковых символов подряд в одном направлении
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


// проверка на победу (5 символов подряд)
bool isWin(char player) {
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == player) {
				if (checkDirection(i, j, 0, 1, player, 5) || //горизонталь
					checkDirection(i, j, 1, 0, player, 5) || //вериткаль
					checkDirection(i, j, 1, 1, player, 5) || //диагональ
					checkDirection(i, j, 1, -1, player, 5))  //диагональ
					return true;
			}
		}
	}
	return false;
}

// проверка на ничью
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

// ход игрока
void playerMove(char player) {
	int row, col;
	while (true) {
		cout << "Игрок " << player << ", введите строку и столбец (от 1 до " << SIZE << "): ";
		cin >> row >> col;
		row--;
		col--;
		if (!isWithinBoard(row, col)) {
			cout << "Указанная ячейка за пределами игрового поля." << endl;
		}
		else if (board[row][col] != ' ') {
			cout << "Ячейчка уже занята." << endl;
		}
		else {
			board[row][col] = player;
			break;
		}
	}
}
// вычисление хода компьютера
// 1000 - победа, 500 - блокировка хода игрока
int winOrBlock(int row, int col, char compChar, char playerChar) {
	int score = 0;

	// проверка на победу
	board[row][col] = compChar;
	if (isWin(compChar)) {
		board[row][col] = ' ';
		return 1000;
	}
	board[row][col] = ' ';


	// проверка на блокировку хода игрока
	board[row][col] = playerChar;
	if (isWin(playerChar)) {
		board[row][col] = ' ';
		return 500;
	}
	board[row][col] = ' ';

	// подсчет очков на основе количества символов компьютера рядом
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
// ход комьютера, выбор наиболее выгодной ячейки для хода
void compMove(char compChar) {
	cout << "Ход компьютера: " << compChar << endl;

	char playerChar = (compChar == 'X') ? 'O' : 'X';

	int bestScore = -1;
	int bestRow = -1;
	int bestCol = -1;

	// перебор и оценка пустых ячеек
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

	// случайный ход
	if (bestRow == -1 || bestCol == -1) {
		do {
			bestRow = rand() % SIZE;
			bestCol = rand() % SIZE;
		} while (board[bestRow][bestCol] != ' ');
	}
	board[bestRow][bestCol] = compChar;
}

// основной процесс игры
void playGame(bool vsComp) {
	initBoard(); // очистка поля
	char player1 = 'X';
	char player2 = 'O';
	char current = player1;

	while (true) {
		printBoard();

		// ход игрока или компьютера
		if (current == player1 || !vsComp) {
			playerMove(current);
		}
		else {
			compMove(current);
		}
		
		// проверка победы
		if (isWin(current)) {
			printBoard();
			cout << "Выиграл игрок " << current << "!" << endl;
			break;
		}
		else if (isDraw()) {
			printBoard();
			cout << "Ничья!" << endl;
			break;
		}
		// смена игрока
		current = (current == player1) ? player2 : player1;
	}
}

int main() {
	setlocale(LC_ALL, "rus");

	int choice;
	// вывод меню
	do {
		cout << "Крестики-нолики 5x5. Меню:" << endl;
		cout << "1. Два игрока" << endl;
		cout << "2. Игрок и компьютер" << endl;
		cout << "3. Выход" << endl;
		cout << "Введите число от 1 до 3: ";
		cin >> choice;

		switch (choice) {
		case 1:
			playGame(false); // два игрока
			break;

		case 2:
			playGame(true); // против компьютера
			break;

		case 3:
			cout << "Выход из игры.." << endl;
			break;

		default:
			cout << "Вы ввели неверное число. Повторите попытку." << endl;
			break;
		}
	}
	while (choice != 3);

	return 0;
}