#include "stm.h"
#include "cell_counter.h"
#include <cstdio>
#include <fstream>
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
	initStructures();
	initConnections();
}
void Stm::initStructures()
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
}

void Stm::exportFile(std::string file_path)
{
	std::ofstream out_file(file_path);
	out_file << "#STM save file: " << file_path << "\n";
	out_file << "#WARNING: Messing around with this file WILL cause problems.\n";
	out_file << "#***********************************************************\n";
	//export settings
	out_file << "csp " << current_step <<"\n";
	out_file << "rnd " << randSeed <<"\n";
	out_file << "cpc " << numCellsPerColumn << "\n";
	out_file << "ncm " << numColumns << "\n";
	out_file << "mcc " << maxCellConnections << "\n";
	out_file << "nic " << numInputConnections << "\n";
	out_file << "lrn " << learn << "\n";
	out_file << "lri " << learn_increment << "\n";
	out_file << "lrd " << learn_decrement << "\n";
	out_file << "pms " << predicted_min_strength << "\n";
	out_file << "pma " << predicted_min_active << "\n";
	out_file << "ini*********************************************************\n";
	//export cell data
	for(int i=0;i<column_vec.size();i++)
	{
		Column* col = column_vec[i];
		for(int j=0;j<col->cell_vec.size();j++)
		{
			Cell* cell = col->cell_vec[j];
			//format:   "cl column_index cell_index active_step predicted_step 
			out_file << "cll " << cell->getColumnIndex() << " " << cell->getCellIndex() << " "
				<< cell->getActiveStep() << " " << cell->getPredictedStep() << "\n";
		}
	}

	//export connections
	for(int i=0;i<column_vec.size();i++)
	{
		Column* col = column_vec[i];
		for(int j=0;j<col->cell_vec.size();j++)
		{
			Cell* cell = col->cell_vec[j];
			for(int k=0;k<cell->send_connection_vec.size();k++)
			{
				Cell_connection* con = cell->send_connection_vec[k];	
				//format:   "cn send_col send_cell receive_col receive_cell
				if(con->strength > 0 && con->strength <= 1)//0.000001
				{
					out_file << "con " << con->cell_send->getColumnIndex() << " "
						<< con->cell_send->getCellIndex() << " " 
						<< con->cell_receive->getColumnIndex() << " "
						<< con->cell_receive->getCellIndex() << " "
						<< con->strength << "\n";
				}
			}
		}
	}




	out_file.close();
}

bool Stm::initImport(std::string file_path)
{
	std::ifstream in_file(file_path);
	if(!in_file.is_open())
	{
		printf("Not opened\n");
		return false;
	}
	char buf[256];
	while(!in_file.eof())
	{
		bool proceed = false;
		in_file.getline(buf, 256);
		std::string line(buf);
		if(line[0] == '#')
			continue;
		else if(line[0] == 'c' && line[1] == 's' && line[2] == 'p')
		{
			int temp = 0;
			sscanf(line.c_str(), "csp %d", &temp);
			current_step = temp;
		}	
		else if(line[0] == 'r' && line[1] == 'n' && line[2] == 'd')
		{
			sscanf(line.c_str(), "rnd %d", &randSeed);
		}	
		else if(line[0] == 'c' && line[1] == 'p' && line[2] == 'c')
		{
			sscanf(line.c_str(), "cpc %d", &initCells);
		}	
		else if(line[0] == 'n' && line[1] == 'c' && line[2] == 'm')
		{
			sscanf(line.c_str(), "ncm %d", &initColumns);
		}	
		else if(line[0] == 'm' && line[1] == 'c' && line[2] == 'c')
		{
			sscanf(line.c_str(), "mcc %d", &initMaxCellConnections);
		}	
		else if(line[0] == 'n' && line[1] == 'i' && line[2] == 'c')
		{
			sscanf(line.c_str(), "nic %d", &initNumInputConnections);
		}	
		else if(line[0] == 'l' && line[1] == 'r' && line[2] == 'n')
		{
			int temp = 0;
			sscanf(line.c_str(), "lrn %d", &temp);
			if(temp==1) learn = true;
			else learn = false;
		}	
		else if(line[0] == 'l' && line[1] == 'r' && line[2] == 'i')
		{
			float temp = 0.0;
			sscanf(line.c_str(), "lri %f", &temp);
			learn_increment = temp;
		}	
		else if(line[0] == 'l' && line[1] == 'r' && line[2] == 'd')
		{
			float temp = 0.0;
			sscanf(line.c_str(), "lrd %f", &temp);
			learn_decrement = temp;
		}	
		else if(line[0] == 'p' && line[1] == 'm' && line[2] == 's')
		{
			float temp = 0.0;
			sscanf(line.c_str(), "pms %f", &temp);
			predicted_min_strength = temp;
		}	
		else if(line[0] == 'p' && line[1] == 'm' && line[2] == 'a')
		{
			sscanf(line.c_str(), "pma %d", &predicted_min_active);
		}	
		else if(line[0] == 'i' && line[1] == 'n' && line[2] == 'i')//init Structure
		{
			initStructures();
		}	
		else if(line[0] == 'c' && line[1] == 'l' && line[2] == 'l')//import cell data
		{
			int col = 0;
			int cell = 0;
			int actv_step = 0;
			int pred_step = 0;
			sscanf(line.c_str(), "cll %d %d %d %d", &col, &cell, &actv_step, &pred_step);
			
			if(col < numColumns && cell < numCellsPerColumn && col > 0 && cell > 0)
			{
				column_vec[col]->cell_vec[cell]->setActiveStep(actv_step);
				column_vec[col]->cell_vec[cell]->setPredictedStep(pred_step);
			}
		}	
		else if(line[0] == 'c' && line[1] == 'l' && line[2] == 'l')//import connection data
		{
			int col1 = 0;
			int cell1 = 0;
			int col2 = 0;
			int cell2 = 0;
			double strength = 0.0;
			float temp = 0.0;
			sscanf(line.c_str(), "con %d %d %d %d %f", &col1, &cell1, &col2, &cell2, &temp);
			strength = temp;
			if(col1 > 0 && cell1 > 0 && col2 > 0 && cell2 > 0)
			{
				if(col1 < numColumns && col2 < numColumns && cell1 < numCellsPerColumn && cell2 < numCellsPerColumn)
				{
					Cell* c_send = column_vec[col1]->cell_vec[cell1];
					Cell* c_receive = column_vec[col2]->cell_vec[cell2];
					new_cell_connection(c_send,c_receive, strength);
				}
			}
		}
		
	}
	in_file.close();	
	return true;
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
		if(bursting)//activate all the cells in a column when bursting
		{
			for(int j=0;j<col->cell_vec.size();j++)
			{
				Cell* cell = col->cell_vec[j];
				cell_set_active_vec.push_back(cell);
				if(learn)
				{
					cell_make_connections_vec.push_back(cell);
				}

			}
			//strengthen/weaken receive connections
			int index = rand() % col->cell_vec.size();
			Cell* cell = col->cell_vec[index];
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
			new_cell_connection(cell_make_connections_vec[i], cell_pre_active_vec[pre_index], 0.1);
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
				if(con->strength < 0.000001)
				{
					delete_cell_connection(con);
				}
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
