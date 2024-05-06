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

#include "NumericLCDDisplay.h"

const wchar_t *NumericLCDDisplay::CLASS_NAME = L"NUMERICLCDDISPLAY";

NumericLCDDisplay::NumericLCDDisplay(HWND hParent)
{
	m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, CLASS_NAME, NULL,
		WS_CHILD,
		//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hParent, NULL, (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE), this);
	if (m_hWnd == NULL)
		MessageBox(0, L"Cannot create window!", L"Error!", MB_OK | MB_ICONERROR);

	m_szText[0] = L'\0';

	m_hbr = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
}

NumericLCDDisplay::~NumericLCDDisplay()
{
	DestroyWindow(m_hWnd);
}

int NumericLCDDisplay::Register()
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 4;
	wc.hbrBackground = CreateSolidBrush(RGB(153, 159, 128));
	wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, 0);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE);
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = NULL;

	return RegisterClassEx(&wc);
}

HWND NumericLCDDisplay::getHandle()
{
	return m_hWnd;
}

void NumericLCDDisplay::setText(const wchar_t * szText)
{
	if (wcslen(szText) > (150 - 1))
		return;

	wcscpy_s(m_szText, szText);

	//InvalidateRect(m_hWnd, NULL, TRUE);
}

void NumericLCDDisplay::setError()
{
	wcscpy_s(m_szText, L"Error");

	//InvalidateRect(m_hWnd, NULL, TRUE);
}

void NumericLCDDisplay::appendChar(const wchar_t ch)
{
	wchar_t szBuffer[150];

	if (wcslen(m_szText) > 148)
		return;

	swprintf(szBuffer, 150, L"%s%c", m_szText, ch);
	wcscpy_s(m_szText, szBuffer);

	//InvalidateRect(m_hWnd, NULL, TRUE);
}

void NumericLCDDisplay::deleteLastChar()
{
	wchar_t szBuffer[150];

	wcsncpy_s(szBuffer, m_szText, wcslen(m_szText) - 1);
	wcscpy_s(m_szText, szBuffer);

	//InvalidateRect(m_hWnd, NULL, TRUE);
}

const wchar_t * NumericLCDDisplay::getText()
{
	return m_szText;
}

unsigned NumericLCDDisplay::getTextLen()
{
	return wcslen(m_szText);
}

unsigned NumericLCDDisplay::getMaxChars()
{
	return (unsigned)(m_cxWidth / (m_cyHeight / 2));
}

void NumericLCDDisplay::flush()
{
	InvalidateRect(m_hWnd, NULL, TRUE);
}

void NumericLCDDisplay::blink(unsigned uMilisec)
{
	m_nBlinkCount = uMilisec / 100;
	SetTimer(m_hWnd, IDT_BLINK, 100, NULL);
}

void NumericLCDDisplay::setSize(int cx, int cy)
{
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, cx, cy, SWP_NOMOVE);
}

