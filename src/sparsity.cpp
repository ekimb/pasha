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

void generateDeBruijn(int t, int p, std::vector<int>& a, const int k, const int alphabet_size, ssize_t &length, ofstream &deBruijnSeq) {
  if(t > k) {
    if(k % p == 0) {
      for(int i = 1; i <= p && length != 0; ++i) {
        string ALPHABET = "ACGT";
        deBruijnSeq << ALPHABET[a[i]];
        length--;
      }
    }
  } else {
    a[t] = a[t-p];
    generateDeBruijn(t + 1, p, a, k, alphabet_size, length, deBruijnSeq);
    if(length == 0) return;
    for(int i = a[t-p] + 1; i < alphabet_size && length != 0; ++i) {
      a[t] = i;
      generateDeBruijn(t + 1, t, a, k, alphabet_size, length, deBruijnSeq);
    }
  }
}

void deBruijn(const int k, const int alphabet_size, ssize_t &length, ofstream &deBruijnSeq) {
  std::vector<int> a(k + 1, 0);

  generateDeBruijn(1, 1, a, k, alphabet_size, length, deBruijnSeq);
  if(length != 0)
    deBruijn(k, alphabet_size, length, deBruijnSeq);
}

ssize_t power(ssize_t x, ssize_t y) {
  ssize_t res = 1;
  for( ; y; y >>= 1, x *= x)
    if(y & 1)
      res *= x;
  return res;
}

int main(int argc, char* argv[]) {
  const int alphabet_size = 4;
  const int k = atoi(argv[1]);
  const int o = k + 20;
  const char* seqPath = argv[2];
  ofstream deBruijnSeq(seqPath);
  ssize_t length = power(alphabet_size, k);
  deBruijn(k, alphabet_size, length, deBruijnSeq);

  return 0;
}