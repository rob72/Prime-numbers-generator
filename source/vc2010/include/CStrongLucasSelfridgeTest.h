//CStrongLucasSelfridgeTest.h

#ifndef __CStrongLucasSelfridgeTestH__
#define __CStrongLucasSelfridgeTestH__

#include <stdint.h>
#include "gmp.h"

class CStrongLucasSelfridgeTest
{
private:
 	unsigned long FulDmax;
    mpz_t  mpzU;
	mpz_t  mpzV;
	mpz_t  mpzNplus1;
	mpz_t  mpzU2m;
	//
	mpz_t  mpzV2m;
	mpz_t  mpzQm;
	mpz_t  mpz2Qm;
    mpz_t  mpzT1;
	//
	mpz_t  mpzT2;
	mpz_t  mpzT3;
	mpz_t  mpzT4;
	mpz_t  mpzD;
	//
	mpz_t  mpzd;
	mpz_t  mpzQkd;
	mpz_t  mpz2Qkd;
 protected:
  uint32_t FBits;
  uint32_t FulMaxBits;

 public:
	 CStrongLucasSelfridgeTest(uint32_t _Bits);
	 ~CStrongLucasSelfridgeTest();
	 void MemSecureRandomFill();
	 bool Test(mpz_t _mpzN);
};
#endif