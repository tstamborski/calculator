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

#include "Calculator.h"

const wchar_t * Calculator::CLASS_NAME = L"CALCULATOR";
const wchar_t * Calculator::m_szLicense = L"The MIT License (MIT)\r\nCopyright (c) 2016 Tobiasz Stamborski\r\n\r\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the\r\nSoftware without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,\r\nand to permit persons to whom the Software is furnished to do so, subject to the following conditions:\r\n\r\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\r\n\r\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\r\nMERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR\r\nANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH\r\nTHE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";

Calculator::Calculator()
{
	HMENU hSysMenu;

	m_hWnd = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_APPWINDOW, CLASS_NAME, L"TS Calculator",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		HWND_DESKTOP, NULL, GetModuleHandle(NULL), this);
	m_hCursor = LoadCursor(NULL, IDC_ARROW);
	//if (m_hCursor == NULL)
		//PostQuitMessage(GetLastError());

	hSysMenu = GetSystemMenu(m_hWnd, FALSE);
	InsertMenu(hSysMenu, GetMenuItemCount(hSysMenu) - 1, MF_BYPOSITION | MF_STRING, IDM_ABOUT, L"About... ");
	InsertMenu(hSysMenu, GetMenuItemCount(hSysMenu) - 1, MF_BYPOSITION | MF_SEPARATOR, 0, L"");

	m_dialogAbout = new AboutDialog(m_hWnd);
	m_dialogAbout->setDefaultSize();
	m_dialogAbout->setIcon((HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CALC), IMAGE_ICON, 0, 0, 0));
	m_dialogAbout->setAppName(L"TS Calculator");
	m_dialogAbout->setVersionInfo(L"Version\t1.0.6");
	m_dialogAbout->setCopyrightNote(L"Copyright (c) 2016 Tobiasz Stamborski");
	m_dialogAbout->setDescription(L"This program is my somewhat decorative,\r\nversion of calculator application.");
	m_dialogAbout->setLicenceText(m_szLicense);

	m_disp = new NumericLCDDisplay(m_hWnd);
	//m_disp->setText(L"0");

	m_btnMR = new FancyButton(m_hWnd, L"MR");
	m_btnMR->setHighlightColor(FancyButton::COLOR_PINK);
	m_btnMS = new FancyButton(m_hWnd, L"MS");
	m_btnMS->setHighlightColor(FancyButton::COLOR_PINK);
	m_btnMC = new FancyButton(m_hWnd, L"MC");
	m_btnMC->setHighlightColor(FancyButton::COLOR_PINK);
	m_btnMAdd = new FancyButton(m_hWnd, L"M+");
	m_btnMAdd->setHighlightColor(FancyButton::COLOR_PINK);
	m_btnMSub = new FancyButton(m_hWnd, L"M-");
	m_btnMSub->setHighlightColor(FancyButton::COLOR_PINK);

	m_btnCE = new FancyButton(m_hWnd, L"CE");
	m_btnCE->setHighlightColor(FancyButton::COLOR_RED);
	m_btnC = new FancyButton(m_hWnd, L"C");
	m_btnC->setHighlightColor(FancyButton::COLOR_RED);
	m_btnDel = new FancyButton(m_hWnd, L"Del");
	m_btnDel->setHighlightColor(FancyButton::COLOR_RED);
	m_btnComma = new FancyButton(m_hWnd, L".");
	m_btnSign = new FancyButton(m_hWnd, L"±");
	m_btnSign->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnEq = new FancyButton(m_hWnd, L"=");

	m_btnDiv = new FancyButton(m_hWnd, L"÷");
	m_btnMul = new FancyButton(m_hWnd, L"×");
	m_btnSub = new FancyButton(m_hWnd, L"-");
	m_btnAdd = new FancyButton(m_hWnd, L"+");

	m_btnSin = new FancyButton(m_hWnd, L"sin");
	m_btnSin->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnCos = new FancyButton(m_hWnd, L"cos");
	m_btnCos->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnTg = new FancyButton(m_hWnd, L"tg");
	m_btnTg->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnLog = new FancyButton(m_hWnd, L"log");
	m_btnLog->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnLn = new FancyButton(m_hWnd, L"ln");
	m_btnLn->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnSq = new FancyButton(m_hWnd, L"x²");
	m_btnSq->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnPow = new FancyButton(m_hWnd, L"xʸ");
	m_btnPow->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnSqrt = new FancyButton(m_hWnd, L"√x");
	m_btnSqrt->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnFact = new FancyButton(m_hWnd, L"n!");
	m_btnFact->setHighlightColor(FancyButton::COLOR_BLUE);
	m_btnPi = new FancyButton(m_hWnd, L"ᴨ");
	m_btnPi->setHighlightColor(FancyButton::COLOR_BLUE);

	for (int i = 0; i < 10; i++) {
		wchar_t szBtnText[2];

		swprintf(szBtnText, 2, L"%d", i);
		m_btn[i] = new FancyButton(m_hWnd, szBtnText);
	}

	
	setViewMode(STANDARD);
	btnCPressed();
	btnMCPressed();
}

Calculator::~Calculator()
{
	DestroyWindow(m_hWnd);
}

