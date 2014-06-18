#include "stm.h"
#include "cell_counter.h"
#include <cstdio>
#include <time.h>
#include <cstdlib>

Stm::Stm()
{
	randSeed = time(NULL);
	srand(randSeed);
}

Stm::~Stm()
{
	clean();
}

void Stm::init()
{
	if(has_init)
		clean();
	has_init = true;
	numColumns = initColumns;
	numCellsPerColumn = initCells;
	maxCellConnections = initMaxCellConnections;
	numInputConnections = initNumInputConnections;

	//set up the columns and cells
	for(int i=0; i<numColumns; i++)
	{
		//create columns
		Column* col = new Column();
		for(int j=0; j<numCellsPerColumn; j++)
		{
			//create cells
			Cell* cell = new Cell();
			cell->setColumnIndex(i);
			cell->setCellIndex(j);
			col->cell_vec.push_back(cell);
		}
		column_vec.push_back(col);
	}

	initConnections();
	


}

void Stm::initConnections()
{
	for(int i=0;i<column_vec.size();i++)
	{
		Column* col = column_vec[i];
		for(int j=0;j<col->cell_vec.size();j++)
		{
			Cell* cell = col->cell_vec[j];
			//create random connections
			int col_index = 0;
			int cell_index = 0;
			for(int k=0;k<maxCellConnections;k++)
			{
				col_index = rand() % numColumns;
				cell_index = rand() % numCellsPerColumn;
				new_cell_connection(cell, column_vec[col_index]->cell_vec[cell_index]);
			}
		}
	}

	//test create connection
//	new_cell_connection(column_vec[0]->cell_vec[0], column_vec[3]->cell_vec[2]);
//	new_cell_connection(column_vec[0]->cell_vec[0], column_vec[2]->cell_vec[2]);
//	new_cell_connection(column_vec[4]->cell_vec[0], column_vec[0]->cell_vec[0]);
}

void Stm::setColumnActive(int col_index)
{
	//store into inputs vector for later during processing
	if(col_index >=0 && col_index < numColumns)
	{
		//check for duplicates
		bool duplicate = false;
		for(int i=0;i<inputs_vec.size();i++)
		{
			if(inputs_vec[i] == col_index)
			{
				duplicate = true;
				break;
			}
		}
		if(!duplicate)
			inputs_vec.push_back(col_index);
	}
}

void Stm::clearActiveColumns()
{
	inputs_vec.clear();
}

void Stm::process()
{
	if(has_init)
	{
		current_step++;
		compute_active();
		make_predictions();	
		cell_pre_active_vec.clear();
		cell_pre_active_vec = cell_set_active_vec;
		cell_set_active_vec.clear();
	}

	inputs_vec.clear();

}

bool Stm::isColumnPredicted(int col_index)
{
	if(col_index >=0 && col_index < numColumns)
	{
		for(int i=0; i<column_vec[col_index]->cell_vec.size();i++)
		{
			if(column_vec[col_index]->cell_vec[i]->getPredictedStep() == current_step+1)
				return true;	
		}
	}
	return false; /////////
}

