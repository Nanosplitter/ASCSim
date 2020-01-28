#pragma GCC optimize("-O3", "unroll-loops", "omit-frame-pointer","inline")

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

using namespace std::chrono;
const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

const int VALID = 4;
const int VOID = -1;
const int HEIGHT = 10;
const int WIDTH = 19;
const int NUMDIR = 4;

int NUMCLIMBS = 5;

int GENSIZE = 30;

const int CHECKGAP = 100;
const float CHECKSTANDARD = 1.05;

inline int timeDiff(std::chrono::time_point<std::chrono::high_resolution_clock> startT, std::chrono::time_point<std::chrono::high_resolution_clock> endT)
{
	return duration_cast<milliseconds>(endT - startT).count();
};

inline int setrand() {
	return 4;
};

inline int rnd(int b)
{
	return rand() % b;
};

inline int rnd(int a, int b)
{
	return a + rnd(b - a + 1);
};
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

class Robot
{
public:

	int cells[HEIGHT][WIDTH]; //Stores the base cells
	bool visited[HEIGHT][WIDTH][NUMDIR]; //Stores visited cells
	bool visitedReset[HEIGHT][WIDTH][NUMDIR];

	int startY;	//Stores starting Y
	int startX; //Stores starting X
	int startDir; //Stores starting Direction

	int y; //Stores Current Y
	int x; //Stores Current X
	int dir; // Stores Current Dir

	int score; //Stores score

	Robot(int sY, int sX, int sDir)
	{
		startY = sY;
		startX = sX;
		startDir = sDir;
	}


	int getScore(int arrowedCells[HEIGHT][WIDTH])
	{

		//std::copy(arrowedCells.begin(), arrowedCells.end(), cells.begin()); //If I need to copy the arrowed cells
		//std::copy(&visited[0][0][0], &visited[0][0][0]+HEIGHT*WIDTH*NUMDIR, &visitedReset[0][0][0]); //Reset VisitedCells

		for (int yi = 0; yi < HEIGHT; yi++)
		{
			for (int xi = 0; xi < WIDTH; xi++)
			{
				for (int di = 0; di < NUMDIR; di++)
				{
					visited[yi][xi][di] = false;
				}
			}
		}

		score = 0;
		y = startY;
		x = startX;
		dir = startDir;

		switch (arrowedCells[y][x])
		{
		case UP:
			dir = UP;
			break;
		case DOWN:
			dir = DOWN;
			break;
		case LEFT:
			dir = LEFT;
			break;
		case RIGHT:
			dir = RIGHT;
			break;
		case VOID:
			return score + 1;
		}

		while (1)
		{
			//Move robot in direction it is facing
			switch (dir)
			{
			case UP:
				y--;
				break;
			case DOWN:
				y++;
				break;
			case LEFT:
				x--;
				break;
			case RIGHT:
				x++;
				break;
			}

			//Portal robot if steps out of grid
			if (x == 19) {
				x = 0;
			}
			else if (x == -1) {
				x = 18;
			}
			else if (y == 10) {
				y = 0;
			}
			else if (y == -1) {
				y = 9;
			}

			//Change direction if on arrow and deactivate if on void
			switch (arrowedCells[y][x])
			{
			case UP:
				dir = UP;
				break;
			case DOWN:
				dir = DOWN;
				break;
			case LEFT:
				dir = LEFT;
				break;
			case RIGHT:
				dir = RIGHT;
				break;
			case VOID:
				//cerr << "Falling off edge at: y=" << y << " x= " << x << " dir= " << dir << endl;
				return score + 1;
			}

			//See if state has been visited before, if so, deactivate. else record state
			if (visited[y][x][dir]) {
				//cerr << "Infinite loop at: y=" << y << " x= " << x << " dir= " << dir << endl;
				return score;
			}
			else {
				score++;
				visited[y][x][dir] = true;
			}
		}

	}
};

class ScoreAndString
{
public:

	int score;
	std::string arrows;
	std::string usedCells;