int Calculator::Register()
{
	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 4;
	//wc.hbrBackground = CreateSolidBrush(RGB(153, 153, 193)); //jasno fioletowy
	//wc.hbrBackground = CreateSolidBrush(RGB(117, 145, 255)); //jasno niebieski
	//wc.hbrBackground = CreateSolidBrush(RGB(108, 49, 49)); //brazowy
	//wc.hbrBackground = CreateSolidBrush(RGB(61, 94, 103)); //ciemno szaro-niebieski
	wc.hbrBackground = CreatePatternBrush((HBITMAP)LoadImage(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDB_BACKGROUND), IMAGE_BITMAP, 0, 0, 0));
	wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, 0);
	wc.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CALC), IMAGE_ICON, 0, 0, 0);
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CALC), IMAGE_ICON, 0, 0, 0);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wc);
}

void Calculator::setSize(short cx, short cy)
{
	RECT rcWindow;

	rcWindow.left = 0;
	rcWindow.top = 0;
	rcWindow.right = cx;
	rcWindow.bottom = cy;
	AdjustWindowRect(&rcWindow, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rcWindow.right, rcWindow.bottom, SWP_NOMOVE);
}

void Calculator::setViewMode(ViewMode fMode)
{
	//kontrolki widoku standard pokaz zawsze
	m_disp->show(true);
	m_btnCE->show(true);
	m_btnC->show(true);
	m_btnDel->show(true);
	m_btnDiv->show(true);
	m_btn[7]->show(true);
	m_btn[8]->show(true);
	m_btn[9]->show(true);
	m_btnMul->show(true);
	m_btn[4]->show(true);
	m_btn[5]->show(true);
	m_btn[6]->show(true);
	m_btnSub->show(true);
	m_btn[1]->show(true);
	m_btn[2]->show(true);
	m_btn[3]->show(true);
	m_btnAdd->show(true);
	m_btnSign->show(true);
	m_btn[0]->show(true);
	m_btnComma->show(true);
	m_btnEq->show(true);
	m_btnMR->show(true);
	m_btnMS->show(true);
	m_btnMC->show(true);
	m_btnMAdd->show(true);
	m_btnMSub->show(true);

	if (fMode == EXTENDED) {
		m_fViewMode = EXTENDED;

		m_btnSq->show(true);
		m_btnPow->show(true);
		m_btnSqrt->show(true);
		m_btnFact->show(true);
		m_btnPi->show(true);
		m_btnSin->show(true);
		m_btnCos->show(true);
		m_btnTg->show(true);
		m_btnLog->show(true);
		m_btnLn->show(true);

		setSize(EXT_WIDTH, EXT_HEIGHT);
	}
	else {
		m_fViewMode = STANDARD;

		m_btnSq->show(false);
		m_btnPow->show(false);
		m_btnSqrt->show(false);
		m_btnFact->show(false);
		m_btnPi->show(false);
		m_btnSin->show(false);
		m_btnCos->show(false);
		m_btnTg->show(false);
		m_btnLog->show(false);
		m_btnLn->show(false);

		setSize(STD_WIDTH, STD_HEIGHT);
	}

	//btnCPressed();
}

void Calculator::setShiftMode(bool bShiftMode)
{
	COLORREF clrRed = RGB(0xff, 0x00, 0x00), clrBlack = RGB(0x00, 0x00, 0x00);

	m_bShiftMode = bShiftMode;

	if (m_bShiftMode) {
		//MessageBox(0, L"Shift mode turned on!", L"Msg", MB_OK);

		m_btnSq->setTextColor(clrRed);
		m_btnPow->setTextColor(clrRed);
		m_btnPi->setTextColor(clrRed);
		m_btnSign->setTextColor(clrRed);
		m_btnLog->setTextColor(clrRed);
		m_btnLn->setTextColor(clrRed);

		m_btnSq->setText(L"x³");
		m_btnPow->setText(L"ʸ√x");
		m_btnPi->setText(L"e");
		m_btnSign->setText(L"1/x");
		m_btnLog->setText(L"10ˣ");
		m_btnLn->setText(L"eˣ");
	}
	else {
		m_btnSq->setTextColor(clrBlack);
		m_btnPow->setTextColor(clrBlack);
		m_btnPi->setTextColor(clrBlack);
		m_btnSign->setTextColor(clrBlack);
		m_btnLog->setTextColor(clrBlack);
		m_btnLn->setTextColor(clrBlack);

		m_btnSq->setText(L"x²");
		m_btnPow->setText(L"xʸ");
		m_btnPi->setText(L"ᴨ");
		m_btnSign->setText(L"±");
		m_btnLog->setText(L"log");
		m_btnLn->setText(L"ln");
	}
}

void Calculator::setError(bool bError)
{
	if (bError) {
		m_bErrorFlag = true;
		m_disp->setError();
		m_disp->flush();
	}
	else {
		m_bErrorFlag = false;
	}
}

double Calculator::deg2rad(double flAngle)
{
	return (flAngle / 180.0) * M_PI;
}

void Calculator::readValue(double * lpflValue)
{
	swscanf_s(m_disp->getText(), L"%lG", lpflValue);
}

