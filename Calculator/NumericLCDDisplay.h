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

#ifndef NUMERIC_LCD_DISPLAY
#define NUMERIC_LCD_DISPLAY

#include <Windows.h>
#include <windowsx.h>
#include <wchar.h>

class NumericLCDDisplay
{
public:
	NumericLCDDisplay(HWND hParent);
	~NumericLCDDisplay();
	static int Register();

	HWND getHandle();

	void setText(const wchar_t *szText);
	void setError();
	void appendChar(const wchar_t ch);
	void deleteLastChar();
	const wchar_t * getText();
	unsigned getTextLen();
	unsigned getMaxChars();
	void flush();
	void blink(unsigned uMilisec);

	void setSize(int cx, int cy);
	void setPlacement(int x, int y);
	void show(bool bShow);

private:
	static const wchar_t *CLASS_NAME;
	static const short IDT_BLINK = 0x0001;
	const int m_iDashWidth = 4;
	const int m_iSpc = 8;

	static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int paintEvent();
	int destroyEvent();
	int sizeEvent(short cx, short cy);
	int timerEvent(WPARAM wTimerID);

	void drawChar(HDC hdc, RECT *lprc, wchar_t ch);
	void drawUpperDash(HDC hdc, RECT *lprc);
	void drawMiddleDash(HDC hdc, RECT *lprc);
	void drawLowerDash(HDC hdc, RECT *lprc);
	void drawLeftUpperDash(HDC hdc, RECT *lprc);
	void drawLeftLowerDash(HDC hdc, RECT *lprc);
	void drawRightUpperDash(HDC hdc, RECT *lprc);
	void drawRightLowerDash(HDC hdc, RECT *lprc);
	void drawComma(HDC hdc, RECT *lprc);

	HWND m_hWnd;
	HBRUSH m_hbr;
	wchar_t m_szText[150];
	bool m_bComma;
	bool m_bBlinkHide;
	int m_nBlinkCount;
	int m_cxWidth, m_cyHeight;
};

#endif
