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
	int* edgeArray;
	int* hittingNumArray;
	int* topoSort;
	static map<char, int> alphabetMap;
    int k;
    int l;
    int edgeNum;
	int edgeCount;
    int vertexCount; 
    int** D;
    int** F;
    double epsilon;
    double delta;
    int vertexExp = pow(ALPHABET_SIZE, k-1);
    int vertexExp2 = vertexExp * 2;
    int vertexExp3 = vertexExp * 3;
    int vertexExpMask = vertexExp - 1;
    int vertexExp_1 = pow(ALPHABET_SIZE, k-2);
    int vertexLog = (int)(log(ALPHABET_SIZE)/log(2));
	void generateGraph(int k) {
		    edgeNum = pow(ALPHABET_SIZE, k);
		    edgeArray = new int[edgeNum];
		    edgeCount = edgeNum;
			fill_n(edgeArray, edgeNum, 1);
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
    int* getAdjacent(int v);
    int depthFirstSearch(bool* used, bool* finished, int* res, int index, int u);
    double calculatePaths(int L);
    int calculateHittingNumber(int L);

};
#endif