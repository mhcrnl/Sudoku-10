#include "Grid.h"

//***********BEGIN GRID CLASS************
Grid::Grid()
{
	menuChoice = 0;
	count = 0;
	srand(time(0));
	generateTimer = 0;
	generateDuration = 0;
	totalDuration = 0;
	backtrackTimer = 0;
	forwardcheckTimer = 0;
	timerFlag = false;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			b[i][j] = 0;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				availible[i][j][k] = true;
			}
		}
	}
}

void Grid::insert(int row, int col, int num)
{
	if (num == 0)
	{

	}
	else
	{
		b[row][col] = num;
		FC_checkAll(row, col, num);
		availible[row][col][0] = false;
	}
}

int Grid::getCount()
{
	return count;
}

void Grid::setMenuChoice(int m)
{
	menuChoice = m;
}

int Grid::getMenuChoice()
{
	return menuChoice;
}

void Grid::setM(int newM)
{
	M = newM;
}

void Grid::setBTtimer(clock_t newClock)
{
	backtrackTimer = newClock;
}

void Grid::setFCtimer(clock_t newClock)
{
	forwardcheckTimer = newClock;
}

clock_t Grid::getBTtimer()
{
	return backtrackTimer;
}

clock_t Grid::getFCtimer()
{
	return forwardcheckTimer;
}

bool Grid::getTimerFlag()
{
	return timerFlag;
}

void Grid::StandardSetup(int choice)
{
	menuChoice = choice;
	switch (choice)
	{
	case 1: BT_StandardSetup();
		break;
	case 2: FC_StandardSetup();
		break;
	}
}

void Grid::BT_StandardSetup()
{
	for (int newM = M; newM > 0; newM--)
	{
		generateTimer = clock();
		int i = rand() % 10;
		int j = rand() % 10;
		if (b[i][j] == 0)
		{
			b[i][j] = BT_Generator(i, j);
		}
		else
		{
			i = rand() % 10;
			j = rand() % 10;
			newM++;
		}
	}
	generateDuration = (clock() - generateTimer) / (CLOCKS_PER_SEC / 1000);
	//cout << "BT Generate Time: " << generateDuration << endl;
}

void Grid::FC_StandardSetup()
{
	for (int newM = M; newM > 0; newM--)
	{
		generateTimer = clock();
		int i = rand() % 10;
		int j = rand() % 10;
		if (b[i][j] == 0)
		{
			int num = BT_Generator(i, j);
			b[i][j] = num;
			FC_checkAll(i, j, num);
			availible[i][j][0] = false;
		}
		else
		{
			i = rand() % 10;
			j = rand() % 10;
			newM++;
		}
	}
	//TESTING
	generateDuration = (clock() - generateTimer) / (CLOCKS_PER_SEC / 1000);
	//cout << "BT+FC Generate Time: " << generateDuration << endl;
	//print(); //*******************PRINT THE GENERATED PUZZLE******************
	//ENDTESTING
}

int Grid::randGen()
{
	int num = rand() % 10;
	return num;
}

int Grid::BT_Generator(int i, int j)
{
	int flag = false;
	int num = randGen();
	while (!flag)
	{
		do { num = randGen(); } while (num == 0);
		
		if (checkRow(j, num))
		{
			//cout << "TRUE ROW\n";
			if (checkCol(i, num))
			{
				//cout << "TRUE COL\n";
				if (checkBox(i, j, num))
				{
					//cout << "TRUE BOX\n";
					flag = true;
					return num;
				}
			}
		}
	}
}

bool Grid::checkRow(int col, int num)
{
	for (int i = 0; i < 9; i++)
	{
		if (num == b[i][col])
			return false;
	}
	return true;
}

bool Grid::checkCol(int row, int num)
{
	for (int j = 0; j < 9; j++)
	{
		if (num == b[row][j])
			return false;
	}
	return true;
}

bool Grid::checkBox(int row, int col, int num)
{
	for (int bx = (row / 3) * 3; bx < (row / 3) * 3 + 3; bx++)
	{
		for (int by = (col / 3) * 3; by < (col / 3) * 3 + 3; by++)
		{
			//cout << "checkbox value: " << b[bx][by] << endl;
			if (num == b[bx][by])
				return false;
		}
	}
	return true;
}

bool Grid::checkAll(int row, int col, int num)
{
	return checkCol(row, num) && checkRow(col, num) && checkBox(row, col, num);
}

bool Grid::backTracking(int row, int col, int number)
{
	int rowTwo;
	int colTwo;

	if (!timerFlag && ((clock() - backtrackTimer / (CLOCKS_PER_SEC / 1000)) > timeOut))
	{
		timerFlag = true;
		return true;
	}

	for (int num = number; num <= 9; num++)
	{
		if (checkAll(row, col, num))
		{
			b[row][col] = num;
			count++;
			//print();

			//this runs when there's an empty index
			if (emptyFinder(rowTwo, colTwo))
			{
				//this statement runs when there's no solution for rwoTwo, colTwo
				if (!backTracking(rowTwo, colTwo, 0))
				{
					b[rowTwo][colTwo] = 0;
					return backTracking(row, col, num + 1);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
	}
	//cout << "No values found" << endl;
	return false;
}

bool Grid::emptyFinder(int& row, int& col)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (b[i][j] == 0)
			{
				col = j;
				row = i;
				return true;
			}
		}
	}

	return false;
}


