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

#include "FancyButton.h"

#define CLASS_NAME L"FANCYBUTTON"
#define FACE_COLOR (RGB(0xff, 0xff, 0xff))
#define HL_COLOR (RGB(0xdd, 0xff, 0xdd))
#define SHADOW_COLOR (RGB(0x00, 0x00, 0x00))

FancyButton::FancyButton(HWND parent, wchar_t * text)
{
	app_handle = (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE);

	wnd_handle = CreateWindowEx(WS_EX_TRANSPARENT, CLASS_NAME, text, WS_CHILD,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		parent, NULL, app_handle, this);

	text_font = CreateFont(-25, 0, 0, 0,
		FW_EXTRABOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		FF_MODERN, L"Courier New");

	face_brush = CreateSolidBrush(FACE_COLOR);
	hl_brush = CreateSolidBrush(HL_COLOR);
	shadow_brush = CreateSolidBrush(SHADOW_COLOR);
	text_color = RGB(0x00, 0x00, 0x00);
}

FancyButton::~FancyButton()
{
	DestroyWindow(wnd_handle);
}

int FancyButton::Register()
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 4;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wc);
}

void FancyButton::show(bool state)
{
	if (state) {
		ShowWindow(wnd_handle, SW_SHOW);
		UpdateWindow(wnd_handle);
	}
	else {
		ShowWindow(wnd_handle, SW_HIDE);
	}
}

void FancyButton::setText(const wchar_t * text)
{
	SetWindowText(wnd_handle, text);
	InvalidateRect(wnd_handle, NULL, TRUE);
}

void FancyButton::setFont(HFONT font)
{
	DeleteObject(text_font);
	text_font = font;
}

void FancyButton::setTextColor(COLORREF color)
{
	text_color = color;
	InvalidateRect(wnd_handle, NULL, TRUE);
}

void FancyButton::setHighlightColor(COLORREF color)
{
	DeleteObject(hl_brush);
	hl_brush = CreateSolidBrush(color);
}

void FancyButton::setSize(int w, int h)
{
	SetWindowPos(wnd_handle, HWND_TOP, 0, 0, w, h, SWP_NOMOVE);
}