void Calculator::dispValue(double flValue)
{
	wchar_t szFormat[20];
	wchar_t szText[150];
	unsigned short wExponent; //wykladnik w golej/binarnej postaci

	if (!isnormal(flValue) && flValue != 0) {
		setError(true);
		return;
	}

	//sprawdza - tak mniej wiecej - czy wykladnik flValue nie jest >99 lub <-99
	wExponent = (unsigned short)((*reinterpret_cast<uint64_t *>(&flValue) >> 52) & 0x07FF);
	if ((wExponent > (320 + 1023) || wExponent < (-320 + 1023)) && wExponent != 0 && wExponent != 2047) { //2^320 ~= 10^96, 2^-320 ~= 10^-97
		setError(true);
		return;
	}

	//potrafilo wyswietlac "-0"
	if (flValue == 0.0) {
		m_disp->setText(L"0");
		return;
	}

	swprintf(szFormat, 20, L"%%.%dlG",
		(m_disp->getMaxChars() - 5 < 15 ? m_disp->getMaxChars() - 5 : 15));
	swprintf(szText, 150, szFormat, flValue);
	m_disp->setText(szText);
}

void Calculator::binaryEqFunc()
{
	if (m_bErrorFlag)
		return;

	if (m_fValueType == LVALUE) {
		readValue(&m_flLValue);
		if (!m_bBtnEqFlag)
			return;
	}


	if (m_fValueType == RVALUE && !m_bBtnEqFlag)
		readValue(&m_flRValue);

	switch (m_fOperand) {
	case ADD:
		m_flLValue += m_flRValue;
		break;
	case SUB:
		m_flLValue -= m_flRValue;
		break;
	case MUL:
		m_flLValue *= m_flRValue;
		break;
	case DIV:
		if (m_flRValue == 0.0) {
			setError(true);
			return;
		}
		m_flLValue /= m_flRValue;
		break;
	case POW:
		if (m_flLValue == 0.0 && m_flRValue == 0.0) {
			setError(true);
			return;
		}
		if (m_flLValue < 0.0 && ((int64_t)m_flRValue) != m_flRValue) {
			setError(true);
			return;
		}
		if (m_flLValue == 0.0) {
			m_flLValue = 0.0;
			break;
		}
		m_flLValue = pow(m_flLValue, m_flRValue);
		break;
	case ROOT:
		if (m_flRValue == 0.0) {
			setError(true);
			return;
		}
		if (m_flLValue < 0.0 && ((int64_t)(1.0 / m_flRValue)) != (1.0 / m_flRValue)) {
			setError(true);
			return;
		}
		if (m_flLValue == 0.0) {
			m_flLValue = 0.0;
			break;
		}
		m_flLValue = pow(m_flLValue, (1.0 / m_flRValue));
		break;
	default:
		break;
	}

	dispValue(m_flLValue);
	m_disp->flush();
}

void Calculator::btnCEPressed()
{
	if (m_bErrorFlag || m_bBtnEqFlag || !m_bWaitForOpFlag)
		return;

	m_disp->setText(L"0");
	m_disp->flush();
}

void Calculator::btnCPressed()
{
	m_flLValue = 0.0;
	m_flRValue = 0.0;
	m_fValueType = LVALUE;
	m_fOperand = ADD;

	m_bErrorFlag = false;
	m_bCommaFlag = false;
	m_bBtnEqFlag = false;
	m_bWaitForOpFlag = false;
	m_bUnaryPerformed = false;

	m_disp->setText(L"0");

	m_disp->flush();
}

void Calculator::btnDelPressed()
{
	if (m_bErrorFlag || m_bBtnEqFlag || !m_bWaitForOpFlag)
		return;

	m_disp->deleteLastChar();
	if (m_disp->getTextLen() > 0 && (m_disp->getText())[m_disp->getTextLen() - 1] == L'.')
		m_disp->deleteLastChar();

	if (m_disp->getTextLen() == 0)
		m_disp->setText(L"0");

	m_disp->flush();
}