bool Grid::FC_checkAll(int row, int col, int num)
{
	return (FC_checkCol(row, num) && FC_checkRow(col, num) && FC_checkBox(row, col, num));
}

void Grid::FC_UncheckAll(int row, int col, int num)
{
	//row
	for (int i = 0; i < 9; i++)
	{
		availible[i][col][num] = true;
	}

	//col
	for (int j = 0; j < 9; j++)
	{
		availible[row][j][num] = true;
	}

	//box
	for (int bx = (row / 3) * 3; bx < (row / 3) * 3 + 3; bx++)
	{
		for (int by = (col / 3) * 3; by < (col / 3) * 3 + 3; by++)
		{
			availible[bx][by][num] = true;
		}
	}
}

//disable all numbers in a specific column
bool Grid::FC_checkRow(int col, int num)
{
	for (int i = 0; i < 9; i++)
	{
		availible[i][col][num] = false;
	}
	return true;
}

//disable all numbers in a specific row
bool Grid::FC_checkCol(int row, int num)
{
	for (int j = 0; j < 9; j++)
	{
		availible[row][j][num] = false;
	}
	return true;
}

bool Grid::FC_checkBox(int row, int col, int num)
{
	for (int bx = (row / 3) * 3; bx < (row / 3) * 3 + 3; bx++)
	{
		for (int by = (col / 3) * 3; by < (col / 3) * 3 + 3; by++)
		{
			availible[bx][by][num] = false;
		}
	}
	return true;
}

bool Grid::forwardChecking(int row, int col, int number)
{
	int rowTwo;
	int colTwo;

	if (!timerFlag && ((clock() - forwardcheckTimer / (CLOCKS_PER_SEC / 1000)) > timeOut))
	{
		timerFlag = true;
		return true;
	}

	for (int num = number; num <= 9; num++)
	{
		//this is checking the cell for the number num if it's available or not
		if (availible[row][col][num])
		{
			//this is disabling number for adjacent row, col, box
			//check this later to see if it's working properly*************************
			if (checkAll(row, col, num))
			{
				FC_checkAll(row, col, num);
				//inserting number into cell
				b[row][col] = num;
				availible[row][col][0] = false;

				count++;
				//print();

				//check if this assignment caused any empty cells
				if (checkDeadCells(row, col, num))
				{
					FC_UncheckAll(row, col, num);
					availible[row][col][0] = true;
					//b[row][col] = 0;
					return forwardChecking(row, col, num + 1);
				}
				else
				{
					//this runs when there's an empty index
					if (emptyFinder(rowTwo, colTwo))
					{
						//this statement runs when there's no solution for rwoTwo, colTwo
						if (!forwardChecking(rowTwo, colTwo, 0))
						{
							b[rowTwo][colTwo] = 0;
							availible[rowTwo][colTwo][0] = true;
							FC_UncheckAll(row, col, num);
							return forwardChecking(row, col, num + 1);
						}
						else
						{
							return true;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}
	}
	//cout << "No values found" << endl;
	return false;
}

bool Grid::checkDeadCells(int row, int col, int num)
{
	//***********************************************
	bool flag = false;
	int k = 1;

	//row
	for (int i = 0; i < 9; i++)
	{
		if (availible[i][col][0])
		{
			while (!flag && k < 10)
			{
				flag = availible[i][col][k];
				k++;
			}

			if (flag == false && k == 10)
				return true;
		}
	}
	//***********************************************

	//***********************************************
	flag = false;
	k = 1;

	//col
	for (int j = 0; j < 9; j++)
	{
		if (availible[row][j][0])
		{
			while (!flag && k < 10)
			{
				flag = availible[row][j][k];
				k++;
			}

			if (flag == false && k == 10)
				return true;
		}
	}
	//***********************************************

	//***********************************************
	flag = false;
	k = 1;

	//box
	for (int bx = (row / 3) * 3; bx < (row / 3) * 3 + 3; bx++)
	{
		for (int by = (col / 3) * 3; by < (col / 3) * 3 + 3; by++)
		{
			if (availible[bx][by][0])
			{
				while (!flag && k < 10)
				{
					flag = availible[bx][by][k];
					k++;
				}
			
				if (flag == false && k == 10)
					return true;
			}
		}
	}
	//***********************************************

	return false; //if all loops failed, then dead cell is not found
}

int Grid::getIndex(int row, int col)
{
	return b[row][col];
}

void Grid::print()
{
	cout << "\n**************************\n";
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cout << b[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
}

int Grid::getTimeOut()
{
	return timeOut;
}

double Grid::getGenTime()
{
	return generateDuration;
}

void Grid::setTimeOut(int time)
{
	timeOut = time * 1000;
}

Grid::~Grid()
{
}
//*************END GRID CLASS************