	ScoreAndString()
	{
		score = 0;
		arrows = "";
		usedCells = "";
	}
};

class Simulation
{
public:

	int hardCells[HEIGHT][WIDTH]; //Base Cells
	vector<Robot*> robots; //A vector of robot objects
	vector<vector<int>> arrows; //A vector of arrows {y, x, dir}

	int arrowedCells[HEIGHT][WIDTH]; //Base Cells with arrows applied to give to the robots
	int totalScore; //The scores of all the robots combined (End score)
	int numSims;

	Simulation()
	{

	}

	Simulation(vector<Robot*> robotsCopy, int hardCellsCopy[HEIGHT][WIDTH])
	{
		robots = robotsCopy;
		std::copy(&hardCellsCopy[0][0], &hardCellsCopy[0][0] + HEIGHT * WIDTH, &hardCells[0][0]);
	}


	int play(vector<vector<int>> arrs)
	{
		arrows = arrs;
		std::reverse(arrows.begin(), arrows.end());
		totalScore = 0;
		numSims++;
		//Copy the hardCells to arrowedCells
		std::copy(&hardCells[0][0], &hardCells[0][0] + HEIGHT * WIDTH, &arrowedCells[0][0]);

		//Arrow the arrowedCells
		for (auto& arr : arrows)
		{
			arrowedCells[arr.at(0)][arr.at(1)] = arr.at(2);
		}

		//Get robot scores
		for (auto& r : robots)
		{
			totalScore += r->getScore(arrowedCells);
			//cerr << r->getScore(arrowedCells) << endl;
		}

		return totalScore;
	}

	string getArrString()
	{
		string res = "";
		for (auto& a : arrows)
		{
			res += std::to_string(a.at(1));
			res += " ";
			res += std::to_string(a.at(0));
			res += " ";
			switch (a.at(2))
			{
			case 0:
				res += "U";
				break;
			case 1:
				res += "D";
				break;
			case 2:
				res += "L";
				break;
			case 3:
				res += "R";
				break;
			}
			res += " ";
		}

		return res;
	}
};

class State
{
public:

	int score;
	vector<vector<int>> arrows;

	bool usedCells[HEIGHT][WIDTH];
	int tempDir = 0;

	State(vector<vector<int>> arrsCopy)
	{
		arrows = arrsCopy;

		for (int yi = 0; yi < HEIGHT; yi++)
		{
			for (int xi = 0; xi < WIDTH; xi++)
			{
				usedCells[yi][xi] = false;
			}
		}


		for (auto& a : arrows)
		{
			usedCells[a.at(0)][a.at(1)] = true;
		}
	}

	void copyUsedCells(bool uCopy[HEIGHT][WIDTH])
	{
		std::copy(&usedCells[0][0], &usedCells[0][0] + HEIGHT * WIDTH, &uCopy[0][0]);
		// for (int yi = 0; yi < HEIGHT; yi++)
		// {
		// 	for (int xi = 0; xi < WIDTH; xi++)
		// 	{
		// 		if (uCopy[yi][xi])
		// 		{
		// 			usedCells[yi][xi] = true;
		// 		}
		// 		else
		// 		{
		// 			usedCells[yi][xi] = false;
		// 		}
		// 	}
		// }

	}

	std::string readable()
	{
		return "State(" + std::to_string(score) + "| " + getArrString() + ")";
	}

	string getArrString()
	{
		string res = "";
		for (auto& a : arrows)
		{
			res += std::to_string(a.at(1));
			res += " ";
			res += std::to_string(a.at(0));
			res += " ";
			switch (a.at(2))
			{
			case 0:
				res += "U";
				break;
			case 1:
				res += "D";
				break;
			case 2:
				res += "L";
				break;
			case 3:
				res += "R";
				break;
			}
			res += " ";
		}

		return res;
	}

