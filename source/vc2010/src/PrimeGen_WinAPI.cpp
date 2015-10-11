//main.cpp
#include "stdafx.h"

#include <windows.h>
#include <time.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")

#include "PrimeGen_WinAPI.h"

static DWORD InputGBList[2]=
    {
     IDC_GROUPBOX1,
     IDC_EDIT1,
    };
static DWORD BaseGBList[2]=
    {
     IDC_BASE_GB,
     IDC_BASE_LIST,
    };
static DWORD OptsGBList[3]=
    {
     IDC_OPTS_GB,
     IDC_BLUME_CHECK,
     IDC_SAFE_CHECK,
    };
static DWORD ParamsGBList[5]=
    {
     IDC_PARAMS_GB,
     IDC_STATIC1,
     IDC_STATIC2,
     IDC_BITS_STT,
     IDC_TRUES_STT,
    };

static DWORD ProcessGBList[9]=
    {
     IDC_PROCESS_GB,
     IDC_TEST1N_STT,
     IDC_TEST2N_STT,
     IDC_TEST3N_STT,
     IDC_STATIC3,
     IDC_STATIC4,
     IDC_STATIC5,
     IDC_STATIC6
    };

//------
#define MAX_BITS 16384
#define MAX_BITS_FOR_SAFE 1024

#define PROGRAM_NAME \
    "Prime Numbers Generator"
#define ERR_BAD_INPUT_001_STR \
    "The input value does not match the number in the selected format"
#define EDIT1_DEFAULT_VALUE_STR \
    "df05df2216b82ee3361e36a4f5821a970b34a0af2ea51c5cdc42f75c317e454a72e98c78828ea647b1d48381f28b9749c0c365f4e60e3aa9dc5d3ba7e5cbc9b9c59f1d66ad128d7a06f36ba70278f46dac153a75cc89128701d321c1f6d4b0d6b74ac3d9ca1fda9167a079a02c056d7a47d692f5dba4516f2e3c9ae8f5ad67eb"
#define ERR_ABORTED_001_STR \
    "Aborted"
#define ERR_TOO_BIG_001_STR \
    "Too Big number"
#define ERR_TOO_BIG_002_STR \
    "Too Big number to generate safe prime (Max. <=1024 bits)"
static BOOL Busy=FALSE;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    DialogBox(hInstance, (LPCTSTR)IDD_MAIN_DIALOG, NULL, (DLGPROC)MainDlgProc);
    return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
  WORD wmId;
  WORD wmEvent;

  MakePrimeParams Params;
 //----------------------------------------------------------------------------
 //-- WM_INITDIALOG -----------------------------------------------------------
 //----------------------------------------------------------------------------
  if(wMsg==WM_INITDIALOG)
  {
    //
    ::SetWindowText(hDlg, TEXT(PROGRAM_NAME));
    // IDC_BASE_LIST
    SendMessage(GetDlgItem(hDlg,IDC_BASE_LIST), LB_ADDSTRING, 0, (LPARAM)TEXT("10"));
    SendMessage(GetDlgItem(hDlg,IDC_BASE_LIST), LB_ADDSTRING, 1, (LPARAM)TEXT("16L"));
    SendMessage(GetDlgItem(hDlg,IDC_BASE_LIST), LB_ADDSTRING, 2, (LPARAM)TEXT("16U"));
    SendMessage(GetDlgItem(hDlg,IDC_BASE_LIST), LB_ADDSTRING, 3, (LPARAM)TEXT("32"));
    SendMessage(GetDlgItem(hDlg,IDC_BASE_LIST), LB_ADDSTRING, 4, (LPARAM)TEXT("62"));
    //
    //
    SendMessage(GetDlgItem(hDlg,IDC_BASE_LIST), LB_SETCURSEL, 1, (LPARAM)0);
    // IDC_EDIT1
    SendMessage(GetDlgItem(hDlg, IDC_EDIT1), EM_LIMITTEXT, 1023, (LPARAM)0);
    SendMessage(GetDlgItem(hDlg, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)TEXT(EDIT1_DEFAULT_VALUE_STR));
    // IDC_BLUME_CHECK
    SendMessage(GetDlgItem(hDlg, IDC_BLUME_CHECK), BM_SETCHECK, BST_CHECKED, 0);
    // IDC_SAFE_CHECK
    SendMessage(GetDlgItem(hDlg, IDC_SAFE_CHECK), BM_SETCHECK, BST_UNCHECKED, 0);
    //
    SetLabelUI(hDlg, IDC_BITS_STT, 0xFFFFFFFF);
    SetLabelUI(hDlg, IDC_TRUES_STT, 0xFFFFFFFF);

    return TRUE;
  }
