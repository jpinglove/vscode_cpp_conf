

#ifndef _SETTINGS_DLG_H
#define _SETTINGS_DLG_H

#include "DockingDlgInterface.h"
#include "resource.h"

// CSettingsDlg 对话框

class CSettingsDlg : public DockingDlgInterface
{
public:
	//CSettingsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	CSettingsDlg() : DockingDlgInterface(IDD_SETTINGS) {};

	//virtual ~CSettingsDlg();

	virtual void display(bool toShow = true) const {
		DockingDlgInterface::display(toShow);
		if (toShow)
			::SetFocus(::GetDlgItem(_hSelf, IDC_EDIT_PORT));
	};

	void setParent(HWND parent2set) {
		_hParent = parent2set;
	};

protected:
	virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void SaveBtnClick();

};

#endif
