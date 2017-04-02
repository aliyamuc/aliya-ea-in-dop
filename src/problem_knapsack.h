
int values[NGENES];
int weigths[NGENES];
int capacity;
double capacity_ratio = 0.75;	//Capacity is %75 of the sum of all items' weight

//Randomly initilize knapsack items
void initilizeKnapsackItems()
{
	int i;
	for(i=0;i<NGENES;i++)
	{
		values[i] = (int)randNumber(10,50);
		weigths[i] = (int)randNumber(1,10);
		capacity += weigths[i];
	}
	capacity = (int) (capacity * capacity_ratio);
}

/*********************************************************/
/* Dynamic Knapsack Problem function */
/*********************************************************/
void DKP(Individual *individual) {
  int i;
  double Z, total_weight;

  for (i=0; i<NGENES; ++i) // perform XOR operation
	  individual->phenotype[i] = ((int)individual->genotype[i]) ^ xorMask[i];

  individual->fitness = 0.0;
  Z = 0, total_weight = 0;
  for (i=0; i<NGENES; ++i)
  {
	  Z += values[i]  * individual->phenotype[i];
	  total_weight += weigths[i] * individual->phenotype[i];
  }

  if(total_weight<=capacity)
	individual->fitness  =  Z;
  else
	  individual->fitness  = pow((double)10,-10)*(capacity - total_weight);
}