//----------------------------------------------------------------------------
//-- WM_CLOSE ----------------------------------------------------------------
//----------------------------------------------------------------------------
  else if(wMsg==WM_CLOSE)
  {
   if(!Busy)
   {
    EndDialog(hDlg, 0);
   }
   // PostQuitMessage(0);//if (wmId == IDOK || wmId == IDCANCEL){EndDialog(hDlg, wmId);}
   return !Busy;
  }
//----------------------------------------------------------------------------
//-- WM_COMMAND --------------------------------------------------------------
//----------------------------------------------------------------------------
  else if(wMsg==WM_COMMAND)
  {
   wmId    = LOWORD(wParam);
   wmEvent = HIWORD(wParam);
//----------------------------------------------------------------------------
//-- WM_COMMAND:IDC_BASE_LIST:LBN_SELCHANGE ------------------------------------
//----------------------------------------------------------------------------
   if(wmId==IDC_BASE_LIST&&(wmEvent==LBN_SELCHANGE))
   {
    if(!Busy)
    {
     INT SLen=::SendMessage(GetDlgItem(hDlg, IDC_EDIT1), EM_LINELENGTH, 0, 0);
       if((SLen>0)&&GetEdit1TextAsMPZ(hDlg, Params.mpzP))
       {
        mpz_clear(Params.mpzP);
       }
    }
   }
//----------------------------------------------------------------------------
//-- WM_COMMAND:IDC_SAFE_CHECK:BN_CLICKED ------------------------------------
//----------------------------------------------------------------------------
   else if(wmId==IDC_SAFE_CHECK&&(wmEvent==BN_CLICKED))
   {
     if(BST_CHECKED==
         (SendMessage(GetDlgItem(hDlg, IDC_SAFE_CHECK), BM_GETCHECK, 0, 0))
       )
     {
      SendMessage(GetDlgItem(hDlg, IDC_BLUME_CHECK), BM_SETCHECK, 1, 0);
      EnableWindow(GetDlgItem(hDlg, IDC_BLUME_CHECK), 0);
     }
     else
     {
      EnableWindow(GetDlgItem(hDlg, IDC_BLUME_CHECK), 1);
     }
   }
//----------------------------------------------------------------------------
//-- WM_COMMAND:IDC_EDIT1:EN_CHANGE ------------------------------------------
//----------------------------------------------------------------------------
   else if(wmId==IDC_EDIT1&&(wmEvent==EN_CHANGE))
   {
    if(!Busy)
    {
     INT SLen=::SendMessage(GetDlgItem(hDlg, IDC_EDIT1), EM_LINELENGTH, 0, 0);
       if((SLen>0)&&GetEdit1TextAsMPZ(hDlg, Params.mpzP))
       {
        mpz_clear(Params.mpzP);
       }
       else if(SLen>0)
       {
        ::MessageBox(hDlg,TEXT(ERR_BAD_INPUT_001_STR),TEXT(PROGRAM_NAME),MB_OK); 
       }
    }
   }
//----------------------------------------------------------------------------
//-- IDC_NEXT_PRIME_BUTTON ---------------------------------------------------
//----------------------------------------------------------------------------
   else if(wmId==IDC_NEXT_PRIME_BUTTON)
   {
    if(!Busy)
    {
     ::SetWindowText(GetDlgItem(hDlg, IDC_NEXT_PRIME_BUTTON), TEXT("Abort"));
     Busy=TRUE;
     SetLabelUI(hDlg, IDC_TEST1N_STT, 0xFFFFFFFF);
     SetLabelUI(hDlg, IDC_TEST2N_STT, 0xFFFFFFFF);
     SetLabelUI(hDlg, IDC_TEST3N_STT, 0xFFFFFFFF);

     EnableElements(hDlg, &InputGBList[0], ARRAYSIZE(InputGBList), 0);
     EnableElements(hDlg, &BaseGBList[0], ARRAYSIZE(BaseGBList), 0);
     EnableElements(hDlg, &OptsGBList[0], ARRAYSIZE(OptsGBList), 0);
     EnableElements(hDlg, &ParamsGBList[0], ARRAYSIZE(ParamsGBList), 0);

     memset(&Params, 0, sizeof(Params));
     Params.Blum=IsChecked(hDlg, IDC_BLUME_CHECK);
     Params.hWnd=hDlg;
       if(GetEdit1TextAsMPZ(hDlg, Params.mpzP))
       {
         if(mpz_sizeinbase(Params.mpzP, 2)>MAX_BITS)
         {
          ::MessageBox(hDlg,TEXT(ERR_TOO_BIG_001_STR),TEXT(PROGRAM_NAME),MB_OK);
         }
         else
         {
          if(IsChecked(hDlg, IDC_SAFE_CHECK))
          {
           if(mpz_sizeinbase(Params.mpzP, 2)>MAX_BITS_FOR_SAFE)
           {
            ::MessageBox(hDlg,TEXT(ERR_TOO_BIG_002_STR),TEXT(PROGRAM_NAME),MB_OK);
           }
           else MakeSafePrime(&Params);
          }                   
           else
             {MakePrime(&Params);}

          SetEdit1TextFromMPZ(hDlg, Params.mpzP);
         }
        mpz_clear(Params.mpzP);
       }
       else
       {
        ::MessageBox(hDlg,TEXT(ERR_BAD_INPUT_001_STR),TEXT(PROGRAM_NAME),MB_OK); 
       }
      if(Busy==FALSE)
      {
       ::MessageBox(hDlg,TEXT(ERR_ABORTED_001_STR),TEXT(PROGRAM_NAME),MB_OK);
      }
     EnableElements(hDlg, &InputGBList[0], ARRAYSIZE(InputGBList), 1);
     EnableElements(hDlg, &BaseGBList[0], ARRAYSIZE(BaseGBList), 1);
     EnableElements(hDlg, &OptsGBList[0], ARRAYSIZE(OptsGBList), 1);
     EnableElements(hDlg, &ParamsGBList[0], ARRAYSIZE(ParamsGBList), 1);
     SendMessage(GetDlgItem(hDlg, IDC_SAFE_CHECK), BM_SETCHECK, 0, 0);

     SetLabelUI(hDlg, IDC_TEST1N_STT, Params.Cnt[0]);
     SetLabelUI(hDlg, IDC_TEST2N_STT, Params.Cnt[1]);
     SetLabelUI(hDlg, IDC_TEST3N_STT, Params.Cnt[2]);
     SetLabelUI(hDlg, IDC_SECS_STT,   Params.mSecs);
     ::SetWindowText(GetDlgItem(hDlg, IDC_NEXT_PRIME_BUTTON), TEXT("Make prime"));
     ::FlashWindow(hDlg, 1);
     Busy=FALSE;
    }
    else
    {
     Busy=FALSE;
    }

   }
   return TRUE; 
  }
 return FALSE; 
}//--//

