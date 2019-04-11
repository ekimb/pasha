#include "decycling.h"
#include "graph.h"
int graph::calculateHittingNumber(int L) {
	int maxHittingNum = 0;
	int imaxHittingNum = -1;
	hittingNumArray = new int[sizeof(edgeArray)];
	for (int i = 0; i < sizeof(edgeArray); i++) hittingNumArray[i] = 0;
    for (int i = 0; i < sizeof(edgeArray); i++) {
        int hittingNum = 0;
        for (int j = (1 - edgeArray[i]) * L; j < L; j++) {
        	hittingNum = hittingNum + F[j][i % vertexExp] * D[L-j-1][i / ALPHABET_SIZE];
			hittingNumArray[i] = hittingNum;
        }
        if (hittingNum > maxHittingNum) {maxHittingNum = hittingNum; imaxHittingNum = i;}
    }
    return imaxHittingNum;
}
double graph::calculatePaths(int L) {
	for (int i = 0; i < vertexExp; i++) {F[0][i] = D[0][i] = 1;}
	for (int j = 1; j <= L; j++) {
		for (int i = 0; i < vertexExp; i++) {
			int index = (i * 4);
            F[j][i] = edgeArray[index] * F[j-1][index & vertexExpMask] + edgeArray[index + 1] * F[j-1][(index + 1) & vertexExpMask] + edgeArray[index + 2] * F[j-1][(index + 2) & vertexExpMask] + edgeArray[index + 3] * F[j-1][(index + 3) & vertexExpMask];
            D[j][i] = edgeArray[i] * D[j-1][(i >> 2)] + edgeArray[i + vertexExp] * D[j-1][((i + vertexExp) >> 2)] + edgeArray[i + vertexExp2] * D[j-1][((i + vertexExp2) >> 2)] + edgeArray[i + vertexExp3] * D[j-1][((i + vertexExp3) >> 2)];
        }
	}
return 0;
}
int graph::depthFirstSearch(bool* used, bool* finished, int* res, int index, int u) {
	used[u] = true;
	bool cycle = false;
	int* adjVertex = getAdjacent(u);
	for (int v = 0; v < sizeof(adjVertex); v++) {
		// Return true for cycle
		if (used[v] && !finished[v]) cycle = true;
		if (!used[v]) {
			index = depthFirstSearch(used, finished, res, index, v);
			cycle = cycle || (index == -1);
		}
	}
	finished[u] = true;
	res[index] = u;
	if (cycle) return -1;
	else return index + 1;
}
int* graph::getAdjacent(int v) {
	int count = 0;
	int adjVertex[ALPHABET_SIZE];
	int vertexExp = pow(ALPHABET_SIZE, k-1);
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		int index = v + i * vertexExp;
		if (edgeArray[index] == 1)
			adjVertex[count++] = index / ALPHABET_SIZE;
	}
	int rc[count];
	for (int i = 0; i < count; i++) {
		rc[i] = adjVertex[i];
	}
	return rc;
}
int graph::Hitting(int L, string hittingFile) {
    int imaxHittingNum = -1;
    ofstream hittingStream;
    int hittingCount = 0;
    int l = L-k+1;
    D = new int*[l];
    for (int i = 0; i < l; i++)	D[i] = new int[vertexExp];
    F = new int*[l];
    for (int i = 0; i < l; i++)	F[i] = new int[vertexExp];
    hittingStream.open(hittingFile);
    while (calculatePaths(l) && (imaxHittingNum = calculateHittingNumber(l)) >= 0) {
        if (imaxHittingNum > -1) {
                removeEdge(imaxHittingNum);
	            string label = getLabel(imaxHittingNum); 
	            cout << "Writing " << label << endl;
	            hittingStream << label << "\n";
	            hittingCount++;
        }
    }
    int* topoSort = topologicalSort();
	cout << "Length of longest remaining path: " <<  maxLength() << endl;
    return hittingCount;
}

int graph::maxLength() {
	int depth[sizeof(topoSort)];
	int maxDepth = -1;
	for (int i = 0; i < sizeof(topoSort); i++) {
		int maxVertDepth = -1;
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			int edgeIndex = topoSort[i] + j * vertexExp;
			int vertexIndex = edgeIndex / ALPHABET_SIZE; // >> log;
			if (depth[vertexIndex] > maxVertDepth && edgeArray[edgeIndex] == 1) maxVertDepth = depth[vertexIndex];
		}
		depth[topoSort[i]] = maxVertDepth + 1;
		if (depth[topoSort[i]] > maxDepth) {maxDepth = depth[topoSort[i]];}
	}
	return maxDepth;
}
void graph::removeEdge(int i) {
	if (edgeArray[i] == 1) {
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
	edgeArray[i] = 0;
}
int* graph::topologicalSort() {
	int n = vertexCount;
	bool used[n];
	bool finished[n];
	int res[n];
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (!used[i]) {
			index = depthFirstSearch(used, finished, res, index, i);

			if (index == -1) return NULL;
		}
	}
	int rc[sizeof(res)];
	for (int i = 0; i < sizeof(rc); i++)
		rc[i] = res[sizeof(res)-i-1];
	return res;
}










