#include <vector>
#include <random>
#include "cell.h"

using namespace std;

class PrimsSimp {
public:

	int randint(int high, int low = 0) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(low, high);
		return distr(generator);
	};

	void on(vector<vector<Cell*>> maze) {
		// random starting cell
		vector<Cell*> active = { maze[randint(maze.size() - 1)][randint(maze[0].size() - 1)] };

		while (active.size() > 0) {
			Cell* cell = active[randint(active.size() - 1)];
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