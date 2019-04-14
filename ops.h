#ifndef OPS_H
#define OPS_H
#include "decycling.h"
#include "graph.h"
#include <cstdlib>
#include <iomanip>
#include <omp.h>
void graph::calculateForEach(int i, int L){
	double hittingNum = 0;
    for (int j = (1 - edgeVector[i]) * L; j < L; j++) {
    	hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
		//cout << hittingNum << endl;
    }
    hittingNumVector[i] = hittingNum;
}
void graph::calculateForEachAny(int i){
	double hittingNum = 0;
        hittingNum =  edgeVector[i] * F[0][i % vertexExp] * D[0][i / ALPHABET_SIZE];
		hittingNumAnyVector[i] = hittingNum;
}
int graph::calculateHittingNumber(int L) {
	double maxHittingNum = 0;
	int imaxHittingNum = -1;
	hittingNumVector = new double[edgeNum];
	//hittingNumVector.resize(edgeNum, 0);
    for (int i = 0; i < edgeNum; i++) {
        int hittingNum = 0;
        for (int j = (1 - edgeVector[i]) * L; j < L; j++) {
        	hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
			hittingNumVector[i] = hittingNum;
        }
        if (hittingNum > maxHittingNum) {maxHittingNum = hittingNum; imaxHittingNum = i;}
    }
    return imaxHittingNum;
}
vector<int> graph::calculateHittingNumberAny(int x) {
	vector<double> maxHittingNum (x);
	vector<int> imaxHittingNum (x);
	hittingNumAnyVector = new double[edgeNum];
    for (int i = 0; i < edgeNum; i++) {
        double hittingNum = 0;
        hittingNum = edgeVector[i] * F[0][i % vertexExp] * D[0][i / ALPHABET_SIZE];
		hittingNumAnyVector[i] = hittingNum;
			//cout << hittingNum << endl;
        int j = 0;
        while (j < x && hittingNum <= maxHittingNum[j]) j++;
        if (j < x && hittingNum > maxHittingNum[j]) {maxHittingNum[j] = hittingNum; imaxHittingNum[j] = i;}
    }
    //cout << imaxHittingNum << endl;
    return imaxHittingNum;
}
int graph::calculateHittingNumberParallel(int L) {
	omp_set_dynamic(0);
	//cout << "calchit" << endl;
	double maxHittingNum = 0;
	int imaxHittingNum = -1;
	//cout << vertexExp << ALPHABET_SIZE << F.size() << D.size() << edgeVector.size() << endl;
	hittingNumVector = new double[edgeNum];
  	#pragma omp parallel for num_threads(16)
    for (int i = 0; i < edgeNum; i++) {
    	calculateForEach(i, L);
    }
    for (int i = 0; i < edgeNum; i++) {
    	if (hittingNumVector[i]*edgeVector[i] > maxHittingNum) {maxHittingNum = hittingNumVector[i]; imaxHittingNum = i;};
    }
    //cout << imaxHittingNum << endl;
    return imaxHittingNum;
}
vector<int> graph::calculateHittingNumberParallelAny(int x) {
	omp_set_dynamic(0);
	vector<double> maxHittingNum (x);
	vector<int> imaxHittingNum (x);
	hittingNumAnyVector = new double[edgeNum];
	#pragma omp parallel for num_threads(16)
    for (int i = 0; i < edgeNum; i++) {
        calculateForEachAny(i);
	}
    imaxHittingNum = findMaxAny(x);
    //cout << imaxHittingNum << endl;
    return imaxHittingNum;
}
int graph::calculatePaths(int L) {
	vertexExp2 = vertexExp * 2;
    vertexExp3 = vertexExp * 3;
    vertexExpMask = vertexExp - 1;
    vertexExp_1 = pow(ALPHABET_SIZE, k-2);
	for (int i = 0; i < vertexExp; i++) {D[0][i] = 1; F[0][i] = 1;}
	for (int j = 1; j <= L; j++) {
		for (int i = 0; i < vertexExp; i++) {
			int index = (i * 4);
            F[j][i] = edgeVector[index]*F[j-1][index & vertexExpMask] + edgeVector[index + 1]*F[j-1][(index + 1) & vertexExpMask] + edgeVector[index + 2]*F[j-1][(index + 2) & vertexExpMask] + edgeVector[index + 3]*F[j-1][(index + 3) & vertexExpMask];
            D[j][i] = edgeVector[i]*D[j-1][(i >> 2)] + edgeVector[i + vertexExp]*D[j-1][((i + vertexExp) >> 2)] + edgeVector[i + vertexExp2]*D[j-1][((i + vertexExp2) >> 2)] + edgeVector[i + vertexExp3]*D[j-1][((i + vertexExp3) >> 2)];
        }
	}
	//cout << "Calculated paths." << endl;
	return 1;
}
int graph::calculatePathsAny() {
	vertexExp2 = vertexExp * 2;
    vertexExp3 = vertexExp * 3;
    vertexExpMask = vertexExp - 1;
    vertexExp_1 = pow(ALPHABET_SIZE, k-2);
    for (int i = 0; i < vertexExp; i++) {F[0][i] = 1; D[0][i] = 1;}
    for (int i = 0; i < topoSort.size(); i++) {
    	D[0][topoSort[i]] += edgeVector[topoSort[i]]*D[0][(topoSort[i] >> 2)] + edgeVector[topoSort[i] + vertexExp]*D[0][((topoSort[i] + vertexExp) >> 2)] + edgeVector[topoSort[i] + vertexExp2]*D[0][((topoSort[i] + vertexExp2) >> 2)] + edgeVector[topoSort[i] + vertexExp3]*D[0][((topoSort[i] + vertexExp3) >> 2)];
   	 	int index = (topoSort[topoSort.size()-i-1] * 4);
   	 	F[0][topoSort[topoSort.size()-i-1]] += edgeVector[index]*F[0][index & vertexExpMask] + edgeVector[index+1]*F[0][(index+1) & vertexExpMask] + edgeVector[index+2]*F[0][(index+2) & vertexExpMask] + edgeVector[index+3]*F[0][(index+3) & vertexExpMask];
    }
    return 1;
	}
