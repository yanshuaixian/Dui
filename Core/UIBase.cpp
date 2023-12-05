#include "stdafx.h"
#include "UIBase.h"

namespace UILib
{
	void UI_LOG(LPCTSTR szKey, LPCTSTR szLog, ...)
	{
#ifdef DEBUG
		TCHAR szText[2000 + 1] = { 0 };

		va_list args;
		va_start(args, szLog);
		StringCchVPrintf(szText, 2000, szLog, args);
		va_end(args);

		CTime time = CTime::GetCurrentTime();
		CString date = time.Format(L"%Y-%m-%d %H:%M:%S");
		CString strLog;
		strLog.Format(_T("%s [%s] %s\r\n"), date, szFilter, szText);
		OutputDebugString(strLog);
#endif // DEBUG
	}

	CWindowWnd::CWindowWnd()
	{
	}

	CWindowWnd::~CWindowWnd()
	{
	}

	HWND CWindowWnd::GetHWND() const
	{
		return m_hWnd;
	}

	CWindowWnd::operator HWND() const
	{
		return m_hWnd;
	}

	UINT CWindowWnd::GetClassStyle() const
	{
		return 0;
	}

	LPCTSTR CWindowWnd::GetSuperClassName() const
	{
		return NULL;
	}

	bool CWindowWnd::RegisterWindowClass()
	{
		WNDCLASS wndclass = { 0 };
		wndclass.style = GetClassStyle();
		wndclass.lpfnWndProc = CWindowWnd::WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		//wndclass.hInstance = CPaintManagerUI:Getinstance();TODO:add later
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = NULL;
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = GetWindowClassName();
		ATOM ret = ::RegisterClass(&wndclass);

		_ASSERTE(ret != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS);//����ִ�гɹ����������Ѿ����ڴ��ڶ�ִ��ʧ��
		return(ret != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
	}

	//���ڳ��໯�����໯��Ŀ���Ǹ������д��ڵĹ��ܡ��´���ʹ���Լ��Ĵ��ڹ��̺������Լ��Ĵ��ڹ��̺��������Լ�����Ȥ����Ϣ����������Ȥ����Ϣ����ԭ���ڹ��̺���
	//���໯����ע�ᴰ��ǰ����ȡ��ע�ᴰ�ڵ�WNDCLASSEX���и��ã�������������ʵ�������ڹ��̸�Ϊ�Լ��ģ������䴰�ڹ��̺�����֮��ע�ᴰ�ڡ����Լ��Ĵ�����ѡ���Ե���ԭ���ڹ��̺���
	//���໯��ʹ��GetWindowLong�õ�ԭ���ڹ��̺�����ʹ��SetWindowLong�����µĴ��ڹ��̺������������Լ�����Ϣ���������������ԭ�еġ���׼�Ĵ�����
	bool CWindowWnd::RegisterSuperClass()
	{
		WNDCLASSEX wndclass = { 0 };
		wndclass.cbSize = sizeof(WNDCLASSEX);

		if (!::GetClassInfoEx(NULL, GetSuperClassName(), &wndclass))//hInstanceΪ�������ģ����,ΪNULL����ϵͳ����Ĵ�����
		{
			if (!::GetClassInfoEx(CPaintManagerUI::GetInstance(), GetSuperClassName(), &wndclass))
			{
				_ASSERTE(!"unable to find super class name");
				return false;
			}
		}

		m_OldWndProc = wndclass.lpfnWndProc;
		//wndclass.lpfnWndProc = CWindowWnd::__ControlProc;TODO:add later
		wndclass.hInstance = CPaintManagerUI::GetInstance();
		wndclass.lpszClassName = GetWindowClassName();

		ATOM ret = ::RegisterClass(&wndclass);

		_ASSERTE(ret != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
		return(ret != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
	}

	HWND CWindowWnd::Create(HWND hwndParent, LPCTSTR szName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu /*=NULL*/)
	{
		return Create(hwndParent, szName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
	}

	HWND CWindowWnd::Create(HWND hwndParent, LPCTSTR szName, DWORD dwStyle, DWORD dwExStyle, int x /*=CW_USEDEFAULT*/, int y /*=CW_USEDEFAULT*/, int cx /*=CW_USEDEFAULT*/, int cy /*=CW_USEDEFAULT*/, HMENU hMenu /*=NULL*/)
	{
		if (GetSuperClassName() && !RegisterSuperClass())//����������Ʒǿգ���ע�ᳬ�࣬���ڳ��໯
			return NULL;
		if (GetSuperClassName() && !RegisterWindowClass())//�����������Ϊ�գ���ע����
			return NULL;

		m_hWnd = ::CreateWindowEx(dwStyle, GetWindowClassName(), szName, dwStyle, x, y, cx, cy, hwndParent, CPaintManagerUI::GetInstance(), this);
		_ASSERTE(m_hWnd != NULL);
		retun m_hWnd;
	}

	HWND CWindowWnd::Subclass(HWND hWnd)
	{
		_ASSERTE(::IsWindow(hWnd));
		_ASSERTE(m_hWnd == NULL));
		m_OldWndProc = SubclassWindow(hWnd, __WndProc);//ʹ���Լ��Ĵ��ڹ��̴���ԭ�еĴ��ڹ���
		if (m_OldWndProc = NULL)
			return NULL;
		m_bSubclassed = true;
		m_hwnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));
		return m_hWnd;
	}

	void CWindowWnd::UnSubclass()
	{
		_ASSERTE(::IsWindow(hWnd));
		if (!::IsWindow(m_hWnd))
			return;
		if (!m_bSubclassed)
			return;
		SubclassWindow(m_hWnd, m_OldWndProc);
		m_OldWndProc = ::DefWindowProc;
		m_bSubclassed = false;
	}

	void CWindowWnd::SetIcon(UINT nRes)
	{
		HICON hIcon = (HICON)::LoadImage(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(resID), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		ASSERT(hIcon);
		::SendMessage(m_hWnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
		hIcon = (HICON)::LoadImage(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(resID), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		ASSERT(hIcon);
		::SendMessage(m_hWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIconSM);
	}

	LRESULT CALLBACK CWindowWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CWindowWnd* pThis = NULL;
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCTW* lpcs = reinterpret_cast<CREATESTRUCTW*>(lParam);
			pThis = static_cast<CWindowWnd*>(lpcs->lpCreateParams);
			pThis->m_hWnd = hWnd;//TODO:???�к���
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>pThis);
		}
		else
		{
			pThis = reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (uMsg == WM_NCDESTROY && pThis != NULL)
			{
				LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
				::SetWindowLongPtr(hWnd, GWLP_USERDATA, 0L);
				if (pThis->m_bSubclassed)
					pThis->UnSubclass();
				pThis->m_hWnd = NULL;
				pThis->OnFinalMessage(hWnd);
				return lRes;
			}
		}
		if (pThis != NULL)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
	}

	void CWindowWnd::OnFinalMessage(HWND hWnd)
	{
	}
}