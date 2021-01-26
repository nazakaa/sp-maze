
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;
//----CONSTANTS----------------------------
int width, height;
//----DEFINES------------------------------
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define START 'S'
#define END 'E'
#define FREE ' '
#define VISITEDPOINT '.'
#define WALL (char)219
	//----FUNCTIONS PROTOTYPES-----------------
void fillMaze(char** maze);
void createMaze(char** maze, int x, int y);
bool findWay(char** maze, int x, int y);
void output(char** maze);
void printMaze(char** maze);
void printMaze(char** maze, string path);
void enterSizes();
bool checkSizes(int width, int height);
bool inLimits(int x, int y);
void badInput();
void setColour(int text, int back);
void oneMore();
//----MAIN FUNCTION------------------------
int main() {
	srand(time(0));
	setColour(10, 0);
	cout << "\n\t\t--- THE MAZE GENERATION AND SOLVING APPLICATION ---\n\n";
	enterSizes();
	// Створення масиву для зберігання лабіринта
	char** maze = new char* [::width];
	for (int i = 0; i < ::width; i++)
		maze[i] = new char[::height];
	// Створюю повноцінний лабіринт
	fillMaze(maze);
	createMaze(maze, 1, 1);
	system("cls");
	// Виводжу в консоль згенерований лабіринт
	cout << "....The maze is created" << endl;
	output(maze);
	// Виводжу в консоль пройдений лабіринт
	findWay(maze, 1, 1);
	cout << "....The way is found" << endl;
	output(maze);
	// Видаляю двумірний динамічний масив
	for (int i = 0; i < ::width; i++)
		delete maze[i];
	delete[]maze;
	// Пропоную користувачу почати заново
	oneMore();
	return 0;
}

//----OTHER FUNCTIONS----------------------
void fillMaze(char** maze) {
	// Заповнення масива "стінами"
	for (int y = 0; y < ::height; y++)
		for (int x = 0; x < ::width; x++)
			maze[x][y] = WALL;
}

void createMaze(char** maze, int x, int y) {
	// Створення "проходів" між стінами 
	maze[x][y] = FREE;
	// Створюю массив для зберігання чотирьох можливих напрямків
	int way[4];
	for (int i = 0; i < 4; i++)	way[i] = i;
	// Змінюю порядок массива
	for (int i = 0; i < 4; i++) {
		int j = rand() & 3;
		int temp = way[j];
		way[j] = way[i];
		way[i] = temp;
	}
	// Перебираю кожен напрямок і намагаюсь його пройти.
	for (int i = 0; i < 4; i++) {
		// remX,remY це зміщення відносно [x][y]
		int remX = 0, remY = 0;
		switch (way[i]) {
		case UP: remY = -1; break;
		case RIGHT: remX = 1; break;
		case DOWN: remY = 1; break;
		case LEFT: remX = -1; break;
		}
		// Знаходжу наступні координати [x][y] через два елементи
		int nextX = x + (remX << 1);
		int nextY = y + (remY << 1);
		if (inLimits(nextX, nextY)) {
			if (maze[nextX][nextY] == WALL) {
				//"рyйуную" стіну між [x][y] та [nextX][nextY]
				maze[nextX - remX][nextY - remY] = FREE;
				createMaze(maze, nextX, nextY);
			}
		}
	}
	// Координати початку та кінця
	maze[0][1] = START;
	maze[::width - 1][::height - 2] = END;
}

bool findWay(char** maze, int x, int y) {
	// Рекурсивна функція для знаходження маршруту
	if (maze[x][y] == END)
		return true;
	if (maze[x][y] == WALL || maze[x][y] == VISITEDPOINT)
		return false;
	maze[x][y] = VISITEDPOINT;
	if (findWay(maze, x, y + 1))	//вниз
		return true;
	if (findWay(maze, x + 1, y))	//вправо
		return true;
	if (findWay(maze, x - 1, y))	//вліво
		return true;
	if (findWay(maze, x, y - 1))	//вверх
		return true;
	return false;
}

void output(char** maze) {
	// Меню вибора способа показу лабіринта
	string path;
	cout << "Choose how would u like to view it?";
	cout << "\n\n....press 1 to choose file output";
	cout << "\n....press 2 to choose console output";
	cout << "\n....press 3 to skip the step\n\n";
	int choice; cin >> choice;
	switch (choice) {
	case 1:
		system("cls");
		cout << "Enter the path -> "; cin >> path;
		system("cls");
		printMaze(maze, path);
		break;
	case 2:
		system("cls");
		printMaze(maze);
		break;
	case 3:
		system("cls");
		return;
	default:
		badInput();
		output(maze);
		break;
	}
}
void printMaze(char** maze) {
	// Функція виводу лабіринта в консоль
	for (int y = 0; y < ::height; y++) {
		cout << "\n";
		for (int x = 0; x < ::width; x++) {
			setColour(11, 0);
			if (maze[x][y] == START || maze[x][y] == END)setColour(15, 0);
			if (maze[x][y] == VISITEDPOINT) setColour(9, 9);
			cout << maze[x][y];
		}
	}
	cout << "\n\n";
	setColour(10, 0);
}
void printMaze(char** maze, string path) {
	// Функція виводу в файл
	ofstream fout(path);
	for (int y = 0; y < ::height; y++) {
		fout << "\n ";
		for (int x = 0; x < ::width; x++) {
			if (maze[x][y] == WALL) fout << '@';
			else fout << maze[x][y];
		}
	}
	fout.close();
}

void enterSizes() {
	// Ввод ширини та висоти і перевірка їх на валідність
	cout << "Enter the maze WIDTH and HEIGHT of the maze -> ";
	cin >> ::width >> ::height;
	if (cin.fail() || !checkSizes(::width, ::height)) {
		badInput();
		cout << "....You can enter unpair numbers only in [5, 163] for a WIDTH \n\t\t\t\t  and in [5, 41] for a HEIGHT\n";
		enterSizes();
	}
}

bool checkSizes(int width, int height) {
	// Допоміжна функція для перевірки ширини в висоти
	if (width < 5 || height < 5)
		return false;
	else if (width > 163 || height > 41)
		return false;
	else if (width % 2 == 0 || height % 2 == 0)
		return false;
	return true;
}

bool inLimits(int x, int y) {
	// Перевірка індексів на коректність
	if (x < 0 || x >= ::width)
		return false;
	if (y < 0 || y >= ::height)
		return false;
	return true;
}

void badInput() {
	// функція для виправлення некоректного вводу
	cin.clear();
	cin.ignore(132, '\n');
	system("cls");
}

void setColour(int text, int back) {
	// Встановлення кольору консолі
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((back << 4) | text));
}

void oneMore() {
	// Запит користувача почати спочатку
	cout << "Would u like to generate the maze one more time?" << endl;
	cout << "\n\n....press 1 to generate the maze one more time";
	cout << "\n....press 2 to exit\n\n";
	int choice; cin >> choice;
	switch (choice) {
	case 1:
		system("cls");
		main();
		break;
	case 2:
		system("cls");
		cout << "\n\t\t--- THANK U FOR USING --- \n\n ";
		break;
	default:
		badInput();
		oneMore();
		break;
	}
}
