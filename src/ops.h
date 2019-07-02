/**
    Pasha: Parallel Algorithms for Approximating Compact Universal Hitting Sets
    ops.h
    Header file for main operations involving the calculation of the hitting set
    after the generation of the graph and the removal of the decycling set.
    @author Baris Ekim
    @version 1.0 4/15/19
*/
#ifndef OPS_H
#define OPS_H
#include "decycling.h"
#include "graph.h"

void graph::calculateForEach(int i, int L) {
/**
Calculates hitting number for an edge of specified index with respect to a specified
sequence length, counting paths of length L-k+1.
@param i: Index of edge, L: Sequence length.
*/
	double hittingNum = 0;
	for (int j = (1 - edgeArray[i]) * L; j < L; j++) hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
	hittingNumArray[i] = hittingNum;
}

void graph::calculateForEachAny(int i) {
/**
Calculates hitting number for an edge of specified index with respect to a specified
sequence length, counting all paths.
@param i: Index of edge.
*/
	double hittingNum = 0;
    hittingNum = edgeArray[i] * F[0][i % vertexExp] * D[0][i / ALPHABET_SIZE];
	hittingNumAnyArray[i] = hittingNum;
}
int graph::calculateHittingNumber(int L) {
/**
Calculates hitting number of all edges, counting paths of length L-k+1.
@param L: Sequence length.
@return imaxHittingNum: Index of vertex with maximum hitting number.
*/	
	double maxHittingNum = 0;
	int imaxHittingNum = -1;
    for (int i = 0; i < edgeNum; i++) {
        double hittingNum = 0;
        for (int j = (1 - edgeArray[i]) * L; j < L; j++) {
        	hittingNum = hittingNum + F[j][i % vertexExp] * D[(L-j-1)][i / ALPHABET_SIZE];
			hittingNumArray[i] = hittingNum;
        }
    	if (hittingNum > maxHittingNum) {maxHittingNum = hittingNum; imaxHittingNum = i;}
    }
    return imaxHittingNum;
}

byte* graph::calculateHittingNumberAny(int x) {
/**
Calculates hitting number of all edges, counting all paths.
@param x: Number of vertices to consider.
@return imaxHittingNum: Array of index of x edges with maximum hitting number.
*/
	maxHittingNum = new byte[x];
	imaxHittingNum = new byte[x];
    for (int i = 0; i < edgeNum; i++) {
        double hittingNum = 0;
        hittingNum = edgeArray[i] * F[0][i % vertexExp] * D[0][i / ALPHABET_SIZE];
		hittingNumAnyArray[i] = hittingNum;
        int j = 0;
        while (j < x && hittingNum <= maxHittingNum[j]) j++;
        if (j < x && hittingNum > maxHittingNum[j]) {maxHittingNum[j] = hittingNum; imaxHittingNum[j] = i;}
    }
    return imaxHittingNum;
}
int graph::calculateHittingNumberParallel(int L, bool random, int threads) {
/**
Calculates hitting number of all edges, counting paths of length L-k+1, in parallel.
@param L: Sequence length.
@return imaxHittingNum: Index of vertex with maximum hitting number.
*/	
	omp_set_dynamic(0);
	double maxHittingNum = 0;
	int imaxHittingNum = -1;
	int count = 0;
  	#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < edgeNum; i++) {
    	calculateForEach(i, L);
		if (random == true) {
			if (((hittingNumArray[i]) >= pow((1.0+epsilon), h-1)) && ((hittingNumArray[i]) <= pow((1.0+epsilon), h))) {
				stageArray[i] = 1;
				pick[i] = false;
				total += hittingNumArray[i] * stageArray[i];
				count++;
			}
			else {
				stageArray[i] = 0;
				pick[i] = false;
			}	
		}
    }
    for (int i = 0; i < edgeNum; i++) {
    	if (hittingNumArray[i]*edgeArray[i] > maxHittingNum) {maxHittingNum = hittingNumArray[i]; imaxHittingNum = i;}
    }
    return imaxHittingNum;
}
vector<int> graph::pushBackVector() {
    vector<int> stageVertices;
    for(int i = 0; i < edgeNum; i++) {
    	if (stageArray[i] == 1) stageVertices.push_back(i);
    }
    return stageVertices;
}