	std::string getUsedArrows()
	{
		std::string res = "";
		for (int yi = 0; yi < HEIGHT; yi++)
		{
			for (int xi = 0; xi < WIDTH; xi++)
			{
				if (usedCells[yi][xi])
				{
					res += "(" + std::to_string(xi) + ", " + std::to_string(yi) + " : " + std::to_string(usedCells[yi][xi]) + ")";
				}
			}
		}
		return res;
	}

	void flipArr(int arrIndex)
	{
		tempDir = rnd(7);
		if (tempDir < 4)
		{
			arrows.at(arrIndex).at(2) = tempDir;
		}
		else
		{
			usedCells[arrows.at(arrIndex).at(0)][arrows.at(arrIndex).at(1)] = false;
			arrows.erase(arrows.begin() + arrIndex);
		}
	}

	void addArrow(int arr, int tDir)
	{
		vector<int> arrV;
		int arrY = (int)(arr / 19);
		int arrX = arr - 19 * arrY;
		if (tDir < 4 && !usedCells[arrY][arrX])
		{
			arrV.push_back(arrY);
			arrV.push_back(arrX);
			arrV.push_back(tDir);
			arrows.push_back(arrV);
			//usedCells[arrY][arrX] = true;
		}

		for (int yi = 0; yi < HEIGHT; yi++)
		{
			for (int xi = 0; xi < WIDTH; xi++)
			{
				usedCells[yi][xi] = false;
			}
		}

		for (auto& a : arrows)
		{
			usedCells[a.at(0)][a.at(1)] = true;
		}
	}
};

bool stateCompare(State* state1, State* state2)
{
	return (state1->score < state2->score);
};

class Manager
{
public:

	vector<State*> states;
	vector<int> validCells;
	Simulation sim;
	bool add;
	int rn;
	int gens;


	Manager(vector<int> vc, Simulation simOG)
	{
		validCells = vc;
		gens = 0;
		sim = simOG;
		vector<vector<int>> none;
		//Fill states
		for (int i = 0; i < GENSIZE; i++)
		{
			states.push_back(new State(genRandArrows()));
			//states.push_back(new State(none));
		}
	}


	vector<State*> generateNewArrows(vector<State*> seedStates)
	{
		State* state;
		int validCell;
		int tDir;
		int chance;
		//auto runStart = high_resolution_clock::now();
		for (int i = 1; i < GENSIZE; i++)
		{
			//cerr << timeDiff(runStart, high_resolution_clock::now()) << endl;
			state = seedStates.at(i);
			add = false;
			chance = rnd(100);
			if (state->arrows.empty())
			{
				add = true;
			}

			//Fip an arrow, possibly delete it
			if (chance < 30 && !add)
			{
				//cerr << "Fip an arrow, possibly delete it" << endl;
				rn = rnd(state->arrows.size());
				state->flipArr(rn);
			}
			//Flip up to 2 arrows, possibly delete them
			else if (chance < 60 && !add)
			{
				//cerr << "Flip up to 2 arrows, possibly delete them" << endl;
				rn = rnd(state->arrows.size());
				state->flipArr(rn);
				if (!state->arrows.empty())
				{
					rn = rnd(state->arrows.size());
					state->flipArr(rn);
				}

			}
			//Add an arrow, possibly
			else if (chance < 80 && !add)
			{
				//cerr << "Add an arrow, possibly" << endl;
				validCell = validCells.at(rnd(validCells.size()));
				tDir = rnd(6);
				if (tDir < 4)
				{
					vector<int> validCellVec = { (int)(validCell / 19), validCell - 19 * (int)(validCell / 19) };
					state->addArrow(validCell, tDir);
				}
			}
			//Add up to 5 arrows
			else
			{
				//cerr << "Add up to 5 arrows" << endl;
				validCell = validCells.at(rnd(validCells.size()));
				tDir = rnd(6);
				if (tDir < 4)
				{
					vector<int> validCellVec = { (int)(validCell / 19), validCell - 19 * (int)(validCell / 19) };
					state->addArrow(validCell, tDir);
				}

				validCell = validCells.at(rnd(validCells.size()));
				tDir = rnd(6);
				if (tDir < 4)
				{
					vector<int> validCellVec = { (int)(validCell / 19), validCell - 19 * (int)(validCell / 19) };
					state->addArrow(validCell, tDir);
				}

				validCell = validCells.at(rnd(validCells.size()));
				tDir = rnd(6);
				if (tDir < 4)
				{
					vector<int> validCellVec = { (int)(validCell / 19), validCell - 19 * (int)(validCell / 19) };
					state->addArrow(validCell, tDir);
				}

				validCell = validCells.at(rnd(validCells.size()));
				tDir = rnd(6);
				if (tDir < 4)
				{
					vector<int> validCellVec = { (int)(validCell / 19), validCell - 19 * (int)(validCell / 19) };
					state->addArrow(validCell, tDir);
				}

				validCell = validCells.at(rnd(validCells.size()));
				tDir = rnd(6);
				if (tDir < 4)
				{
					vector<int> validCellVec = { (int)(validCell / 19), validCell - 19 * (int)(validCell / 19) };
					state->addArrow(validCell, tDir);
				}
			}

			seedStates.at(i) = state;
		}

		return seedStates;
	}

