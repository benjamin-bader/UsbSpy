#pragma once

#include <atlctrlx.h>
#include <atlframe.h>

#include <WinUser.h>

class MainFrame : public CFrameWindowImpl<MainFrame>
{
	HDEVNOTIFY m_hdevnotify;
	CListViewCtrl m_eventListView;

	void AddDeviceEvent(_In_z_ const wchar_t* eventType, _In_opt_z_ const wchar_t* deviceName);

public:
	DECLARE_FRAME_WND_CLASS(L"UsbSpy_MainFrame", 1);

	MainFrame();

	BEGIN_MSG_MAP(MainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_DEVICECHANGE, OnDeviceChange)
		CHAIN_MSG_MAP(CFrameWindowImpl<MainFrame>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled);
	LRESULT OnDeviceChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled);
};
