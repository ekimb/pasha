#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
class graph {
	public:
	int ALPHABET_SIZE;
	string ALPHABET;
	int* edgeArray;
	static map<char, int> alphabetMap;
    int k;
    int l;
    int edgeNum;
	int edgeCount;
    int vertexCount; 
    double* D;
    double* F;
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
		    edgeArray[edgeNum];
		    edgeCount = edgeNum;
			fill_n(edgeArray, edgeNum, 1);
			vertexCount = edgeNum / ALPHABET_SIZE; 

    }
    char getChar(int i, string ALPHABET) {
    	return ALPHABET[i];
    }
    string getLabel(int i, int k, int ALPHABET_SIZE, string ALPHABET) {
    		return getString(i, k, ALPHABET_SIZE, ALPHABET);
    }
    string getString(int a, int k, int ALPHABET_SIZE, string ALPHABET) {
		string finalString = "";
		for (int i = 0; i < k; i++) {
			finalString = getChar((a % ALPHABET_SIZE), ALPHABET) + finalString;
			cout << finalString << endl;
			a = a / ALPHABET_SIZE;
		}
		return finalString;
	}
	graph(int _k, int _ALPHABET_SIZE, string _ALPHABET) {
		int k = _k;
		int ALPHABET_SIZE = _ALPHABET_SIZE;
		string ALPHABET = _ALPHABET;
		generateGraph(k);
		map<char, int> alphabetMap;
		for (int i = 0; i < ALPHABET_SIZE; i++)
			alphabetMap.insert(pair<char,int>(ALPHABET[i], i));
    	}

};
#endif