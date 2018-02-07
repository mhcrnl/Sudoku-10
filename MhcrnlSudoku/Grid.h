#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;

class Grid
{
public:
	Grid();
	//default constructor

	void insert(int, int, int);

	int getIndex(int, int);

	void StandardSetup(int);
	//setup standard sudoku

	void BT_StandardSetup();
	//setup standard sudoku for backtracking

	void FC_StandardSetup();
	//setup standard sudoku for forward checking

	int BT_Generator(int, int);
	//checks generated number and inserts
	//@param int - i
	//@param int - j

	int randGen();
	//generates a random number

	bool checkRow(int, int);
	// checks row if valid
	//@param int - column
	//@param int - number

	bool checkCol(int, int);
	// check column
	//@param int - row
	//@param int - number

	bool checkBox(int, int, int);
	// checks box (3x3 box for standard)
	//@param int - row that box begins on
	//@param int - column that box begins on
	//@param int - number

	bool FC_checkRow(int, int);
	// checks row if valid
	//@param int - column
	//@param int - number

	bool FC_checkCol(int, int);
	// check column
	//@param int - row
	//@param int - number

	bool FC_checkBox(int, int, int);
	// checks box (3x3 box for standard)
	//@param int - row that box begins on
	//@param int - column that box begins on
	//@param int - number

	bool checkAll(int, int, int);
	// checks all
	//@param int - row
	//@param int - column
	//@param int - number

	bool FC_checkAll(int, int, int);
	// checks all and disables in availible
	//@param int - row
	//@param int - column
	//@param int - number

	void FC_UncheckAll(int, int, int);
	// Unchecks all
	//@param int - row
	//@param int - column
	//@param int - number


	bool backTracking(int, int, int);
	// backtracking algorithm
	//@param int - row
	//@param int - column
	//@param int - number in loop to update

	bool emptyFinder(int&, int&);
	// find empty index
	//@param int - row
	//@param int - column

	bool forwardChecking(int, int, int);
	// forward checking + backtracking algorithm
	//@param int - row
	//@param int - column
	//@param int - number in loop to update

	clock_t getBTtimer();
	// returns backtrackTimer

	clock_t getFCtimer();
	// returns forwardcheckTimer

	void setBTtimer(clock_t);
	// sets backtrack timer to clock()

	void setFCtimer(clock_t);
	// sets forward checking timer to clock()

	void print();
	//print - prints out puzzle

	int getCount();
	//getCount - returns number of assignments made

	void setMenuChoice(int);
	// sets menu choice
	//@int - choice

	int getMenuChoice();
	// remembers the choice for the menu

	void setM(int);
	// sets the M value
	//@param int - m value

	bool getTimerFlag();
	// returns timer flag status

	int getTimeOut();
	// returns timeOut value

	void setTimeOut(int);
	// sets the value in seconds for timeOut
	//@param int - timeOut in seconds

	double getGenTime();
	// returns generate time

	bool checkDeadCells(int, int, int);
	//checkDeadCells - checks adjacent cells if they're empty
	//@param int - row
	//@param int - col
	//@param int - num

	~Grid();
	//destructor

private:
	int b[9][9]; //standard sudoku grid
	bool availible[9][9][10]; //@xcoord @ycoord @10 numbers (0 to 9)
	int M; //M value for graph
	int count;
	int timeOut;
	int menuChoice;
	double generateDuration;
	double totalDuration;
	clock_t generateTimer, backtrackTimer, forwardcheckTimer;
	bool timerFlag;
};
#endif
