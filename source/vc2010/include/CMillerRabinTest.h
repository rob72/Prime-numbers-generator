//CMillerRabinTest.h
#ifndef __CMillerRabinTestH__
#define __CMillerRabinTestH__

#include <stdint.h>
#include <time.h>
#include "gmp.h"

//typedef VOID (CMillerRabinTest_OnPassEvent)(UINT*);

class CMillerRabinTest
{
 private:
    gmp_randstate_t FFGMPRandState;
    mpz_t           FFn_minus_one;
    mpz_t           FFd;
    mpz_t           FFa_to_power;
    mpz_t           FFtmpRnd;
 protected:
    uint32_t FulMaxBits;
    uint32_t FBits;
 public:
    CMillerRabinTest(uint32_t _Bits);
    ~CMillerRabinTest();
	 void MemSecureRandomFill();
	 bool Test(mpz_t _mpzN, uint32_t _NumPasses);
};
#endif

 