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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#define _USE_MATH_DEFINES

#include <Windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdint.h>
#include <wchar.h>

#include "resource.h"
#include "AboutDialog.h"
#include "FancyButton.h"
#include "NumericLCDDisplay.h"

class Calculator
{
public:
	Calculator();
	~Calculator();
	static int Register();

	void show(bool bShow);

private:
	static const wchar_t *CLASS_NAME;
	static const wchar_t *m_szLicense;
	static const short IDM_ABOUT = 0x0001;
	const short STD_WIDTH = 500, STD_HEIGHT = 400;
	const short EXT_WIDTH = 730, EXT_HEIGHT = 400;
	enum ViewMode { STANDARD, EXTENDED };

	void setSize(short cx, short cy);
	void setViewMode(ViewMode fMode);
	void setShiftMode(bool bShiftMode);
	void setError(bool bError); //w zasadzie tylko setError(true), a odwrotnie uzyc btnCPressed()
	double deg2rad(double flAngle);
	void readValue(double *lpflValue);
	void dispValue(double flValue);
	void binaryEqFunc();

	void btnCEPressed();
	void btnCPressed();
	void btnDelPressed();
	void unaryOperandPressed(HWND hSender);
	void btnCommaPressed();
	void btnEqPressed();
	void binaryOperandPressed(HWND hSender);
	void digitPressed(HWND hSender);
	bool copyToClipboard();
	void btnMRPressed();
	void btnMSPressed();
	void btnMCPressed();
	void btnMAddPressed();
	void btnMSubPressed();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int destroyEvent();
	int sysCommandEvent(WPARAM wParam, LPARAM lParam);
	int sizeEvent(short cx, short cy);
	int mouseMoveEvent(short x, short y);
	int commandEvent(HWND hSender);
	int keyDownEvent(WPARAM wKeyCode);
	int keyUpEvent(WPARAM wKeyCode);

	HWND m_hWnd;
	AboutDialog *m_dialogAbout;
	NumericLCDDisplay *m_disp;
	FancyButton *m_btn[10];
	FancyButton *m_btnDiv, *m_btnMul, *m_btnSub, *m_btnAdd;
	FancyButton *m_btnCE, *m_btnC, *m_btnDel, *m_btnComma, *m_btnSign, *m_btnEq;
	FancyButton *m_btnMR, *m_btnMS, *m_btnMC, *m_btnMAdd, *m_btnMSub;
	FancyButton *m_btnSin, *m_btnCos, *m_btnTg, *m_btnLog, *m_btnLn;
	FancyButton *m_btnSq, *m_btnPow, *m_btnSqrt, *m_btnFact, *m_btnPi;

	HCURSOR m_hCursor;

	enum { LVALUE, RVALUE }m_fValueType; //ktora wartosc jest na ekranie
	enum { ADD, SUB, MUL, DIV, POW, ROOT }m_fOperand;
	ViewMode m_fViewMode;
	bool m_bErrorFlag, m_bCommaFlag, m_bWaitForOpFlag, m_bBtnEqFlag, m_bUnaryPerformed;
	bool m_bShiftMode;
	double m_flLValue, m_flRValue, m_flMemValue;
};

#endif
