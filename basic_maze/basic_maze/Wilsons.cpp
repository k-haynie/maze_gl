#include <random>
#include <vector>
#include "cell.h"

using namespace std;

class Wilsons {
public:
	int randint(int high, int low = 0) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(low, high);
		return distr(generator);
	};

	void on(vector<vector<Cell*>> maze) {

		// create a vector of pointers to all unvisited cells
		vector<Cell*> unvisited;
		for (int i = 0; i < maze.size(); i++) {
			for (int j = 0; j < maze[0].size(); j++) {
				unvisited.push_back(maze[i][j]);
			}
		}

		// choose a random starting cell and mark it as visited
		Cell* first = unvisited[randint(unvisited.size() - 1)];
		unvisited.erase(remove(unvisited.begin(), unvisited.end(), first), unvisited.end());


		// while there are unvisited cells
		while (unvisited.size() > 0) {
			// choose a root cell and build a random walk path from it
			Cell* cell = unvisited[randint(unvisited.size() - 1)];
			vector<Cell*> path;
			path.push_back(cell);

			// while chosen cell is unvisited
			// std::find returns an iterator, and if the element is not found,
			//		the iterator points to beyond the end of the vector for whatever reason.

			while (find(unvisited.begin(), unvisited.end(), cell) != unvisited.end()) {
				// walk to a random neighbor
				cell = cell->adj()[randint(cell->adj().size() - 1)];
				bool traced = find(path.begin(), path.end(), cell) != path.end();

				if (traced) {
					// truncate path if a loop has been created
					path.resize(find(path.begin(), path.end(), cell) - path.begin() + 1);
				}
				else {
					// otherwise mark new addition to path
					path.push_back(cell);
				}
			}
			// mark every cell in the path as visited
			for (int i = 0; i + 1 < path.size(); i++) {
				path[i]->link(path[i + 1]);
				unvisited.erase(remove(unvisited.begin(), unvisited.end(), path[i]), unvisited.end());
			}
		}
	}
};