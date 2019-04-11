#ifndef DECYCLING_H
#define DECYCLING_H
#include "graph.h"
class decycling {
public:
int ALPHABET_SIZE = 4;
string ALPHABET = "ACGT";
vector<int> computeDecyclingSet(int k) {
	vector<int> decyclingSet;
	// Iterate for each cycle in the graph
	int a[k+1];
	for (int i = 0; i <= k; i++) a[i] = 0;
	int i = 1; a[0] = -1;
	do {
		if (k % i == 0) {
			int decyclingNode = getEdge(a, i, decyclingSet.size()+1, k);
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
int getEdge(int a[], int i, int d, int k){
    double u = 3.1415926 * 2.0 / (double)k;
    int q;
    if (i < k)
    	q = k;
    else {
    	for (q = 1;;q++) {
    		double s = 0;
            for (int l =1; l <= k; l++) {
            	s += a[l] * sin(((l - 1 + k - q) % k) * u);
            }
            if (s < .0001) break;
        }
        for (q++; q < k+k; q++) {
        	double s = 0;
            for (int l = 1; l <= k; l++)
            	s += a[l] * sin(((l - 1 + k + k - q) % k) * u);
            if (s >= 0.0001) break;
        }
        if (q > k) q -= k;
    }
    int code = 0;
    for (int j = q+1; j <= k; j++) code = ALPHABET_SIZE * code + a[j];
    for (int j = 1; j <= q; j++) code = ALPHABET_SIZE * code + a[j];

    return code;



}
};
#endif