#include "cell.h"
#include <cstdio>

Cell::Cell()
{
	active_step = -2;
	predicted_step = -2;
//	printf("cell created\n");

}

Cell::~Cell()
{

//	printf("cell deleted\n");
}

