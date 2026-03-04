#include "MainFrame.h"

#include <Dbt.h>

// USB serial host PnP drivers
GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72,
					  0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };

// Other USB devices
// TODO: wtf guid(s) do we actually care about
const GUID usbDevices{
	0x36fc9e60, 0xc465, 0x11cf, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00
};

MainFrame::MainFrame()
	: m_hdevnotify{nullptr}
{}

LRESULT MainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	DEV_BROADCAST_DEVICEINTERFACE filter{};
	filter.dbcc_size = sizeof(decltype(filter));
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = WceusbshGUID;

	m_hdevnotify = ::RegisterDeviceNotification(m_hWnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if (m_hdevnotify == NULL)
	{
		MessageBox(L"Failed to register for device notifications!");
		ExitProcess(1);
	}

	return 0;
}

LRESULT MainFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	if (m_hdevnotify != nullptr)
	{
		::UnregisterDeviceNotification(m_hdevnotify);
		m_hdevnotify = nullptr;
	}

	handled = false; // propagate the destroy message

	return 0;
}

LRESULT MainFrame::OnDeviceChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	PDEV_BROADCAST_DEVICEINTERFACE b = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(lParam);

	switch (wParam)
	{
	case DBT_DEVICEARRIVAL:
		OutputDebugString(L"Device arrived!");
		break;

	case DBT_DEVICEREMOVECOMPLETE:
		OutputDebugString(L"Device removed.");
		break;
	}
	return 0;
}