void Calculator::unaryOperandPressed(HWND hSender)
{
	double flValue;

	if (m_bErrorFlag)
		return;

	readValue(&flValue);
	m_bUnaryPerformed = true; //na poczatku bo potrzebny jeden wyjatek


	if (hSender == m_btnSign->getHandle()) {
		if (m_bShiftMode) {
			if (flValue == 0) {
				setError(true);
				return;
			}

			flValue = 1.0 / flValue;
		}
		else {
			flValue *= -1.0;
			m_bUnaryPerformed = false; //wyjatek bo wiadomo
		}
	}
	else if (hSender == m_btnSq->getHandle()) {
		if (m_bShiftMode)
			flValue = flValue * flValue * flValue;
		else
			flValue = flValue * flValue;
	}
	else if (hSender == m_btnSqrt->getHandle()) {
		if (flValue < 0.0) {
			setError(true);
			return;
		}
		flValue = pow(flValue, 0.5);
	}
	else if (hSender == m_btnFact->getHandle()) {
		if (flValue < 0.0 || ((int64_t)flValue) != flValue) {
			setError(true);
			return;
		}

		if (flValue == 0.0) {
			flValue = 1.0;
		}
		else {
			for (int64_t i = (int64_t)flValue - 1; i > 1; i--)
				flValue *= i;
		}
	}
	else if (hSender == m_btnPi->getHandle()) {
		if (m_bShiftMode)
			flValue = M_E;
		else
			flValue = M_PI;
	}
	else if (hSender == m_btnLog->getHandle()) {
		if (m_bShiftMode) {
			flValue = pow(10.0, flValue);
		}
		else {
			if (flValue <= 0.0) {
				setError(true);
				return;
			}
			flValue = log10(flValue);
		}
	}
	else if (hSender == m_btnLn->getHandle()) {
		if (m_bShiftMode) {
			flValue = pow(M_E, flValue);
		}
		else {
			if (flValue <= 0.0) {
				setError(true);
				return;
			}
			flValue = log(flValue);
		}
	}
	else if (hSender == m_btnSin->getHandle()) {
		if ((int64_t)flValue == flValue)
			if ((int64_t)flValue % 180 == 0)
				flValue = 0.0;
			else if ((int64_t)flValue % 90 == 0 && (int64_t)flValue % 180 != 0)
				if (flValue > 0)
					flValue = ((int64_t)flValue % 360) < 180 ? 1.0 : -1.0;
				else
					flValue = ((int64_t)flValue % 360) > -180 ? -1.0 : 1.0;
			else if ((int64_t)flValue % 30 == 0 && (int64_t)flValue % 60 != 0 && (int64_t)flValue % 90 != 0)
				if (flValue > 0)
					flValue = ((int64_t)flValue % 360) < 180 ? 0.5 : -0.5;
				else
					flValue = ((int64_t)flValue % 360) > -180 ? -0.5 : 0.5;
			else
				flValue = sin(deg2rad(flValue));
		else
			flValue = sin(deg2rad(flValue));
	}
	else if (hSender == m_btnCos->getHandle()) {
		if ((int64_t)flValue == flValue) {
			if ((int64_t)flValue % 90 == 0 && (int64_t)flValue % 180 != 0)
				flValue = 0.0;
			else if ((int64_t)flValue % 180 == 0)
				if (((int64_t)flValue / 180) % 2 == 0)
					flValue = 1.0;
				else
					flValue = -1.0;
			else if ((int64_t)flValue % 60 == 0 && (int64_t)flValue % 180 != 0)
				if (((int64_t)flValue / 60) % 2 == 0)
					flValue = -0.5;
				else
					flValue = 0.5;
			else
				flValue = cos(deg2rad(flValue));
		}
		else
			flValue = cos(deg2rad(flValue));
	}
	else if (hSender == m_btnTg->getHandle()) {
		if ((int64_t)flValue == flValue) {
			if ((int64_t)flValue % 90 == 0 && (int64_t)flValue % 180 != 0) {
				setError(true);
				return;
			}

			if ((int64_t)flValue % 180 == 0)
				flValue = 0.0;
			else if ((int64_t)flValue % 45 == 0 && (int64_t)flValue % 90 != 0)
				if (flValue > 0)
					flValue = ((int64_t)flValue % 180) < 90 ? 1.0 : -1.0;
				else
					flValue = ((int64_t)flValue % 180) > -90 ? -1.0 : 1.0;
			else
				flValue = tan(deg2rad(flValue));
		}
		else {
			flValue = tan(deg2rad(flValue));
		}
	}


	if (m_fValueType == LVALUE)
		m_flLValue = flValue;
	else
		m_flRValue = flValue;
	dispValue(flValue);
	m_disp->flush();

	m_bWaitForOpFlag = true;
	return;
}

void Calculator::btnCommaPressed()
{
	m_bCommaFlag = true;
}

void Calculator::btnEqPressed()
{
	if (m_bErrorFlag)
		return;
	if (!m_bWaitForOpFlag && !m_bBtnEqFlag && m_fValueType == LVALUE)
		return;

	//if (!m_bWaitForOpFlag)
		//m_fValueType = LVALUE;

	binaryEqFunc();
	m_bBtnEqFlag = true;
	m_fValueType = LVALUE;
}

void Calculator::binaryOperandPressed(HWND hSender)
{
	if (m_bErrorFlag)
		return;

	if (m_fValueType == LVALUE && !m_bBtnEqFlag) {
		if (hSender == m_btnAdd->getHandle())
			m_fOperand = ADD;
		else if (hSender == m_btnSub->getHandle())
			m_fOperand = SUB;
		else if (hSender == m_btnMul->getHandle())
			m_fOperand = MUL;
		else if (hSender == m_btnDiv->getHandle())
			m_fOperand = DIV;
		else if (hSender == m_btnPow->getHandle())
			if (m_bShiftMode)
				m_fOperand = ROOT;
			else
				m_fOperand = POW;

		if (m_bWaitForOpFlag)
			binaryEqFunc();

		m_fValueType = RVALUE;
	}
	else if (m_fValueType == RVALUE || m_bBtnEqFlag) {
		if (m_bWaitForOpFlag && !m_bBtnEqFlag)
			binaryEqFunc();

		if (hSender == m_btnAdd->getHandle())
			m_fOperand = ADD;
		else if (hSender == m_btnSub->getHandle())
			m_fOperand = SUB;
		else if (hSender == m_btnMul->getHandle())
			m_fOperand = MUL;
		else if (hSender == m_btnDiv->getHandle())
			m_fOperand = DIV;
		else if (hSender == m_btnPow->getHandle())
			if (m_bShiftMode)
				m_fOperand = ROOT;
			else
				m_fOperand = POW;

		m_fValueType = RVALUE;
	}

	m_bBtnEqFlag = false;
	m_bWaitForOpFlag = false;
	m_bUnaryPerformed = false;
	return;
}

