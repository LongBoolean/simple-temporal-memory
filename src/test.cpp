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
void test_sine_wave();
void test_rock_paper_scissors();
void test_enum_like();

Stm* stm;
int cols;
int cells;
int connections;
int predictedMinActive = 1;
std::string save_location = "../save/test.txt";

double sineWave(double amp, double freq, double t, double phase)
{
	return amp * sin(2*PI*freq*t + phase);
}
void menu()
{
	printf("Simple Temporal Memory tests\n");
	printf("Tests to run:\n1\tSine Wave Prediction\n2\trock paper scissors game\n");
	printf("0\texit\n");
	int choice = 0;
	printf("Choice: ");
	std::cin >> choice;
	printf("\n");
	switch(choice)
	{
		case 1:
			test_sine_wave();
			break;
		case 2:
			test_rock_paper_scissors();
			break;
		default:
			break;
	}

}

int main()
{
	menu();
	return 0;
}
void setup()
{
	printf("stm test\n");
	stm = new Stm();	
	//*
	stm->preInitCellsPerColumn(cells);
	stm->preInitNumColumns(cols);
	stm->preInitMaxCellConnections(connections);
	stm->preInitNumInputConnections(5);//currently does nothing
	stm->setPredictedMinActive(predictedMinActive);
	stm->setLearning(true);
//	*/
	
	//stm->init();
	//if(!stm->initImport(save_location))
	{
		stm->init();
	}
}
void test_rock_paper_scissors()
{
	cols = 12;
	cells = 20;
	connections = cols*cells*.9;
	predictedMinActive = 2;
	setup();
	printf("RockPaperScissors test\n");
	printf("1: Rock\t2: Paper\t3:Scissors\t0: exit\n");
	int count = 0;
	bool done = false;
	int player_choice = 0;
	int computer_choice = rand()%3 + 1;
	bool player_win = false;
	bool computer_win = false;
	bool tie = false;
	int stat_player_wins = 0;
	int stat_computer_wins = 0;
	int stat_ties = 0;
	bool print = false;
	//while(!done)
	while(count < 10000)
	{
		count++;
		if(print && count%10==0)
		{
			printf("\nRockPaperScissors test\n");
			printf("1: Rock\t2: Paper\t3:Scissors\t0: exit\n\n");
			
		}
		//get player input
		//std::cin >> player_choice;
		player_choice = count % 3 + 1;
		if(player_choice > 0 && player_choice < 4 )
		{
			//calculate winner
			tie = false;
			player_win = false;
			computer_win = false;
			if(player_choice == computer_choice)
			{
				tie = true;
				stat_ties++;
			}
			else if(player_choice == 1 && computer_choice == 2)
			{
				computer_win = true;
				stat_computer_wins++;
			}
			else if(player_choice == 2 && computer_choice == 1)
			{
				player_win = true;
				stat_player_wins++;
			}
			else if(player_choice == 1 && computer_choice == 3)
			{
				player_win = true;
				stat_player_wins++;
			}
			else if(player_choice == 3 && computer_choice == 1)
			{
				computer_win = true;
				stat_computer_wins++;
			}
			else if(player_choice == 3 && computer_choice == 2)
			{
				player_win = true;
				stat_player_wins++;
			}
			else if(player_choice == 2 && computer_choice == 3)
			{
				computer_win = true;
				stat_computer_wins++;
			}
			//output results
			if(print)
			{
				printf("Player: %s\t Computer: %s\t\t%s\n", 
						(player_choice==1)?("Rock"):((player_choice==2)?("Paper"):("Scissors")),
						(computer_choice==1)?("Rock"):((computer_choice==2)?("Paper"):("Scissors")),
						(player_win)?("PLayer Wins!!!"):((computer_win)?("Computer Wins"):("It's a Tie")));
			}

			//feed data into stm
			/* How the data is stored.....
			   -Things to store
			   			r  p  s
			   --Player choice 	0  1  2
			   --Computer choice	3  4  5 

			   			t  f
			   --Player Win		6  7 
			   --Computer Win	8  9 
			   --Tie		10 11

			 */
			switch(player_choice)
			{
				case 1:
					stm->setColumnActive(0);
					break;
				case 2:
					stm->setColumnActive(1);
					break;
				case 3:
					stm->setColumnActive(2);
					break;
				default:
					break;
			}
			switch(computer_choice)
			{
				case 1:
					stm->setColumnActive(3);
					break;
				case 2:
					stm->setColumnActive(4);
					break;
				case 3:
					stm->setColumnActive(5);
					break;
				default:
					break;
			}
			if(player_win)
			{
				stm->setColumnActive(6);
			}
			else
			{
				stm->setColumnActive(7);
			}
			if(computer_win)
			{
				stm->setColumnActive(8);
			}
			else
			{
				stm->setColumnActive(9);
			}
			if(tie)
			{
				stm->setColumnActive(10);
			}
			else
			{
				stm->setColumnActive(11);
			}

			//stm makes choice for next round
			stm->process();
			if(stm->isColumnPredicted(8))//predict win
			{
				if(stm->isColumnPredicted(3))
					computer_choice = 1;//rock
				else if(stm->isColumnPredicted(4))
					computer_choice = 2;//paper
				else if(stm->isColumnPredicted(5))
					computer_choice = 3;//sissors
			}
			else if(stm->isColumnPredicted(9))//predict loss
			{
				if(stm->isColumnPredicted(3))
					computer_choice = 1;//rock
				else if(stm->isColumnPredicted(4))
					computer_choice = 2;//paper
				else if(stm->isColumnPredicted(5))
					computer_choice = 3;//sissors
				int rnum = rand() % 2 + 1;
				if(computer_choice == 1)
				{
					computer_choice += rnum;
				}
				else if(computer_choice == 2)
				{
					computer_choice += rnum;
					if(computer_choice == 4)
						computer_choice = 1;
				}
				else
				{
					computer_choice -= rnum;
				}
	/*
				if(stm->isColumnPredicted(0))
					computer_choice = 2;//rock
				else if(stm->isColumnPredicted(1))
					computer_choice = 3;//paper
				else if(stm->isColumnPredicted(2))
					computer_choice = 1;//sissors
			*/
			}
			else//tie
			{
				if(stm->isColumnPredicted(3))
					computer_choice = 1;//rock
				else if(stm->isColumnPredicted(4))
					computer_choice = 2;//paper
				else if(stm->isColumnPredicted(5))
					computer_choice = 3;//scissors
			}

		}
		else
		{
			done = true;
		}
	}
	printf("RockPaperScissors Stats\n");
	printf("Player Wins: %d\nComputer Wins: %d\nTies: %d\n", stat_player_wins, stat_computer_wins, stat_ties);
	stm->exportFile(save_location);
	clean();	
}
void test_sine_wave()
{
	cols = 30;
	cells = 30;
	connections = cols*cells*.4;
	predictedMinActive = 1;
	setup();
	int iterations = 500000;
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
	clean();
}

void test_enum_like()
{

}
void clean()
{
	delete stm;
}
