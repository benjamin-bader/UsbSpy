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

void MainFrame::AddDeviceEvent(_In_z_ const wchar_t* eventType, _In_opt_z_ const wchar_t* deviceName)
{
	SYSTEMTIME localTime{};
	::GetLocalTime(&localTime);

	wchar_t timestamp[64]{};
	::swprintf_s(timestamp,
		L"%04hu-%02hu-%02hu %02hu:%02hu:%02hu",
		localTime.wYear,
		localTime.wMonth,
		localTime.wDay,
		localTime.wHour,
		localTime.wMinute,
		localTime.wSecond);

	const int index = m_eventListView.GetItemCount();
	m_eventListView.InsertItem(index, eventType);
	m_eventListView.SetItemText(index, 1, timestamp);
	m_eventListView.SetItemText(index, 2, deviceName != nullptr ? deviceName : L"<unknown>");
}

LRESULT MainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	m_eventListView.Create(
		m_hWnd,
		rcDefault,
		nullptr,
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | LVS_REPORT,
		WS_EX_CLIENTEDGE);

	m_eventListView.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_eventListView.InsertColumn(0, L"Event", LVCFMT_LEFT, 140);
	m_eventListView.InsertColumn(1, L"Time", LVCFMT_LEFT, 170);
	m_eventListView.InsertColumn(2, L"Device", LVCFMT_LEFT, 700);
	m_hWndClient = m_eventListView;

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
	const auto* deviceInfo = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(lParam);
	const wchar_t* deviceName = (deviceInfo != nullptr) ? deviceInfo->dbcc_name : L"<unknown>";

	switch (wParam)
	{
	case DBT_DEVICEARRIVAL:
		AddDeviceEvent(L"Arrived", deviceName);
		break;

	case DBT_DEVICEREMOVECOMPLETE:
		AddDeviceEvent(L"Removed", deviceName);
		break;
	}
	return 0;
}
