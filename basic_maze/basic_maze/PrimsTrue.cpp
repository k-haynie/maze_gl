#include <vector>
#include <random>
#include <unordered_map>
#include "cell.h"

using namespace std;

// Introduces randomized weights
class PrimsTrue {
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
		unordered_map<Cell*, int> costs;

		// randomizing weights
		for (int i = 0; i < maze.size(); i++) {
			for (int j = 0; j < maze[0].size(); j++) {
				costs[maze[i][j]] = randint(100);
			}
		}

		while (active.size() > 0) {
			Cell* cell = active[0];

			for (int i = 0; i < active.size(); i++) {
				if (costs[active[i]] < costs[cell])
					cell = active[i];
			}

			vector<Cell*> nbrs = cell->neighbors();

			if (nbrs.size() > 0) {
				Cell* nbr = nbrs[randint(nbrs.size() - 1)];

				for (int i = 0; i < nbrs.size(); i++) {
					if (costs[nbrs[i]] < costs[nbr])
						nbr = nbrs[i];
				}

				cell->link(nbr);
				active.push_back(nbr);
			}
			else {
				active.erase(remove(active.begin(), active.end(), cell), active.end());
			}
		}
	}
};