void Calculator::digitPressed(HWND hSender)
{
	if (m_bErrorFlag || m_bBtnEqFlag)
		return;

	if (!m_bWaitForOpFlag || m_bUnaryPerformed)
		m_disp->setText(L"");

	if ((wcschr(m_disp->getText(), L'.') != NULL && m_disp->getTextLen() > 15) || m_disp->getTextLen() > 14)
		return;

	if (m_bCommaFlag) {
		m_bCommaFlag = false;
		if (m_disp->getTextLen() > 0 && wcschr(m_disp->getText(), L'.') == NULL)
			m_disp->appendChar(L'.');
	}

	if (wcscmp(m_disp->getText(), L"0") == 0) //ale jesli "0." to juz nie
		m_disp->setText(L"");


	if (hSender == m_btn[0]->getHandle())
		m_disp->appendChar(L'0');
	else if (hSender == m_btn[1]->getHandle())
		m_disp->appendChar(L'1');
	else if (hSender == m_btn[2]->getHandle())
		m_disp->appendChar(L'2');
	else if (hSender == m_btn[3]->getHandle())
		m_disp->appendChar(L'3');
	else if (hSender == m_btn[4]->getHandle())
		m_disp->appendChar(L'4');
	else if (hSender == m_btn[5]->getHandle())
		m_disp->appendChar(L'5');
	else if (hSender == m_btn[6]->getHandle())
		m_disp->appendChar(L'6');
	else if (hSender == m_btn[7]->getHandle())
		m_disp->appendChar(L'7');
	else if (hSender == m_btn[8]->getHandle())
		m_disp->appendChar(L'8');
	else if (hSender == m_btn[9]->getHandle())
		m_disp->appendChar(L'9');

	m_bWaitForOpFlag = true;
	m_bUnaryPerformed = false;
	m_disp->flush();
}

bool Calculator::copyToClipboard()
{
	HGLOBAL hglbCopy;
	LPWSTR lpwstrCopy;

	if (!OpenClipboard(m_hWnd))
		return false;
	EmptyClipboard();

	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (m_disp->getTextLen() + 1) * sizeof(wchar_t));
	lpwstrCopy = (LPWSTR)GlobalLock(hglbCopy);
	memcpy_s(lpwstrCopy, (m_disp->getTextLen() + 1) * sizeof(wchar_t), m_disp->getText(), (m_disp->getTextLen() + 1) * sizeof(wchar_t));
	GlobalUnlock(hglbCopy);

	SetClipboardData(CF_UNICODETEXT, hglbCopy);

	CloseClipboard();
	m_disp->blink(400);
	return true;
}

void Calculator::btnMRPressed()
{
	if (m_bErrorFlag)
		return;

	if (m_fValueType == LVALUE)
		m_flLValue = m_flMemValue;
	else
		m_flRValue = m_flMemValue;
	dispValue(m_flMemValue);

	m_bUnaryPerformed = true; //uzyte ze wzgledu na te same skutki
	m_bWaitForOpFlag = true;
	m_disp->flush();
}

void Calculator::btnMSPressed()
{
	if (m_bErrorFlag)
		return;

	readValue(&m_flMemValue);
	m_disp->blink(200);
}

void Calculator::btnMCPressed()
{
	m_flMemValue = 0.0;
}

void Calculator::btnMAddPressed()
{
	double flValue;

	if (m_bErrorFlag)
		return;

	readValue(&flValue);
	m_flMemValue += flValue;
	m_disp->blink(200);
}

void Calculator::btnMSubPressed()
{
	double flValue;

	if (m_bErrorFlag)
		return;

	readValue(&flValue);
	m_flMemValue -= flValue;
	m_disp->blink(200);
}

void Calculator::show(bool bShow)
{
	if (bShow)
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
	else
		ShowWindow(m_hWnd, SW_HIDE);
}

LRESULT Calculator::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Calculator *p = 0;

	if (msg == WM_CREATE)
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)((CREATESTRUCT *)lParam)->lpCreateParams);
	else
		p = (Calculator *)GetWindowLong(hwnd, GWL_USERDATA);

	switch (msg)
	{
	case WM_DESTROY:
		return p->destroyEvent();
	case WM_SYSCOMMAND:
		return p->sysCommandEvent(wParam, lParam);
	case WM_SIZE:
		return p->sizeEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_MOUSEMOVE:
		return p->mouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_COMMAND:
		return p->commandEvent((HWND)lParam);
	case WM_KEYDOWN:
		return p->keyDownEvent(wParam);
	case WM_KEYUP:
		return p->keyUpEvent(wParam);
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO *lpmmi = (MINMAXINFO *)lParam;
		RECT rcWnd;

		GetWindowRect(hwnd, &rcWnd);
		lpmmi->ptMaxPosition.x = rcWnd.left;
		lpmmi->ptMaxPosition.y = rcWnd.top;
		lpmmi->ptMaxSize.x = rcWnd.right - rcWnd.left;
		lpmmi->ptMaxSize.y = rcWnd.bottom - rcWnd.top;
		lpmmi->ptMaxTrackSize.x = GetSystemMetrics(SM_CXMAXTRACK);
		lpmmi->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK);
		lpmmi->ptMinTrackSize.x = GetSystemMetrics(SM_CXMINTRACK);
		lpmmi->ptMinTrackSize.y = GetSystemMetrics(SM_CYMINTRACK);
	}
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int Calculator::destroyEvent()
{
	delete m_dialogAbout;
	delete m_disp;
	delete m_btnAdd;
	delete m_btnSub;
	delete m_btnMul;
	delete m_btnDiv;
	delete m_btnDel;
	delete m_btnCE;
	delete m_btnC;
	delete m_btnSign;
	delete m_btnComma;
	delete m_btnEq;
	for (int i = 0; i < 10; i++)
		delete m_btn[i];
	delete m_btnMR;
	delete m_btnMS;
	delete m_btnMC;
	delete m_btnMAdd;
	delete m_btnMSub;
	delete m_btnSin;
	delete m_btnCos;
	delete m_btnTg;
	delete m_btnLog;
	delete m_btnLn;
	delete m_btnSq;
	delete m_btnPow;
	delete m_btnSqrt;
	delete m_btnFact;
	delete m_btnPi;

	PostQuitMessage(0);
	return 0;
}

