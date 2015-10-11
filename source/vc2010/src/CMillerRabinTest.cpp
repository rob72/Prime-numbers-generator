//CMillerRabinTest.cpp
#include "stdafx.h"
#include "CMillerRabinTest.h"


CMillerRabinTest::CMillerRabinTest(uint32_t _Bits)
{
  FBits=_Bits;
  FulMaxBits=2*FBits + mp_bits_per_limb;

  mpz_init2(FFn_minus_one,      FulMaxBits);
  mpz_init2(FFd,                FulMaxBits);
  mpz_init2(FFa_to_power,       FulMaxBits);
  mpz_init2(FFtmpRnd,           FulMaxBits);

  gmp_randinit_mt(FFGMPRandState);
}//-.-//

//-------

CMillerRabinTest::~CMillerRabinTest()
{
   mpz_clear(FFn_minus_one);
   mpz_clear(FFd);
   mpz_clear(FFa_to_power);
   mpz_clear(FFtmpRnd);

   gmp_randclear(FFGMPRandState);
}//-.-//

//-------

 void CMillerRabinTest::MemSecureRandomFill()
  {
   mpz_urandomb(FFn_minus_one,      FFGMPRandState, FulMaxBits);
   mpz_urandomb(FFd,                FFGMPRandState, FulMaxBits);
   mpz_urandomb(FFa_to_power,       FFGMPRandState, FulMaxBits);
   mpz_urandomb(FFtmpRnd,           FFGMPRandState, FulMaxBits);
  };//--//

//-------

 bool CMillerRabinTest::Test
     (mpz_t _mpzN, uint32_t _NumPasses)
 {
  uint32_t np, cp=0;
  //
  bool rv;
  np=_NumPasses;//np=mpz_sizeinbase(_mpzN, 2)+1;
  if(np<10)np=10;
   else if(np>10000)np=10000;
    
  int i, s;
  do
  {
    do
    {
    //генерирует равномерно распределенную случайную величину в
    //диапазоне от 0 до n-1 включительно
     mpz_urandomm(FFtmpRnd, FFGMPRandState, _mpzN);
    }while(mpz_sgn(FFtmpRnd)==0);
  //<pass>
 //-----------------------------------------------------	
  mpz_sub_ui(FFn_minus_one, _mpzN, 1);
  mpz_set(FFd, FFn_minus_one);
  
  s=0;
 
    while(mpz_even_p(FFd))
    {
     mpz_fdiv_q_2exp(FFd, FFd, 1);
     s++;
    }

  mpz_powm(FFa_to_power, FFtmpRnd, FFd, _mpzN);

  if (mpz_cmp_ui(FFa_to_power, 1) == 0)
  {
   return true;
  }

     for(i=0; i < s-1; i++)
     {
         if(mpz_cmp(FFa_to_power, FFn_minus_one)==0)
         {
          return true;
         }
      mpz_powm_ui(FFa_to_power, FFa_to_power, 2, _mpzN);
     }

   rv=(mpz_cmp(FFa_to_power, FFn_minus_one)==0);
        
  //</pass>
      if(!rv)break;
//-----------------------------------------------------	
//...........
  }while(--np);
 return rv;
 }//--//

 //-------