int graph::depthFirstSearch(int index, int u) {
	//cout << u << endl;
	used[u] = 1;
	bool cycle = false;
	//cout << "here dfs" << endl;
	for (int v : getAdjacent(u)) {
		// Return true for cycle
		if (used[v] == 1 && finished[v] == 0) {cycle = true;}
		if (used[v] == 0) {
			index = depthFirstSearch(index, v);
			cycle = cycle || (index == -1);
		}
	}
	//cout << "dfs done" << endl;
	finished[u] = 1;
	//cout << "finished assign" << endl;
	res[index] = u;
	//cout << "finished res" << endl;
	if (cycle){	/*cout << "dfs done cycle " << u << endl;*/ return -1;}
	else { return index + 1;}
}
vector<int> graph::getAdjacent(int v) {
	int count = 0;
	int adjVertex[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		int index = v + i * vertexExp;
		if (edgeVector[index] == 1) adjVertex[count++] = index / ALPHABET_SIZE;
	}
	vector<int> rc(count);
	for (int i = 0; i < count; i++) {
		rc[i] = adjVertex[i];
	}
	return rc;
}
vector<int> graph::findMaxAny(int x) {
	vector<double> max(x);
	vector<int> imax(x);
	for (int i = 0; i < edgeNum; i++){
		int j = 0;
		while (j < x && hittingNumAnyVector[i] <= max[j]) j++;
		if (j < x && hittingNumAnyVector[i] > max[j]) {
			max[j] = hittingNumAnyVector[i]; imax[j] = i;}
	}
	return imax;
}
int graph::maxLength() {
	vector<int> depth(topoSort.size());
	int maxDepth = -1;
	for (int i = 0; i < topoSort.size(); i++) {
		int maxVertDepth = -1;
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			int edgeIndex = topoSort[i] + j * vertexExp;
			int vertexIndex = edgeIndex / ALPHABET_SIZE; // >> log;
			if (depth[vertexIndex] > maxVertDepth && edgeVector[edgeIndex] == 1) maxVertDepth = depth[vertexIndex];
		}
		depth[topoSort[i]] = maxVertDepth + 1;
		if (depth[topoSort[i]] > maxDepth) {maxDepth = depth[topoSort[i]];}
	}
	return maxDepth;
}
void graph::removeEdge(int i) {
	if (edgeVector[i] == 1) {
			edgeCount--;
			//if (S != null){
			//	S[i] = -1;
			//}
			//if (P != null){
			//	P[i] = -1;
			//}
			//if (pick != null){
			//	pick[i] = true;
			//}
			//System.out.println("Removing edge " + getEdgeLabel(i) + ", " + numEdges + " edges remaining");
	}
	edgeVector[i] = 0;
}
vector<int> graph::topologicalSort() {
	int n = vertexExp;
	vector<int> nullVector(n, 0);
	used.resize(n, 0);
	finished.resize(n, 0);
	res.resize(n);
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (used[i] == 0) {
			index = depthFirstSearch(index, i);
			if (index == -1) {cout << "null return" << endl; return nullVector;}
		}
	}
	vector<int> rc(res.size());
	for (int i = 0; i < rc.size(); i++)
		rc[i] = res[res.size()-i-1];
	return res;
}
#endif








