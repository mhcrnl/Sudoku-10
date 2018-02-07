/*
*Usman Majid
*Patrick Nguyen
* CS171 Sudoku Project*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "Grid.h"

using namespace std;

void generateNew(Grid*, int, int);
//generates new puzzle, next menu

double backtrackSolver(Grid*);
//starts backtracking algorithm

double forwardcheckingSolver(Grid*);
//stars forward checking + backtracking algorithm

void strategy(Grid*);
//choose which strategy to use

int main() {
	int a, b, c, d, e, f, g, h, i;
	int initial;
	int iterations;
	int m;
	int choice;
	int timeOut;
	string filename;
	string line;
	string firstline;
	ifstream input;
	string output;


	cout << "Choose (1) solver (2) generator or (3) mass generator: ";
	cin >> initial;
	cout << "Choose a timeout time (in seconds): ";
	cin >> timeOut;

	if (initial == 1)
	{
		cout << "Please enter the filename: ";
		cin >> filename;
		input.open(filename);

		Grid* sudoku = new Grid();
		sudoku->setTimeOut(timeOut);
		//skip the first line
		getline(input, line);
		for (int j = 0; j < 9; j++)
		{
			getline(input, line);
			istringstream is(line);

			is >> a >> b >> c >> d >> e >> f >> g >> h >> i;
			sudoku->insert(j, 0, a);
			sudoku->insert(j, 1, b);
			sudoku->insert(j, 2, c);
			sudoku->insert(j, 3, d);
			sudoku->insert(j, 4, e);
			sudoku->insert(j, 5, f);
			sudoku->insert(j, 6, g);
			sudoku->insert(j, 7, h);
			sudoku->insert(j, 8, i);
		}

		cout << "Choose (1) backtracking or (2) forwardchecking: ";
		cin >> choice;

		if (choice == 1)
		{
			backtrackSolver(sudoku);
		}
		else
		{
			forwardcheckingSolver(sudoku);
		}
	}
	else if (initial == 2)
	{
		cout << "Please enter the input filename: ";
		cin >> filename;
		input.open(filename);

		cout << "Please enter the output filename: ";
		cin >> output;
		


		//skip the first line
		getline(input, firstline);

		istringstream iss(firstline);
		iss >> a >> b >> c >> d;

		Grid *sudoku = new Grid();
		sudoku->setTimeOut(timeOut);
		
		generateNew(sudoku, d, 2);

		ofstream outputFile(output);
		outputFile << firstline;
		outputFile << "\n";

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				outputFile << sudoku->getIndex(i, j);
				outputFile << " ";
			}
			outputFile << "\n";
		}

		outputFile.close();

		delete sudoku;
	}
	else
	{
		cout << "Please enter the output filename: ";
		cin >> output;
		cout << "Choose how many times to run: ";
		cin >> iterations;
		cout << "Choose the M value: ";
		cin >> m;
		cout << "* 1 * Backtracking \n";
		cout << "* 2 * Backtracking + Forward Checking \n";
		cout << "Choice: ";
		cin >> choice;

		ofstream outputFile(output);

		for (int i = 0; i < iterations; i++)
		{
			Grid *sudoku = new Grid();
			sudoku->setTimeOut(timeOut);
			
			generateNew(sudoku, m, choice);
			if (choice == 1)
			{
				outputFile << backtrackSolver(sudoku);
				outputFile << "\n";
			}
			else
			{
				outputFile << forwardcheckingSolver(sudoku);
				outputFile << "\n";
			}
			delete sudoku;
			this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		outputFile.close();
	}

	cout << endl;
	system("PAUSE");
	return 0;
}

void generateNew(Grid* sudoku, int newM, int choice)
{
	sudoku->setM(newM);
	sudoku->StandardSetup(choice);
}

void strategy(Grid* g)
{
	int choice = g->getMenuChoice();
	switch (choice) {
	case 1: backtrackSolver(g);
		break;
	case 2: forwardcheckingSolver(g);
		break;
	default: cout << "Invalid option.";
		break;
	}
	cout << endl;
}

double backtrackSolver(Grid* g)
{
	clock_t backtrackTimer;
	int row = 0;
	int col = 0;
	double btDuration = 0;

	g->emptyFinder(row, col);

	backtrackTimer = clock();
	g->setBTtimer(backtrackTimer);
	
	bool flag = g->backTracking(row, col, 0);
	g->print();


	btDuration = ((clock() - backtrackTimer) / (CLOCKS_PER_SEC / 1000));
	//cout << "Total Time: " << g->getGenTime() + btDuration << "ms" << endl; //This is to get total time = generate + solve
	cout << "Time: " << fixed << setprecision(2) << btDuration << "ms" << endl;
	cout << "Assignments: " << g->getCount() << endl;
	if (g->getTimerFlag() && flag)
	{
		cout << "Solution: No" << endl;
		cout << "Timeout: Yes" << endl;
	}
	else if (!g->getTimerFlag() && !flag)
	{
		cout << "Solution: No" << endl;
		cout << "Timeout: No" << endl;
	}
	else
	{
		cout << "Solution: Yes" << endl;
		cout << "Timeout: No" << endl;
	}
	return btDuration;
}

double forwardcheckingSolver(Grid* g)
{
	clock_t forwardcheckTimer;
	double fcDuration;
	int row = 0;
	int col = 0;

	g->emptyFinder(row, col);

	forwardcheckTimer = clock();
	g->setFCtimer(forwardcheckTimer);

	bool flag = g->forwardChecking(row, col, 0);
	g->print();

	fcDuration = ((clock() - forwardcheckTimer) / (CLOCKS_PER_SEC / 1000));
	//cout << "Total Time: " << g->getGenTime() + fcDuration << "ms" << endl; //This is to get the total time of search + generate
	cout << "Time: " << fixed << setprecision(2) << fcDuration << "ms" << endl;
	cout << "Assignments: " << g->getCount() << endl;
	if (g->getTimerFlag() && flag)
	{
		cout << "Solution: No" << endl;
		cout << "Timeout: Yes" << endl;
	}
	else if (!g->getTimerFlag() && !flag)
	{
		cout << "Solution: No" << endl;
		cout << "Timeout: No" << endl;
	}
	else
	{
		cout << "Solution: Yes" << endl;
		cout << "Timeout: No" << endl;
	}

	return fcDuration;
}