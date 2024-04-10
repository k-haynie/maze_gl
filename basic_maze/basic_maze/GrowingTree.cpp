#include <vector>
#include <random>
#include "cell.h"

using namespace std;

class GrowingTree {
public:

	int randint(int high, int low = 0) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(low, high);
		return distr(generator);
	};

	void on(vector<vector<Cell*>> maze, bool last=false) {
		// random starting cell
		vector<Cell*> active;
		active.push_back(maze[randint(maze.size() - 1)][randint(maze[0].size() - 1)]);

		while (active.size() > 0) {
			Cell* cell;
			if (last) {
				cell = active[randint(active.size() - 1)];
			}
			else {
				cell = active[active.size()-1];
			}

			vector<Cell*> nbrs = cell->neighbors();

			if (nbrs.size() > 0) {
				Cell* nbr = nbrs[randint(nbrs.size() - 1)];
				cell->link(nbr);
				active.push_back(nbr);
			}
			else {
				active.erase(remove(active.begin(), active.end(), cell), active.end());
			}
		}
	}
};