void FancyButton::setPlacement(int x, int y)
{
	SetWindowPos(wnd_handle, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

HWND FancyButton::getHandle()
{
	return wnd_handle;
}

void FancyButton::click()
{
	//prev_state = btn_state;
	setButtonState(ON);
	SetTimer(wnd_handle, IDT_CLICK_FUNC, 150, FALSE);
}

LRESULT FancyButton::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	FancyButton *pointer = NULL;

	if (msg == WM_CREATE)
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
	else
		pointer = (FancyButton *)GetWindowLong(hwnd, GWL_USERDATA);

	switch (msg)
	{
	case WM_DESTROY:
		return pointer->destroyEvent();
	case WM_SIZE:
		return pointer->sizeEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_MOUSEMOVE:
		return pointer->mouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_LBUTTONDOWN:
		return pointer->lButtonDownEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_LBUTTONUP:
		return pointer->lButtonUpEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_PAINT:
		return pointer->paintEvent();
	case WM_TIMER:
		return pointer->timerEvent(wParam);
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
		return SendMessage(GetParent(pointer->getHandle()), msg, wParam, lParam);
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int FancyButton::destroyEvent()
{
	DeleteObject(text_font);
	DeleteObject(face_brush);
	DeleteObject(hl_brush);
	DeleteObject(shadow_brush);

	return 0;
}

int FancyButton::sizeEvent(short w, short h)
{
	width = w;
	height = h;
	updateRects();
	return 0;
}

int FancyButton::mouseMoveEvent(short x, short y)
{
	RECT wnd_rect = {0, 0, width, height};

	SetCapture(wnd_handle);

	if (!isInside(&hl_rect, x, y))
		setButtonState(OFF);
	else
		if (btn_state == OFF)
			setButtonState(HL);

	if (!isInside(&wnd_rect, x, y))
		ReleaseCapture();

	return 0;
}

int FancyButton::lButtonDownEvent(short x, short y)
{
	if (isInside(&hl_rect, x, y))
		setButtonState(ON);

	return 0;
}

int FancyButton::lButtonUpEvent(short x, short y)
{
	if (btn_state == ON) {
		PostMessage(GetParent(wnd_handle), WM_COMMAND, 0x0, (LPARAM)wnd_handle);
		//setButtonState(OFF);
		if (isInside(&hl_rect, x, y))
			setButtonState(HL);
		else
			setButtonState(OFF);
	}

	return 0;
}

int FancyButton::paintEvent()
{
	const int ctxt = 20;

	HDC hdc;
	PAINTSTRUCT ps;
	wchar_t txt[ctxt];

	hdc = BeginPaint(wnd_handle, &ps);

	SelectObject(hdc, GetStockObject(NULL_PEN));

	SelectObject(hdc, (HBRUSH)GetClassLong(GetParent(wnd_handle), GCL_HBRBACKGROUND));
	Rectangle(hdc, 0, 0, width, height);

	SelectObject(hdc, shadow_brush);
	Ellipse(hdc, btn_shadow_rect.left, btn_shadow_rect.top, btn_shadow_rect.right, btn_shadow_rect.bottom);

	if (btn_state == HL || btn_state == ON)
		SelectObject(hdc, hl_brush);
	else
		SelectObject(hdc, face_brush);
	Ellipse(hdc, btn_face_rect.left, btn_face_rect.top, btn_face_rect.right, btn_face_rect.bottom);

	//if (btn_state == HL || btn_state == ON)
		//SetBkColor(hdc, HL_COLOR);
	//else
		//SetBkColor(hdc, FACE_COLOR);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, text_color);
	SelectObject(hdc, text_font);
	GetWindowText(wnd_handle, txt, ctxt);
	DrawText(hdc, txt, -1, &btn_face_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	EndPaint(wnd_handle, &ps);

	return 0;
}

int FancyButton::timerEvent(WPARAM timer_id)
{
	POINT ptMouse;

	if (timer_id == IDT_CLICK_FUNC) {
		KillTimer(wnd_handle, IDT_CLICK_FUNC);
		//setButtonState(prev_state);
		GetCursorPos(&ptMouse);
		ScreenToClient(wnd_handle, &ptMouse);
		if (isInside(&hl_rect, ptMouse.x, ptMouse.y)) {
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				setButtonState(ON);
			else
				setButtonState(HL);
		}
		else {
			setButtonState(OFF);
		}
		PostMessage(GetParent(wnd_handle), WM_COMMAND, 0, (LPARAM)wnd_handle);
	}

	return 0;
}

void FancyButton::updateRects()
{
	hl_rect.left = 0;
	hl_rect.top = 0;
	hl_rect.right = width - 10;
	hl_rect.bottom = height - 10;

	if (btn_state == OFF) {
		btn_face_rect.left = 0;
		btn_face_rect.top = 0;
		btn_face_rect.right = width - 10;
		btn_face_rect.bottom = height - 10;
	}
	else if (btn_state == HL) {
		btn_face_rect.left = 2;
		btn_face_rect.top = 2;
		btn_face_rect.right = width - 8;
		btn_face_rect.bottom = height - 8;
	}
	else if (btn_state == ON) {
		btn_face_rect.left = 7;
		btn_face_rect.top = 7;
		btn_face_rect.right = width - 3;
		btn_face_rect.bottom = height - 3;
	}

	btn_shadow_rect.left = 10;
	btn_shadow_rect.top = 10;
	btn_shadow_rect.right = width;
	btn_shadow_rect.bottom = height;
}

bool FancyButton::isInside(RECT * rect, long x, long y)
{
	return (x > rect->left && x < rect->right && y > rect->top && y < rect->bottom);
}

void FancyButton::setButtonState(ButtonState state)
{
	if (btn_state == state)
		return;

	btn_state = state;
	updateRects();
	InvalidateRect(wnd_handle, NULL, TRUE);
}
