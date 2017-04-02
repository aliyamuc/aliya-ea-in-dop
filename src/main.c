#define POPULATION_SIZE 100 //Size of solution space of the problem
#define TOURNAMENT_SIZE 5
#define NGENES 100        // number of genes in a chromosome

#define PROBLEM 0		//0: OneMax, 1:Knapsack, 2: Moving Peaks
#define METHOD	0		//0:SGA, 1:RI, 2: HM 
int performance_metric = 0; //0: Offline Performance, 1: Stability, 2: Recovery Rate, 3: Area Below Curve

int NUM_OF_INDEPENDENT_RUN = 30;

//Dynamism Parameters
int changeMode = 1;		   //0: Renew population, 1: Gain Dynamisim via changing XOR Mask, 2: Gain Dynamism via Changing Peaks.

double changeRatio	=	0.6;    //Severity of change. the ratio of loci changed in dynamic environment, valid in XOR
int changePeriod	=	50;		//The period of environment changing in generations
int totalChanges	=	10;		//The total number of environment changes in a run

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#include "init.h"
#include "util.h"

//METRICS
#include "metrics.h"

#include "dynamism.h"

//PROBLEMS
#include "problem_onemax.h"
#include "problem_knapsack.h"
#include "problem_movingpeaks.h"
//METHODS
#include "SGA.h"
#include "RI.h"
#include "HM.h"


void initilizeProblem(Population *p){

	if(PROBLEM == 0){	
		initializeXORMask();
		min_f = 0;
		max_f = 100;
	}else if(PROBLEM == 1)
	{
		initializeXORMask();
		initilizeKnapsackItems();
	}else if(PROBLEM ==2)
	{
		init_peaks();
		min_f = 0;
		max_f = 100;
	}
}

double runExperiment()
{
	Population population;

	srand ( (unsigned int) time (NULL) );
	population.individuals = (Individual*) malloc (sizeof (Individual) * POPULATION_SIZE); //Memory allocation
		
	initializePopulation(&population);
	initilizeProblem(&population);
	initMetricParameters();

	evaluatePopulation(&population);

	switch(METHOD){
		case 0: simpleGA(&population);break;
		case 1: RIGA(&population); break;
		case 2: HM(&population); break;
	}
	
	return getPerformance();
}

int main()
{
	int i;
	double performance, total_performance = 0;

	for(i=0;i<NUM_OF_INDEPENDENT_RUN;i++)
	{
		performance = runExperiment();
		/*printf("performance: %f\n",performance);
		system("pause");*/
		total_performance += performance;
	}
	
	printf("\nPerformance:%f",total_performance / NUM_OF_INDEPENDENT_RUN);
	system("pause");
	return 0;
}







