#include <vector>
#include <iostream>
#include "cell.h"

using namespace std;

// Recursive Backtracking algorithm
class RecBktr {
public:
	void on(vector<vector<Cell*>> grid, int st_x, int st_y) {
		vector<Cell*> stack;
		stack.push_back(grid[st_x][st_y]);

		while (stack.size() > 0) {
			Cell* current = stack.back();
			vector<Cell*> neighbors = current->neighbors();

			if (neighbors.size() == 0) {
				stack.pop_back();
			}
			else {
				Cell* neighbor = neighbors[rand() % neighbors.size()];
				//cout << "Linking: " << current->row << current->col << " with " << neighbor->row << neighbor->col << endl;
				current->link(neighbor);
				stack.push_back(neighbor);
			}
		};

		cout << "Recursive Backtracking" << endl;
	};
};