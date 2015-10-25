#include <iostream>
#include <cstdio>
#include <unistd.h>
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
void test_two_sine_wave();
void test_rand_num();
void test_rock_paper_scissors();
void test_enum_like();
void test_enum();
void test_letters();
void test_words();
void test_follow();

Stm* stm;
int numberOfBits = 0;
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
	printf("Tests to run:\n1\tSine Wave Prediction\n2\tSummed Sine Waves Prediction\n3\trandom number prediction\n");//4\trock paper scissors game\n");
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
			test_two_sine_wave();
			break;
		case 3:
			test_rand_num();
			break;
		case 4:
			test_rock_paper_scissors();
			break;
		default:
			break;
	}

}

int main()
{
//	menu();
//	test_sine_wave();
//	test_words();
//	test_two_sine_wave();
//	test_rock_paper_scissors();
//	test_follow();
	test_enum();
	return 0;
}
void setup()
{
	printf("stm setup\n");
	stm = new Stm();	
	//*
	stm->preInitCellsPerColumn(cells);
	stm->preInitNumColumns(cols);
	stm->preInitMaxCellConnections(connections);
	stm->preInitNumInputConnections(5);//currently does nothing
	stm->setPredictedMinActive(predictedMinActive);
	stm->setLearning(true);
	
	bool genBits = false;
	//if(!stm->initImport(save_location))
	{
		stm->init();
		genBits = true;
	}

/*
	printf("creating input bits...\n");
	if(genBits)
	{
		for(int i=0; i<numberOfBits;i++)
		{
			stm->addInputBit();

		}
	}
*/

}
void test_enum()
{
	cols = 9;//12;
	cells = 20;
	connections = cols*cells*.9;
	predictedMinActive = 1;
	numberOfBits = 12;
	
	//define enums 
	stm->preInitDefineEnum("choice",10,5);
	stm->preInitDefineEnum("outcome",10,5);
	setup();
	
	//create enum types
	stm->postInitAddEnumType("choice", "r");
	stm->postInitAddEnumType("choice", "p");
	stm->postInitAddEnumType("choice", "s");
	stm->postInitAddEnumType("outcome", "win");
	stm->postInitAddEnumType("outcome", "loss");
	stm->postInitAddEnumType("outcome", "tie");


	stm->postInitAddInputEnum("p_choice","choice");
	stm->postInitAddInputEnum("c_choice","choice");
	//stm->postInitAddInputDouble("p_choice",1,3,8,2,2);
	//stm->postInitAddInputDouble("c_choice",1,3,8,2,2);
	stm->postInitAddInputEnum("p_state","outcome");
	//stm->postInitAddInputDouble("c_state","outcome");
	
	//stm->postInitAddInputDouble("p_win",1,4,8,2,2);
	//stm->postInitAddInputDouble("c_win",1,4,8,2,2);
	//stm->postInitAddInputDouble("tie",1,4,8,2,2);
	
	//set overlap weights
	stm->setInputEntryOverlap("c_choice", 2.0);
	stm->postInitFinalizeInputs();

	printf("RockPaperScissors test\n");
	printf("1: Rock \t2: Paper \t3:Scissors \t0: exit\n");
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
	bool incr = true;

	printf("Please wait for results...\n");
	//while(!done)
	while(count < 1000000 && !done)
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
			std::string player_str = "";
			std::string computer_str = "";
			switch(player_choice)
			{
				case 0:
					player_str = "r";
					break;
				case 1:
					player_str = "p";
					break;
				case 2:
					player_str = "s";
					break;
			}
			switch(computer_choice)
			{
				case 3:
					computer_str = "r";
					break;
				case 4:
					computer_str = "p";
					break;
				case 5:
					computer_str = "s";
					break;
			}
			stm->setInputEnumValue("p_choice", player_str);
			stm->setInputEnumValue("c_choice", computer_str);
			if(player_win)
			{
				printf("Player\t\t");
				stm->setInputEntryValue("p_win", 3);
			}
			else
			{
				stm->setInputEntryValue("p_win", 2);
			}
			if(computer_win)
			{
				printf("Computer\t");
				stm->setInputEntryValue("c_win", 3);
			}
			else
			{
				stm->setInputEntryValue("c_win", 2);
			}
			if(tie)
			{
				printf("Tie\t\t");
				stm->setInputEntryValue("tie", 3);
			}
			else
			{
				stm->setInputEntryValue("tie", 2);
			}

			//stm makes choice for next round
			stm->process();
			double raw_c_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);

			//printf("c_win: %f\n", stm->getInputEntryPrediction("c_win"));
			//printf("p_win: %f\n", stm->getInputEntryPrediction("p_win"));
			if(stm->getInputEntryPrediction("c_win") > 2.5)//predict win
			{
				computer_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);
			}
			else if(stm->getInputEntryPrediction("c_win") < 2.5)//predict loss
			{
				computer_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);
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
				   if(stm->isInputBitPredicted(0))
				   computer_choice = 2;//rock
				   else if(stm->isInputBitPredicted(1))
				   computer_choice = 3;//paper
				   else if(stm->isInputBitPredicted(2))
				   computer_choice = 1;//sissors
				 */
			}
			else//tie
			{
				computer_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);
			}

		}
		else
		{
			done = true;
		}

		/*	
			for(int k=0;k<numberOfBits;k++)
			{
		//if(stm->isInputBitPredicted(k))
		//	printf(" %d",k);
		if (stm->isInputBitPredicted(k) && stm->isInputBitActive(k))
		{
		printf("X");
		}
		else if(stm->isInputBitPredicted(k))
		{
		printf("P");
		}
		else if(stm->isInputBitActive(k))
		{
		printf(".");
		}
		else
		{
		printf(" ");
		}
		}
		//	*/
		printf("\n");

	}
	printf("RockPaperScissors Stats\n");
	printf("Player Wins: %d\nComputer Wins: %d\nTies: %d\n", stat_player_wins, stat_computer_wins, stat_ties);
	stm->exportFile(save_location);
	clean();	
}
void test_follow()
{
	int width = 20;
	int height = 20;
	cols = 60;
	cells = 30;
	connections = cols*cells*.4;
	predictedMinActive = 1;
	setup();

	stm->postInitAddInputDouble("pain",0,30,16,4,2);
	stm->postInitAddInputDouble("distanceX",-width,width,16,4,2);
	stm->postInitAddInputDouble("distanceY",-height,height,16,4,2);
	stm->postInitAddInputDouble("directionX",-1,1,8,2,2);
	stm->postInitAddInputDouble("directionY",-1,1,8,2,2);
	stm->postInitAddInputDouble("aiX",0,width,16,4,2);
	stm->postInitAddInputDouble("aiY",0,height,16,4,2);
	stm->postInitAddInputDouble("dotX",0,width,16,4,2);
	stm->postInitAddInputDouble("dotY",0,height,16,4,2);
	stm->postInitFinalizeInputs();
	//int iterations = 1000;
	bool clear = false;
	int iterations = 500000;
	int dot_x =0;// rand() % width;
	int dot_y =0;// rand() % height;
	int dot_dir_x = 1;
	int dot_dir_y = 1;
	//ai vars
	int ai_x = width/2;//rand() % width;
	int ai_y = height/2;//rand() % height;
	int ai_dir_x = 1;
	int ai_dir_y = 1;
	double pain_avg = 0;
	double pain = 0;
	double guess_pain = 0;
	for(int i=0;i<iterations;i++)
	{
		//print map
		for(int j=0;i%1==0 && j<height;j++)
		{
			for(int k=0;k<width;k++)
			{
				if(j==ai_x && k==ai_y)
					printf("C ");
				else if(j==dot_x && k==dot_y)
					printf("* ");
				else
					printf(". ");
			}
			printf("\n");
			clear = true;
		}
		printf("Average Pain: %f\nCurrent Pain: %f\nGuess Pain: %f\n", pain_avg, pain, guess_pain);
		if(clear)
		{
			printf("\n");
			clear = false;
		}
		usleep(100000);
		//dot movement
/*		if(i%20==0)
		{
			dot_x = rand()%width;
			dot_y = rand()%height;
		}
 */
		if(i%20==0)
		{
			int which = rand() % 2;
			if(which == 1)
				dot_dir_x = rand()%3 - 1;
			else
				dot_dir_y = rand()%3 - 1;
		}

		//dot_x += dot_dir_x;
		dot_y += dot_dir_y;

		if(dot_x < 0 || dot_x > width-1)
			dot_x -= dot_dir_x;
		if(dot_y < 0 || dot_y > height-1)
			dot_y -= dot_dir_y;
		//ai movement
		ai_x += ai_dir_x;
		ai_y += ai_dir_y;
		if(ai_x < 0 || ai_x > width-1)
			ai_x -= ai_dir_x;
		if(ai_y < 0 || ai_y > height-1)
			ai_y -= ai_dir_y;
		if(i%15==0)
		{
			//calculate pain
			pain = sqrt((ai_x-dot_x)*(ai_x-dot_x) + (ai_y-dot_y)*(ai_y-dot_y));
			if(pain_avg != 0)
			{
				pain_avg += pain;
				pain_avg = pain_avg/2;
			}
			else
			{
				pain_avg = pain;
			}
			stm->setInputEntryValue("pain", pain);
			//update vars
			stm->setInputEntryValue("distanceX", ai_x-dot_x);
			stm->setInputEntryValue("distanceY", ai_y-dot_y);
			stm->setInputEntryValue("directionX", ai_dir_x);
			stm->setInputEntryValue("directiony", ai_dir_y);
			stm->setInputEntryValue("aiX", ai_x);
			stm->setInputEntryValue("aiy", ai_y);
			stm->setInputEntryValue("dotX", dot_x);
			stm->setInputEntryValue("dotY", dot_y);

			stm->process();
			//make decisions
			guess_pain = stm->getInputEntryPrediction("pain");
			bool has_guess = stm->getInputEntryHasPrediction("pain");
			//if(has_guess && guess_pain + (pain*0.2) < pain)
			//if(guess_pain > pain)
			if(guess_pain * 1.1 < pain)// && guess_pain < 10)
			{
				ai_dir_x = floor(stm->getInputEntryPrediction("directionX")+0.5);
				has_guess = stm->getInputEntryHasPrediction("directionX");
				if(!has_guess)
					ai_dir_x = rand()%3 -1;

				ai_dir_y = floor(stm->getInputEntryPrediction("directionY")+0.5);
				has_guess = stm->getInputEntryHasPrediction("directionY");
				if(!has_guess)
					ai_dir_y = rand()%3 -1;
			}
			else
			{
				ai_dir_x = rand()%3 -1;
				ai_dir_y = rand()%3 -1;
			}
		}

	}
}
void test_rock_paper_scissors()
{
	//
	cols = 9;//12;
	cells = 20;
	connections = cols*cells*.9;
	predictedMinActive = 1;
	numberOfBits = 12;
	setup();
	stm->postInitAddInputDouble("p_choice",1,3,8,2,2);
	stm->postInitAddInputDouble("c_choice",1,3,8,2,2);
	stm->postInitAddInputDouble("p_win",1,4,8,2,2);
	stm->postInitAddInputDouble("c_win",1,4,8,2,2);
	stm->postInitAddInputDouble("tie",1,4,8,2,2);
	//set overlap weights
	stm->setInputEntryOverlap("c_choice", 2.0);
	stm->postInitFinalizeInputs();
	printf("RockPaperScissors test\n");
	printf("1: Rock \t2: Paper \t3:Scissors \t0: exit\n");
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
	bool incr = true;

	printf("Please wait for results...\n");
	//while(!done)
	while(count < 1000000 && !done)
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
			stm->setInputEntryValue("p_choice", player_choice);
			stm->setInputEntryValue("c_choice", computer_choice);
			if(player_win)
			{
				printf("Player\t\t");
				stm->setInputEntryValue("p_win", 3);
			}
			else
			{
				stm->setInputEntryValue("p_win", 2);
			}
			if(computer_win)
			{
				printf("Computer\t");
				stm->setInputEntryValue("c_win", 3);
			}
			else
			{
				stm->setInputEntryValue("c_win", 2);
			}
			if(tie)
			{
				printf("Tie\t\t");
				stm->setInputEntryValue("tie", 3);
			}
			else
			{
				stm->setInputEntryValue("tie", 2);
			}

			//stm makes choice for next round
			stm->process();
			double raw_c_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);

			//printf("c_win: %f\n", stm->getInputEntryPrediction("c_win"));
			//printf("p_win: %f\n", stm->getInputEntryPrediction("p_win"));
			if(stm->getInputEntryPrediction("c_win") > 2.5)//predict win
			{
				computer_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);
			}
			else if(stm->getInputEntryPrediction("c_win") < 2.5)//predict loss
			{
				computer_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);
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
				   if(stm->isInputBitPredicted(0))
				   computer_choice = 2;//rock
				   else if(stm->isInputBitPredicted(1))
				   computer_choice = 3;//paper
				   else if(stm->isInputBitPredicted(2))
				   computer_choice = 1;//sissors
				 */
			}
			else//tie
			{
				computer_choice = (int) floor(stm->getInputEntryPrediction("c_choice")+0.5);
			}

		}
		else
		{
			done = true;
		}

		/*	
			for(int k=0;k<numberOfBits;k++)
			{
		//if(stm->isInputBitPredicted(k))
		//	printf(" %d",k);
		if (stm->isInputBitPredicted(k) && stm->isInputBitActive(k))
		{
		printf("X");
		}
		else if(stm->isInputBitPredicted(k))
		{
		printf("P");
		}
		else if(stm->isInputBitActive(k))
		{
		printf(".");
		}
		else
		{
		printf(" ");
		}
		}
		//	*/
		printf("\n");

	}
	printf("RockPaperScissors Stats\n");
	printf("Player Wins: %d\nComputer Wins: %d\nTies: %d\n", stat_player_wins, stat_computer_wins, stat_ties);
	stm->exportFile(save_location);
	clean();	
}
void test_rand_num()
{
	cols = 30;
	cells = 30;
	connections = cols*cells*.4;
	predictedMinActive = 1;
	numberOfBits = 30;
	setup();
	int iterations = 500000;
	int y =0;
	int stats_after = 90000;
	int stat_no_prediction = 0;
	int stat_first_no_prediction = 0;
	int stat_last_no_prediction = 0;
	for(int i=0;i<iterations;i++)
	{
		y = rand()%30;	
		stm->setInputBitActive(y);
		stm->process();
		int no_p_count = numberOfBits;
		for(int k=0;k<numberOfBits;k++)
		{
			//if(stm->isInputBitPredicted(k))
			//	printf(" %d",k);
			if(stm->isInputBitPredicted(k) && k==y%30)
			{
				printf("X");
				no_p_count--;
			}
			else if(stm->isInputBitPredicted(k))
			{
				printf("P");
				no_p_count--;
			}
			else if(k==y%numberOfBits)
			{
				printf(".");
			}
			else
			{
				printf(" ");
			}
		}
		if(i % numberOfBits == 0)
			printf("\tstep: %d\n", i);
		else
			printf("\n");
		if(no_p_count == numberOfBits)
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

void test_words()
{
	cols = 100;
	cells = 30;
	connections = cols*cells*.4;
	predictedMinActive = 1;
	numberOfBits = 128;
	setup();
	int y =0;
	int iterations = 1;
	//std::string str = " abc def ghi jkl mno. pqr def ghi stu mno. vwx def yzA ghi mno."
	std::string str = " The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on. The boy has the key. His key is not in the chest. My chest is far gone. He must find it if he wants to go on.";
	//	std::string str = " Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy. Jonhithan is a bad boy. Roberto is a good boy. Wendy is not a boy.";
	//taken from wikipedia 
	//std::string str = "The domestic cat (Felis catus or Felis silvestris catus) is a small, usually furry, domesticated, and carnivorous mammal. It is often called the housecat when kept as an indoor pet, or simply the cat when there is no need to distinguish it from other felids and felines. Cats are often valued by humans for companionship, and their ability to hunt vermin and household pests. Cats are similar in anatomy to the other felids, with strong, flexible bodies, quick reflexes, sharp retractable claws, and teeth adapted to killing small prey. Cat senses fit a crepuscular and predatory ecological niche. Cats can hear sounds too faint or too high in frequency for human ears, such as those made by mice and other small animals. They can see in near darkness. Like most other mammals, cats have poorer color vision and a better sense of smell than humans. Despite being solitary hunters, cats are a social species, and cat communication includes the use of a variety of vocalizations (mewing, purring, trilling, hissing, growling, and grunting), as well as cat pheromones, and types of cat-specific body language. Cats have a rapid breeding rate. Under controlled breeding, they can be bred and shown as registered pedigree pets, a hobby known as cat fancy. Failure to control the breeding of pet cats by neutering, and the abandonment of former household pets, has resulted in large numbers of feral cats worldwide, requiring population control. Since cats were cult animals in ancient Egypt, they were commonly believed to have been domesticated there, but there may have been instances of domestication as early as the Neolithic from around 9500 years ago (7500 BC). A genetic study in 2007 concluded that domestic cats are descended from African wildcats (Felis silvestris lybica) around 8000 BC, in West Asia. Cats are the most popular pet in the world, and are now found in almost every place where humans live.";
	for(int i=0;i<iterations;i++)
	{
		for(int j=0;j<str.size();j++)
		{
			//feed in whole words
			while(str[j]!=' ')
			{
				y = (int)str[j];
				stm->setInputBitActive(y);
				printf("%c", str[j]);
				j++;
			}
			stm->process();
			printf("\t");
			for(int k=0;k<numberOfBits;k++)
			{
				if(stm->isInputBitPredicted(k))
				{
					char temp = k;
					printf("%c ", temp);
				}
			}
			printf("\n");

		}
	}
	stm->printSettings();
	stm->printStatus();

	//stm->exportFile(save_location);
	//stm->initImport(save_location);
	clean();
}
void test_letters()
{
	cols = 50;
	cells = 100;
	connections = cols*cells*.2;
	predictedMinActive = 1;
	numberOfBits = 128;
	setup();
	int y =0;
	int iterations = 1;
	//taken from wikipedia 
	std::string str = "The domestic cat (Felis catus or Felis silvestris catus) is a small, usually furry, domesticated, and carnivorous mammal. It is often called the housecat when kept as an indoor pet, or simply the cat when there is no need to distinguish it from other felids and felines. Cats are often valued by humans for companionship, and their ability to hunt vermin and household pests. Cats are similar in anatomy to the other felids, with strong, flexible bodies, quick reflexes, sharp retractable claws, and teeth adapted to killing small prey. Cat senses fit a crepuscular and predatory ecological niche. Cats can hear sounds too faint or too high in frequency for human ears, such as those made by mice and other small animals. They can see in near darkness. Like most other mammals, cats have poorer color vision and a better sense of smell than humans. Despite being solitary hunters, cats are a social species, and cat communication includes the use of a variety of vocalizations (mewing, purring, trilling, hissing, growling, and grunting), as well as cat pheromones, and types of cat-specific body language. Cats have a rapid breeding rate. Under controlled breeding, they can be bred and shown as registered pedigree pets, a hobby known as cat fancy. Failure to control the breeding of pet cats by neutering, and the abandonment of former household pets, has resulted in large numbers of feral cats worldwide, requiring population control. Since cats were cult animals in ancient Egypt, they were commonly believed to have been domesticated there, but there may have been instances of domestication as early as the Neolithic from around 9500 years ago (7500 BC). A genetic study in 2007 concluded that domestic cats are descended from African wildcats (Felis silvestris lybica) around 8000 BC, in West Asia. Cats are the most popular pet in the world, and are now found in almost every place where humans live.";
	for(int i=0;i<iterations;i++)
	{
		for(int j=0;j<str.size();j++)
		{
			y = (int)str[j];
			stm->setInputBitActive(y);
			stm->process();
			printf("%c\t", str[j]);
			for(int k=0;k<numberOfBits;k++)
			{
				if(stm->isInputBitPredicted(k))
				{
					char temp = k;
					printf("%c ", temp);
				}
			}
			printf("\n");

		}
	}
	stm->printSettings();
	stm->printStatus();

	//stm->exportFile(save_location);
	//stm->initImport(save_location);
	clean();
}

void test_sine_wave()
{
	cols = 30;
	cells = 30;
	connections = cols*cells*.4;
	predictedMinActive = 1;
	numberOfBits = 30;
	setup();
	stm->postInitAddInputDouble("wave",0,numberOfBits,16,2,2);
	stm->postInitFinalizeInputs();
	//int iterations = 1000;
	int iterations = 500000;
	double y =0;
	int stats_after = 90000;
	int stat_no_prediction = 0;
	int stat_first_no_prediction = 0;
	int stat_last_no_prediction = 0;
	double prev_pre = 0;
	for(int i=0;i<iterations;i++)
	{
		y = sineWave((numberOfBits/2), 2000, i, 0) + numberOfBits/2;	
		//y+=9;
		int rounded_y = floor(y);
		stm->setInputEntryValue("wave", rounded_y % numberOfBits);
		stm->process();
		double ans = stm->getInputEntryPrediction("wave");
		double prevDiff = y - prev_pre;
		prev_pre = ans;
		int rounded_ans = floor(ans + 0.5);
		/*	printf("%f\t%f\t\t%f",y,ans,prevDiff);
			if(stm->getInputEntryHasPrediction("wave"))
			printf("\n");
			else
			printf("\tNP\n");
		 */
		//		int no_p_count = numberOfBits;
		if(stm->getInputEntryHasPrediction("wave"))
			printf("    ");
		else
			printf("N   ");
		for(int k=0;k<numberOfBits;k++)
		{
			//if(stm->isInputBitPredicted(k))
			//	printf(" %d",k);
			if(k == rounded_ans && k== rounded_y%30)
			{
				printf("X");
				//no_p_count--;
			}
			else if(k == rounded_ans)
			{
				printf("P");
				//no_p_count--;
			}
			else if(k==rounded_y%numberOfBits)
			{
				printf(".");
			}
			else
			{
				printf(" ");
			}
		}

		if(i % 50== 0)
			printf("\tstep: %d\n", i);
		else
			printf("\n");
		/*		if(no_p_count == numberOfBits)
				{
				stat_no_prediction++;
				if(stat_first_no_prediction == 0)
				stat_first_no_prediction = i;
				stat_last_no_prediction = i;
				}
		 */
	}
	stm->printSettings();
	stm->printStatus();
	/*	printf("*****Test Summary*****\n");

		printf("Stat Num No Predictions: %d\n", stat_no_prediction);
		printf("Stat first No Prediction: %d\n", stat_first_no_prediction);
		printf("Stat last No Prediction: %d\n", stat_last_no_prediction);
	 */
	stm->exportFile(save_location);
	//stm->initImport(save_location);
	clean();
}
void test_two_sine_wave()
{
	cols = 40;
	cells = 30;
	connections = cols*cells*.4;
	predictedMinActive = 1;
	numberOfBits = 40;
	setup();
	stm->postInitAddInputDouble("wave",0,numberOfBits,16,2,2);
	stm->postInitAddInputDouble("wave1",0,numberOfBits,16,2,2);
	stm->postInitAddInputDouble("wave2",0,numberOfBits,16,2,2);
	stm->setInputEntryOverlap("wave", 1.0);
	stm->setInputEntryOverlap("wave1", 1.0);
	stm->setInputEntryOverlap("wave2", 1.0);
	stm->postInitFinalizeInputs();
	int iterations = 500000;
	double y1 =0;
	double y2 =0;
	double ysum =0;
	int stats_after = 90000;
	int stat_no_prediction = 0;
	int stat_first_no_prediction = 0;
	int stat_last_no_prediction = 0;
	double prev_pre = 0;
	for(int i=0;i<iterations;i++)
	{
		y1 = sineWave(numberOfBits/4, 1000, i, 0) + numberOfBits/4;	
		y2 = sineWave(numberOfBits/4, 4000, i, 0) + numberOfBits/4;	
		ysum = y1 + y2;

		int rounded_y = floor(ysum);
		int rounded_y1 = floor(y1);
		int rounded_y2 = floor(y2);
		stm->setInputEntryValue("wave", rounded_y % numberOfBits);
		stm->setInputEntryValue("wave1", rounded_y1 % numberOfBits);
		stm->setInputEntryValue("wave2", rounded_y2 % numberOfBits);
		stm->process();
		double ans = stm->getInputEntryPrediction("wave");
		double prevDiff = ysum - prev_pre;
		prev_pre = ans;
		int rounded_ans = floor(ans + 0.5);
		if(stm->getInputEntryHasPrediction("wave"))
			printf("    ");
		else
			printf("N   ");
		for(int k=0;k<numberOfBits+5;k++)
		{
			//if(stm->isInputBitPredicted(k))
			//	printf(" %d",k);
			if(k == rounded_ans && k== rounded_y%30)
			{
				printf("X");
				//no_p_count--;
			}
			else if(k == rounded_ans)
			{
				printf("P");
				//no_p_count--;
			}
			else if(k==rounded_y%numberOfBits)
			{
				printf(".");
			}
			else
			{
				printf(" ");
			}
		}

		if(i % 50== 0)
			printf("\tstep: %d\n", i);
		else
			printf("\n");
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
