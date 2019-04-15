#ifndef HITTING_H
#define HITTING_H
#include "decycling.h"
#include "graph.h"
#include <cstdlib>
#include <iomanip>
#include <omp.h>
int graph::Hitting(int L, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
    ofstream hittingStream;
    int imaxHittingNum = -1;
    int hittingCount = 0;
    l = L-k+1;

    hittingNumVector = new double[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new int[vertexExp];

    cout << "D initialized." << endl;
    cout << "F initialized." << endl;

    hittingStream.open(hittingFile); 

    D = new double*[l + 1];
    double* Dpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new double*[l + 1];
    double* Fpool = new double[(l+1)*vertexExp];
	for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;

    while (calculatePaths(l)){
    	//cout << "in while" << endl;
    	imaxHittingNum = calculateHittingNumber(l);
    	if (imaxHittingNum < 0) break;
    	//cout << imaxHittingNum << endl;
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

    delete [] used;
    delete [] finished;

	cout << "Length of longest remaining path: " <<  maxLength() << "\n";

   	delete [] topoSort;
    delete [] edgeVector;
    delete [] hittingNumVector;
    return hittingCount;
}
int graph::HittingAny(int L, int x, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
    ofstream hittingStream;
    vector<int> imaxHittingNum;
    int hittingCount = 0;
    l = L-k+1;

    hittingNumAnyVector = new double[edgeNum];
    used = new bool[vertexExp];
	finished = new bool[vertexExp];
	topoSort = new int[vertexExp];

    //cout << "D initialized." << endl;
    //cout << "F initialized." << endl;
    hittingStream.open(hittingFile); 

    //cout << l << endl;
    //cout << vertexExp << endl;
   	topologicalSort();
    //cout << l << endl;
    //cout << vertexExp << endl;
    //D.resize(l + 1, vector<int>(vertexExp));
   	//cout << "D initialized." << endl;
    //F.resize(l + 1, vector<int>(vertexExp));
    //cout << "F initialized." << endl;
    D = new double*[1];
    double* Dpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new double*[1];
    double* Fpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Fpool += vertexExp) F[i] = Fpool;
    while(maxLength() >= l) { 
    	calculatePathsAny();
    	imaxHittingNum = calculateHittingNumberAny(x);
    	for (int i = 0; i < imaxHittingNum.size(); i++) {
	    	removeEdge(imaxHittingNum[i]);
        	string label = getLabel(imaxHittingNum[i]);
        	hittingStream << label << "\n";
	    	hittingCount++;
        }
    	//cout << imaxHittingNum << endl;
        //cout << "Writing " << label << endl;
   	}
   	hittingStream.close();
   	delete [] *D;
	delete [] D;
	delete [] *F;
	delete [] F;
	delete [] hittingNumAnyVector;
    topologicalSort();
    delete [] used;
    delete [] finished;
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
   	delete [] topoSort;
    delete [] edgeVector;
	//delete [] topoSort;
    return hittingCount;
}
int graph::HittingParallel(int L, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
    int imaxHittingNum = -1;
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    //cout << l << endl;
    //cout << vertexExp << endl;
    //D.resize(l + 1, vector<int>(vertexExp));
   	//cout << "D initialized." << endl;
    //F.resize(l + 1, vector<int>(vertexExp));
    //cout << "F initialized." << endl;
    D = new double*[l + 1];
    double* Dpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
	hittingStream.open(hittingFile); 
    F = new double*[l + 1];
    double* Fpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
    while (calculatePaths(l)){
    	int imaxHittingNum = calculateHittingNumberParallel(l);
    	if (imaxHittingNum < 0) break;
    	//cout << imaxHittingNum << endl;
        removeEdge(imaxHittingNum);
        string label = getLabel(imaxHittingNum);
        //cout << "Writing " << label << endl;
        hittingStream << label << "\n";
        hittingCount++;
   	}
   	hittingStream.close();
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
	delete [] *D;
	delete [] D;
	//for(int i = 0; i < vertexExp; i++) {delete [] F[i];}
	delete [] *F;
	delete [] F;
    return hittingCount;
}
int graph::HittingParallelAny(int L, int x, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
	vector<int> imaxHittingNum(x);
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    //cout << l << endl;
    //cout << vertexExp << endl;
   	topologicalSort();
    //cout << l << endl;
    //cout << vertexExp << endl;
    //D.resize(l + 1, vector<int>(vertexExp));
   	//cout << "D initialized." << endl;
    //F.resize(l + 1, vector<int>(vertexExp));
    //cout << "F initialized." << endl;
    D = new double*[1];
    double* Dpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new double*[1];
    double* Fpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Fpool += vertexExp) F[i] = Fpool;
    hittingStream.open(hittingFile);
    while(maxLength() >= l) { 
    	calculatePathsAny();
    	imaxHittingNum = calculateHittingNumberParallelAny(x);
    	for (int i = 0; i < imaxHittingNum.size(); i++) {
        	string label = getLabel(imaxHittingNum[i]);
        	removeEdge(imaxHittingNum[i]);
        	hittingStream << label << "\n";
	    	hittingCount++;
        }
   	}
   	hittingStream.close();
   	int* topoSort;
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
	delete [] *D;
	delete [] D;
	//for(int i = 0; i < vertexExp; i++) {delete [] F[i];}
	delete [] *F;
	delete [] F;
	delete [] edgeVector;
    return hittingCount;
}
int graph::HittingParallelRandom(int L, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
    int imaxHittingNum = -1;
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    //cout << l << endl;
    //cout << vertexExp << endl;
    //D.resize(l + 1, vector<int>(vertexExp));
   	//cout << "D initialized." << endl;
    //F.resize(l + 1, vector<int>(vertexExp));
    //cout << "F initialized." << endl;
    D = new double*[l + 1];
    double* Dpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Dpool += vertexExp) D[i] = Dpool;
	hittingStream.open(hittingFile); 
    F = new double*[l + 1];
    double* Fpool = new double[(l+1)* vertexExp];
	for(int i = 0; i < l+1; i++, Fpool += vertexExp) F[i] = Fpool;
    while (calculatePaths(l)){
    	int imaxHittingNum = calculateHittingNumberParallel(l);
    	if (imaxHittingNum < 0) break;
    	//cout << imaxHittingNum << endl;
        removeEdge(imaxHittingNum);
        string label = getLabel(imaxHittingNum);
        //cout << "Writing " << label << endl;
        hittingStream << label << "\n";
        hittingCount++;
   	}
   	hittingStream.close();
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
	delete [] *D;
	delete [] D;
	//for(int i = 0; i < vertexExp; i++) {delete [] F[i];}
	delete [] *F;
	delete [] F;
    return hittingCount;
}
int graph::HittingParallelRandomAny(int L, int x, string hittingFile) {
	vertexExp = pow(ALPHABET_SIZE, k-1);
	vector<int> imaxHittingNum(x);
    ofstream hittingStream;
    int hittingCount = 0;
    l = L-k+1;
    //cout << l << endl;
    //cout << vertexExp << endl;
   	topologicalSort();
    //cout << l << endl;
    //cout << vertexExp << endl;
    //D.resize(l + 1, vector<int>(vertexExp));
   	//cout << "D initialized." << endl;
    //F.resize(l + 1, vector<int>(vertexExp));
    //cout << "F initialized." << endl;
    D = new double*[1];
    double* Dpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Dpool += vertexExp) D[i] = Dpool;
    F = new double*[1];
    double* Fpool = new double[(1)* vertexExp];
	for(int i = 0; i < 1; i++, Fpool += vertexExp) F[i] = Fpool;
    hittingStream.open(hittingFile);
    while(maxLength() >= l) { 
    	calculatePathsAny();
    	imaxHittingNum = calculateHittingNumberParallelAny(x);
    	for (int i = 0; i < imaxHittingNum.size(); i++) {
        	string label = getLabel(imaxHittingNum[i]);
        	removeEdge(imaxHittingNum[i]);
        	hittingStream << label << "\n";
	    	hittingCount++;
        }
   	}
   	hittingStream.close();
   	int* topoSort;
    topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << "\n";
	delete [] *D;
	delete [] D;
	//for(int i = 0; i < vertexExp; i++) {delete [] F[i];}
	delete [] *F;
	delete [] F;
	delete [] edgeVector;
    return hittingCount;
}
#endif
