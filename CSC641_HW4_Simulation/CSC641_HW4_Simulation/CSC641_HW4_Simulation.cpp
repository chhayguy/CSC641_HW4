//File: CSC641_HW4_Simulation.cpp
//Author: Richard Chhay
//Institution: San Francisco State University
//Date: April 2, 2018
//Course: CSC 641 COMPUTER PERFORMANCE EVALUATION
//Summary: This program seeks to provide two features both involving G/G/1 Queues. 
//			In the QueueSimulation, 
//				a simulation of G/G/1 queues is provided with queue sizes of size 1000. The function returns
//				for a number of jobs: total time, avg interarrival time, stddev interarrival time, average
//				service time, stddev service time, average response time, stddev response time, average # of jobs,
//				stddev # of jobs, utilization time, and throughput rate.
//			In the QueueCalculator,
//				the function asks for the desired distribution of interarrival time and service time. Parameters
//				for each distribution are asked for. The function then returns server utilization, mean response
//				time, mean queue length, and time spent waiting in queue (before service).
//Sources:
//			Queue Calculator Function
//			Author: Jozo Dujmovic
//			Title: G/G/1 Calculator
//			Retrieved from iLearn

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <random>
#include <chrono>

using namespace std;
// Main program provided by Jozo Dujmovic, Fall 2010

/* Uniform Distribution*/
double uniRNG(double TMIN, double TMAX) {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_real_distribution<double> distribution(TMIN, TMAX); // uniform distribution with lower and upper bounds (tMin, tMax)
	return distribution(generator);
}

/* Exponential Distribution*/
double expRNG(double lambda) {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	exponential_distribution<double> distribution(lambda); // exponential distribution with mean time (lambda)
	return distribution(generator);
}

/* Constant Distribution*/
double constRNG(double a) {
	return a;
}

/* Queue Max Size = 1000
	Calculation interval on powers of 10
	Distribution Choices: (1) Constant, (2) Exponential, (3) Uniform*/
