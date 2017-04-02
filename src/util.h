//int randBit(void);
//double randNumber(double low, double high);


/***********************************************************/
/* Random bit generator: Generates a random binary bit     */
/***********************************************************/
int randBit(void) { return (rand()%2); }

/***********************************************************/
/* Random value generator: Generates a value within bounds */
/***********************************************************/
double randNumber(double low, double high) {
  return ((double)(rand()%10000)/10000.0)*(high - low) + low;
}

/***********************************************************/
/* Random sequence generator:                              */
/*   Generates a random sequence of seqSize numbers from   */
/*   the natural number array [0, 1, ..., numSize-1]       */
/***********************************************************/
void randomSequence(int* seqArr, int seqSize, int numSize) {
  int i, j, idx, count, numleft;
  int *number;
  // constructs a natural number array
  number =  (int *) malloc(numSize*sizeof(int));

  for (i=0; i<numSize; ++i) number[i] = i; 

  // select seqSize numbers from number[] without repeat
  numleft = numSize;
  for (i=0; i<seqSize; ++i) { 
    idx = rand() % numleft;
    count = 0;
    for (j=0; j<numSize; ++j) { 
      if (number[j] != -1) // found one not selected number
        ++count;

      if (count > idx) break; // found the idx-th not selected number
    }

    seqArr[i] = number[j];
    number[j] = -1; // marked as selected
    --numleft;
  }
  free(number);
}

int compareFitness(const void * elem1, const void * elem2) {
  Individual * i1, *i2;
  i1 = (Individual*)elem1;
  i2 = (Individual*)elem2;
  return (int)(i2->fitness - i1->fitness);
}

void sortPopulation(Individual* arr, size_t num) {
	qsort(arr, num, sizeof(Individual), compareFitness);	
}
