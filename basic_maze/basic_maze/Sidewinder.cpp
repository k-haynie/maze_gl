#include <iostream>
#include <random>
#include <vector>
#include "cell.h"

using namespace std;

class Sidewinder {
public:
	int randint(int high, int low = 0) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(low, high);
		return distr(generator);
	};

	void on(vector<vector<Cell*>> maze) {
		for (int i = 0; i < maze.size(); i++) {

			vector<Cell*> run;

			for (int j = 0; j < maze[0].size(); j++) {
				run.push_back(maze[i][j]);
				bool east = (maze[i][j]->east == nullptr);
				bool north = (maze[i][j]->north == nullptr);

				bool halt = (east || (!north && (rand() % 2 == 0)));

				if (halt) {

					Cell* member = run[randint(run.size() - 1)];

					if (member->north) {
						member->link(member->north);
					};

					run.clear();
				}
				else {
					maze[i][j]->link(maze[i][j]->east);
				}
			};
		}
	};
};