int graph::calculatePaths(int L, int threads) {
/**
Calculates number of L-k+1 long paths for all vertices.
@param L: Sequence length.
@return 1: True if path calculation completes.
*/
	omp_set_dynamic(0);
	vertexExp2 = vertexExp * 2;
    vertexExp3 = vertexExp * 3;
    vertexExpMask = vertexExp - 1;
    vertexExp_1 = pow(ALPHABET_SIZE, k-2);
	for (int i = 0; i < vertexExp; i++) {D[0][i] = 1; F[0][i] = 1;}
	for (int j = 1; j <= L; j++) {
  		#pragma omp parallel for num_threads(threads)
		for (int i = 0; i < vertexExp; i++) {
			int index = (i * 4);
            F[j][i] = edgeArray[index]*F[j-1][index & vertexExpMask] + edgeArray[index + 1]*F[j-1][(index + 1) & vertexExpMask] + edgeArray[index + 2]*F[j-1][(index + 2) & vertexExpMask] + edgeArray[index + 3]*F[j-1][(index + 3) & vertexExpMask];
            D[j][i] = edgeArray[i]*D[j-1][(i >> 2)] + edgeArray[i + vertexExp]*D[j-1][((i + vertexExp) >> 2)] + edgeArray[i + vertexExp2]*D[j-1][((i + vertexExp2) >> 2)] + edgeArray[i + vertexExp3]*D[j-1][((i + vertexExp3) >> 2)];
        }
	}
	return 1;
}
int graph::calculatePathsSeq(int L) {
/**
Calculates number of L-k+1 long paths for all vertices.
@param L: Sequence length.
@return 1: True if path calculation completes.
*/
	omp_set_dynamic(0);
	vertexExp2 = vertexExp * 2;
    vertexExp3 = vertexExp * 3;
    vertexExpMask = vertexExp - 1;
    vertexExp_1 = pow(ALPHABET_SIZE, k-2);
	for (int i = 0; i < vertexExp; i++) {D[0][i] = 1; F[0][i] = 1;}
	for (int j = 1; j <= L; j++) {
		for (int i = 0; i < vertexExp; i++) {
			int index = (i * 4);
            F[j][i] = edgeArray[index]*F[j-1][index & vertexExpMask] + edgeArray[index + 1]*F[j-1][(index + 1) & vertexExpMask] + edgeArray[index + 2]*F[j-1][(index + 2) & vertexExpMask] + edgeArray[index + 3]*F[j-1][(index + 3) & vertexExpMask];
            D[j][i] = edgeArray[i]*D[j-1][(i >> 2)] + edgeArray[i + vertexExp]*D[j-1][((i + vertexExp) >> 2)] + edgeArray[i + vertexExp2]*D[j-1][((i + vertexExp2) >> 2)] + edgeArray[i + vertexExp3]*D[j-1][((i + vertexExp3) >> 2)];
        }
	}
	return 1;
}
int graph::calculatePathsAny() {
/**
Calculates number of paths of any length for all vertices.
@return 1: True if path calculation completes.
*/
	vertexExp2 = vertexExp * 2;
    vertexExp3 = vertexExp * 3;
    vertexExpMask = vertexExp - 1;
    vertexExp_1 = pow(ALPHABET_SIZE, k-2);
    for (int i = 0; i < vertexExp; i++) {F[0][i] = 1; D[0][i] = 1;}
    for (int i = 0; i < vertexExp; i++) {
    	D[0][topoSort[i]] += edgeArray[topoSort[i]]*D[0][(topoSort[i] >> 2)] + edgeArray[topoSort[i] + vertexExp]*D[0][((topoSort[i] + vertexExp) >> 2)] + edgeArray[topoSort[i] + vertexExp2]*D[0][((topoSort[i] + vertexExp2) >> 2)] + edgeArray[topoSort[i] + vertexExp3]*D[0][((topoSort[i] + vertexExp3) >> 2)];
   	 	int index = (topoSort[vertexExp-i-1] * 4);
   	 	F[0][topoSort[vertexExp-i-1]] += edgeArray[index]*F[0][index & vertexExpMask] + edgeArray[index+1]*F[0][(index+1) & vertexExpMask] + edgeArray[index+2]*F[0][(index+2) & vertexExpMask] + edgeArray[index+3]*F[0][(index+3) & vertexExpMask];
    }
    return 1;
}
int graph::depthFirstSearch(int index, int u) {
/**
Depth-first search of a given index of an edge.
@param index: Depth of recursion, u: Index of edge.
@return -1: The search cycles, index+1: Current depth.
*/
	used[u] = true;
	bool cycle = false;
	for (int v : getAdjacent(u)) {
		if (used[v] == true && finished[v] == false) cycle = true;
		if (used[v] == false) {
			index = depthFirstSearch(index, v);
			cycle = cycle || (index == -1);
		}
	}
	finished[u] = true;
	topoSort[index] = u;
	if (cycle) return -1;
	else return index + 1;
}
vector<int> graph::getAdjacent(int v) {
/**
Get adjacent vertices to a given index of a vertex.
@param v: Index of vertex.
@return rc: Array of adjacent vertices.
*/
	int count = 0;
	int adjVertex[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		int index = v + i * vertexExp;
		if (edgeArray[index] == 1) adjVertex[count++] = index / ALPHABET_SIZE;
	}
	vector<int> rc(count);
	for (int i = 0; i < count; i++) {
		rc[i] = adjVertex[i];
	}
	return rc;
}

