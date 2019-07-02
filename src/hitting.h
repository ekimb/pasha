/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    hitting.h
    Header file for main operations for different options of hitting set calculations.
    @author Baris Ekim
    @version 1.0 4/15/19
*/
#ifndef HITTING_H
#define HITTING_H
#include "decycling.h"
#include "graph.h"
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <omp.h>
int graph::Hitting(int L, string hittingFile) {
/**
Performs hitting set calculations without parallelization
or randomization, counting L-k+1-long paths.
@param L: Sequence length, hittingFile: Output file destination.
@return hittingCount: Size of hitting set.
*/
	vertexExp = pow(ALPHABET_SIZE, k-1);
    int imaxHittingNum = -1;
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    hittingNumArray = new byte[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new byte[vertexExp];
    D = new long double*[l + 1];
    long double* Dpool = new long double[(l+1)* vertexExp];
    for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
    hittingStream.open(hittingFile); 
    F = new long double*[l + 1];
    long double* Fpool = new long double[(l+1)* vertexExp];
    for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
    while (calculatePathsSeq(l)) {
    	int imaxHittingNum = calculateHittingNumber(l);
    	if (imaxHittingNum < 0) break;
        removeEdge(imaxHittingNum);
        string label = getLabel(imaxHittingNum);
        hittingStream << label << "\n";
        hittingCount++;
   	}
   	hittingStream.close();
   	delete [] *D;
	delete [] D;
	delete [] *F;
	delete [] F;
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
    return hittingCount;
}
int graph::HittingAny(int L, int x, string hittingFile) {
/**
Performs hitting set calculations without parallelization
or randomization, counting paths of all length.
@param L: Sequence length, x: Number of vertices, hittingFile: Output file destination.
@return hittingCount: Size of hitting set.
*/
	vertexExp = pow(ALPHABET_SIZE, k-1);
    ofstream hittingStream;
    byte* imaxHittingNum;
    int hittingCount = 0;
    l = L-k+1;
    hittingNumAnyArray = new byte[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new byte[vertexExp];
    hittingStream.open(hittingFile); 
   	topologicalSort();
    D = new long double*[1];
    long double* Dpool = new long double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new long double*[1];
    long double* Fpool = new long double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Fpool += vertexExp) F[i] = Fpool;
    while (maxLength() >= l) { 
    	calculatePathsAny();
    	imaxHittingNum = calculateHittingNumberAny(x);
    	for (int i = 0; i < x; i++) {
	    	removeEdge(imaxHittingNum[i]);
        	string label = getLabel(imaxHittingNum[i]);
        	hittingStream << label << "\n";
	    	hittingCount++;
        }
   	}
   	hittingStream.close();
   	delete [] *D;
	delete [] D;
	delete [] *F;
	delete [] F;
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
    return hittingCount;
}
int graph::HittingParallel(int L, string hittingFile, int threads) {
/**
Performs hitting set calculations with parallelization
and without randomization, counting L-k+1-long paths.
@param L: Sequence length, hittingFile: Output file destination.
@return hittingCount: Size of hitting set.
*/
	vertexExp = pow(ALPHABET_SIZE, k-1);
    int imaxHittingNum = -1;
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    hittingNumArray = new byte[edgeNum];
    stageArray = new byte[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new byte[vertexExp];
    D = new long double*[l + 1];
    long double* Dpool = new long double[(l+1)* vertexExp];
    for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
    hittingStream.open(hittingFile); 
    F = new long double*[l + 1];
    long double* Fpool = new long double[(l+1)* vertexExp];
    for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
    while (calculatePaths(l, threads)) {
    	int imaxHittingNum = calculateHittingNumberParallel(l, false, threads);
    	if (imaxHittingNum < 0) break;
        removeEdge(imaxHittingNum);
        string label = getLabel(imaxHittingNum);
        hittingStream << label << "\n";
        hittingCount++;
   	}
   	hittingStream.close();
    delete [] *D;
	delete [] D;
	delete [] *F;
	delete [] F;
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
    return hittingCount;
}
int graph::HittingRandomParallel(int L, string hittingFile, int threads) {
/**
Performs hitting set calculations with parallelization
and with randomization, counting L-k+1-long paths.
@param L: Sequence length, hittingFile: Output file destination.
@return hittingCount: Size of hitting set.
*/
    omp_set_dynamic(0);
    vertexExp = pow(ALPHABET_SIZE, k-1);
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    epsilon = 0.1;
    delta = 1/(double)l;
    if (l <= 19) {
        delta = 0.01;
        epsilon = 0.05;
    }
    double alpha = 1 - 4*delta -2*epsilon;
    cout << "Alpha: " << 1/alpha << endl;
    cout << "Delta: " << delta << endl;
    cout << "Epsilon: " << epsilon << endl;
    int i;
    int j;
    hittingNumArray = new byte[edgeNum];
    stageArray = new byte[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	pick = new bool[edgeNum];
	topoSort = new byte[vertexExp];
    D = new long double*[l + 1];
    long double* Dpool = new long double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
	hittingStream.open(hittingFile); 
    F = new long double*[l + 1];
    long double* Fpool = new long double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
	calculatePaths(l, threads);
	int imaxHittingNum = calculateHittingNumberParallel(l, false, threads);
	h = findLog((1.0+epsilon), hittingNumArray[imaxHittingNum]);
    double prob = delta/l;
    while (h > 0) {
        total = 0;
    	int hittingCountStage = 0;
    	double pathCountStage = 0;
        calculatePaths(l, threads);
        if (calculateHittingNumberParallel(l, true, threads) < 0) break;
        stageVertices = pushBackVector();
        #pragma omp parallel for num_threads(threads)
		for (int it = 0; it < stageVertices.size(); it++) {
            i = stageVertices[it];
            #pragma omp critical
        	if ((pick[i] == false) && (hittingNumArray[i] > (pow(delta, 3) * total))) {
                stageArray[i] = 0;
				pick[i] = true;
    			hittingCountStage++;
    			pathCountStage += hittingNumArray[i];
    		}
        }
        #pragma omp parallel for collapse (2) num_threads(threads) 
        for (int it = 0; it < stageVertices.size(); it++) {
            for (int jt = 0; jt < stageVertices.size(); jt++) {
                i = stageVertices[it];
                #pragma omp critical
                if (pick[i] == false) {
                    if (((double) rand() / (RAND_MAX)) <= prob) {
                        stageArray[i] = 0;
                        pick[i] = true;
                        hittingCountStage += 1;
                        pathCountStage += hittingNumArray[i];
                    }
                    j = stageVertices[jt];
                    if (pick[j] == false) {
                        if (((double) rand() / (RAND_MAX)) <= prob) {
                            stageArray[j] = 0;
                            pick[j] = true;
                            hittingCountStage += 1;
                            pathCountStage += hittingNumArray[j];
                        }
                    }
                }
            }
    	}
        hittingCount += hittingCountStage;
        if (pathCountStage >= hittingCountStage * pow((1.0 + epsilon), h) * (1 - 6*delta - 2*epsilon)) {
            for (int it = 0; it < stageVertices.size(); it++) {
                i = stageVertices[it];
                if (pick[i] == true) {
                    removeEdge(i);
                    string label = getLabel(i);
                    hittingStream << label << "\n";
                }
            }
        	h--;
        }
        else hittingCount -= hittingCountStage;
   	}
   	hittingStream.close();
    delete [] *D;
	delete [] D;
	delete [] *F;
	delete [] F;
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
    return hittingCount;
}
#endif
