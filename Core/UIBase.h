#pragma once

namespace UILib
{
	//TODO:???
	#define UI_WNDSTYLE_CONTAINER        (0)
	#define UI_WNDSTYLE_FRAME            (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
	#define UI_WNDSTYLE_CHILD            (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
	#define UI_WNDSTYLE_DIALOG           (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
	#define UI_WNDSTYLE_EX_FRAME         (WS_EX_WINDOWEDGE)
	#define UI_WNDSTYLE_EX_DIALOG        (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)
	#define UI_CLASSSTYLE_CONTAINER      (0)
	#define UI_CLASSSTYLE_FRAME          (CS_VREDRAW | CS_HREDRAW)
	#define UI_CLASSSTYLE_CHILD          (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
	#define UI_CLASSSTYLE_DIALOG         (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

	void UILIB_API UI_LOG(LPCTSTR szKey, LPCTSTR szLog, ...);

	class UILIB_API CWindowWnd
	{
	public:
		CWindowWnd();
		~CWindowWnd();

		HWND GetHWND() const;
		operator HWND() const;

		bool RegisterWindowClass();
		bool RegisterSuperClass();

		HWND Create(HWND hwndParent, LPCTSTR szName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
		HWND Create(HWND hwndParent, LPCTSTR szName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);

		HWND Subclass(HWND hWnd);
		void UnSubclass();

		void SetIcon(UINT nRes);



	protected:
		virtual UINT GetClassStyle() const;
		virtual LPCTSTR GetWindowClassName() const = 0;
		virtual LPCTSTR GetSuperClassName() const;

		virtual void OnFinalMessage(HWND hWnd);

		static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		HWND m_hWnd;
		WNDPROC m_OldWndProc; //窗口超类的窗口过程
		bool m_bSubclassed; //窗口是否被子类化
	};
} // namespace UILib