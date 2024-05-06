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

#ifndef FANCY_BUTTON_H
#define FANCY_BUTTON_H

#include <Windows.h>
#include <windowsx.h>
#include <wchar.h>

class FancyButton
{
public:
	FancyButton(HWND parent, wchar_t *text = NULL);
	~FancyButton();
	static int Register();

	void show(bool state);
	void setText(const wchar_t *text);
	void setFont(HFONT font); //normalnie uzywa czcionki "Courier New"
	void setTextColor(COLORREF color);
	void setHighlightColor(COLORREF color);

	void setSize(int w, int h);
	void setPlacement(int x, int y);

	HWND getHandle();
	void click(); //symuluje klikniecie (prosta animacja)

	static const COLORREF COLOR_RED = RGB(0xff, 0xc0, 0xc0);
	static const COLORREF COLOR_GREEN = RGB(0xdd, 0xff, 0xdd);
	static const COLORREF COLOR_BLUE = RGB(0xd0, 0xd0, 0xf0);
	static const COLORREF COLOR_PINK = RGB(0xff, 0xdd, 0xff);

private:
	enum ButtonState { OFF, HL, ON };

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int destroyEvent();
	int sizeEvent(short w, short h);
	int mouseMoveEvent(short x, short y);
	int lButtonDownEvent(short x, short y);
	int lButtonUpEvent(short x, short y);
	int paintEvent();
	int timerEvent(WPARAM timer_id);

	void updateRects();
	void setButtonState(ButtonState state);

	static inline bool isInside(RECT *rect, long x, long y);

	HWND wnd_handle;
	HINSTANCE app_handle;
	HFONT text_font;
	HBRUSH face_brush, hl_brush, shadow_brush;
	COLORREF text_color;

	short width, height;
	RECT btn_face_rect, btn_shadow_rect;
	RECT hl_rect; //to obszar do klikniecia, nie cos do narysowania
	ButtonState btn_state;
	//ButtonState prev_state; //na potrzeby funkcji click()

	static const short IDT_CLICK_FUNC = 0x0001;
};

#endif