void queueSim(void) {
	char adist, sdist;
	double a, tMin, tMax;
	const int QL = 10;
	int JOBS[QL];
	double TIME[QL], ATA[QL], SDTA[QL], ATS[QL], SDTS[QL], ART[QL], SDRT[QL], AJOB[QL], SDJOB[QL], UTIL[QL], X[QL]; 
	/* total time, average time arrival, stddev time arrival, average time service, 
		stddev time service, average response time, stddev response time, 
		average # of jobs, stddev # of jobs, utilization time, throughput rate*/
	
	cout << "\n\n========== G/G/1 Simulator ===========";
	double QA[1000]; // Arrival Queue of size 1000
	double QS[1000]; // Service Queue of size 1000

	// Begin interarrival
	cout << "\n\n-------------------------------------------------------"
		<< "\n\nThe following choices for interarrival time distributions are available: "
		<< "\n	(1) Constant"
		<< "\n	(2) Exponential"
		<< "\n	(3) Uniform"
		<< "\nEnter your choice: ";
	cin >> adist;
	if (adist == '1') { cout << "You chose Constant Distribution. Enter the mean interarrival time: "; cin >> a;
		for (int i = 0; i < 1000; i++) {
			QA[i] = constRNG(a); // Populate interarrival queue, where a is the time until next arrival from index i
		}
	}
	else if (adist == '2') { cout << "You chose Exponential Distribution. Enter the mean interarrival time: "; cin >> a;
		for (int i = 0; i < 1000; i++) {
			QA[i] = expRNG(a);
		}
	}
	else if (adist == '3') { cout << "You chose Uniform Distribution. Enter the minimum and maximum arrival time: "; cin >> tMin >> tMax;
		for (int i = 0; i < 1000; i++) {
			QA[i] = uniRNG(tMin, tMax); 
		}
	}
	// end interarrival
	
	// begin service
	cout << "\n\n-------------------------------------------------------"
		<< "\n\nThe following choices for service time distributions are available: "
		<< "\n	(1) Constant"
		<< "\n	(2) Exponential"
		<< "\n	(3) Uniform"
		<< "\nEnter your choice: ";
	cin >> sdist;
	if (sdist == '1') {
		cout << "You chose Constant Distribution. Enter the mean interarrival time: "; cin >> a;
		for (int i = 0; i < 1000; i++) {
			QS[i] = constRNG(a); // Populate service time queue, where a is the time until next arrival from index i
		}
	}
	else if (sdist == '2') {
		cout << "You chose Exponential Distribution. Enter the mean interarrival time: "; cin >> a;
		for (int i = 0; i < 1000; i++) {
			QS[i] = expRNG(a); // Populate service time queue, where a is the mean interarrival time used for expo dist
		}
	}
	else if (sdist == '3') {
		cout << "You chose Uniform Distribution. Enter the minimum and maximum arrival time: "; cin >> tMin >> tMax;
		for (int i = 0; i < 1000; i++) {
			QS[i] = uniRNG(tMin, tMax); // Populate service time queue with equally probable time between tMin and tMax
		}
	}
	// end service
	// Queues have been filled with 1000 values according to distribution

	// # of jobs
	for (int i = 0; i < 10; i++) {
		JOBS[i] = (i + 1) * 10000;
	}

	// Total time
	double QT = 0.;
	for (int j = 0; j < 1000; j++) {
		QT += (QA[j] + QS[j]);
	}
	for (int i = 0; i < 10; i++) {
		TIME[i] = QT * 10*(i + 1);
	}

	// Average Interarrival Time
	double QAT = 0.;
	for (int j = 0; j < 1000; j++) {
		QAT += QA[j];
	}
	for (int i = 0; i < 10; i++) {
		ATA[i] = QAT * 10*(i + 1);
	}

	// Standard Deviation Interarrival
	double sqDiff = 0., var = 0.;
	for (int j = 0; j < 1000; j++) {
		sqDiff += (QA[j] - QAT)*(QA[j] - QAT);
	}
	for (int i = 0; i < 10; i++) {
		var = (sqDiff*(i + 1) * 10) / ((i + 1) * 10000);
		SDTA[i] = sqrt(var);
	}
	
	// Average Service Time
	double QST = 0.;
	for (int j = 0; j < 1000; j++) {
		QST += QS[j];
	}
	for (int i = 0; i < 10; i++) {
		ATS[i] = QST * 10 * (i + 1);
	}
	
	// SDTS
	sqDiff = 0., var = 0.;
	for (int j = 0; j < 1000; j++) {
		sqDiff += (QS[j] - QST)*(QS[j] - QST);
	}
	for (int i = 0; i < 10; i++) {
		var = (sqDiff*(i + 1) * 10) / ((i + 1) * 10000);
		SDTS[i] = sqrt(var);
	}

	// ART
	double TART = 0.;
	for (int j = 0; j < 1000; j++) {
		TART += fabs((QA[j] - QS[j])); // sum all abs differences
	}
	TART /= 1000; // Avg found
	for (int i = 0; i < 10; i++) {
		ART[i] = (TART * 10000 * (i + 1));
	}
	
	//SDRT
	sqDiff = 0., var = 0.;
	for (int j = 0; j < 1000; j++) {
		sqDiff += (fabs((QA[j] - QS[j])) - TART)*(fabs((QA[j] - QS[j])) - TART);
	}
	for (int i = 0; i < 10; i++) {
		var = (sqDiff*(i + 1) * 10) / ((i + 1) * 10000);
		SDRT[i] = sqrt(var);
	}

	//AJOB
	for (int i = 0; i < 10; i++) {
		AJOB[i] = JOBS[i] / TIME[i];
	}
	
	//SDJOB
	sqDiff = 0., var = 0.;
	for (int j = 0; j < 10; j++) {
		sqDiff += (AJOB[j])*(AJOB[j]);
	}
	for (int i = 0; i < 10; i++) {
		var = (sqDiff*(i + 1)) / ((i + 1));
		SDJOB[i] = sqrt(var);
	}

	//UTIL
	for (int i = 0; i < 10; i++) {
		UTIL[i] = ATS[i] / ATA[i];
	}

	// X
	for (int i = 0; i < 10; i++) {
		X[i] = 1 / ATA[i];
	}

	// print results
	cout << "\n\n";
	cout << left
		<< setw(10) << "JOBS"
		<< setw(10) << "TIME"
		<< setw(10) << "ATA"
		<< setw(10) << "SDTA"
		<< setw(10) << "ATS"
		<< setw(10) << "SDTS"
		<< setw(10) << "ART"
		<< setw(10) << "SDRT"
		<< setw(10) << "AJOB"
		<< setw(10) << "SDJOB"
		<< setw(10) << "UTIL"
		<< setw(10) << "X";
	cout << "=================================================================================================================\n";
	for (int i = 0; i < 10; i++) {
		cout << left
			<< setw(10) << JOBS[i]
			<< setw(10) << TIME[i]
			<< setw(10) << ATA[i]
			<< setw(10) << SDTA[i]
			<< setw(10) << ATS[i]
			<< setw(10) << SDTS[i]
			<< setw(10) << ART[i]
			<< setw(10) << SDRT[i]
			<< setw(10) << AJOB[i]
			<< setw(10) << SDJOB[i]
			<< setw(10) << UTIL[i]
			<< setw(10) << X[i] << endl;
	}
}

