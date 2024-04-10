#include <vector>
#include "cell.h"
#include <iostream>
#include <random>

using namespace std;

class AldousBroder {
public:
	int randint(int high, int low = 0) {

		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(low, high);
		return distr(generator);
	};

	void on(vector<vector<Cell*>> maze) {
		Cell* cell = maze[randint(maze.size() - 1)][randint(maze[0].size() - 1)];
		int unvisited = (maze.size() * maze[0].size()) - 1;

		while (unvisited > 0) {

			if (cell->neighbors().size() != 0) {
				int index = rand() % cell->neighbors().size();
				Cell* nbr = cell->neighbors()[index];
				cell->link(nbr);
				unvisited -= 1;
				cell = nbr;
			}

			cell = cell->neighbors(true)[randint(cell->neighbors(true).size() - 1)];
		}
	}
};