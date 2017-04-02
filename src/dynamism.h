
/* whenever this function is called, the peaks are changed */ 
void change_peaks();

void reInitilizePopulation(Population *p);

int xorMask[NGENES];     // the mask for XORing environment


void initializeXORMask() {
	int i;
	for (i = 0; i<NGENES; ++i)
		xorMask[i] = 0;
}

void displayXORMask(){
	int i;
	printf("XOR Mask:\n");
	for (i = 0; i<NGENES; i++)
		printf("%d -",xorMask[i]);
	printf("\n");
}


void changeEnvironment(Population *p, int changeMode) {

	 int i,numOnes;
	 int interimTemplate[NGENES];  // interim binary template
	 int *index2;

	  // initialize interimTemplate to a zero vector
	  for (i=0; i<NGENES; ++i) interimTemplate[i] = 0; 

	  numOnes = (int)(changeRatio * NGENES);


	 switch(changeMode) {
	 case 0: //Reinitilize
		 reInitilizePopulation(p);
		 break;

	 case 1: // random (non-cyclic) environments
		  index2 = (int *) malloc(numOnes*sizeof(int));
		  randomSequence(index2, numOnes, NGENES);
		  
		  for (i=0; i<numOnes; ++i) // create numOnes 1's in interimTemplate
		  {
			interimTemplate[index2[i]] = 1;
		  }
		  break;
	 case 2: //Move peaks in moving peaks problem to change environment
		 change_peaks();
		 break;
	 }

	  for (i=0; i<NGENES; ++i) // integrate interimTemplate into xorMask
		xorMask[i] ^= interimTemplate[i];

	
	
}


