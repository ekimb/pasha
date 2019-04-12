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
	vector<int> edgeVector;
	vector<int> hittingNumVector;
	vector<int> topoSort;
	static map<char, int> alphabetMap;
    int k;
    int l;
    int edgeNum;
	int edgeCount;
    int vertexCount; 
    vector<vector<int>> D;
    vector<vector<int>> F;
    vector<bool> used;
    vector<bool> finished;
    vector<int> res;
    double epsilon;
    double delta;
    int vertexExp;
    int vertexExp2;
    int vertexExp3;
    int vertexExpMask;
    int vertexExp_1;
	void generateGraph(int k) {
		    edgeNum = pow(ALPHABET_SIZE, k);
		    edgeVector.resize(edgeNum, 1);
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
    vector<int> topologicalSort();
    int Hitting(int L, string hittingFile);
    int HittingParallel(int L, string hittingFile);
    vector<int> getAdjacent(int v);
    int calculatePaths(int L);
    int calculateHittingNumber(int L);
    int calculateHittingNumberParallel(int L);
    int calculateForEach(int i, int L);
	private:
	int depthFirstSearch(int index, int u);


};
#endif