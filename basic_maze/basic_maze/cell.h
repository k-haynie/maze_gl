#ifndef Cell_h
#define Cell_h

#include <vector>
#include <iostream>

using namespace std;

// a class with row/column properties and pointers/boolean values for neighboring cells
class Cell {
public:
	// int and col variables for x and y cell value
	int row, col;

	// pointers to neighbors
	Cell* north, * south, * east, * west;

	// boolean values for linked cells
	bool n_conn, s_conn, e_conn, w_conn = false;

	// given a cell, modify the appropriate connection booleans
	void link(Cell* cell, bool join = true) {
		if (cell == north) {
			n_conn = join;
			cell->s_conn = join;
		}
		else if (cell == south) {
			s_conn = join;
			cell->n_conn = join;
		}
		else if (cell == east) {
			e_conn = join;
			cell->w_conn = join;
		}
		else if (cell == west) {
			w_conn = join;
			cell->e_conn = join;
		}
		else {
			cout << "Invalid connection" << endl;
		}
	};

	// compile a vector of linked cells
	vector<Cell*> links() {
		vector<Cell*> l;
		if (n_conn) {
			l.push_back(north);
		};
		if (s_conn) {
			l.push_back(south);
		};
		if (e_conn) {
			l.push_back(east);
		};
		if (w_conn) {
			l.push_back(west);
		};
		return l;
	}

	// check if a given cell is linked
	bool linked(Cell* cell) {
		if (cell == north) {
			return n_conn;
		}
		else if (cell == south) {
			return s_conn;
		}
		else if (cell == east) {
			return e_conn;
		}
		else if (cell == west) {
			return w_conn;
		}
	}

	// check if a given cell has been visited
	bool visited(Cell* nbr) {
		if (nbr->n_conn || nbr->e_conn || nbr->w_conn || nbr->s_conn) {
			return true;
		};
		return false;
	};

	vector<Cell*> adj() {
		vector<Cell*> adj;
		if (north) {
			adj.push_back(north);
		}
		if (south) {
			adj.push_back(south);
		}
		if (east) {
			adj.push_back(east);
		}
		if (west) {
			adj.push_back(west);
		}
		return adj;
	}

	// return unvisited neighbors
	vector<Cell*> neighbors(bool visit=false) {
		vector<Cell*> neighbor;

		if ( (!visit && north && !visited(north)) || (visit && north && visited(north)) ) {
			neighbor.push_back(north);
		};
		if ((!visit && south && !visited(south)) || (visit && south && visited(south))) {
			neighbor.push_back(south);
		};
		if ((!visit && east && !visited(east)) || (visit && east && visited(east))) {
			neighbor.push_back(east);
		};
		if ((!visit && west && !visited(west)) || (visit && west && visited(west))) {
			neighbor.push_back(west);
		};
		return neighbor;
	};
};

#endif