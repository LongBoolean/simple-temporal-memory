#include <iostream>
#include <cstdio>
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
	connections = cols*cells*.8;
	printf("stm test\n");
	stm = new Stm();	
	stm->preInitCellsPerColumn(cells);
	stm->preInitNumColumns(cols);
	stm->preInitMaxCellConnections(connections);
	stm->preInitNumInputConnections(5);
	stm->setLearning(true);
	
	stm->init();
}
void test_single()
{
	int iterations = 100000;
	int y =0;
	for(int i=0;i<iterations;i++)
	{
		y = sineWave(cols/2, 1000, i, 0) + cols/2;	
		stm->setColumnActive(y);
		stm->process();
		for(int k=0;k<cols;k++)
		{
			//if(stm->isColumnPredicted(k))
			//	printf(" %d",k);
			if(stm->isColumnPredicted(k) && k==y%30)
				printf("X");
			else if(stm->isColumnPredicted(k))
				printf("P");
			else if(k==y%cols)
				printf(".");
			else
				printf(" ");
		}
		if(i % cols == 0)
			printf("\tstep: %d\n", i);
		else
			printf("\n");
	}
	stm->printSettings();
	stm->printStatus();
}

void test_enum_like()
{

}
void clean()
{
	delete stm;
}
