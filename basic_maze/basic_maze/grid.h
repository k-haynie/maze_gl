#ifndef Grid_h
#define Grid_h

#include <iostream>
#include <vector>
#include "cell.h"

using namespace std;

// build a 2-D vector to represent the grid
class Grid {
public:
	// populate a grid r * c units' worth of cells
	vector<vector<Cell*>> prepare_grid(int row, int col) {
		vector<vector<Cell*>> grid;

		for (int i = 0; i < row; i++) {
			// establish a temporary row
			vector<Cell*> temp;

			// add cells with appropriate x/y values
			for (int j = 0; j < col; j++) {
				Cell* cell = new Cell;
				cell->row = i;
				cell->col = j;
				temp.push_back(cell);
			};
			grid.push_back(temp);
		};
		return grid;
	};

	// takes an initialized grid and logically connects neighbors
	void configure_cells(int r, int c, vector<vector<Cell*>> maze) {
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				if (i > 0 && i < r) {
					maze[i][j]->north = maze[i - 1][j];
				};
				if (i >= 0 && i < r - 1) {
					maze[i][j]->south = maze[i + 1][j];
				};
				if (j > 0 && j < c) {
					maze[i][j]->west = maze[i][j - 1];
				};
				if (j >= 0 && j < c - 1) {
					maze[i][j]->east = maze[i][j + 1];
				};
			};
		};
	};
};

#endif
