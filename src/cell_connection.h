#ifndef CELL_CONNECTION_H
#define CELL_CONNECTION_H
class Cell;
class Cell_connection
{
public:
	Cell* cell_receive = nullptr;
	Cell* cell_send = nullptr;
	double strength = 0;

private:

};
#endif