	void reset()
	{
		for (auto& s : states)
		{
			s->arrows = genRandArrows();
		}
	}

	void scoreAndSort()
	{
		for (auto& s : states)
		{
			s->score = sim.play(s->arrows);
		}
		// for (auto & s : states)
		// {
		// 	cerr << s->readable() << ", ";
		// }
		// cerr << endl;
		std::sort(states.begin(), states.end(), stateCompare);
		// for (auto & s : states)
		// {
		// 	cerr << s->readable() << ", ";
		// }
		// cerr << endl;
	}

	void copyStates()
	{
		State* best = states.back();

		for (auto& s : states)
		{
			s->arrows = best->arrows;
			s->copyUsedCells(best->usedCells);
			s->score = best->score;
		}
	}

	ScoreAndString getHillClimbArrows(std::chrono::time_point<std::chrono::high_resolution_clock> startTime)
	{
		auto currTime = high_resolution_clock::now();
		int checkPointScore = 0;
		auto lastCheckpointTime = high_resolution_clock::now();
		ScoreAndString AABest;
		scoreAndSort();
		copyStates();

		while (timeDiff(startTime, currTime) < ((int)999 / NUMCLIMBS))
		{
			//if (sim.numSims % 1000 == 0) {
				//cout << sim.numSims << endl;
			//}
			gens++;
			states = generateNewArrows(states);
			scoreAndSort();
			copyStates();
			currTime = high_resolution_clock::now();
		}
		states = generateNewArrows(states);
		scoreAndSort();
		ScoreAndString sas;
		sas.score = states.back()->score;
		sas.arrows = states.back()->getArrString();
		sas.usedCells = states.back()->getUsedArrows();

		return sas;
	}

	vector<vector<int>> genRandArrows()
	{
		vector<vector<int>> randArrows;
		std::random_shuffle(validCells.begin(), validCells.end());
		int numRandArrows = rnd(validCells.size());
		int arrY;
		int arrX;
		for (int i = 0; i < numRandArrows; i++)
		{
			vector<int> arrow;
			arrow.reserve(3);
			arrY = (int)(validCells.at(i) / 19);
			arrX = validCells.at(i) - 19 * arrY;
			arrow.push_back(arrY);
			arrow.push_back(arrX);
			arrow.push_back(rnd(4));

			randArrows.push_back(arrow);

		}

		return randArrows;

	}
};