//-------

BOOL IsChecked(HWND hDlg, DWORD _CheckBoxID)
{
 return (BST_CHECKED==(SendMessage(GetDlgItem(hDlg, _CheckBoxID), BM_GETCHECK, 0, 0)));
}//--//

//-------

INT GetLBItemIndex(HWND hDlg, DWORD _ListBoxID)
{
 return (INT)SendMessage(GetDlgItem(hDlg, _ListBoxID), LB_GETCURSEL, 0, 0);
}//--//

//-------

VOID SetLabelUI(HWND hDlg, DWORD _LabelID, UINT _UI)
{
 CHAR  b[16];
 #ifdef  UNICODE
  LPWSTR buf=(LPWSTR)b;
 #else
  LPSTR buf=(LPSTR)b;
 #endif
  if(_UI<0xFFFFFFFF) 
   wsprintf(buf, TEXT("%u"), _UI);
  else 
   wsprintf(buf, TEXT("%s"), TEXT("_"));
 //SendMessage(GetDlgItem(hDlg, _LabelID), WM_SETTEXT, 0, (LPARAM)buf);
 SetDlgItemText(hDlg, _LabelID, buf);
}//-//

//-------
 
VOID EnableElements(HWND hDlg, DWORD* _pIDs, UINT _IDsSize, BOOL _En)
{
 UINT i;
 _IDsSize&=0xFF;
 for(i=0; i<_IDsSize; i++)
 EnableWindow(GetDlgItem(hDlg, _pIDs[i]), _En);
}//--//

