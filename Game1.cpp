#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>

//using namespace std;

//������� ����
const int rowsCount = 10;
const int columnsCount = 15;


const unsigned char symbolHero = 2;
const unsigned char symbolWall = 177;
const unsigned char symbolBox = 254;
const unsigned char symbolExitOn = 178;
const unsigned char symbolMine = 15;
const unsigned char symbolCrystal = 4;
const unsigned char symbolExitOff = 176;

const unsigned char levelData0[rowsCount][columnsCount + 1] = {
	"#####2#########",
	"#  x   #  x  ##",
	"# x ### x  #  #",
	"#x x  x  ## x #",
	"# x    ##  #  #",
	"#######    # x#",
	"#   x   xx #x #",
	"#xxx # # x   ##",
	"#1 x #   x x  #",
	"###############" };

const unsigned char levelData1[rowsCount][columnsCount + 1] = {
	"###############",
	"#! #   x  ##  2",
	"# x x  # !## ##",
	"#*   *  *#   ##",
	"#   # x # xxx #",
	"#  #  x ##    #",
	"# x# x #  x x #",
	"#x #x * x   * #",
	"#1#     x ##*!#",
	"###############" };

//������//////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
HANDLE consoleHandle = 0;//���������� ��� ��������� �����������
//������ �������
bool isGameActive = true;//�������� �� ����������� �������� ����� ����� �����
unsigned char levelData[rowsCount][columnsCount];
int heroRow = 0;
int heroColumn = 0;//���������� ������
int crystalNumber = 0;   //���������� ��������� ����������
int crystalOnLevel;      //���������� ���������� �� ������

void setupSystem()//��������� �������
{
	srand(time(0));

	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);//������ �����������
}

void initialise()//������� ��������� �������� ��� ����
{
	crystalOnLevel = 0;

	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columnsCount; c++)
		{
			unsigned char symbol = levelData0[r][c];

			switch (symbol)
			{
			case '#':
			{
				levelData[r][c] = symbolWall;
				break;
			}
			case '1':
			{
				levelData[r][c] = symbolHero;

				heroRow = r;
				heroColumn = c;
				break;
			}
			case '2':
			{
				levelData[r][c] = symbolExitOn;
				break;
			}
			case 'x':
			{
				levelData[r][c] = symbolBox;
				break;
			}
			default:
				levelData[r][c] = symbol;
				break;
			}
		}
	}
}

void initialise2()//������� ��������� �������� ��� ������ ����
{
	crystalOnLevel = 3;

	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columnsCount; c++)
		{
			unsigned char symbol = levelData1[r][c];

			switch (symbol)
			{
			case '#':
			{
				levelData[r][c] = symbolWall;
				break;
			}
			case '1':
			{
				levelData[r][c] = symbolHero;

				heroRow = r;
				heroColumn = c;
				break;
			}
			case '2':
			{
				levelData[r][c] = symbolExitOff;
				break;
			}
			case 'x':
			{
				levelData[r][c] = symbolBox;
				break;
			}
			case '*':
			{
				levelData[r][c] = symbolMine;
				break;
			}
			case '!':
			{
				levelData[r][c] = symbolCrystal;
				break;
			}
			default:
				levelData[r][c] = symbol;
				break;
			}
		}
	}
}

