#ifndef COLUMN_H
#define COLUMN_H
#include <vector>
#include "cell.h"
class Column
{
public:
	Column();
	~Column();
	std::vector<Cell*> cell_vec;

private:

};
#endif
