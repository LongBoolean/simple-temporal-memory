#include <iostream>
#include <cstdio>
#include "stm.h"
#include "cell.h"
#include "cell_connection.h"
#include "column.h"


int main()
{
	int cols = 10;
	int cells = 10;
	int connections = cols*cells*.8;
	printf("stm test\n");
	Stm* stm = new Stm();	
	stm->preInitCellsPerColumn(cells);
	stm->preInitNumColumns(cols);
	stm->preInitMaxCellConnections(connections);
	stm->preInitNumInputConnections(5);
	stm->setLearning(true);
	
	stm->init();

	//stm->setColumnActive(0);
	//stm->printStm();
	int count = 0;
	int iterations = 100;
	for(int i=0;i<iterations;i++)
	{
		for(int j=0;j<cols;j++)
		{
			//////testing purposes
			//if(j==0)
			//	stm->setColumnActive(cols-1);
		//	else
		//		stm->setColumnActive(j-1);
			stm->setColumnActive(j);
		//	if(j==4)
		//		stm->setColumnActive(0);
		//	else
		//		stm->setColumnActive(j+1);
			stm->process();
			printf("Step: %d\n", count);
			printf(" col: %d\n", j);
			printf("\t");
			for(int k=0;k<cols;k++)
			{
				if(stm->isColumnPredicted(k))
					printf(" %d",k);
			}
			printf("\n");
			count++;
		}
	}
	//stm->process();
	//stm->isColumnPredicted(0);

	//stm->printStm();

	delete stm;
	
	return 0;
}