//-------

VOID ShowElements(HWND hDlg, DWORD* _pIDs, UINT _IDsSize, BOOL _Show)
{
 UINT i;
 _IDsSize&=0xFF;
 for(i=0; i<_IDsSize; i++)
  ShowWindow(GetDlgItem(hDlg, _pIDs[i]), _Show?SW_SHOW:SW_HIDE);
}//--//

//-------

BOOL GetEdit1TextAsMPZ(HWND hDlg, mpz_t _P)
{
 char b[2048];
 LPSTR buf=(LPSTR)b;
 GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT1), &buf[0], 2047);
 INT ii=(INT)SendMessage(GetDlgItem(hDlg, IDC_BASE_LIST), LB_GETCURSEL, 0, 0);
 UINT i, Base, Bits, Trues;
 BOOL Success=TRUE;
  switch(ii)
  {
   case 0: Base=10; break;
   case 1: case 2: Base=16; break;
   case 3: Base=32; break;
   case 4: Base=62; break;
   default: Success=FALSE; break;
  }
  //uint32_t ulMaxBits=2*sizeof(uint32_t)*8 + mp_bits_per_limb;mpz_init2(FFmpzTMP, ulMaxBits);
  Success &= (mpz_init_set_str(_P, (char*)buf, Base)!=-1);
   if(Success)
   {
    Bits=mpz_sizeinbase(_P, 2);
    Trues=0;
      for(i=0; i<Bits; i++)
       if(mpz_tstbit(_P, i))Trues++;
    SetLabelUI(hDlg, IDC_BITS_STT,  Bits);
    SetLabelUI(hDlg, IDC_TRUES_STT, Trues);
   }
   else
   {
    SetLabelUI(hDlg, IDC_BITS_STT, 0xFFFFFFFF);
    SetLabelUI(hDlg, IDC_TRUES_STT, 0xFFFFFFFF);
   }
  return Success;
}//--//

//-------------------------------------------

VOID SetEdit1TextFromMPZ(HWND hDlg, mpz_t _P)
{
 char b[2048];
 LPSTR buf=(LPSTR)b;
 INT ii=(INT)SendMessage(GetDlgItem(hDlg, IDC_BASE_LIST), LB_GETCURSEL, 0, 0);
 UINT i, Base, Bits, Trues;
  switch(ii)
  {
   case 0: Base=10; break;
   case 1: case 2: Base=16; break;
   case 3: Base=32; break;
   case 4: Base=62; break;
   default: return;
  }
  mpz_get_str(&b[0], Base, _P);
   if(ii==2)
   {
    for(i=0; i<2048; i++)
    {
      if(b[i]>='0'&&b[i]<='9')continue;
       else if(b[i]>='a'&&b[i]<='z')b[i]-=('a'-'A');       
        else break;
    }
   }
  SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT1), &b[0]);

    Bits=mpz_sizeinbase(_P, 2);
    SetLabelUI(hDlg, IDC_TRUES_STT, Bits);
    Trues=0;
      for(i=0; i<Bits; i++)
       if(mpz_tstbit(_P, i))Trues++;
    SetLabelUI(hDlg, IDC_BITS_STT,  Bits);
    SetLabelUI(hDlg, IDC_TRUES_STT, Trues);
}//--//

//-------------------------------------------

