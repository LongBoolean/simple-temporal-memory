#ifndef STM_H
#define STM_H
#include <vector>
#include <string>
#include "column.h"
#include "cell_counter.h"
#include "inputBit.h"
#include "inputEntry.h"
class Stm
{
public:
	Stm();
	~Stm();

	//init
	void preInitCellsPerColumn(int numCells){initCells = numCells;}
	void preInitNumColumns(int numColumns){initColumns = numColumns;}
	void preInitMaxCellConnections(int numConnections){initMaxCellConnections = numConnections;}
	void preInitNumInputConnections(int numConnections){initNumInputConnections = numConnections;}
	void init();
	void postInitFinalizeInputs();
	//input
	void postInitAddInputDouble(std::string identifier, double min, double max, int numBuckets, int bucketWidth, int inputWidth);
	void setInputBitActive(int input_bit_index); ////////future: use input parser instead of direct access///
	void clearInputBitActive(); ////////future: use input parser instead of direct access/////////////
	void addInputBit();
	void addInputBits(int num);
	void setInputEntryValue(std::string identifier, double value);
	
	//compute active
	//make predictions
	void process();
	
	//output predictions	
	bool isColumnPredicted(int col_index); ///////future: make private ///////////////////////////
	bool isInputBitPredicted(int input_bit_index); ///////future: make private ///////////////////////////
	bool isInputBitActive(int input_bit_index);
	double getInputEntryPrediction(std::string identifier);
	bool getInputEntryHasPrediction(std::string identifier);

	//file import/export 
	void exportFile(std::string file_path);
	bool initImport(std::string file_path);

	//other settings
	void setLearning(bool learning){learn = learning;}
	bool getLearning(){return learn;}
	void setPredictedMinActive(int min_active){predicted_min_active = min_active;}
	int getCellsPerColumn(){return numCellsPerColumn;}
	int getNumColumns(){return numColumns;}
	void printSettings();
	void printStatus();
	void printInnerds();
	void printAll();
	

private:
	std::vector<InputEntry*> entrys_vec; //store input entries
	std::vector<int> inputs_vec;//store the inputs bit activation
	std::vector<Column*> chosen_columns_vec;//store the columns to be activated
	std::vector<Column*> column_vec;//store the columns
	std::vector<InputBit*> input_bit_vec;//store the inputBits
	std::vector<int> column_connection_queue_vec;//columns queued to connect with inputbits
	std::vector<int> input_bit_connection_queue_vec;//input bits queued to connect with columns
	std::vector<Cell*> cell_pre_active_vec;
	std::vector<Cell*> cell_temp_active_vec;
	std::vector<Cell*> cell_set_active_vec;
	std::vector<Cell*> cell_make_connections_vec;
	std::vector<Cell_counter*> cell_count_predicted_vec;

	bool has_init = false;
	bool file_init = false;
	int initCells;
	int initColumns;
	int initMaxCellConnections;
	int initNumInputConnections;

	int current_step = 0;
	int current_num_input_bits = 0;
	int numCellsPerColumn;
	int numColumns;
	int maxCellConnections;
	int numInputConnections;
	bool learn;
	double temporal_learn_increment = 0.05;//future: needs setter
	double temporal_learn_decrement = 0.05;//future: needs setter
	double predicted_min_strength = 0.2;//future: needs setter
	int predicted_min_active = 1;
	double spatial_learn_increment = 0.02;//future: needs setter
	double spatial_learn_decrement = 0.02;//future: needs setter
	double overlap_min_strength = 0.2;//future: needs setter
	double overlap_chosen_percentage = 0.02;
	double trim_input_percentage = 0.2;//future: needs setter
	int spatial_min_active = 1;
	int randSeed;
	
	void initConnections();
	void initStructures();
	void initInputBitConnections();
	void updateBitsFromEntrys();
	void trimInputBits();
	void compute_overlap();
	void compute_active();
	void make_predictions();
	void new_cell_connection(Cell* c_send, Cell* c_receive);
	void new_cell_connection(Cell* c_send, Cell* c_receive, double strength);
	void delete_cell_connection(Cell_connection* connection);
	void new_input_bit_connection(InputBit* inputBit, Column* column, double strength);
	void delete_input_bit_connection(Input_connection* connection);
	void addInputEntry(std::string identifier, enum e_EntryType entryType, double min, double max, int numBuckets, int bucketWidth, int inputWidth);
	void clean();
};
#endif