void queueCalc(void) {
	char adist, sdist;
	double S, U, vs, va, tMin, tMax, a, X, R;

	cout << "\n========== G/G/1 Calculator ==========";
		cout << "\n\n-------------------------------------------------------"
			<< "\n\nThe following choices for interarrival time distributions are available: "
			<< "\n	(1) Constant"
			<< "\n	(2) Exponential"
			<< "\n	(3) Uniform"
			<< "\nEnter your choice: ";
		cin >> adist;
		if (adist == '1') { va = 0.; cout << "Constant interarrival time = "; cin >> a; X = 1. / a; }
		else if (adist == '2') { va = 1.; cout << "Mean interarrival time = "; cin >> a; X = 1. / a; }
		else if (adist == '3') {
			cout << "Enter min and max interarrival time, separate the values with spaces: ";
			cin >> tMin >> tMax;
			a = (tMax + tMin) / 2;
			X = 1. / a;
			va = (tMax - tMin) / ((tMax + tMin)*sqrt(3.));
		}
		cout << "\n\nThe following choices for service time distributions are available: "
			<< "\n	(1) Constant"
			<< "\n	(2) Exponential"
			<< "\n	(3) Uniform"
			<< "\nEnter your choice: ";
		cin >> sdist;
		if (sdist == '1') { vs = 0.; cout << "Constant service time = "; cin >> S; }
		else if (sdist == '2') { vs = 1.; cout << "Mean service time = "; cin >> S; }
		else if (sdist == '3') {
			cout << "Enter min and max service time, separate the values with spaces: ";
			cin >> tMin >> tMax;
			S = (tMax + tMin) / 2;
			vs = (tMax - tMin) / ((tMax + tMin)*sqrt(3.));
		}
		U = S * X;
		R = (S / (1. - U))*(1 - 0.5*U*(1. - vs * vs - ((vs*vs + 1.)*(va*va - 1.) / (U*U*vs*vs + 1.))));
		cout << "\nServer utilization U = " << U
			<< "\nMean response time R = " << R
			<< "\nMean queue length Q = " << ((adist == '1'&&sdist == '1') ? U : U / (1 - U))
			<< "\nTime spent waiting W = " << R - S;
}

int main()
{
	//queueSim();
	queueCalc();
	
	system("pause");
	return 0;
}

