/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    decycling.cpp
    Finds the minimum decycling set of a complete de Bruijn graph of order k using Mykkeltveit's algorithm.
    @author Baris Ekim
    @version 1.0 4/09/19
*/
#include "decycling.h"
using namespace std;
void main(string outputFile, int k) {
	const int ALPHABET_SIZE = 4;
	const double PI = 3.1415926;
	const string ALPHABET = "ACGT";
	ofstream outputStream;
    vector<int> decyclingSet = computeDecyclingSet(k, ALPHABET_SIZE, ALPHABET); // Generate decycling set of order k
    graph newGraph = generateGraph(k, ALPHABET_SIZE, ALPHABET);
    outputStream.open(outputFile);
    for (int i = 0; i < decyclingSet.size(); i++) {
            	outputFile << newGraph.getLabel(decyclingSet.at(i)) + "\n";
    }
    outputStream.close();
}

vector<int> computeDecyclingSet(int k, const int ALPHABET_SIZE, const string ALPHABET) {
	vector<int> decyclingSet;
	// Iterate for each cycle in the graph
	int a[k+1];
	for (int i = 0; i <= k; i++) a[i] = 0;
	int i = 1; a[0] = -1;
	do {
		if (k % i == 0) {
			int decyclingNode = getEdge(a, i, decyclingSet.size()+1, k, ALPHABET_SIZE);
	        decyclingSet.push_back(decyclingNode);
		}
		for (i = k; a[i] == ALPHABET_SIZE-1; i--);
		if (i != 0) {
			a[i] = a[i] + 1;
			for (int j = i+1; j <= k; j++) a[j] = a[j-i];
		}
	} while (i != 0);
	return decyclingSet;


}
int getEdge(int a[], int k, int d, int k, const int ALPHABET_SIZE){
    double u = PI * 2.0 / (double)k;
    int q;
    if (i < k)
    	q = k;
    else {
    	for (q = 1;;q++) {
    		double s = 0;
            for (int l =1; l <= k; l++) {
            	s += a[l] * Math.sin(((l - 1 + k - q) % k) * u);
            }
            if (s < .0001) break;
        }
        for (q++; q < k+k; q++) {
        	double s = 0;
            for (int l = 1; l <= k; l++)
            	s += a[l] * Math.sin(((l - 1 + k + k - q) % k) * u);
            if (s >= 0.0001) break;
        }
        if (q > k) q -= k;
    }
    int code = 0;
    for (int j = q+1; j <= k; j++) code = ALPHABET_SIZE * code + a[j];
    for (int j = 1; j <= q; j++) code = ALPHABET_SIZE * code + a[j];

    return code;



}