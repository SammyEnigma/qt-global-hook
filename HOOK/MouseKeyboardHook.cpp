#include "MouseKeyboardHook.h"  


//�����ڴ����
#pragma data_seg("MouseKeyboardHook")  
HHOOK g_hMouseHook = NULL;
HHOOK g_hKeyboardHook = NULL;
HWND g_lhDisplayWnd = NULL;
#pragma data_seg()  

int UnSetHook(void);
int SetHook(HWND main);

//���Ӻ����ĸ�ʽ LRESULT CALLBACK ������(int ��������, WPARAM wParam, LPARAM lParam);
//�������Ĺ��Ӻ���  
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
//������̵Ĺ��Ӻ���
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

//����ǻ�ȡDLL���ڴ��ַ�������ظ�ʹ�ã�����ģ��  
HMODULE WINAPI ModuleFromAddress(PVOID pv);


LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(g_lhDisplayWnd, WM_USER + 100, 0, 0);
	return ::CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(g_lhDisplayWnd, WM_USER + 100, 0, 0);
	return ::CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}

//��װ���Ӻ���  
MOUSEKEYBOARDHOOK_API int SetHook(HWND main)
{
	g_lhDisplayWnd = main;
	g_hMouseHook = ::SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc,
		ModuleFromAddress(MouseHookProc), 0);
	g_hKeyboardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc,
		ModuleFromAddress(KeyboardHookProc), 0);

	return g_hMouseHook && g_hKeyboardHook ? 1 : 0;
}

MOUSEKEYBOARDHOOK_API int UnSetHook(void)
{
	g_lhDisplayWnd = NULL;
	BOOL b1 = ::UnhookWindowsHookEx(g_hMouseHook);
	BOOL b2 = ::UnhookWindowsHookEx(g_hKeyboardHook);

	return b1 && b2 ? 1 : 0;		
}

HMODULE WINAPI ModuleFromAddress(PVOID pv)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (::VirtualQuery(pv, &mbi, sizeof(mbi)) != 0) {
		return (HMODULE)mbi.AllocationBase;
	} else {
		return NULL;
	}
}