int graph::findLog(double base, double x) {
/**
Finds the logarithm of a given number with respect to a given base.
@param base: Base of logartihm, x: Input number.
@return (int)(log(x) / log(base)): Integer logarithm of the number and the given base.
*/
    return (int)(log(x) / log(base));
}

byte* graph::findMaxAny(int x) {
/**
Finds the x vertices with maximum hitting number.
@param x: Number of vertices to consider.
@return imaxHittingNum: Array of index of x edges with maximum hitting number.
*/
	for (int i = 0; i < edgeNum; i++){
		int j = 0;
		while (j < x && hittingNumAnyArray[i] <= maxHittingNum[j]) j++;
		if (j < x && hittingNumAnyArray[i] > maxHittingNum[j]) {
			maxHittingNum[j] = hittingNumAnyArray[i]; imaxHittingNum[j] = i;}
	}
	return imaxHittingNum;
}
int graph::maxLength() {
/**
Calculates the length of the maximum length path in the graph.
@return maxDepth: Maximum length.
*/
	vector<int> depth(vertexExp);
	int maxDepth = -1;
	for (int i = 0; i < vertexExp; i++) {
		int maxVertDepth = -1;
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			int edgeIndex = topoSort[i] + j * vertexExp;
			int vertexIndex = edgeIndex / ALPHABET_SIZE;
			if ((depth[vertexIndex] > maxVertDepth) && (edgeArray[edgeIndex] == 1)) maxVertDepth = depth[vertexIndex];
		}
		depth[topoSort[i]] = maxVertDepth + 1;
		if (depth[topoSort[i]] > maxDepth) {maxDepth = depth[topoSort[i]];}
	}
	return maxDepth;
}

void graph::removeEdge(int i) {
/**
Removes an edge from the graph.
@param i: Index of edge.
*/
	if (edgeArray[i] == 1) edgeCount--;
	edgeArray[i] = 0;
}

void graph::topologicalSort() {
/**
Traverses the graph in topological order.
*/
	for (int i = 0; i < vertexExp; i++) {used[i] = false; finished[i] = false;}
	int index = 0;
	for (int i = 0; i < vertexExp; i++) {
		if (used[i] == false) {
			index = depthFirstSearch(index, i);
			if (index == -1) {topoSort = NULL; return;}
		}
	}
	int rc[vertexExp];
	for (int i = 0; i < vertexExp; i++) rc[i] = topoSort[vertexExp-i-1];
}

#endif








