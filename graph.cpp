#include "graph.h"
using namespace std;
class graph {
	public:
	const int ALPHABET_SIZE = 4;
	const string ALPHABET = "ACGT";
	static int edgeArray[];
	int edgeCount = 0;
	int vertexCount = 0;

	void generateGraph(int k) {
    		int edgeNum = (int)Math.pow(ALPHABET_SIZE, k);
    		int edgeArray[edgeNum];
			fill_n(edgeArray, edgeNum, 1);
			edgeCount = edgeNum;
    		vertexCount = edgeNum / ALPHABET_SIZE;
    }
    static char getChar(int i) {
    	return ALPHABET[i];
    }
    string getEdgeLabel(int i) {
    		return getString(i, k, alphabetSize);
    }
    static string getString(int a, int k, int alphabetSize) {
		string finalString = "";
		for (int i = 0; i < k; i++) {
			finalString = getChar(a % ALPHABET_SIZE) + finalString;
			a = a / ALPHABET_SIZE;
		}
		return finalString;
	}







};