/* FOR CODINGAME USE

int main()
{
	Simulation sim;
	srand((unsigned)time(0));
	//srand(546312785432167453367);
	vector<int> openCells;

	for (int y = 0; y < 10; y++) {
		string line;
		getline(cin, line);
		for (int x = 0; x < line.size(); x++) {
			switch (line[x])
			{
			case 'U':
				sim.hardCells[y][x] = UP;
				break;
			case 'D':
				sim.hardCells[y][x] = DOWN;
				break;
			case 'L':
				sim.hardCells[y][x] = LEFT;
				break;
			case 'R':
				sim.hardCells[y][x] = RIGHT;
				break;
			case '#':
				sim.hardCells[y][x] = VOID;
				break;
			case '.':
				openCells.push_back((19 * y) + x);
				sim.hardCells[y][x] = VALID;
				break;
			}
		}
	}

	auto start_time = high_resolution_clock::now();
	int robotCount;
	cin >> robotCount; cin.ignore();
	sim.robots.reserve(robotCount);

	for (int i = 0; i < robotCount; i++) {
		int x;
		int y;
		string direction;
		cin >> x >> y >> direction; cin.ignore();

		int dir;
		switch (direction[0])
		{
		case 'U':
			dir = UP;
			break;
		case 'D':
			dir = DOWN;
			break;
		case 'L':
			dir = LEFT;
			break;
		case 'R':
			dir = RIGHT;
			break;
		case '#':
			dir = VOID;
			break;
		case '.':
			dir = VALID;
			break;
		}

		sim.robots.push_back(new Robot(y, x, dir));

	}

	Manager manager = Manager(openCells, sim);
	//cerr << timeDiff(start_time, high_resolution_clock::now()) << endl;
	ScoreAndString bestSolution;
	ScoreAndString tempSolution;
	std::chrono::time_point<std::chrono::high_resolution_clock> curr_time = high_resolution_clock::now();
	for (int i = 0; i < NUMCLIMBS; i++)
	{
		tempSolution = manager.getHillClimbArrows(curr_time);
		if (tempSolution.score > bestSolution.score)
		{
			bestSolution.score = tempSolution.score;
			bestSolution.arrows = tempSolution.arrows;
		}
		curr_time = high_resolution_clock::now();
	}

	//std::cerr << "Time difference (sec) = " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() << std::endl;

	std::chrono::time_point<std::chrono::high_resolution_clock> end_time = high_resolution_clock::now();
	cout << bestSolution.arrows << endl;
	cerr << "Best Score: " << bestSolution.score << endl;
	cerr << "Best Arrows: " << bestSolution.arrows << endl;
	cerr << "Num Sims: " << manager.sim.numSims << endl;
	//cerr << "Used Cells: " << solution.usedCells << endl;
	// cerr << sim.play() << endl;
	//cerr << "C: " << c << endl;
	cerr << "Time Elapsed: " << duration_cast<milliseconds>(end_time - start_time).count() << endl;
	//cerr << "Sim Pred: " << sim.play() << endl;
	//cerr << "Sim Count: " << sim.numSims << endl;


}

*/

