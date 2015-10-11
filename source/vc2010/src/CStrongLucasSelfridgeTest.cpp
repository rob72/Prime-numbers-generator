//CStrongLucasSelfridgeTest.cpp
#include "stdafx.h"
#include "CStrongLucasSelfridgeTest.h"


CStrongLucasSelfridgeTest::CStrongLucasSelfridgeTest(uint32_t _Bits)
{
  FBits=_Bits;
  FulMaxBits=2*FBits + mp_bits_per_limb;
  mpz_init2(mpzU,      FulMaxBits);
  mpz_init2(mpzV,      FulMaxBits);
  mpz_init2(mpzNplus1, FulMaxBits);
  mpz_init2(mpzU2m,    FulMaxBits);
  //
  mpz_init2(mpzV2m,    FulMaxBits);
  mpz_init2(mpzQm,     FulMaxBits);
  mpz_init2(mpz2Qm,    FulMaxBits);
  mpz_init2(mpzT1,     FulMaxBits);
  //
  mpz_init2(mpzT2,     FulMaxBits);
  mpz_init2(mpzT3,     FulMaxBits);
  mpz_init2(mpzT4,     FulMaxBits);
  mpz_init(mpzD);
  //
  mpz_init2(mpzd,      FulMaxBits);
  mpz_init2(mpzQkd,    FulMaxBits);
  mpz_init2(mpz2Qkd,   FulMaxBits);
}//-.-//

//-------

CStrongLucasSelfridgeTest::~CStrongLucasSelfridgeTest()
{
   mpz_clear(mpzU);
   mpz_clear(mpzV);
   mpz_clear(mpzNplus1);
   mpz_clear(mpzU2m);
   //
   mpz_clear(mpzV2m);
   mpz_clear(mpzQm);
   mpz_clear(mpz2Qm);
   mpz_clear(mpzT1);
   //
   mpz_clear(mpzT2);
   mpz_clear(mpzT3);
   mpz_clear(mpzT4);
   mpz_clear(mpzD);
   //
   mpz_clear(mpzd);
   mpz_clear(mpzQkd);
   mpz_clear(mpz2Qkd);
}//-.-//

//-------

