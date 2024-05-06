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

#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <wchar.h>

class AboutDialog
{
public:
	enum {PAGE_ABOUT, PAGE_LICENSE};

	AboutDialog(HWND hParent);
	~AboutDialog();
	static bool Register(); //COMMONCTRLS rejestruje automatycznie

	void show();
	void setSize(int cx, int cy);
	void setDefaultSize();
	void setTabPage(int iPage);

	void setIcon(HICON hicon);
	void setAppName(const wchar_t *szName);
	void setVersionInfo(const wchar_t *szInfo);
	void setCopyrightNote(const wchar_t *szNote);
	void setDescription(const wchar_t *szDescription);
	void setLicenceText(const wchar_t *szText);

private:
	static const wchar_t *CLASS_NAME;

	void showTabPage(int iPage);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int destroyEvent();
	int closeEvent();
	int commandEvent(HWND hSender);
	int notifyEvent(LPARAM lParam);
	int sizeEvent(short cx, short cy);

	HWND m_hWnd, m_hParent;
	HWND m_hTab, m_hBtnOk;
	HWND m_hIconLabel, m_hNameLabel, m_hVersionLabel, m_hCopyrightLabel, m_hDescriptionLabel;
	HWND m_hLicenceEdit;
};

#endif
