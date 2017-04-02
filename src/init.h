
//Each individiual in population represents a candidate solution in the problem.
typedef struct Individual {
	int index;
	double genotype[NGENES];	//chromosome
	double phenotype[NGENES];	
	double fitness;	
} Individual;

//represents a solution space
typedef struct Population {
	Individual *individuals; 
} Population;


//Only valid in RI
double pr = 0.03; // Replacement rate for Random Immigrants
int replacement_strategy = 1; // 0: Random 1: worst individuals. Replace with random immigrants.

double pim = (double) 2/NGENES; //Initial Mutation Rate

//Only valid in HM
double phm =0.5; //Adaptive Mutation Rate
