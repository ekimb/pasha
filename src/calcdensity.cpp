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
  if ((count+1)*w > len) {
    window = new char[len-(count*w)];
    for (int i = 0; i < len-(count*w); i++) {
      window[i] = seq[i + count*w];
      cout << window[i];
      foo++;
    }
  }
  else {
    window = new char[w];
    for (int i = 0; i < w; i++) {

        window[i] = seq[i + count*w];
        cout << window[i];
        foo++;
    }
  }
  cout << endl;
  cout << foo << endl;
  return window;
}
string findMin (char* window, const int k, const int w, int count, const int len, unordered_set<string> UHS) {
  unordered_set<string>:: iterator check;
  string min = "Z";
  if ((count+1)*w > len) {
    for (int pos = 0; pos <= len-(count*w) - k + 1; pos++) {
      string testKmer = findTestKmer(window, k, pos);
      //cout << testKmer << pos << endl;
      if (min > testKmer) min = testKmer;

    }
  }
  else {
    for (int pos = 0; pos < w - k + 1; pos++) {
      string testKmer = findTestKmer(window, k, pos);
      if (min > testKmer) min = testKmer; 
      check = UHS.find(testKmer);
      if (check != UHS.end()) {
        cout << "kmer in UHS" << endl;
        if (testKmer == min) {
          cout << "kmer both minimizer and in UHS" << endl;
        }
        else cout << "kmer in UHS but not minimizer" << endl;
      } 
      //cout << testKmer << pos << endl;
      
    }
  }
  if (min.length() < k) {
    cout << "No minimizer" << endl;
    return "";
  }
  //cout << min << endl;
  return min;

}
void findMinSeq(char* seq, const int k, const int w, const int len, unordered_set<string> UHS) {

  int count = (len / w) + 1;
  for (int i = 0; i < count; i++) {
    char* testWindow = genWindow(seq, k, w, i, len);
    string min = findMin(testWindow, k, w, i, len, UHS);
    cout << min << "for window " << i+1 << endl;

  }
}

int main (int argc, char* argv[]) {
  const int len = atoi(argv[1]);
  const int k = atoi(argv[2]);
  const int L = atoi(argv[3]);
  const int w = L - k + 1;
  char* seq = genRandomSeq(len);
  const char* UHSfile = argv[4];
  cout << UHSfile << endl;
  string UHSkmer;
  unordered_set<string> UHS;
  ifstream fin(UHSfile);
  while (getline(fin, UHSkmer)) {
    UHS.insert(UHSkmer);
  }
  findMinSeq(seq, k, w, len, UHS);
  //char* testWindow = genWindow(seq, k, w, 1, len);
  //string min = findMin(testWindow, k, w, 1, len);

}