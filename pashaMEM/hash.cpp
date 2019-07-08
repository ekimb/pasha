//Modified from Almutairy and Torng, PLoS One (2019)

#include "hash.h"

kmerHash  _getHash(const char * kmer, const unsigned int k) {
  unsigned int limit = strlen(kmer);
  kmerHash hash = 0;
  h |= BLAST2Bit(kmer[0]);
  for (unsigned int i = 1; (i < k) && (i < limit); i++) {
    h = h << 2;
    h |= BLAST2Bit(kmer[i]);
  }
  return h;
}

std::string _revHash(kmerHash hash, unsigned int k) {
  std::string kmer = "";
  unsigned int val = hash & 3;
  kmer += BLAST2BitRev(val);
  for (unsigned int i = 1; i < k; i++) {
      hash = hash >> 2;
      val = hash & 3;
      kmer += BLAST2BitRev(val);
  }
  std::reverse(kmer.begin(), kmer.end());
  return kmer;
}

unsigned int popCount(unsigned long long x) {
  unsigned int count;
  for (count = 0; x; count++) x &= x - 1;
  return count;
}

bool popCountBounded(unsigned long long x, unsigned int error) {
  unsigned int count;
  count = popcount(x);
  return (count < error + 1);
}

bool checkSim(unsigned long long x, unsigned long long y, unsigned int error) {
  unsigned long long z;
  z = x ^ y; 
  std::cout<<"Z: "<< z << std::endl;
  std::cout<<"Z: "<< _BLAST2BitRev(z, 4) << std::endl;
  return popCountBounded(z, error);
}