VOID UpdateLabels(const MakePrimeParams* _pParams)
{
 if(_pParams->hWnd)
 {
  SetLabelUI(_pParams->hWnd, IDC_TEST1N_STT, _pParams->Cnt[0]);
  SetLabelUI(_pParams->hWnd, IDC_TEST2N_STT, _pParams->Cnt[1]);
  SetLabelUI(_pParams->hWnd, IDC_TEST3N_STT, _pParams->Cnt[2]);
  SetLabelUI(_pParams->hWnd, IDC_SECS_STT,   _pParams->mSecs);
 }
}//--//

//-------

VOID ProcessMessages(HWND _hWnd)
{
 MSG Msg;
 do
 {
   if(!::PeekMessage(&Msg, _hWnd, 0, 0, PM_NOREMOVE))break;
	if (::GetMessage(&Msg, _hWnd, 0, 0))
    {
	 ::TranslateMessage(&Msg);
	 ::DispatchMessage(&Msg);
    }	
 }while(1);
}//--//

//-------------------------------------------

VOID MakePrime(MakePrimeParams *_pParams)
{
 BOOL Success;
 UINT Bits;
 //UINT i;
// MSG Msg;

 uint32_t MaxSmallPrimeSqr;
 clock_t clock1=clock();
 clock_t clock2;

 _pParams->Cnt[0]=0;
 _pParams->Cnt[1]=0;
 _pParams->Cnt[2]=0;
  UpdateLabels(_pParams);

  Bits=mpz_sizeinbase(_pParams->mpzP, 2);
 
   if(Bits>MAX_BITS)
   { 
    return;      
   }

 CTrialDivisionPrimeTest*   pTrialDivisionTest =new CTrialDivisionPrimeTest();
 MaxSmallPrimeSqr=pTrialDivisionTest->GetMaxSmallPrime();
 MaxSmallPrimeSqr*=MaxSmallPrimeSqr;

 CMillerRabinTest*          pMillerRabinTest   =new CMillerRabinTest(Bits);
 CStrongLucasSelfridgeTest* pStrongLucasSelfridgeTest;

 mpz_setbit(_pParams->mpzP, 0);     //Чётное

 do
 {
  if(Busy==FALSE)break;
   if(_pParams->Blum)
    mpz_setbit(_pParams->mpzP, 1); 

  Success=pTrialDivisionTest->Test(_pParams->mpzP);
  _pParams->Cnt[0]++;

   if(!Success){mpz_add_ui(_pParams->mpzP, _pParams->mpzP, 2); continue;}

   //Для маленьких чисел остальные тесты не нужны
   if(mpz_cmp_ui(_pParams->mpzP, MaxSmallPrimeSqr)<=0)break;
       
  Success=pMillerRabinTest->Test(_pParams->mpzP, 14);
  _pParams->Cnt[1]++;

  //
    clock2=clock();
   _pParams->mSecs=(int)( (((float)(clock2)-(float)clock1)/CLOCKS_PER_SEC)*1000);
    UpdateLabels(_pParams);
    ProcessMessages(NULL);
  //

   if(!Success){mpz_add_ui(_pParams->mpzP, _pParams->mpzP, 2); continue;}

  pStrongLucasSelfridgeTest=new CStrongLucasSelfridgeTest(Bits);
  Success=pStrongLucasSelfridgeTest->Test(_pParams->mpzP);
  _pParams->Cnt[2]++;

   if(!Success){delete pStrongLucasSelfridgeTest; mpz_add_ui(_pParams->mpzP, _pParams->mpzP, 2); continue;}
  delete pStrongLucasSelfridgeTest;
  break;

 }while(1);

 delete  pMillerRabinTest;
 delete  pTrialDivisionTest;
 }//--//

//-----------------------------------------------

