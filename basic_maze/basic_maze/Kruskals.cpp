#include <random>
#include <vector>
#include <map>
#include "cell.h"

using namespace std;

// RANDOMIZED Kruskal's
class Kruskals {
public:

	class State {
	public:
		vector<vector<Cell*>> nbrs;
		map<Cell*, int> set_for_cell;
		map<int, vector<Cell*>> cells_in_set;

		// constructor
		State(vector<vector<Cell*>> maze) {
			for (int i = 0; i < maze.size(); i++) {
				for (int j = 0; j < maze[0].size(); j++) {
					// create a unique set identifier
					int set_num = (i * maze[0].size()) + j;
					Cell* cell = maze[i][j];

					// map cell to set identifier in set_for_cell
					set_for_cell[cell] = set_num;
					// map set_num to a vector of *cells in cells_in_set
					cells_in_set[set_num] = { cell };

					if (cell->south) {
						nbrs.push_back({ cell, cell->south });
					}
					if (cell->east) {
						nbrs.push_back({ cell, cell->east });
					}
				}
			}
		}

		bool can_merge(Cell* left, Cell* right) {
			return set_for_cell[left] != set_for_cell[right];
		}

		void merge(Cell* left, Cell* right) {
			left->link(right);

			int winner = set_for_cell[left];
			int loser = set_for_cell[right];
			vector<Cell*> losers = cells_in_set[loser];

			for (int i = 0; i < losers.size(); i++) {
				cells_in_set[winner].push_back(losers[i]);
				set_for_cell[losers[i]] = winner;
			}

			cells_in_set.erase(loser);
		}
	};

	int randint(int high, int low = 0) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(low, high);
		return distr(generator);
	};

	void on(vector<vector<Cell*>> maze) {
		State state(maze);

		while (state.nbrs.size() > 0) {
			vector<Cell*> pair = state.nbrs[randint(state.nbrs.size() - 1)];
			state.nbrs.erase(remove(state.nbrs.begin(), state.nbrs.end(), pair), state.nbrs.end());
			Cell* left = pair[0];
			Cell* right = pair[1];

			if (state.can_merge(left, right)) {
				state.merge(left, right);
			}
		}

	}

};