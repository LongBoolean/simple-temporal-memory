#ifndef CELL_H
#define CELL_H
#include <vector>
#include "cell_connection.h"
class Cell
{
public:
	Cell();
	~Cell();
	void setActiveStep(int step){active_step = step;}
	void setPredictedStep(int step){predicted_step = step;}
	int getActiveStep(){return active_step;}
	int getPredictedStep(){return predicted_step;}
	void setColumnIndex(int index){column_index = index;}
	int getColumnIndex(){return column_index;}
	void setCellIndex(int index){cell_index = index;}
	int getCellIndex(){return cell_index;}
	
	std::vector<Cell_connection *> receive_connection_vec;
	std::vector<Cell_connection *> send_connection_vec;


private:
	int active_step = -2;
	int predicted_step = -2;
	int column_index = -1;
	int cell_index = -1;

};
#endif