VOID MakeSafePrime(MakePrimeParams *_pParams)
{
 // 5, 7, 11, 23, 47, 59, 83, 107, 167, 179, 227, 263, 347, 359, 383, 467, 479,
 // 503, 563, 587, 719, 839, 863, 887, 983, 1019, 1187, 1283, 1307, 1319, 1367,
 // 1439, 1487, 1523, 1619, 1823, 1907...
 const uint32_t StepVal=2;
 BOOL Success;
 UINT Bits;

 uint32_t MaxSmallPrimeSqr;
// uint32_t tmpUI;
 const clock_t clock1=clock();
 clock_t clock2;

 _pParams->Cnt[0]=0;
 _pParams->Cnt[1]=0;
 _pParams->Cnt[2]=0;
 _pParams->mSecs =0;

 UpdateLabels(_pParams);

  Bits=mpz_sizeinbase(_pParams->mpzP, 2);
  
  if(Bits>MAX_BITS_FOR_SAFE)
  { 
   return;
  }

  else if(mpz_cmp_ui(_pParams->mpzP, 5)<=0)
  {
   mpz_set_ui(_pParams->mpzP, 5);
   return;
  }

 CTrialDivisionPrimeTest*   pTrialDivisionTest =new CTrialDivisionPrimeTest();
 MaxSmallPrimeSqr=pTrialDivisionTest->GetMaxSmallPrime();
 MaxSmallPrimeSqr*=MaxSmallPrimeSqr;

 CMillerRabinTest*          pMillerRabinTest   =new CMillerRabinTest(Bits);
 CStrongLucasSelfridgeTest* pStrongLucasSelfridgeTest;

 uint32_t ulMaxBits=2*Bits + mp_bits_per_limb;
 mpz_init2(_pParams->mpzP2, ulMaxBits);

 mpz_init(_pParams->mpzP3);

 do
 {
  if(Busy==FALSE)break;

     mpz_setbit(_pParams->mpzP, 0); //Чётное
   if(_pParams->Blum)
     mpz_setbit(_pParams->mpzP, 1);

     mpz_set(_pParams->mpzP2, _pParams->mpzP);
     mpz_clrbit(_pParams->mpzP2, 0);//-1

  //TrialDivisionTest Не проверяет на 2,
  //поэтому пока не делим _pParams->mpzP2 на 2, сделаем это позже.
    // MessageBox(_pParams->hWnd, TEXT(""), TEXT(""), MB_OK);
  Success=pTrialDivisionTest->Test(_pParams->mpzP, _pParams->mpzP2);
  _pParams->Cnt[0]++;
   if(!Success){ mpz_add_ui(_pParams->mpzP, _pParams->mpzP, StepVal); continue;}
   
   //А сейчас уже надо делить на 2
   mpz_div_ui(_pParams->mpzP2, _pParams->mpzP2, 2);

  Success=pMillerRabinTest->Test(_pParams->mpzP, 14);
   if(Success)
     Success=pMillerRabinTest->Test(_pParams->mpzP2, 14);

   _pParams->Cnt[1]++;

   if(_pParams->Cnt[1]%17==0)
   {
    clock2=clock();
   _pParams->mSecs=(int)( (((float)(clock2)-(float)clock1)/CLOCKS_PER_SEC)*1000);
   }
   else if(_pParams->Cnt[1]%7==0)
   {
    UpdateLabels(_pParams);
    ProcessMessages(NULL);
   }
   

   if(!Success){mpz_add_ui(_pParams->mpzP, _pParams->mpzP, StepVal); continue;}

  pStrongLucasSelfridgeTest=new CStrongLucasSelfridgeTest(Bits);
  Success=pStrongLucasSelfridgeTest->Test(_pParams->mpzP);
  _pParams->Cnt[2]++;

   if(!Success){delete pStrongLucasSelfridgeTest; mpz_add_ui(_pParams->mpzP, _pParams->mpzP, StepVal); continue;}
  delete pStrongLucasSelfridgeTest;
  break;

 }while(1);

  clock2=clock();
  _pParams->mSecs=(int)( (((float)(clock2)-(float)clock1)/CLOCKS_PER_SEC)*1000);
  UpdateLabels(_pParams);
  ProcessMessages(NULL);

  delete  pMillerRabinTest;
  delete  pTrialDivisionTest;
  mpz_clear(_pParams->mpzP2);
  mpz_clear(_pParams->mpzP3);
 }//--//