int Calculator::sysCommandEvent(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDM_ABOUT) {
		m_dialogAbout->setTabPage(AboutDialog::PAGE_ABOUT);
		m_dialogAbout->show();
		return 0;
	}
	else if (wParam == SC_MAXIMIZE) {
		if (m_fViewMode == STANDARD)
			setViewMode(EXTENDED);
		else
			setViewMode(STANDARD);
		return 0;
	}
	else
		return DefWindowProc(m_hWnd, WM_SYSCOMMAND, wParam, lParam);
}

int Calculator::sizeEvent(short cx, short cy)
{
	const int iSpc = 5;
	RECT rcLeft, rcRight;
	int cxBtn, cyBtn;
	int cxCell, cyCell;

	if (GetWindowLong(m_hWnd, GWL_STYLE) & WS_MAXIMIZE) {
		ShowWindow(m_hWnd, SW_RESTORE);
		if (m_fViewMode == STANDARD)
			setViewMode(EXTENDED);
		else
			setViewMode(STANDARD);
		return 0;
	}

	if (m_fViewMode == STANDARD) {
		m_disp->setPlacement(iSpc, iSpc);
		m_disp->setSize(cx - 2 * iSpc, cy / 6 - 2 * iSpc);

		rcRight.left = iSpc;
		rcRight.top = cy / 6 + iSpc;
		rcRight.right = cx;
		rcRight.bottom = cy;
		cxCell = (rcRight.right - rcRight.left) / 5;
		cyCell = (rcRight.bottom - rcRight.top) / 5;
		cxBtn = cxCell - iSpc * 2;
		cyBtn = cyCell - iSpc * 2;

		m_btnMR->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 0);
		m_btnMR->setSize(cxBtn, cyBtn);
		m_btnDel->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 0);
		m_btnDel->setSize(cxBtn, cyBtn);
		m_btnCE->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 0);
		m_btnCE->setSize(cxBtn, cyBtn);
		m_btnC->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 0);
		m_btnC->setSize(cxBtn, cyBtn);
		m_btnDiv->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 0);
		m_btnDiv->setSize(cxBtn, cyBtn);

		m_btnMS->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 1);
		m_btnMS->setSize(cxBtn, cyBtn);
		m_btn[7]->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 1);
		m_btn[7]->setSize(cxBtn, cyBtn);
		m_btn[8]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 1);
		m_btn[8]->setSize(cxBtn, cyBtn);
		m_btn[9]->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 1);
		m_btn[9]->setSize(cxBtn, cyBtn);
		m_btnMul->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 1);
		m_btnMul->setSize(cxBtn, cyBtn);

		m_btnMC->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 2);
		m_btnMC->setSize(cxBtn, cyBtn);
		m_btn[4]->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 2);
		m_btn[4]->setSize(cxBtn, cyBtn);
		m_btn[5]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 2);
		m_btn[5]->setSize(cxBtn, cyBtn);
		m_btn[6]->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 2);
		m_btn[6]->setSize(cxBtn, cyBtn);
		m_btnSub->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 2);
		m_btnSub->setSize(cxBtn, cyBtn);

		m_btnMAdd->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 3);
		m_btnMAdd->setSize(cxBtn, cyBtn);
		m_btn[1]->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 3);
		m_btn[1]->setSize(cxBtn, cyBtn);
		m_btn[2]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 3);
		m_btn[2]->setSize(cxBtn, cyBtn);
		m_btn[3]->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 3);
		m_btn[3]->setSize(cxBtn, cyBtn);
		m_btnAdd->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 3);
		m_btnAdd->setSize(cxBtn, cyBtn);

		m_btnMSub->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 4);
		m_btnMSub->setSize(cxBtn, cyBtn);
		m_btnSign->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 4);
		m_btnSign->setSize(cxBtn, cyBtn);
		m_btn[0]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 4);
		m_btn[0]->setSize(cxBtn, cyBtn);
		m_btnComma->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 4);
		m_btnComma->setSize(cxBtn, cyBtn);
		m_btnEq->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 4);
		m_btnEq->setSize(cxBtn, cyBtn);
	}
	else if (m_fViewMode == EXTENDED) {
		m_disp->setPlacement(iSpc, iSpc);
		m_disp->setSize(cx - 2 * iSpc, cy / 6 - 2 * iSpc);

		rcRight.left = cx - STD_WIDTH + iSpc;
		rcRight.top = cy / 6 + iSpc;
		rcRight.right = cx;
		rcRight.bottom = cy;
		cxCell = (rcRight.right - rcRight.left) / 5;
		cyCell = (rcRight.bottom - rcRight.top) / 5;
		cxBtn = cxCell - iSpc * 2;
		cyBtn = cyCell - iSpc * 2;

		m_btnMR->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 0);
		m_btnMR->setSize(cxBtn, cyBtn);
		m_btnDel->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 0);
		m_btnDel->setSize(cxBtn, cyBtn);
		m_btnCE->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 0);
		m_btnCE->setSize(cxBtn, cyBtn);
		m_btnC->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 0);
		m_btnC->setSize(cxBtn, cyBtn);
		m_btnDiv->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 0);
		m_btnDiv->setSize(cxBtn, cyBtn);

		m_btnMS->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 1);
		m_btnMS->setSize(cxBtn, cyBtn);
		m_btn[7]->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 1);
		m_btn[7]->setSize(cxBtn, cyBtn);
		m_btn[8]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 1);
		m_btn[8]->setSize(cxBtn, cyBtn);
		m_btn[9]->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 1);
		m_btn[9]->setSize(cxBtn, cyBtn);
		m_btnMul->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 1);
		m_btnMul->setSize(cxBtn, cyBtn);

		m_btnMC->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 2);
		m_btnMC->setSize(cxBtn, cyBtn);
		m_btn[4]->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 2);
		m_btn[4]->setSize(cxBtn, cyBtn);
		m_btn[5]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 2);
		m_btn[5]->setSize(cxBtn, cyBtn);
		m_btn[6]->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 2);
		m_btn[6]->setSize(cxBtn, cyBtn);
		m_btnSub->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 2);
		m_btnSub->setSize(cxBtn, cyBtn);

		m_btnMAdd->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 3);
		m_btnMAdd->setSize(cxBtn, cyBtn);
		m_btn[1]->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 3);
		m_btn[1]->setSize(cxBtn, cyBtn);
		m_btn[2]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 3);
		m_btn[2]->setSize(cxBtn, cyBtn);
		m_btn[3]->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 3);
		m_btn[3]->setSize(cxBtn, cyBtn);
		m_btnAdd->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 3);
		m_btnAdd->setSize(cxBtn, cyBtn);

		m_btnMSub->setPlacement(rcRight.left + cxCell * 0, rcRight.top + cyCell * 4);
		m_btnMSub->setSize(cxBtn, cyBtn);
		m_btnSign->setPlacement(rcRight.left + cxCell * 1, rcRight.top + cyCell * 4);
		m_btnSign->setSize(cxBtn, cyBtn);
		m_btn[0]->setPlacement(rcRight.left + cxCell * 2, rcRight.top + cyCell * 4);
		m_btn[0]->setSize(cxBtn, cyBtn);
		m_btnComma->setPlacement(rcRight.left + cxCell * 3, rcRight.top + cyCell * 4);
		m_btnComma->setSize(cxBtn, cyBtn);
		m_btnEq->setPlacement(rcRight.left + cxCell * 4, rcRight.top + cyCell * 4);
		m_btnEq->setSize(cxBtn, cyBtn);
		
		rcLeft.left = iSpc;
		rcLeft.top = cy / 6 + iSpc;
		rcLeft.right = rcLeft.left + cxCell * 2;
		rcLeft.bottom = cy;

		m_btnSq->setPlacement(rcLeft.left + cxCell * 0, rcLeft.top + cyCell * 0);
		m_btnSq->setSize(cxBtn, cyBtn);
		m_btnPow->setPlacement(rcLeft.left + cxCell * 0, rcLeft.top + cyCell * 1);
		m_btnPow->setSize(cxBtn, cyBtn);
		m_btnSqrt->setPlacement(rcLeft.left + cxCell * 0, rcLeft.top + cyCell * 2);
		m_btnSqrt->setSize(cxBtn, cyBtn);
		m_btnFact->setPlacement(rcLeft.left + cxCell * 0, rcLeft.top + cyCell * 3);
		m_btnFact->setSize(cxBtn, cyBtn);
		m_btnPi->setPlacement(rcLeft.left + cxCell * 0, rcLeft.top + cyCell * 4);
		m_btnPi->setSize(cxBtn, cyBtn);

		m_btnSin->setPlacement(rcLeft.left + cxCell * 1, rcLeft.top + cyCell * 0);
		m_btnSin->setSize(cxBtn, cyBtn);
		m_btnCos->setPlacement(rcLeft.left + cxCell * 1, rcLeft.top + cyCell * 1);
		m_btnCos->setSize(cxBtn, cyBtn);
		m_btnTg->setPlacement(rcLeft.left + cxCell * 1, rcLeft.top + cyCell * 2);
		m_btnTg->setSize(cxBtn, cyBtn);
		m_btnLog->setPlacement(rcLeft.left + cxCell * 1, rcLeft.top + cyCell * 3);
		m_btnLog->setSize(cxBtn, cyBtn);
		m_btnLn->setPlacement(rcLeft.left + cxCell * 1, rcLeft.top + cyCell * 4);
		m_btnLn->setSize(cxBtn, cyBtn);
	}

	return 0;
}

