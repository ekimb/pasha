#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


using namespace std;
class graph {
	public:
	int ALPHABET_SIZE;
	string ALPHABET;
	int* edgeVector;
    double* hittingNumVector;
	//vector<int> hittingNumVector;
    double* hittingNumAnyVector;
	int* topoSort;
	static map<char, int> alphabetMap;
    int k;
    int l;
    int edgeNum;
	int edgeCount;
    int vertexCount; 
    //vector<vector<int>> D;
    double** D;
    //vector<vector<int>> F;
    double** F;
    vector<bool> used;
    vector<bool> finished;
    int* res;
    double epsilon;
    double delta;
    int vertexExp;
    int vertexExp2;
    int vertexExp3;
    int vertexExpMask;
    int vertexExp_1;
	void generateGraph(int k) {      
        edgeNum = pow(ALPHABET_SIZE, k);
        edgeVector = new int[edgeNum];
        //edgeVector.resize(edgeNum, 1);
        for (int i = 0; i < edgeNum; i++) edgeVector[i] = 1;
		edgeCount = edgeNum;
		vertexCount = edgeNum / ALPHABET_SIZE; 
    }
    char getChar(int i) {
    	return ALPHABET[i];
    }
    string getLabel(int i) {
		string finalString = "";
		for (int j = 0; j < k; j++) {
			//cout << k << ALPHABET_SIZE << ALPHABET << finalString << endl;
			finalString = getChar((i % ALPHABET_SIZE)) + finalString;
			i = i / ALPHABET_SIZE;
		}
		return finalString;
	}
	graph(int argK) {
		ALPHABET = "ACGT";
		ALPHABET_SIZE = 4;
		k = argK;
		generateGraph(k);
		map<char, int> alphabetMap;
		for (int i = 0; i < ALPHABET_SIZE; i++)
			alphabetMap.insert(pair<char,int>(ALPHABET[i], i));
    }
    int maxLength();
    void removeEdge(int i);
    int* topologicalSort();
    int Hitting(int L, string hittingFile);
    int HittingParallel(int L, string hittingFile);
    int HittingAny(int L, int x, string hittingFile);
    int HittingParallelAny(int L, int x, string hittingFile);
    vector<int> getAdjacent(int v);
    int calculatePaths(int L);
    int calculatePathsAny();
    int calculateHittingNumber(int L);
    int calculateHittingNumberParallel(int L);
    vector<int> calculateHittingNumberAny(int x);
    vector<int> calculateHittingNumberParallelAny(int x);
    void calculateForEach(int i, int L);
    void calculateForEachAny(int i);
    vector<int> findMaxAny(int x);
    private:
	int depthFirstSearch(int index, int u);


};
#endif