void Stm::compute_active()
{
	//fornow: go through all of the columns with input
	for(int i=0;i<inputs_vec.size();i++)
	{
		Column* col = column_vec[inputs_vec[i]];
		bool bursting = true;
		cell_temp_active_vec.clear();
		for(int j=0;j<col->cell_vec.size();j++)
		{
			Cell* cell = col->cell_vec[j];
			if(cell->getPredictedStep() == current_step)
			{
				bursting = false;
				cell_temp_active_vec.push_back(cell);
			}
		}
		//pick a random cell to become active
		if(!bursting)
		{
			int index = rand() % cell_temp_active_vec.size();
			Cell* cell = cell_temp_active_vec[index];
			//que the cell to become active 
			cell_set_active_vec.push_back(cell);
			//strengthen/weaken receive connections
			for(int k=0;learn && k<cell->receive_connection_vec.size();k++)
			{
				Cell_connection* con = cell->receive_connection_vec[k];
				if(con->cell_send->getActiveStep() == current_step-1)
				{
					if(con->strength < 1)
						con->strength+=learn_increment;
				}
				else
				{
					if(con->strength > 0)
						con->strength-=learn_decrement;
				}
			}

		}
		else//activate all the cells in a column when bursting
		{
			for(int j=0;j<col->cell_vec.size();j++)
			{
				Cell* cell = col->cell_vec[j];
				cell_set_active_vec.push_back(cell);
				if(learn)
					cell_make_connections_vec.push_back(cell);
			}
		}
	}
	//make cells active
	for(int i=0;i<cell_set_active_vec.size();i++)
	{
		Cell* cell = cell_set_active_vec[i];
		cell->setActiveStep(current_step);
	}
	//make random connectons with previously active cells
	if(cell_pre_active_vec.size()>0)
	{
		int pre_index = 0;
		for(int i=0;i<cell_make_connections_vec.size();i++)
		{
			pre_index = rand() % cell_pre_active_vec.size();
			new_cell_connection(cell_make_connections_vec[i], cell_pre_active_vec[pre_index]);
			///future: perhaps remove from pre_vec??
		}	
	}
	cell_make_connections_vec.clear();


}

void Stm::make_predictions()
{
	//go through all of the active cells to rate the possible predicted cells
	for(int i=0;i<cell_set_active_vec.size();i++)
	{
		Cell* cell = cell_set_active_vec[i];
		for(int j=0;j<cell->send_connection_vec.size();j++)//through all sending connections
		{
			Cell_connection* con = cell->send_connection_vec[j];
			if(con->strength >= predicted_min_strength)
			{
				bool inPre = false;
				Cell* next_cell = con->cell_receive;
				//count the number of times the cell is connected to an active cell
				for(int k=0;k<cell_count_predicted_vec.size();k++)
				{
					if(next_cell == cell_count_predicted_vec[k]->cell)
					{
						cell_count_predicted_vec[k]->count+=1;
						inPre = true;
						break;
					}

				}
				//if not all ready in the prediction vec, put it in
				if(!inPre)
				{
					Cell_counter* c_count = new Cell_counter();
					c_count->count = 1;
					c_count->cell = next_cell;
					cell_count_predicted_vec.push_back(c_count);	
				}
			}
		}
	}
	//use the calculated rating to set the predicted cells
	// and the delete the predicted vector
	for(int i=0;i<cell_count_predicted_vec.size();i++)
	{
		if(cell_count_predicted_vec[i]->count >= predicted_min_active)
		{
			cell_count_predicted_vec[i]->cell->setPredictedStep(current_step+1);
		}
		delete cell_count_predicted_vec[i];
	}
	cell_count_predicted_vec.clear();
}

void Stm::new_cell_connection(Cell* c_send, Cell* c_receive)
{
	//double strength = 0.1 + ((rand() % 5)/(double)10);
	double strength = 0.15 + ((rand() % 3)*0.05);
	new_cell_connection(c_send, c_receive, strength);
}

void Stm::new_cell_connection(Cell* c_send, Cell* c_receive, double strength)
{
	if(c_send != nullptr && c_receive != nullptr && c_send != c_receive)
	{
		bool connect = true;
		//check for duplicates
		for(int i=0;connect && i<c_send->send_connection_vec.size();i++)
		{
			Cell_connection* con = c_send->send_connection_vec[i];
			if(con->cell_send == c_send && con->cell_receive == c_receive)
			{
				connect = false;
			}
		}
		if(connect && c_send->send_connection_vec.size() >= maxCellConnections)
		{
			//future: find the worst "bad" connection and delete it, 
			//replacing with this new one.
			///for now just
			for(int i=0;i<c_send->send_connection_vec.size();i++)
			{
				Cell_connection* con = c_send->send_connection_vec[i];
				if(con->strength <= 0)
					delete_cell_connection(con);
			}
			connect = false;
		}
		//create connection
		if(connect)
		{
			Cell_connection* connection = new Cell_connection();
			connection->cell_send = c_send;
			connection->cell_receive = c_receive;
			connection->strength = strength;
			c_send->send_connection_vec.push_back(connection);
			c_receive->receive_connection_vec.push_back(connection);
		}
	}
}

