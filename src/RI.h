int ReplaceFractionPop(Population *p, double pr);

void RIGA(Population *p)
{
	int currentGenNo;

	Individual *parent1, *parent2;
	Individual *offspring1, *offspring2, *newIndividual;
	double current_indv_fitness;

	currentGenNo = 0;
	while( currentGenNo < changePeriod * totalChanges)
	{
		currentGenNo += 1;
		//Environmental change detected.
		if(( currentGenNo % changePeriod ) == 0) 
		{
			changeEnvironment(p, changeMode);
			evaluatePopulation(p);
			notifyChangeOccured(p); //Adjust related parameters.
			/*printf(" -change occured");
			system("pause");*/

		}else{

			ReplaceFractionPop(p,pr);
			parent1 = TournamentSelection(p);
			parent2 = TournamentSelection(p);

			//New members of the population are created.
			offspring1 = createNewIndividual();
			offspring2 = createNewIndividual();

			//Selected parents' genes are assigned to offsprings
			cloneIndividual(parent1,offspring1);
			cloneIndividual(parent2,offspring2);

			newIndividual = crossover(offspring1, offspring2); 
			mutation(newIndividual,pim);
			evaluateIndividual(newIndividual);
			current_indv_fitness = newIndividual->fitness;
			replacement(p,newIndividual);

			calcPerformance(currentGenNo,current_indv_fitness);
		}

		//printf("\n%d:  %d",currentGenNo, (int)getBestFitness(p));
		
	}
}

int ReplaceFractionPop(Population *p, double pr){
	Individual *randomImmigrant;
	int *immigrantsIndex, num_of_immigrant,i;
	if(pr<0 || pr >1)
		return -1;
		
	num_of_immigrant = (int) (POPULATION_SIZE * pr);

	if(replacement_strategy == 0)
	{
		immigrantsIndex = (int *) malloc(num_of_immigrant*sizeof(int));
		randomSequence(immigrantsIndex, num_of_immigrant, POPULATION_SIZE);
		  
		for (i=0; i<num_of_immigrant; ++i) 
		{
			randomImmigrant = createNewIndividual();
			evaluateIndividual(randomImmigrant);
			//printf("%d: immg:%d - ",i, immigrants[i]);
			cloneIndividual(randomImmigrant,&p->individuals[immigrantsIndex[i]]);
			free(randomImmigrant);
		}

	}else if(replacement_strategy == 1){
			
		sortPopulation(p->individuals, POPULATION_SIZE);

		for (i=1; i<=num_of_immigrant; i++) 
		{
			randomImmigrant = createNewIndividual();
			evaluateIndividual(randomImmigrant);
			cloneIndividual(randomImmigrant,&p->individuals[POPULATION_SIZE - i]);
			free(randomImmigrant);
			
		}
	}
	//system("pause");
	return 1;
}

