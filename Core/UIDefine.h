#pragma once

namespace UILib
{
	typedef struct tagTNotifyUI
	{
		std::wstring strType;
		std::wstring strVirtualWnd;
		//CControlUI* pSender;
		DWORD dwTimestamp;
		POINT ptMouse;
		WPARAM wParam;
		LPARAM lParam;
	}TNotifyUI;
}  