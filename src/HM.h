void IncreaseMutationRate(double* pm,double phm)
{
	*pm = phm;
}

void ResetMutationRate(double* pm, double pim)
{
	*pm = pim;
}

void HM(Population *p)
{
	int currentGenNo;
	Individual *parent1, *parent2;
	Individual *offspring1, *offspring2, *newIndividual;
	double current_indv_fitness;

	double pm; //Mutation Rate

	pm = pim; //The mutation rate is initilized to a small value

	currentGenNo = 0;
	while( currentGenNo < changePeriod * totalChanges)
	{
		currentGenNo += 1;
		//Environmental change detected.
		if(( currentGenNo % changePeriod ) == 0) 
		{
			IncreaseMutationRate(&pm,phm);
			changeEnvironment(p, changeMode);
			evaluatePopulation(p);
			notifyChangeOccured(p); //Adjust related parameters.
		/*	printf(" -change occured");
			system("pause");*/

		}else{

			parent1 = TournamentSelection(p);
			parent2 = TournamentSelection(p);

			//New members of the population are created.
			offspring1 = createNewIndividual();
			offspring2 = createNewIndividual();

			//Selected parents' genes are assigned to offsprings
			cloneIndividual(parent1,offspring1);
			cloneIndividual(parent2,offspring2);

			newIndividual = crossover(offspring1, offspring2); 
			mutation(newIndividual,pm);
			evaluateIndividual(newIndividual);
			current_indv_fitness = newIndividual->fitness;
			replacement(p,newIndividual);

			ResetMutationRate(&pm,pim);

			calcPerformance(currentGenNo,current_indv_fitness);
		}

		//printf("\n%d:  %d",currentGenNo, (int)getBestFitness(p));
		
	}
}
