/* -------------------------------------------------------------------------------------------------------------------------------------
TS Calculator
Copyright (c) 2016 Tobiasz Stamborski

This program is free software; you can freely redistribute/modify it under terms of MIT License.
Full text avaible at http://opensource.org/licenses/MIT.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------------------------*/

#include "AboutDialog.h"

const wchar_t *AboutDialog::CLASS_NAME = L"ABOUTDIALOG";

AboutDialog::AboutDialog(HWND hParent)
{
	TCITEM tc;
	HINSTANCE hInst = GetModuleHandle(NULL);

	m_hWnd = CreateWindowEx(0, CLASS_NAME, L"About... ",
		WS_DLGFRAME | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hParent, NULL, hInst, this);

	m_hParent = hParent;

	m_hBtnOk = CreateWindowEx(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE |  BS_DEFPUSHBUTTON,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	SetWindowFont(m_hBtnOk, (HFONT)GetStockFont(DEFAULT_GUI_FONT), TRUE);
	
	m_hTab = CreateWindowEx(0, WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE | TCS_FOCUSNEVER,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	tc.mask = TCIF_IMAGE | TCIF_TEXT;
	tc.iImage = -1;
	tc.pszText = L"About... ";
	TabCtrl_InsertItem(m_hTab, 0, &tc);
	tc.pszText = L"License";
	TabCtrl_InsertItem(m_hTab, 1, &tc);
	SetWindowFont(m_hTab, (HFONT)GetStockFont(DEFAULT_GUI_FONT), TRUE);
	TabCtrl_SetCurSel(m_hTab, 0);

	m_hLicenceEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
		WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	SetWindowFont(m_hLicenceEdit, (HFONT)GetStockFont(DEFAULT_GUI_FONT), TRUE);

	m_hIconLabel = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_ICON,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	m_hNameLabel = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	m_hVersionLabel = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	m_hCopyrightLabel = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	m_hDescriptionLabel = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInst, NULL);
	SetWindowFont(m_hNameLabel, (HFONT)GetStockFont(DEFAULT_GUI_FONT), TRUE);
	SetWindowFont(m_hVersionLabel, (HFONT)GetStockFont(DEFAULT_GUI_FONT), TRUE);
	SetWindowFont(m_hCopyrightLabel, (HFONT)GetStockFont(DEFAULT_GUI_FONT), TRUE);
	SetWindowFont(m_hDescriptionLabel, (HFONT)GetStockFont(DEFAULT_GUI_FONT), TRUE);
}

AboutDialog::~AboutDialog()
{
	DestroyWindow(m_hWnd);
}

bool AboutDialog::Register()
{
	WNDCLASSEX wc = { 0 };
	INITCOMMONCONTROLSEX icc;

	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_TAB_CLASSES;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 4;
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, 0);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wc) && InitCommonControlsEx(&icc);
}

void AboutDialog::show()
{
	EnableWindow(m_hParent, FALSE);
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
}

void AboutDialog::setSize(int cx, int cy)
{
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, cx, cy, SWP_NOMOVE);
}

void AboutDialog::setDefaultSize()
{
	setSize(400, 320);
}

void AboutDialog::setTabPage(int iPage)
{
	TabCtrl_SetCurSel(m_hTab, iPage);
	showTabPage(iPage);
}

void AboutDialog::setIcon(HICON hicon)
{
	Static_SetIcon(m_hIconLabel, hicon);
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
	SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
}

void AboutDialog::setAppName(const wchar_t * szName)
{
	Static_SetText(m_hNameLabel, szName);
}

void AboutDialog::setVersionInfo(const wchar_t * szInfo)
{
	Static_SetText(m_hVersionLabel, szInfo);
}

void AboutDialog::setCopyrightNote(const wchar_t * szNote)
{
	Static_SetText(m_hCopyrightLabel, szNote);
}

void AboutDialog::setDescription(const wchar_t * szDescription)
{
	Static_SetText(m_hDescriptionLabel, szDescription);
}

void AboutDialog::setLicenceText(const wchar_t * szText)
{
	Edit_SetText(m_hLicenceEdit, szText);
}

