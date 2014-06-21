#include <iostream>
#include <cstdio>
#include <string>
#include <math.h>
#define PI 3.1415
#include "stm.h"
#include "cell.h"
#include "cell_connection.h"
#include "column.h"
void setup();
void clean();
void test_single();
void test_enum_like();

Stm* stm;
int cols;
int cells;
int connections;
std::string save_location = "../save/test.txt";

double sineWave(double amp, double freq, double t, double phase)
{
	return amp * sin(2*PI*freq*t + phase);
}

int main()
{
	setup();
	test_single();
	clean();	
	return 0;
}
void setup()
{
	cols = 30;
	cells = 30;
	connections = cols*cells*.4;
	printf("stm test\n");
	stm = new Stm();	
	//*
	stm->preInitCellsPerColumn(cells);
	stm->preInitNumColumns(cols);
	stm->preInitMaxCellConnections(connections);
	stm->preInitNumInputConnections(5);//currently does nothing
	stm->setLearning(true);
//	*/
	
	stm->init();
//	stm->initImport(save_location);
}
void test_single()
{
	int iterations = 100000;
	int y =0;
	int stats_after = 90000;
	int stat_no_prediction = 0;
	int stat_first_no_prediction = 0;
	int stat_last_no_prediction = 0;
	for(int i=0;i<iterations;i++)
	{
		y = sineWave(cols/2, 1000, i, 0) + cols/2;	
		stm->setColumnActive(y);
		stm->process();
		int no_p_count = cols;
		for(int k=0;k<cols;k++)
		{
			//if(stm->isColumnPredicted(k))
			//	printf(" %d",k);
			if(stm->isColumnPredicted(k) && k==y%30)
			{
				printf("X");
				no_p_count--;
			}
			else if(stm->isColumnPredicted(k))
			{
				printf("P");
				no_p_count--;
			}
			else if(k==y%cols)
			{
				printf(".");
			}
			else
			{
				printf(" ");
			}
		}
		if(i % cols == 0)
			printf("\tstep: %d\n", i);
		else
			printf("\n");
		if(no_p_count == cols)
		{
			stat_no_prediction++;
			if(stat_first_no_prediction == 0)
				stat_first_no_prediction = i;
			stat_last_no_prediction = i;
		}
	}
	stm->printSettings();
	stm->printStatus();
	printf("*****Test Summary*****\n");

	printf("Stat Num No Predictions: %d\n", stat_no_prediction);
	printf("Stat first No Prediction: %d\n", stat_first_no_prediction);
	printf("Stat last No Prediction: %d\n", stat_last_no_prediction);
	
	stm->exportFile(save_location);
	//stm->initImport(save_location);
}

void test_enum_like()
{

}
void clean()
{
	delete stm;
}
