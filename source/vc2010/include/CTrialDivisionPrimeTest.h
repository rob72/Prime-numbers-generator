//CTrialDivisionPrimeTest.h
#ifndef __CTrialDivisionPrimeTestH__
#define __CTrialDivisionPrimeTestH__

#include <stdint.h>
#include "gmp.h"

#ifndef max
 #define max(a,b) (((a)>(b))?(a):(b))
#endif

#define CTrialDivisionPrimeTest_USE_TEST_MOD60 1

class CTrialDivisionPrimeTest
{
 private:
  mpz_t FFmpzTMP;
protected:
   uint32_t FMaxSmallPrime;
 public:
  CTrialDivisionPrimeTest();
  ~CTrialDivisionPrimeTest();
  bool TestForSmalls(mpz_t _mpzN1);
  bool Test(mpz_t _mpzN);
  bool Test(mpz_t _mpzN1, mpz_t _mpzN2);
  uint32_t GetMaxSmallPrime();
};

#endif

