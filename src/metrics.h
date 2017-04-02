
double m_offline_performance = 0.0;

int counter_of_evaluation = 0;   /* number of evaluations so far */

double best_f = 0; //the best solution of the population in change period.
double min_f; //information about the absolute best and worst fitness values in the search space,
double max_f; //the best fitness value of the search space

double prev_accuracy = 0;
double m_stability_performance = 0;

double f_best_1 = 0;

double total_diff_best_and_f_best1 = 0;

double total_rr = 0;
double epsilon = 0.00000000001;

double prev_current_indv_fitness = 0;
double m_abc_performance = 0;

void initMetricParameters()
{
	//Related with Offline Performance
	m_offline_performance = 0.0;
	counter_of_evaluation = 0;   /* number of evaluations so far */
	
	//Related with Accuracy
	prev_accuracy = 0;
	m_stability_performance = 0;
	
	//Related with Recovery Rate
	f_best_1 = 0;
	total_diff_best_and_f_best1 = 0;
	total_rr = 0;

	m_abc_performance = 0;
}


void notifyChangeOccured(){
	  total_diff_best_and_f_best1 = 0; //This variable is used at recovery rate performance metric.
}

void calcOfflinePerformance(int current_gen_no, double  current_indv_fitness)
{
	//last evolution in the change period
	if(( current_gen_no % changePeriod ) == changePeriod - 1){

		m_offline_performance += (best_f * (changePeriod - 1));
	}
}

double getOfflinePerformance()
{
	  return (m_offline_performance/(double)counter_of_evaluation);
}

double calcAccuracy()
{
	return (best_f - min_f)/(max_f - min_f);
}

void calcStabilityPerformance(double current_indv_fitness)
{
	double accuracy_dif, stability;

	accuracy_dif = prev_accuracy - calcAccuracy();
	prev_accuracy = calcAccuracy();

	if( accuracy_dif < 0 )
		stability = 0;
	else
		stability = accuracy_dif; 

	m_stability_performance += stability;
}

double getStabilityPerformance()
{
	return (m_stability_performance/(double)counter_of_evaluation);
}


void calcRecoveryRatePerformance(int current_gen_no)
{
	//First evolution after change
	if(( current_gen_no % changePeriod ) == 1)
	{
		f_best_1 = best_f;

	}else{
		total_diff_best_and_f_best1 += (best_f - f_best_1);	
	}

	if(( current_gen_no % changePeriod ) == changePeriod - 1){

		total_rr += (total_diff_best_and_f_best1 + epsilon) / ( ( (changePeriod - 1) * (best_f - f_best_1) ) + epsilon ) ;
	}
}

double getRecoveryRatePerformance()
{
	return (double)(total_rr / totalChanges);
}

void calcAreaBelowCurvePerformance(int current_gen_no, double current_indv_fitness)
{
	if( current_gen_no == 0){
		prev_current_indv_fitness = current_indv_fitness;
	}else{
		m_abc_performance += (prev_current_indv_fitness + ((current_indv_fitness - prev_current_indv_fitness)/2));
	}
}

double getAreaBelowCurve()
{
	return (double)(m_abc_performance / (double)counter_of_evaluation);
}


//invoked at evolution segment of EA
void calcPerformance(int current_gen_no, double current_indv_fitness)
{
	//best_f initilized when the environment has been changed and evaluation of the new environment.
	if(current_indv_fitness > best_f )
		best_f = current_indv_fitness;
	
	switch(performance_metric)
	{
		case 0: calcOfflinePerformance(current_gen_no,current_indv_fitness); break;
		case 1: calcStabilityPerformance(current_indv_fitness); break;
		case 2: calcRecoveryRatePerformance(current_gen_no); break;
		case 3: calcAreaBelowCurvePerformance(current_gen_no,current_indv_fitness); break;	
	}

	counter_of_evaluation++;
}

double getPerformance()
{
	switch(performance_metric)
	{
		case 0: return getOfflinePerformance(); break; 
		case 1: return getStabilityPerformance(); break; 
		case 2: return getRecoveryRatePerformance(); break; 
		case 3: return getAreaBelowCurve(); break; 
		default: return -1;
	}
}
