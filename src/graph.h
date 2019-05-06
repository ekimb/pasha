/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    graph.h
    Header file for graph-related attributes, as well as operations prior to the
    hitting set calculations.
    @author Baris Ekim
    @version 1.0 4/15/19
*/
#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <iomanip>
#include <omp.h>
using namespace std;

class graph {

	public:
    bool* finished;
    bool* pick;
    bool* used;
    bool* tried;
    double delta;
	double epsilon;
    double* hittingNumAnyArray;
    double* hittingNumArray;
    double* maxHittingNum;
    double** D;
    double** F;
    int ALPHABET_SIZE;
	int edgeCount;
    int edgeNum;
    int k;
    int l;
    int h;
    int tries;
    double total;
    int vertexCount; 
    int vertexExp;
    int vertexExp2;
    int vertexExp3;
    int vertexExpMask;
    int vertexExp_1;
    int* edgeArray;
    int* imaxHittingNum;
    int* stageArray;
    int* topoSort;
    static map<char, int> alphabetMap;
    string ALPHABET;
    vector<int> stageVertices;

	void generateGraph(int k) {  
    /**
    Generates a complete de Bruijn graph of order k.
    @param k: Desired k-mer length (order of complete graph).
    */
        for (int i = 0; i < edgeNum; i++) edgeArray[i] = 1;
		edgeCount = edgeNum;
		vertexCount = edgeNum / ALPHABET_SIZE; 
    }

    char getChar(int i) {
    /**
    Gets alphabet character from index.
    @param i: Index of character.
    @return The character in the alphabet.
    */
    	return ALPHABET[i];
    }

    string getLabel(int i) {
    /**
    Gets label of the input edge index.
    @param i: Index of edge.
    @return The label of the edge.
    */
		string finalString = "";
		for (int j = 0; j < k; j++) {
			finalString = getChar((i % ALPHABET_SIZE)) + finalString;
			i = i / ALPHABET_SIZE;
		}
		return finalString;
	}

	graph(int argK) {
    /**
    Definition of a graph object. Generates a graph of order k, creates an empty
    edge index array, calculates number of edges, builds a character-index map.
    @param argK: Argument passed as k-mer length.
    */
		ALPHABET = "ACGT";
		ALPHABET_SIZE = 4;
		k = argK;
        edgeNum = pow(ALPHABET_SIZE, k);
        edgeArray = new int[edgeNum];
		generateGraph(k);
		map<char, int> alphabetMap;
		for (int i = 0; i < ALPHABET_SIZE; i++) alphabetMap.insert(pair<char,int>(ALPHABET[i], i));
    }

    void calculateForEach(int i, int L);
    void calculateForEachAny(int i);
    int calculateHittingNumber(int L);
    int* calculateHittingNumberAny(int x);
    int calculateHittingNumberParallel(int L, bool random);
    int calculatePaths(int L);
    int calculatePathsSeq(int L);
    int calculatePathsAny();
    int* findMaxAny(int x);
    int findLog(double base, double x);
    vector<int> getAdjacent(int v);
    int Hitting(int L, string hittingFile);
    int HittingAny(int L, int x, string hittingFile);
    int HittingParallel(int L, string hittingFile);
    int HittingRandomParallel(int L, string hittingFile);
    int maxLength();
    vector<int> pushBackVector();
    void removeEdge(int i);
    int stageOps(int l, double maxPtr, string hittingFile);
    void topologicalSort();
    private:
	int depthFirstSearch(int index, int u);

};
#endif