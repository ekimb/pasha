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
	static int ALPHABET_SIZE;
	static string ALPHABET;
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
			ALPHABET_SIZE = 4;
			ALPHABET = "ACGT";
		    edgeNum = pow(ALPHABET_SIZE, k);
		    edgeArray[edgeNum];
		    edgeCount = edgeNum;
			fill_n(edgeArray, edgeNum, 1);
			vertexCount = edgeNum / ALPHABET_SIZE; 

    }
    static char getChar(int i) {
    	return ALPHABET[i];
    }
    string getEdgeLabel(int i) {
    		return getString(i, k, ALPHABET_SIZE);
    }
    static string getString(int a, int k, int ALPHABET_SIZE) {
		string finalString = "";
		for (int i = 0; i < k; i++) {
			finalString = getChar(a % ALPHABET_SIZE) + finalString;
			a = a / ALPHABET_SIZE;
		}
		return finalString;
	}
	graph(int _k, const int ALPHABET_SIZE, string ALPHABET) {
		k = _k;
		generateGraph(k);
		map<char, int> alphabetMap;
		for (int i = 0; i < ALPHABET_SIZE; i++)
			alphabetMap.insert(pair<char,int>(ALPHABET[i], i));
    	}

};
#endif