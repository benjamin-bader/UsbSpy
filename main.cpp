// main.cpp : Defines the entry point for the application.
//

#include "MainFrame.h"

int Run(CAppModule& theModule, LPWSTR lpCmdLine, int nCmdShow)
{
	CMessageLoop theLoop;
	theModule.AddMessageLoop(&theLoop);

	MainFrame w;
	if (w.Create(NULL) == NULL)
	{
		ATLTRACE(L"Failed to initialize the main window");
		return 1;
	}

	w.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	theModule.RemoveMessageLoop();
	return nRet;
}


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT res = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(res));

	::DefWindowProc(0, 0, 0, 0L); // cargo-cult, fixing some old ATL "thunking problem" under unicode, allegedly.

	ATLASSERT(AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES));

	CAppModule theModule;
	res = theModule.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(res));

	int nRet = Run(theModule, lpCmdLine, nCmdShow);

	theModule.Term();

	::CoUninitialize();

	return 0;
}
