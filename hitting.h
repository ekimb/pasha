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
    hittingNumArray = new double[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new int[vertexExp];
    D = new double*[l + 1];
    double* Dpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
	hittingStream.open(hittingFile); 
    F = new double*[l + 1];
    double* Fpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
    while (calculatePaths(l)) {
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
    int* imaxHittingNum;
    int hittingCount = 0;
    l = L-k+1;
    hittingNumAnyArray = new double[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new int[vertexExp];
    hittingStream.open(hittingFile); 
   	topologicalSort();
    D = new double*[1];
    double* Dpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new double*[1];
    double* Fpool = new double[(1)* vertexExp];
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
int graph::HittingParallel(int L, string hittingFile) {
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
    hittingNumArray = new double[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new int[vertexExp];
    D = new double*[l + 1];
    double* Dpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
	hittingStream.open(hittingFile); 
    F = new double*[l + 1];
    double* Fpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
    while (calculatePaths(l)) {
    	int imaxHittingNum = calculateHittingNumberParallel(l);
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
int graph::HittingParallelAny(int L, int x, string hittingFile) {
/**
Performs hitting set calculations with parallelization and
without randomization, counting paths of all length.
@param L: Sequence length, x: Number of vertices, hittingFile: Output file destination.
@return hittingCount: Size of hitting set.
*/
	vertexExp = pow(ALPHABET_SIZE, k-1);
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    hittingNumAnyArray = new double[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new int[vertexExp];
   	topologicalSort();
    D = new double*[1];
    double* Dpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new double*[1];
    double* Fpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Fpool += vertexExp) F[i] = Fpool;
    hittingStream.open(hittingFile);
    while(maxLength() >= l) { 
    	calculatePathsAny();
    	int* imaxHittingNum;
    	imaxHittingNum = calculateHittingNumberParallelAny(x);
    	for (int i = 0; i < x; i++) {
        	string label = getLabel(imaxHittingNum[i]);
        	removeEdge(imaxHittingNum[i]);
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
int graph::HittingRandomParallel(int L, string hittingFile) {
/**
Performs hitting set calculations with parallelization
and with randomization, counting L-k+1-long paths.
@param L: Sequence length, hittingFile: Output file destination.
@return hittingCount: Size of hitting set.
*/
	vertexExp = pow(ALPHABET_SIZE, k-1);
    int imaxHittingNum = -1;
    l = L-k+1;
    epsilon = 0.083333333;
    delta = 1.0;
    hittingNumArray = new double[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new int[vertexExp];
    D = new double*[l + 1];
    double* Dpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new double*[l + 1];
    double* Fpool = new double[(l+1)*vertexExp];
	for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
	calculatePaths(l);
	imaxHittingNum = calculateHittingNumberParallel(l);
	pick = new bool[edgeNum];
	for (int i = 0; i < edgeNum; i++) pick[i] = false;
	double maxPtr = hittingNumArray[imaxHittingNum];
	int hittingCount = stageOps(l, maxPtr, hittingFile);
   	delete [] *D;
	delete [] D;
	delete [] *F;
	delete [] F;
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
    return hittingCount;
}

#endif