void render()//����������� �����
{
	COORD cursorCoord;
	cursorCoord.X = 0;
	cursorCoord.Y = 0;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);

	printf("\n\n\t");
	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columnsCount; c++)
		{
			unsigned char symbol = levelData[r][c];

			switch (symbol)
			{
			case symbolWall: 
			{
				SetConsoleTextAttribute(consoleHandle, 15);
				break;
			}
			case symbolBox: 
			{
				SetConsoleTextAttribute(consoleHandle, 14);
				break;
			}
			case symbolHero: 
			{
				SetConsoleTextAttribute(consoleHandle, 10);
				break;
			}
			case symbolExitOff: 
			{
				SetConsoleTextAttribute(consoleHandle, 8);
				break;
			}
			case symbolExitOn: 
			{
				SetConsoleTextAttribute(consoleHandle, 12);
				break;
			}
			case symbolMine: 
			{
				SetConsoleTextAttribute(consoleHandle, 13);
				break;
			}
			case symbolCrystal:
			{
				SetConsoleTextAttribute(consoleHandle, 11);
				break;
			}
			}
			printf("%c", symbol);
		}
		printf("\n\t");
	}


	SetConsoleTextAttribute(consoleHandle, 7);
	printf("\n\tUsed W,A,S,D to move ");
	SetConsoleTextAttribute(consoleHandle, 10);
	printf("Hero");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(". Be carefull to ");
	SetConsoleTextAttribute(consoleHandle, 13);
	printf("mines");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(".\n\tFind all ");
	SetConsoleTextAttribute(consoleHandle, 11);
	printf("crystals");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(" for open the ");
	SetConsoleTextAttribute(consoleHandle, 12);
	printf("door");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(".\n\tR - restart level.");
	printf("\n\n\tYou have %i crystals.", crystalNumber);

}

void moveHeroTo(int row, int column)//�������� �����
{
	unsigned char destinationCell = levelData[row][column];
	bool canMoveToCell = false;//���������� ��� ������������ �����������
	//����������� �� �������� ������

	switch (destinationCell)
	{
	case ' ':
	{
		canMoveToCell = true;
		break;
	}
	case symbolExitOn:
	{
		isGameActive = false;
		break;
	}
	case symbolMine:
	{
		initialise2();
		crystalNumber = 0;
		break;
	}
	case symbolBox:
	{
		int heroDirectionR = row - heroRow;
		int heroDirectionC = column - heroColumn;
		//����������� �������� �����
		if (levelData[row + heroDirectionR][column + heroDirectionC] == ' ')        //���������, ���� �� ������ ����� �� ��������
		{
			canMoveToCell = true;
			//levelData[row][column] = ' ';     //������� ���������� ������ �������; ��������, ����� ���������� ����� ��� ����������
			levelData[row + heroDirectionR][column + heroDirectionC] = symbolBox;       //������ ������� �� ����� ����������
		}
	}
	case symbolCrystal:
	{
		int heroDirectionR = row - heroRow;
		int heroDirectionC = column - heroColumn;

		if (levelData[row][column] - levelData[heroRow][heroColumn] <= levelData[1][1])     //��������, ���� �� ������ � ������� ����� ������ �� �����
		{
			canMoveToCell = true;
			levelData[row][column] = ' ';
			crystalNumber++;
		}
		break;
	}
	}

	if (canMoveToCell)
	{
		levelData[heroRow][heroColumn] = ' ';       //������� ���������� ������ �����

		heroRow = row;                              //���������� ����� ����������
		heroColumn = column;

		levelData[heroRow][heroColumn] = symbolHero;
	}
}

void update()//���������� ������� ������
{
	unsigned char inputChar = _getch();
	inputChar = tolower(inputChar);

	switch (inputChar)
	{
	case 'w':
	{
		moveHeroTo(heroRow - 1, heroColumn);
		break;
	}
	case 'a':
	{
		moveHeroTo(heroRow, heroColumn - 1);
		break;
	}
	case 's':
	{
		moveHeroTo(heroRow + 1, heroColumn);
		break;
	}
	case 'd':
	{
		moveHeroTo(heroRow, heroColumn + 1);
		break;
	}
	case 'r':
	{
		initialise();
		break;
	}
	}
}

void shutdown()//����� ����
{
	system("cls");
	printf("\nPress any key to continue...");
	_getch();
}

int main()
{
	setupSystem();
	initialise();

	do
	{
		render();
		update();

	} while (isGameActive);

	initialise2();
	isGameActive = true;
	do
	{
		render();
		update();
		if (crystalOnLevel == crystalNumber) //��������� �����, ���� ����� ������ ��� ���������
		{
			for (int r = 0; r < rowsCount; r++)
			{
				for (int c = 0; c < columnsCount; c++)
				{
					unsigned char symbol = levelData1[r][c];

					switch (symbol)
					{
					case '2':
					{
						levelData[r][c] = symbolExitOn;
						break;
					}
					}
				}
			}
		}


	} while (isGameActive);
	shutdown();

	return 0;
}