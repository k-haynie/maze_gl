#include <random>
#include <vector>

#include "cell.h"

using namespace std;

class BinaryTree {
public:
	int randint(int high, int low = 0) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(low, high);
		return distr(generator);
	};

	void on(vector<vector<Cell*>> maze) {
		for (int i = 0; i < maze.size(); i++) {
			for (int j = 0; j < maze[0].size(); j++) {
				vector<Cell*> nbrs;

				if (maze[i][j]->north) {
					nbrs.push_back(maze[i][j]->north);
				};
				if (maze[i][j]->east) {
					nbrs.push_back(maze[i][j]->east);
				};

				if (nbrs.size() > 0) {
					int index = randint(nbrs.size() - 1);
					Cell* nbr = nbrs[index];
					maze[i][j]->link(nbr);
				}
			}
		};
	}
};