void AboutDialog::showTabPage(int iPage)
{
	if (iPage == 0) {
		ShowWindow(m_hLicenceEdit, SW_HIDE);

		ShowWindow(m_hIconLabel, SW_SHOW);
		ShowWindow(m_hNameLabel, SW_SHOW);
		ShowWindow(m_hVersionLabel, SW_SHOW);
		ShowWindow(m_hCopyrightLabel, SW_SHOW);
		ShowWindow(m_hDescriptionLabel, SW_SHOW);
	}
	else if (iPage == 1) {
		ShowWindow(m_hIconLabel, SW_HIDE);
		ShowWindow(m_hIconLabel, SW_HIDE);
		ShowWindow(m_hNameLabel, SW_HIDE);
		ShowWindow(m_hVersionLabel, SW_HIDE);
		ShowWindow(m_hCopyrightLabel, SW_HIDE);
		ShowWindow(m_hDescriptionLabel, SW_HIDE);

		ShowWindow(m_hLicenceEdit, SW_SHOW);
	}
}

LRESULT AboutDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	AboutDialog *p = 0;

	if (msg == WM_CREATE)
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)((CREATESTRUCT *)lParam)->lpCreateParams);
	else
		p = (AboutDialog *)GetWindowLong(hwnd, GWL_USERDATA);

	switch (msg)
	{
	case WM_DESTROY:
		return p->destroyEvent();
	case WM_CLOSE:
		return p->closeEvent();
	case WM_COMMAND:
		return p->commandEvent((HWND)lParam);
	case WM_SIZE:
		return p->sizeEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_NOTIFY:
		return p->notifyEvent(lParam);
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int AboutDialog::destroyEvent()
{
	DestroyWindow(m_hLicenceEdit);
	DestroyWindow(m_hIconLabel);
	DestroyWindow(m_hNameLabel);
	DestroyWindow(m_hVersionLabel);
	DestroyWindow(m_hCopyrightLabel);
	DestroyWindow(m_hDescriptionLabel);
	DestroyWindow(m_hTab);
	DestroyWindow(m_hBtnOk);

	return 0;
}

int AboutDialog::closeEvent()
{
	EnableWindow(m_hParent, TRUE);
	ShowWindow(m_hWnd, SW_HIDE);

	return 0;
}

int AboutDialog::commandEvent(HWND hSender)
{
	if (hSender == m_hBtnOk)
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);

	return 0;
}

int AboutDialog::notifyEvent(LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case TCN_SELCHANGING:
		return FALSE;
	case TCN_SELCHANGE:
		int iPage = TabCtrl_GetCurSel(m_hTab);
		showTabPage(iPage);
		break;
	}

	return TRUE;
}

int AboutDialog::sizeEvent(short cx, short cy)
{
	const int iSpc = 10;

	SetWindowPos(m_hBtnOk, HWND_TOP, (cx / 5) * 3, (cy / 6) * 5 + iSpc, cx / 5, (cy / 6) - iSpc * 2, 0);

	SetWindowPos(m_hTab, HWND_TOP, iSpc, iSpc, cx - iSpc * 2, (cy / 6) * 5 - iSpc, 0);

	SetWindowPos(m_hLicenceEdit, m_hTab, iSpc * 2, iSpc * 4,
		cx - iSpc * 4, (cy / 6) * 5 - iSpc * 6, 0);

	SetWindowPos(m_hIconLabel, m_hTab, iSpc * 3, iSpc * 5, 48, 48, 0);

	SetWindowPos(m_hNameLabel, m_hTab, iSpc * 5 + 48, iSpc * 5,
		cx - iSpc * 2 - iSpc * 5 - 48, iSpc * 2, 0);
	SetWindowPos(m_hVersionLabel, m_hTab, iSpc * 5 + 48, iSpc * 8,
		cx - iSpc * 2 - iSpc * 5 - 48, iSpc * 2, 0);
	SetWindowPos(m_hCopyrightLabel, m_hTab, iSpc * 5 + 48, iSpc * 11,
		cx - iSpc * 2 - iSpc * 5 - 48, iSpc * 2, 0);
	SetWindowPos(m_hDescriptionLabel, m_hTab, iSpc * 5 + 48, iSpc * 14,
		cx - iSpc * 2 - iSpc * 5 - 48, iSpc * 7, 0);

	return 0;
}