void Stm::delete_cell_connection(Cell_connection* connection)
{
	if(connection != nullptr)
	{
		Cell* c_send = connection->cell_send;
		Cell* c_receive = connection->cell_receive;
		//remove the connection from the sending cell 
		for(int i=0;c_send!=nullptr && i<c_send->send_connection_vec.size();i++)
		{
			if(c_send->send_connection_vec[i] == connection)
			{
				c_send->send_connection_vec.erase(c_send->send_connection_vec.begin()+i);
			}
		}

		//remove the connection from the receiving cell 
		for(int i=0;c_receive!=nullptr && i<c_receive->receive_connection_vec.size();i++)
		{
			if(c_receive->receive_connection_vec[i] == connection)
			{
				c_receive->receive_connection_vec.erase(c_receive->receive_connection_vec.begin()+i);
			}
		}
		delete connection;
	}
}

void Stm::printAll()
{
	printSettings();
	printStatus();
	printInnerds();

}
void Stm::printSettings()
{

	printf("**************************************************************************\n");
	printf("****Stm Settings****\n");
	printf("numCellsPerColumn:%d\t", numCellsPerColumn);
	printf("numColumns:%d\t", numColumns);
	printf("maxCellConnections:%d\n", maxCellConnections);
	if(learn)
		printf("Learning:ON\t");
	else
		printf("Learning:OFF\t");
	printf("learn_increment:%f\t", learn_increment);
	printf("learn_decrement:%f\n", learn_decrement);
	printf("predicted:%f(min strength)   %d(min num active)\n", predicted_min_strength, predicted_min_active);
	
}
void Stm::printStatus()
{
	printf("**************************************************************************\n");
	printf("****Stm Status****\n");
	printf("current step: %d\n", current_step);
}
void Stm::printInnerds()
{

	printf("**************************************************************************\n");
	printf("****Stm Innerds****\n");
	printf("     \tColumn, Cell\tStrength|S cell|R cell\n");
	for(int i=0; i<column_vec.size(); i++)
	{
		Column* col = column_vec[i];
		for(int j=0; j<col->cell_vec.size(); j++)
		{
			Cell* cell = col->cell_vec[j];
			printf("Cell:\t%d, %d\n", cell->getColumnIndex(),cell->getCellIndex());
			printf("  active: %d \tpredicted: %d\n", cell->getActiveStep(),cell->getPredictedStep());

			for(int k=0; k < cell->send_connection_vec.size(); k++)
			{
				Cell_connection* con = cell->send_connection_vec[k];
				if(k==0)printf("\t\t\tSend Connections:\n");
				printf("\t\t\t%f\t%d, %d\t %d, %d\n",con->strength, con->cell_send->getColumnIndex(),con->cell_send->getCellIndex(), con->cell_receive->getColumnIndex(),con->cell_receive->getCellIndex());
			}
			for(int k=0; k < cell->receive_connection_vec.size(); k++)
			{
				Cell_connection* con = cell->receive_connection_vec[k];
				if(k==0)printf("\t\t\tReceive Connections:\n");
				printf("\t\t\t%f\t%d, %d\t %d, %d\n", con->strength,  con->cell_send->getColumnIndex(),con->cell_send->getCellIndex(), con->cell_receive->getColumnIndex(),con->cell_receive->getCellIndex());
			}

		}
	}
}

void Stm::clean()
{
	has_init = false;
	for(int i=0; i<column_vec.size(); i++)
	{
		Column* col = column_vec[i];
		for(int j=0; j<col->cell_vec.size(); j++)
		{
			Cell* cell = col->cell_vec[j];
			for(int k=0; k < cell->send_connection_vec.size(); k++)
			{
				Cell_connection* con = cell->send_connection_vec[k];
				delete_cell_connection(con);
			}
			//	delete cell;
		}
		//delete col;
	}
	for(int i=0; i<column_vec.size(); i++)
	{
		Column* col = column_vec[i];
		for(int j=0; j<col->cell_vec.size(); j++)
		{
			Cell* cell = col->cell_vec[j];
			delete cell;
		}
		delete col;
	}

	column_vec.clear();
}
