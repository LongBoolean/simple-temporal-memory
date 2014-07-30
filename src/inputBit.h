#ifndef INPUT_BIT_H
#define INPUT_BIT_H
#include <vector>
#include "input_connection.h"
class InputBit
{
public:
	//InputBit();
	int getPredictedStep(){return predicted_step;}	
	void setPredictedStep(int step){predicted_step = step;}	
	int getActiveStep(){return active_step;}	
	void setActiveStep(int step){active_step = step;}	
	int getInputIndex(){return input_index;}
	void setInputIndex(int index){input_index = index;}
	std::vector<Input_connection*> input_bit_connection_vec;

private:
	int active_step = -2;
	int predicted_step = -2;
	int input_index = -1;


};
#endif
