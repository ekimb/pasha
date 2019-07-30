  #include <iostream>
  #include <sys/resource.h> 
  #include <sys/time.h> 
  #include <unistd.h> 
  #include <stdio.h> 
  #include <time.h>
  #include<bits/stdc++.h>
  #include <sys/stat.h> 
  #include <sys/types.h> 
  #include "pdocks.h"
  #include <unordered_set>
  #include <iostream>
  #include <fstream>
  #include <cmath>
  #include <string>
  #include <vector>
  #include <algorithm>
  #include <initializer_list>
  #include <map>
  #include <cstdlib>
  #include <iomanip>
  #include <cstdint>
  #include <omp.h>
  #include <limits>
  #include "half.hpp"

char* genRandomSeq(const int len) {
    char* seq = new char[len];
    static const char alphabet[] =
        "A"
        "C"
        "G"
        "T";

    for (int i = 0; i < len; ++i) {
        seq[i] = alphabet[rand() % (sizeof(alphabet) - 1)];
        //cout << seq[i] << endl;
    }
    return seq;
}

string findTestKmer(char* window, const int k, int pos) {
  string kmer = "";
  for(int i = 0; i < k; i++) {
    kmer = kmer + window[pos+i];
    //cout << kmer << i << endl;
  }

  return kmer;
}
char* genWindow (char* seq, const int k, const int w, int count, const int len) {
  int foo = 0;
  char* window;
  window = new char[w];
  for (int i = 0; i < w; i++) {
    window[i] = seq[i + count];
    //cout << window[i];
    foo++;
  }
  //cout << endl;
  //cout << foo << endl;
  return window;
}
int findMin (char* window, const int k, const int w, int count, const int len, unordered_set<string> UHS) {
  unordered_set<string>:: iterator check;
  string min = "Z";
  int minPos;
  for (int pos = 0; pos < w - k + 1; pos++) {
    string testKmer = findTestKmer(window, k, pos);
    //cout << "Testing " << testKmer << endl;
    check = UHS.find(testKmer);
    if (check != UHS.end()) {
      //cout << "kmer in UHS" << endl;
      if (min > testKmer) {
        min = testKmer;
        minPos = pos;
      }
    }

  }
  //cout << "Min: " << min << " in position: " << minPos+count << endl;
  return minPos+count;

}
int findUmer (char* window, const int k, const int w, int count, const int len, unordered_set<string> UHS) {
  unordered_set<string>:: iterator check;
  int umerCount = 0;
  for (int pos = 0; pos < w - k + 1; pos++) {
    string testKmer = findTestKmer(window, k, pos);
    //cout << "Testing " << testKmer << endl;
    check = UHS.find(testKmer);
    if (check != UHS.end()) {
      umerCount++;
    }
  }
  //cout << "Universal k-mer count: " << umerCount << endl;
  return umerCount;

}
double calcSparse(vector<int> umerVector, int count) {
  int sparseCount = 0;
  for (int i = 0; i < umerVector.size(); i++) {
    if (umerVector[i] == 1) sparseCount++;
  }
  double sparsity = (double)sparseCount / (double)count;
  return sparsity;
}

 vector< vector<int> > findMinSeq(char* seq, const int k, const int w, const int len, unordered_set<string> UHS) {
  omp_set_dynamic(0);
  vector<int> minPosVector;
  vector<int> umerVector;
  vector< vector<int> > res;
  int count = len - w + 1;
  #pragma omp parallel for num_threads(24)
  for (int i = 0; i < count; i++) {
    cout << "Window " << i+1 << endl;
    char* testWindow = genWindow(seq, k, w, i, len);
    int minPos = findMin(testWindow, k, w, i, len, UHS);
    int umerCount = findUmer(testWindow, k, w, i, len, UHS);
    minPosVector.push_back(minPos);
    umerVector.push_back(umerCount);
  }
  res.push_back(minPosVector);
  res.push_back(umerVector);

  return res;
}



int main (int argc, char* argv[]) {
  const int len = atoi(argv[1]);
  const int k = atoi(argv[2]);
  const int L = atoi(argv[3]);
  const int w = L - k + 1;
  char* seq = genRandomSeq(len);
  const char* UHSfile = argv[4];
  const char* decycfile = argv[5];
  string UHSkmer;
  string decyckmer;
  unordered_set<string> UHS;
  ifstream fin(UHSfile);
  ifstream fin2(decycfile);
  while (getline(fin, UHSkmer)) {
    UHS.insert(UHSkmer);
  }
  while(getline(fin2, decyckmer)) {
    UHS.insert(decyckmer);
  }
  vector< vector <int> > resVector = findMinSeq(seq, k, w, len, UHS);
  vector<int> minPosVector = resVector[0];
  vector<int> umerVector = resVector[1];
  double sparsity = calcSparse(umerVector, len - w - 1);
  //cout << minPosVector.size() << endl;
  sort(minPosVector.begin(), minPosVector.end());
  int uniqueCount = set<int>(minPosVector.begin(), minPosVector.end()).size();
  double density = (double)uniqueCount / (len - w - k);
  cout << "Density: " << density << " Sparsity: " << sparsity << endl;

}