int Calculator::mouseMoveEvent(short x, short y)
{
	SetCursor(m_hCursor);
	return 0;
}

int Calculator::commandEvent(HWND hSender)
{
	PlaySound(MAKEINTRESOURCE(IDS_BUTTON_VOICE), GetModuleHandle(NULL), SND_ASYNC | SND_RESOURCE);

	for (int i = 0; i < 10; i++)
		if (hSender == m_btn[i]->getHandle()) {
			digitPressed(hSender);
			return 0;
		}	

	if (hSender == m_btnAdd->getHandle() ||
		hSender == m_btnSub->getHandle() ||
		hSender == m_btnMul->getHandle() ||
		hSender == m_btnDiv->getHandle() ||
		hSender == m_btnPow->getHandle()) {
		binaryOperandPressed(hSender);
		return 0;
	}

	if (hSender == m_btnSign->getHandle() ||
		hSender == m_btnSq->getHandle() ||
		hSender == m_btnSqrt->getHandle() ||
		hSender == m_btnFact->getHandle() ||
		hSender == m_btnPi->getHandle() ||
		hSender == m_btnSin->getHandle() ||
		hSender == m_btnCos->getHandle() ||
		hSender == m_btnTg->getHandle() ||
		hSender == m_btnLog->getHandle() ||
		hSender == m_btnLn->getHandle()) {
		unaryOperandPressed(hSender);
		return 0;
	}

	if (hSender == m_btnC->getHandle())
		btnCPressed();
	else if (hSender == m_btnCE->getHandle())
		btnCEPressed();
	else if (hSender == m_btnDel->getHandle())
		btnDelPressed();
	else if (hSender == m_btnEq->getHandle())
		btnEqPressed();
	else if (hSender == m_btnComma->getHandle())
		btnCommaPressed();
	else if (hSender == m_btnMR->getHandle())
		btnMRPressed();
	else if (hSender == m_btnMS->getHandle())
		btnMSPressed();
	else if (hSender == m_btnMC->getHandle())
		btnMCPressed();
	else if (hSender == m_btnMAdd->getHandle())
		btnMAddPressed();
	else if (hSender == m_btnMSub->getHandle())
		btnMSubPressed();
	return 0;
}

