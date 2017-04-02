/*********************************************************/
/* Dynamic OneMax function */
/*********************************************************/
void OneMax(Individual *individual) {
  int i;
  for (i=0; i<NGENES; ++i) // perform XOR operation
	  individual->phenotype[i] = ((int) individual->genotype[i]) ^ xorMask[i];

  individual->fitness = 0.0;
  for (i=0; i<NGENES; ++i)
    individual->fitness  += individual->phenotype[i];
}