void NumericLCDDisplay::setPlacement(int x, int y)
{
	SetWindowPos(m_hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

void NumericLCDDisplay::show(bool bShow)
{
	if (bShow) {
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);
	}
	else
		ShowWindow(m_hWnd, SW_HIDE);
}

LRESULT NumericLCDDisplay::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	NumericLCDDisplay *p = 0;

	if (msg == WM_CREATE)
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)((CREATESTRUCT *)lParam)->lpCreateParams);
	else
		p = (NumericLCDDisplay *)GetWindowLong(hwnd, GWL_USERDATA);

	switch (msg)
	{
	case WM_PAINT:
		return p->paintEvent();
	case WM_SIZE:
		return p->sizeEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_DESTROY:
		return p->destroyEvent();
	case WM_TIMER:
		return p->timerEvent(wParam);
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int NumericLCDDisplay::paintEvent()
{
	HDC hdc;
	PAINTSTRUCT ps;
	int cxCharW = m_cyHeight / 2;
	int cChars = ((getMaxChars() < wcslen(m_szText)) ? getMaxChars() : wcslen(m_szText));
	RECT rcChar;
	wchar_t ch;

	hdc = BeginPaint(m_hWnd, &ps);

	if (m_bBlinkHide) {
		EndPaint(m_hWnd, &ps);
		return 0;
	}

	rcChar.left = m_cxWidth - cxCharW;
	rcChar.top = 0;
	rcChar.right = m_cxWidth;
	rcChar.bottom = m_cyHeight;

	m_bComma = FALSE;

	SelectObject(hdc, GetStockObject(NULL_PEN));
	SelectObject(hdc, m_hbr);

	for (int i = (cChars - 1); i >= 0; i--) {
		ch = m_szText[i];
		if (ch == L'.' || ch == L',')
			m_bComma = TRUE;
		else {
			drawChar(hdc, &rcChar, ch);
			rcChar.left -= cxCharW;
			rcChar.right -= cxCharW;
		}
	}

	EndPaint(m_hWnd, &ps);

	return 0;
}

int NumericLCDDisplay::destroyEvent()
{
	DeleteObject(m_hbr);
	return 0;
}

int NumericLCDDisplay::sizeEvent(short cx, short cy)
{
	m_cxWidth = cx;
	m_cyHeight = cy;

	InvalidateRect(m_hWnd, NULL, TRUE);

	return 0;
}

int NumericLCDDisplay::timerEvent(WPARAM wTimerID)
{
	if (wTimerID == IDT_BLINK) {
		if (m_nBlinkCount % 2)
			m_bBlinkHide = true;
		else
			m_bBlinkHide = false;

		if (m_nBlinkCount == 0)
			KillTimer(m_hWnd, IDT_BLINK);
		m_nBlinkCount--;

		InvalidateRect(m_hWnd, NULL, TRUE);
	}

	return 0;
}

void NumericLCDDisplay::drawChar(HDC hdc, RECT * lprc, wchar_t ch)
{
	if (ch == L'E' || ch == L'e') {
		drawUpperDash(hdc, lprc);
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawLeftUpperDash(hdc, lprc);
		drawLeftLowerDash(hdc, lprc);
	}
	else if (ch == L'o' || ch == L'O') {
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawLeftLowerDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'r' || ch == L'R') {
		drawMiddleDash(hdc, lprc);
		drawLeftLowerDash(hdc, lprc);
	}
	else if (ch == L'-')
		drawMiddleDash(hdc, lprc);
	else if (ch == L'+')
		drawLowerDash(hdc, lprc);
	else if (ch == L'0') {
		drawUpperDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawLeftUpperDash(hdc, lprc);
		drawLeftLowerDash(hdc, lprc);
		drawRightUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'1') {
		drawRightUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'2') {
		drawUpperDash(hdc, lprc);
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawRightUpperDash(hdc, lprc);
		drawLeftLowerDash(hdc, lprc);
	}
	else if (ch == L'3') {
		drawUpperDash(hdc, lprc);
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawRightUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'4') {
		drawMiddleDash(hdc, lprc);
		drawLeftUpperDash(hdc, lprc);
		drawRightUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'5') {
		drawUpperDash(hdc, lprc);
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawLeftUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'6') {
		drawUpperDash(hdc, lprc);
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawLeftUpperDash(hdc, lprc);
		drawLeftLowerDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'7') {
		drawUpperDash(hdc, lprc);
		drawRightUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'8') {
		drawUpperDash(hdc, lprc);
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawLeftUpperDash(hdc, lprc);
		drawLeftLowerDash(hdc, lprc);
		drawRightUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}
	else if (ch == L'9') {
		drawUpperDash(hdc, lprc);
		drawMiddleDash(hdc, lprc);
		drawLowerDash(hdc, lprc);
		drawLeftUpperDash(hdc, lprc);
		drawRightUpperDash(hdc, lprc);
		drawRightLowerDash(hdc, lprc);
	}

	if (m_bComma) {
		drawComma(hdc, lprc);
		m_bComma = FALSE;
	}
}

void NumericLCDDisplay::drawUpperDash(HDC hdc, RECT * lprc)
{
	Rectangle(hdc, (lprc->left + m_iSpc + m_iDashWidth * 2), (lprc->top + m_iSpc),
		(lprc->right - m_iSpc - m_iDashWidth * 2), (lprc->top + m_iSpc + m_iDashWidth));
}

void NumericLCDDisplay::drawMiddleDash(HDC hdc, RECT * lprc)
{
	Rectangle(hdc, (lprc->left + m_iSpc + m_iDashWidth * 2), (lprc->top + (lprc->bottom - lprc->top) / 2 - m_iDashWidth / 2),
		(lprc->right - m_iSpc - m_iDashWidth * 2), (lprc->bottom - (lprc->bottom - lprc->top) / 2 + m_iDashWidth / 2));
}

void NumericLCDDisplay::drawLowerDash(HDC hdc, RECT * lprc)
{
	Rectangle(hdc, (lprc->left + m_iSpc + m_iDashWidth * 2), (lprc->bottom - m_iSpc),
		(lprc->right - m_iSpc - m_iDashWidth * 2), (lprc->bottom - m_iSpc - m_iDashWidth));
}

void NumericLCDDisplay::drawLeftUpperDash(HDC hdc, RECT * lprc)
{
	Rectangle(hdc, (lprc->left + m_iSpc - m_iDashWidth), (lprc->top + m_iSpc + m_iDashWidth),
		(lprc->left + m_iSpc), (lprc->bottom - (lprc->bottom - lprc->top) / 2 - m_iDashWidth / 2));
}

void NumericLCDDisplay::drawLeftLowerDash(HDC hdc, RECT * lprc)
{
	Rectangle(hdc, (lprc->left + m_iSpc - m_iDashWidth), (lprc->bottom - (lprc->bottom - lprc->top) / 2 + m_iDashWidth / 2),
		(lprc->left + m_iSpc), (lprc->bottom - m_iSpc - m_iDashWidth));
}

void NumericLCDDisplay::drawRightUpperDash(HDC hdc, RECT * lprc)
{
	Rectangle(hdc, (lprc->right - m_iSpc), (lprc->top + m_iSpc + m_iDashWidth),
		(lprc->right - m_iSpc + m_iDashWidth), (lprc->bottom - (lprc->bottom - lprc->top) / 2 - m_iDashWidth / 2));
}

void NumericLCDDisplay::drawRightLowerDash(HDC hdc, RECT * lprc)
{
	Rectangle(hdc, (lprc->right - m_iSpc), (lprc->bottom - (lprc->bottom - lprc->top) / 2 + m_iDashWidth / 2),
		(lprc->right - m_iSpc + m_iDashWidth), (lprc->bottom - m_iSpc - m_iDashWidth));
}

void NumericLCDDisplay::drawComma(HDC hdc, RECT * lprc)
{
	Ellipse(hdc, lprc->right - m_iDashWidth, lprc->bottom - m_iDashWidth, lprc->right, lprc->bottom);
}
