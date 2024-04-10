#include <iostream>
#include <random>
#include <vector>
#include "cell.h"

class HuntAndKill {
public:
	bool nbr_visited(Cell* cell) {
		if (cell->north && cell->north->links().size() >= 1) {
			return true;
		}
		else if (cell->south && cell->south->links().size() >= 1) {
			return true;
		}
		else if (cell->east && cell->east->links().size() >= 1) {
			return true;
		}
		else if (cell->west && cell->west->links().size() >= 1) {
			return true;
		}
		else {
			return false;
		}
	};

	void on(vector<vector<Cell*>> maze, int st_x, int st_y) {
		Cell* current = maze[st_x][st_y];

		// transform to a do/while?
		while (current) {
			vector<Cell*> to_visit = current->neighbors();

			if (to_visit.size() > 0) {
				Cell* nbr = to_visit[rand() % to_visit.size()];
				// cout << current->row << current->col << " to " << nbr->row << nbr->col << endl;
				current->link(nbr);
				current = nbr;
			}
			else {
				current = nullptr;

				for (int i = 0; i < maze.size(); i++) {
					for (int j = 0; j < maze[0].size(); j++) {
						if (maze[i][j]->links().size() == 0 && nbr_visited(maze[i][j])) {
							current = maze[i][j];

							vector<Cell*> to_visit = current->neighbors(true);
							Cell* nbr = to_visit[rand() % to_visit.size()];
							// cout << current->row << current->col << " to " << nbr->row << nbr->col << endl;
							current->link(nbr);
							current = nbr;
						}
					};
				};
			};
		};
		cout << "Hunt and Kill" << endl;
	};
};
