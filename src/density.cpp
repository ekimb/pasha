  #include "density.h"
  using byte = uint8_t;

int main (int argc, char* argv[]) {
  const char* seqFile = argv[1];
  int k = atoi(argv[2]);
  int L = atoi(argv[3]);  
  const char* UHSfile = argv[4];
  const char* decycfile = argv[5];
  density densityObj = density(k, L, seqFile, UHSfile, decycfile);
  densityObj.findMinSeq(densityObj.seq, k, L, densityObj.len, densityObj.UHS);
  double sparsity = (double)densityObj.sparseCount / (double)(densityObj.len - L - 1);
  sort(densityObj.minPosVector.begin(), densityObj.minPosVector.end());
  uint64_t uniqueCount = set<uint64_t>(densityObj.minPosVector.begin(), densityObj.minPosVector.end()).size();
  cout << uniqueCount << endl;
  double density = (double)uniqueCount / (densityObj.len - densityObj.w - k);
  cout << "Density: " << density << " Sparsity: " << sparsity << endl;

}