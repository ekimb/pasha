/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    decycling.h
    Header file for main operations for the decycling set calculation via Mykkeltveit's algorithm.
    @author Baris Ekim
    @version 1.0 4/15/19
*/
#ifndef DECYCLING_H
#define DECYCLING_H
#include "graph.h"
class decycling {
public:
    unsigned int ALPHABET_SIZE = 4;
    string ALPHABET = "ACGT";
    vector<unsigned int> computeDecyclingSet(unsigned int k) {
    /**
    Computes the decycling set for a complete de Bruijn graph.
    @param k: Order of the complete de Bruijn graph.
    @return The decycling set.
    */
    	vector<unsigned int> decyclingSet;
    	unsigned int a[k+1];
    	for (unsigned int i = 0; i <= k; i++) a[i] = 0;
    	unsigned int i = 1; a[0] = -1;
    	do {
    		if (k % i == 0) {
    			unsigned int decyclingNode = getEdge(a, i, decyclingSet.size()+1, k);
    	        decyclingSet.push_back(decyclingNode);
    		}
    		for (i = k; a[i] == ALPHABET_SIZE-1; i--);
    		if (i != 0) {
    			a[i] = a[i] + 1;
    			for (unsigned int j = i+1; j <= k; j++) a[j] = a[j-i];
    		}
    	} while (i != 0);
    	return decyclingSet;


    }
    unsigned int getEdge(unsigned int a[], unsigned int i, unsigned int d, unsigned int k){
    /**
    Computes one edge in the decycling set.
    @param a[]: Edge array, i: Edge index, d: Size of the set, k: Order of the graph.
    @return Index of the edge.
    */
        double u = 3.1415926 * 2.0 / (double)k;
        unsigned int q;
        if (i < k)
        	q = k;
        else {
        	for (q = 1;;q++) {
        		double s = 0;
                for (unsigned int l =1; l <= k; l++) {
                	s += a[l] * sin(((l - 1 + k - q) % k) * u);
                }
                if (s < .0001) break;
            }
            for (q++; q < k+k; q++) {
            	double s = 0;
                for (unsigned int l = 1; l <= k; l++)
                	s += a[l] * sin(((l - 1 + k + k - q) % k) * u);
                if (s >= 0.0001) break;
            }
            if (q > k) q -= k;
        }
        unsigned int code = 0;
        for (unsigned int j = q+1; j <= k; j++) code = ALPHABET_SIZE * code + a[j];
        for (unsigned int j = 1; j <= q; j++) code = ALPHABET_SIZE * code + a[j];

        return code;
    }
};
#endif