int Calculator::keyDownEvent(WPARAM wKeyCode)
{
	switch (wKeyCode) {
	case VK_LSHIFT:
	case VK_RSHIFT:
	case VK_SHIFT:
		if (m_bShiftMode != true)
			setShiftMode(true);
		break;
	default:
		break;
	}

	return 0;
}

int Calculator::keyUpEvent(WPARAM wKeyCode)
{
	switch (wKeyCode) {
	case VK_LSHIFT:
	case VK_RSHIFT:
	case VK_SHIFT:
		setShiftMode(false);
		break;
	case VK_SPACE:
		if (m_bShiftMode == false)
			setShiftMode(true);
		else
			setShiftMode(false);
		break;
	case VK_NUMPAD0:
		m_btn[0]->click();
		break;
	case VK_NUMPAD1:
		m_btn[1]->click();
		break;
	case VK_NUMPAD2:
		m_btn[2]->click();
		break;
	case VK_NUMPAD3:
		m_btn[3]->click();
		break;
	case VK_NUMPAD4:
		m_btn[4]->click();
		break;
	case VK_NUMPAD5:
		m_btn[5]->click();
		break;
	case VK_NUMPAD6:
		m_btn[6]->click();
		break;
	case VK_NUMPAD7:
		m_btn[7]->click();
		break;
	case VK_NUMPAD8:
		m_btn[8]->click();
		break;
	case VK_NUMPAD9:
		m_btn[9]->click();
		break;
	case VK_RETURN:
		m_btnEq->click();
		break;
	case VK_MULTIPLY:
		m_btnMul->click();
		break;
	case VK_DIVIDE:
		m_btnDiv->click();
		break;
	case VK_ADD:
		if (GetAsyncKeyState(VK_MENU) & 0x8000)
			m_btnMAdd->click();
		else
			m_btnAdd->click();
		break;
	case VK_SUBTRACT:
		if (GetAsyncKeyState(VK_MENU) & 0x8000)
			m_btnMSub->click();
		else
			m_btnSub->click();
		break;
	case VK_DECIMAL:
		m_btnComma->click();
		break;
	case VK_INSERT:
		m_btnSign->click();
		break;
	case VK_ESCAPE:
		m_btnC->click();
		break;
	case VK_BACK:
		if (GetAsyncKeyState(VK_MENU) & 0x8000)
			m_btnMC->click();
		else
			m_btnCE->click();
		break;
	case VK_DELETE:
		m_btnDel->click();
		break;
	case 0x43: //klawisz 'C'
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			copyToClipboard();
		break;
	case 0x53: //klawisz 'S'
		if (GetAsyncKeyState(VK_MENU) & 0x8000)
			m_btnMS->click();
		else
			m_btnSin->click();
		break;
	case 0x4f: //klawisz 'O'
		m_btnCos->click();
		break;
	case 0x54: //klawisz 'T'
		m_btnTg->click();
		break;
	case 0x4c: //klawisz 'L'
		m_btnLog->click();
		break;
	case 0x4e: //klawisz 'N'
		m_btnLn->click();
		break;
	case 0x51: //klawisz 'Q'
		m_btnSq->click();
		break;
	case 0x59: //klawisz 'Y'
		m_btnPow->click();
		break;
	case 0x52: //klawisz 'R'
		if (GetAsyncKeyState(VK_MENU) & 0x8000)
			m_btnMR->click();
		else
			m_btnSqrt->click();
		break;
	case 0x46: //klawisz 'F'
		m_btnFact->click();
		break;
	case 0x50: //klawisz 'P'
		m_btnPi->click();
		break;
	default:
		break;
	}

	return 0;
}
