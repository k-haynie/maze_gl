#include <random>
#include <vector>
#include <unordered_map>
#include "cell.h"

using namespace std;

class Ellers {
public:
	class RowState {
	public:
		unordered_map<vector<Cell*>, int> cells_in_set;
		unordered_map<int, vector<Cell*>> set_for_cell;
		int next_set = 0;
	};
}; 
