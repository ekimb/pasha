/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    decycling.cpp
    Finds the minimum decycling set of a complete de Bruijn graph of order k using Mykkeltveit's algorithm.
    @author Baris Ekim
    @version 1.0 4/09/19
*/
#include "decycling.h"
#include "graph.h"
using namespace std;
void main(string outputFile, int k) {
	const int ALPHABET_SIZE = 4;
	const double PI = 3.1415926;
	const string ALPHABET = "ACGT";
	ofstream outputStream;
    vector<int> decyclingSet = computeDecyclingSet(k, ALPHABET_SIZE, ALPHABET); // Generate decycling set of order k
    generateGraph(k);
    outputStream.open(outputFile);
    for (int i = 0; i < decyclingSet.size(); i++) {
            	outputFile << newGraph.getLabel(decyclingSet.at(i)) + "\n";
    }
    outputStream.close();
}

