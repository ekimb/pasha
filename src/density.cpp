  #include "density.h"
  using byte = uint8_t;

int main (int argc, char* argv[]) {
  bool random;
  const char* seqFile;
  int argLen;
  int randswitch = atoi(argv[1]);
  if (randswitch == 1) {
    random = true;
    argLen = atoi(argv[2]);
  }
  else {
    random = false;
    seqFile = argv[2];
  }
  int k = atoi(argv[3]);
  int L = atoi(argv[4]);  
  const char* UHSfile = argv[5];
  const char* decycfile = argv[6];
  if (random == true) {
    double totalDensity = 0;
    for (int i = 0; i < 10; i++) {
      density densityObj = density(k, L, argLen, seqFile, UHSfile, decycfile, random);
      densityObj.findMinSeq(densityObj.seq, k, L, densityObj.len, densityObj.UHS);
      double sparsity = (double)densityObj.sparseCount / (double)(densityObj.len - densityObj.w - k);
      sort(densityObj.minPosVector.begin(), densityObj.minPosVector.end());
      uint64_t uniqueCount = set<uint64_t>(densityObj.minPosVector.begin(), densityObj.minPosVector.end()).size();
      cout << uniqueCount << endl;
      double density = (double)uniqueCount / (densityObj.len - densityObj.w - k);
      totalDensity += density;
      cout << "Density: " << density << " Sparsity: " << sparsity << endl;
    }
    cout << "Avg density: " << totalDensity/10.0 << endl;
  }
}