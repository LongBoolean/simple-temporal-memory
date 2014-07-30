#ifndef COLUMN_H
#define COLUMN_H
#include <vector>
#include "cell.h"
#include "input_connection.h"
class Column
{
public:
	Column();
	~Column();
	std::vector<Cell*> cell_vec;
	std::vector<Input_connection*> input_bit_connection_vec;
	void setIndex(int c_index){index = c_index;}
	int getIndex(){return index;}

private:
	int index = -1;

};
#endif
