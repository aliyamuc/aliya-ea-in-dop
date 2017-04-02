Individual* createNewIndividual();
void initializePopulation(Population *p);
void reInitilizePopulation(Population *p);
void displayIndividual(Individual *s);
void displayPopulation(Population *p);
void evaluateIndividual(Individual *individual);
void evaluatePopulation(Population *p);
Individual* TournamentSelection(Population *p);
void cloneIndividual(Individual *parent, Individual *offspring);
Individual* crossover(Individual *offspring1, Individual *offspring2 );
void mutation(Individual *s,double pm);
void replacement(Population *p, Individual *offspring1);
double getBestFitness(Population *p);
void calcOfflinePerformance(int current_gen_no, double  current_indv_fitness);

//Problems we are dealing
void OneMax(Individual *individual);
void DKP(Individual *individual);

void changeEnvironment(Population *p, int changeMode);

void simpleGA(Population *p)
{
	int currentGenNo;

	Individual *parent1, *parent2;
	Individual *offspring1, *offspring2, *newIndividual;
	double current_indv_fitness;

	currentGenNo = 0;
	while( currentGenNo < changePeriod * totalChanges)
	{
		//Environmental change detected.
		if(( currentGenNo % changePeriod ) == 0) 
		{
			changeEnvironment(p, changeMode);
			evaluatePopulation(p);
			notifyChangeOccured(); //Adjust related parameters.
			/*printf(" -change occured");
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
			mutation(newIndividual,pim);
			evaluateIndividual(newIndividual);
			current_indv_fitness = newIndividual->fitness;
			replacement(p,newIndividual);

			calcPerformance(currentGenNo,current_indv_fitness);
		}
		
		currentGenNo += 1;
		//printf("\n%d:  %d",currentGenNo,  (int)getBestFitness(p));		
		
	}
}

Individual* createNewIndividual()
{
	Individual *s = (Individual*) malloc (sizeof (Individual));

	int i;
	for(i=0; i< NGENES; i++)
	{
		if(PROBLEM == 2)
			s->genotype[i] = randNumber(0,100);
		else
			s->genotype[i] = randBit();
	}
	return s;
}


void initializePopulation(Population *p)
{
	int i;	
	for(i=0; i< POPULATION_SIZE; i++)
	{
		p->individuals[i] = *createNewIndividual();
		p->individuals[i].index = i;
	}
}

void reInitilizePopulation(Population *p)
{
	int i,j;	
	for(i=0; i< POPULATION_SIZE; i++)
	{
		for(j=0; j< NGENES; j++)
		{
			p->individuals[i].genotype[j] = randBit();
		}
	}
}


void displayIndividual(Individual *s)
{
	int i;
	printf("\nIndex: %d \n",s->index);
	for(i=0; i<NGENES; i++)
	{
		printf("%d - ",(int)s->genotype[i]);
	}
	printf("\n-----\n");
}

void displayMPIndividual(Individual *s)
{
	int i;
	printf("\nIndex: %d \n",s->index);
	for(i=0; i<NGENES; i++)
	{
		printf("%f - ",s->genotype[i]);
	}
	printf("\n-----\n");
}

void displayPopulation(Population *p)
{
	int i;
	for(i=0; i< POPULATION_SIZE; i++)
	{	
		displayIndividual(&p->individuals[i]);		
	}
}

void displayMPPopulation(Population *p)
{
	int i;
	for(i=0; i< POPULATION_SIZE; i++)
	{	
		displayMPIndividual(&p->individuals[i]);		
	}
}

void displayPopulationEvaluations(Population *p)
{
	int i;
	for(i=0; i< POPULATION_SIZE; i++)
	{	
		printf("i:%d: %f\n",i, p->individuals[i].fitness);		
	}
}
  
void evaluateIndividual(Individual *individual)
{
	if(PROBLEM == 0)
	{
		OneMax(individual); //Dynamic OneMax is used here as an objective function.

	}else if(PROBLEM == 1){
		DKP(individual);
	}else if(PROBLEM ==2) // Moving Peaks 
	{
		individual->fitness = eval_movpeaks(individual->genotype);
	}
}


void evaluatePopulation(Population *p)
{
	int i;
	for(i=0;i<POPULATION_SIZE;i++)
		evaluateIndividual(&p->individuals[i]);	

	best_f = getBestFitness(p);
}

Individual* TournamentSelection(Population *p){

	int i, selected_individual, ind_best_fitness;
	double current_fitness = 0,best_fitness = 0;
	for(i=0;i<TOURNAMENT_SIZE; i++)
	{
		selected_individual = (int) (rand() % POPULATION_SIZE);
		current_fitness = p->individuals[selected_individual].fitness;
		
		if(best_fitness == 0)
		{
			best_fitness = current_fitness;
			ind_best_fitness = selected_individual;
		}else if(current_fitness < best_fitness){
			best_fitness = current_fitness;
			ind_best_fitness = selected_individual;
		}
		//printf("Selected Indiviudal Fitness Value: %d \n",p->individuals[selected_individual].fitness_value);
	}

	//printf("Best Fitted Individual Ind: %d, Value: %d \n",ind_best_fitness, best_fitness);

	return &p->individuals[ind_best_fitness];
}

void cloneIndividual(Individual *parent, Individual *offspring)
{
	int i;
	for(i=0;i<NGENES; i++)
	{
		offspring->genotype[i] = parent->genotype[i];
		offspring->phenotype[i] = parent->phenotype[i];
	}
	offspring->fitness = parent->fitness;
}

//Firstly we create fresh offsprings, because parents remain in population without no-change.
//Two-point crossover has been implemented here.
Individual* crossover(Individual *offspring1, Individual *offspring2 )
{
	int i,cut1, cut2, low, high;
	double tmp;	
	cut1=rand()%NGENES;
	cut2=rand()%NGENES;
	
	if(cut1<=cut2){
		low = cut1;
		high = cut2;
	}else{
		low = cut2;
		high = cut1;
	}

	for(i=low;i<=high;i++)
	{
		tmp = offspring1->genotype[i];
		offspring1->genotype[i] = offspring2->genotype[i];
		offspring2->genotype[i] = tmp;
	}

	//Evaluate and return the best offspring
	evaluateIndividual(offspring1);
	evaluateIndividual(offspring2);

	if(offspring1->fitness > offspring2->fitness ){
		free(offspring2);
		return offspring1;
	}else{
		free(offspring1);
		return offspring2;
	}

}


void mutation(Individual *s, double pm)
{
	int i;
	//int r;

	for(i=0;i<NGENES;i++)
	{
		if(randNumber(0,1)	<	pm)
		{
			if(PROBLEM == 2)
				s->genotype[i] = randNumber(0,100);
			else
				s->genotype[i] = 1 ^ (int)s->genotype[i];

			//printf("mutated");system("pause");
		}
	}
	//r= rand()%NGENES;
	//s->genotype[r] = 1 ^ s->genotype[r];
}


void replacement(Population *p, Individual *offspring1)
{
	int i,index_worst_fitness;
	double current_fitness, worst_fitness;	
	worst_fitness = p->individuals[0].fitness;
	index_worst_fitness = 0;

	for(i=0;i<POPULATION_SIZE;i++)
	{
		current_fitness = p->individuals[i].fitness;
		if(current_fitness < worst_fitness )
		{
			index_worst_fitness = i;
			worst_fitness = current_fitness;	
		}
	}

	//New offspring is copied to the worst individual.
	cloneIndividual(offspring1, &p->individuals[index_worst_fitness]);
	
	//After copy, obsolute offspring is deallocated
	free(offspring1);
}


double getBestFitness(Population *p)
{
	int i;
	double current_fitness, best_fitness;
	best_fitness = p->individuals[0].fitness;
	for(i=0;i<POPULATION_SIZE;i++)
	{
		current_fitness = p->individuals[i].fitness;
		if(current_fitness > best_fitness )
			best_fitness = current_fitness;		
	}
	return best_fitness;
}