bool CStrongLucasSelfridgeTest::Test(mpz_t _mpzN)
{
int iComp2, iP, iJ, iSign;
long lDabs, lD, lQ;
unsigned long /*ulMaxBits,*/ uldbits, ul, ulGCD, r, s;

FulDmax=0;

iComp2=mpz_cmp_si(_mpzN, 2);
if(iComp2 < 0)return(0);
if(iComp2==0)return(1);
if(mpz_even_p(_mpzN))return(0);
if(mpz_perfect_square_p(_mpzN))return(0);

//ulMaxBits=2*mpz_sizeinbase(_mpzN, 2) + mp_bits_per_limb;

lDabs=5;
iSign=1;
while(1)
  {
  lD=iSign*lDabs;
  iSign = -iSign;
  ulGCD=mpz_gcd_ui(NULL, _mpzN, lDabs);

  if((ulGCD > 1) && mpz_cmp_ui(_mpzN, ulGCD) > 0){return false;};
  mpz_set_si(mpzD, lD);
  iJ=mpz_jacobi(mpzD, _mpzN);
  if(iJ==-1)break;
  lDabs += 2;
  if(lDabs > FulDmax)FulDmax=lDabs;  
  if(lDabs > INT32_MAX-2)
    {
     //D overflows signed long in Lucas-Selfridge test.;
     {return false;}
    }
  }

iP=1;         
lQ=(1-lD)/4;  

mpz_add_ui(mpzNplus1, _mpzN, 1);
s=mpz_scan1(mpzNplus1, 0);
mpz_tdiv_q_2exp(mpzd, mpzNplus1, s);

mpz_set_ui(mpzU, 1);                      
mpz_set_ui(mpzV, iP);                     
mpz_set_ui(mpzU2m, 1);                    
mpz_set_si(mpzV2m, iP);                  
mpz_set_si(mpzQm, lQ);
mpz_set_si(mpz2Qm, 2*lQ);
mpz_set_si(mpzQkd, lQ);  

uldbits=mpz_sizeinbase(mpzd, 2);
  for(ul=1; ul < uldbits; ul++)  
  {
   mpz_mul(mpzU2m, mpzU2m, mpzV2m);
   mpz_mod(mpzU2m, mpzU2m, _mpzN);
   mpz_mul(mpzV2m, mpzV2m, mpzV2m);
   mpz_sub(mpzV2m, mpzV2m, mpz2Qm);
   mpz_mod(mpzV2m, mpzV2m, _mpzN);

   mpz_mul(mpzQm, mpzQm, mpzQm);
   mpz_mod(mpzQm, mpzQm, _mpzN);  
   mpz_mul_2exp(mpz2Qm, mpzQm, 1);
    if(mpz_tstbit(mpzd, ul))
    {
     mpz_mul(mpzT1, mpzU2m, mpzV);
     mpz_mul(mpzT2, mpzU, mpzV2m);
     mpz_mul(mpzT3, mpzV2m, mpzV);
     mpz_mul(mpzT4, mpzU2m, mpzU);
     mpz_mul_si(mpzT4, mpzT4, lD);
     mpz_add(mpzU, mpzT1, mpzT2);
      if(mpz_odd_p(mpzU))mpz_add(mpzU, mpzU, _mpzN);
     mpz_fdiv_q_2exp(mpzU, mpzU, 1);
     mpz_add(mpzV, mpzT3, mpzT4);
      if(mpz_odd_p(mpzV))mpz_add(mpzV, mpzV, _mpzN);
     mpz_fdiv_q_2exp(mpzV, mpzV, 1);
     mpz_mod(mpzU, mpzU, _mpzN);
     mpz_mod(mpzV, mpzV, _mpzN);
     mpz_mul(mpzQkd, mpzQkd, mpzQm); 
     mpz_mod(mpzQkd, mpzQkd, _mpzN);
    }
  }

if(mpz_sgn(mpzU)==0){return true;}
if(mpz_sgn(mpzV)==0){return true;}

mpz_mul_2exp(mpz2Qkd, mpzQkd, 1);  
  for(r=1; r < s; r++)
  {
   mpz_mul(mpzV, mpzV, mpzV);
   mpz_sub(mpzV, mpzV, mpz2Qkd);
   mpz_mod(mpzV, mpzV, _mpzN);
    if(mpz_sgn(mpzV)==0){return true;}

  if(r < s-1)
    {
    mpz_mul(mpzQkd, mpzQkd, mpzQkd);
    mpz_mod(mpzQkd, mpzQkd, _mpzN);
    mpz_mul_2exp(mpz2Qkd, mpzQkd, 1);
    }
  }

{return false;}
}//--//

//-------

 void CStrongLucasSelfridgeTest::MemSecureRandomFill()
  {
   gmp_randstate_t GMPRandState;
   gmp_randinit_mt(GMPRandState);
   //
   mpz_urandomb(mpzU,      GMPRandState, FulMaxBits);
   mpz_urandomb(mpzV,      GMPRandState, FulMaxBits);
   mpz_urandomb(mpzNplus1, GMPRandState, FulMaxBits);
   mpz_urandomb(mpzU2m,    GMPRandState, FulMaxBits);
   //
   mpz_urandomb(mpzT1,     GMPRandState, FulMaxBits);
   mpz_urandomb(mpzV2m,    GMPRandState, FulMaxBits);
   mpz_urandomb(mpzQm,     GMPRandState, FulMaxBits);
   mpz_urandomb(mpz2Qm,    GMPRandState, FulMaxBits);
   //
   mpz_urandomb(mpzD,      GMPRandState, FulMaxBits);
   mpz_urandomb(mpzT2,     GMPRandState, FulMaxBits);
   mpz_urandomb(mpzT3,     GMPRandState, FulMaxBits);
   mpz_urandomb(mpzT4,     GMPRandState, FulMaxBits);
   //
   mpz_urandomb(mpzd,      GMPRandState, FulMaxBits);
   mpz_urandomb(mpzQkd,    GMPRandState, FulMaxBits);
   mpz_urandomb(mpz2Qkd,   GMPRandState, FulMaxBits);
   //
   gmp_randclear(GMPRandState);
  };//--//

//-------