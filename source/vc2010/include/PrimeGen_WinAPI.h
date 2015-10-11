#pragma once

#include <stdint.h>
#include <stdio.h>
#include "resource.h"

#include "gmp.h"
//#define CTrialDivisionPrimeTest_NPRIMES 2000

#include "CTrialDivisionPrimeTest.h"
#include "CMillerRabinTest.h"
#include "CStrongLucasSelfridgeTest.h"

typedef struct
{
 HWND  hWnd;
 mpz_t mpzP;
 mpz_t mpzP2;
 mpz_t mpzP3;
 UINT  Cnt[3];
 BOOL  Blum;
 int mSecs;
}MakePrimeParams;

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);

BOOL IsChecked(HWND hDlg, DWORD _CheckBoxID);
INT GetLBItemIndex(HWND hDlg, DWORD _ListBoxID);
VOID SetLabelUI(HWND hDlg, DWORD _LabelID, UINT _UI);
VOID EnableElements(HWND hDlg, DWORD* _pIDs, UINT _IDsSize, BOOL _En);
VOID ShowElements(HWND hDlg, DWORD* _pIDs, UINT _IDsSize, BOOL _Show);

VOID UpdateLabels(const MakePrimeParams* _pParams);
VOID ProcessMessages(HWND _hWnd);
BOOL GetEdit1TextAsMPZ(HWND hDlg, mpz_t _P);
VOID SetEdit1TextFromMPZ(HWND hDlg, mpz_t _P);

VOID MakePrime(MakePrimeParams *_pParams);
VOID MakeSafePrime(MakePrimeParams *_pParams);