//FOR TESTING USE
int main()
{
	
	//srand(546312785432167453367);
	
	
	ofstream fout;
	fout.open("C:\\CodinGame\\AStar\\output.txt");
	
	int totalScore = 0;
	int bestNumClimbs = -1;
	float bestAverageScore = -1;
	float currAverageScore = 0;
	int currTotalScore = 0;
	//cout << "1" << endl;
		for (int numclimbs = 1; numclimbs < 100; numclimbs++)
		{
			//cout << "2" << endl;
			NUMCLIMBS = numclimbs;
			currAverageScore = 0;
			currTotalScore = 0;
			for (int runs = 0; runs < 20; runs++)
			{
				srand((unsigned)time(0));
				//cout << "3" << endl;
				ifstream fin;
				fin.open("C:\\CodinGame\\AStar\\test30.txt");
				for(int test = 0; test < 1; test++)
				{
					//cout << "4" << endl;
					Simulation sim;
					vector<int> openCells;
					for (int y = 0; y < 10; y++) {
						string line;
						getline(fin, line);
						//cout << line << endl;
						for (int x = 0; x < line.size(); x++) {
							switch (line[x])
							{
							case 'U':
								sim.hardCells[y][x] = UP;
								break;
							case 'D':
								sim.hardCells[y][x] = DOWN;
								break;
							case 'L':
								sim.hardCells[y][x] = LEFT;
								break;
							case 'R':
								sim.hardCells[y][x] = RIGHT;
								break;
							case '#':
								sim.hardCells[y][x] = VOID;
								break;
							case '.':
								openCells.push_back((19 * y) + x);
								sim.hardCells[y][x] = VALID;
								break;
							}
						}
					}
					//cout << "5" << endl;
					
					int robotCount;
					fin >> robotCount; fin.ignore();
					sim.robots.reserve(robotCount);
					//cout << robotCount << endl;
					for (int i = 0; i < robotCount; i++) {
						int x;
						int y;
						string direction;
						fin >> x >> y >> direction; fin.ignore();

						int dir;
						switch (direction[0])
						{
						case 'U':
							dir = UP;
							break;
						case 'D':
							dir = DOWN;
							break;
						case 'L':
							dir = LEFT;
							break;
						case 'R':
							dir = RIGHT;
							break;
						case '#':
							dir = VOID;
							break;
						case '.':
							dir = VALID;
							break;
						}

						sim.robots.push_back(new Robot(y, x, dir));

					}
					//cout << "6" << endl;

					Manager manager = Manager(openCells, sim);
					//cerr << timeDiff(start_time, high_resolution_clock::now()) << endl;
					ScoreAndString bestSolution;
					ScoreAndString tempSolution;
					auto start_time = high_resolution_clock::now();
					std::chrono::time_point<std::chrono::high_resolution_clock> curr_time = high_resolution_clock::now();
					//cout << "5" << endl;
					for (int i = 0; i < NUMCLIMBS; i++)
					{
						tempSolution = manager.getHillClimbArrows(curr_time);
						if (tempSolution.score > bestSolution.score)
						{
							bestSolution.score = tempSolution.score;
							bestSolution.arrows = tempSolution.arrows;
						}
						curr_time = high_resolution_clock::now();
					}

					//std::cerr << "Time difference (sec) = " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() << std::endl;

					std::chrono::time_point<std::chrono::high_resolution_clock> end_time = high_resolution_clock::now();
					//cout << bestSolution.arrows << endl;
					totalScore += bestSolution.score;
					//fout << "Test: " << test + 1 << " | Best Score: " << bestSolution.score << " | Num Sims: " << manager.sim.numSims << " | Time Elapsed: " << duration_cast<milliseconds>(end_time - start_time).count() << endl; 
					//cerr << "Best Score: " << bestSolution.score << endl;
					//cerr << "Best Arrows: " << bestSolution.arrows << endl;
					//cerr << "Num Sims: " << manager.sim.numSims << endl;
					//cerr << "Used Cells: " << solution.usedCells << endl;
					// cerr << sim.play() << endl;
					//cerr << "C: " << c << endl;
					//cerr << "Time Elapsed: " << duration_cast<milliseconds>(end_time - start_time).count() << endl;
					//cerr << "Sim Pred: " << sim.play() << endl;
					//cerr << "Sim Count: " << sim.numSims << endl;
				}
				//cout << "6" << endl;
				fin.close();
				//fout << "Total Score: " << totalScore << endl;
				cout << "Total Score: " << totalScore << endl;
				currTotalScore += totalScore;
				totalScore = 0;
				cout << "CurrTotalScore: " << currTotalScore << endl;
			}
			currAverageScore = (float)currTotalScore / 20;
			if (currAverageScore > bestAverageScore)
			{
				bestNumClimbs = numclimbs;
				bestAverageScore = currAverageScore;
			}
			fout << "NUMCLIMBS: " << numclimbs << " | Average Score: " << currAverageScore << endl;
			cout << "NUMCLIMBS: " << numclimbs << " | Average Score: " << currAverageScore << endl;

			fout << "---- Current Best:" << "NUMCLIMBS: " << bestNumClimbs << " | Average Score: " << bestAverageScore << endl;
			cout << "---- Current Best:" << "NUMCLIMBS: " << bestNumClimbs << " | Average Score: " << bestAverageScore << endl;

			fout << endl;
			cout << endl;
		}
	

}
