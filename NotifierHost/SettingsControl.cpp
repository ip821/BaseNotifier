// SettingsControl.cpp : Implementation of CSettingsControl

#include "stdafx.h"
#include "SettingsControl.h"


// CSettingsControl

STDMETHODIMP CSettingsControl::GetHWND(HWND *hWnd)
{
	*hWnd = m_hWnd;
	return S_OK;
}

STDMETHODIMP CSettingsControl::CreateEx(HWND hWndParent, HWND *hWnd)
{
	*hWnd = Create(hWndParent);
	return S_OK;
}

STDMETHODIMP CSettingsControl::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plResult, BOOL *bResult)
{
	LRESULT lResult = 0;
	*bResult = ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult);
	*plResult = lResult;
	return S_OK;
}

LRESULT CSettingsControl::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CSettingsControl>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	return 0;
}

STDMETHODIMP CSettingsControl::PreTranslateMessage(MSG *pMsg, BOOL *pbResult)
{
	return E_NOTIMPL;
}

STDMETHODIMP CSettingsControl::Load(ISettings *pSettings)
{
	CHECK_E_POINTER(pSettings);
	RETURN_IF_FAILED(LoadEditBoxText(IDC_EDITSERVER, KEY_SERVER, pSettings));
	RETURN_IF_FAILED(LoadEditBoxText(IDC_EDITUSER, KEY_USER, pSettings));
	RETURN_IF_FAILED(LoadEditBoxText(IDC_EDITPASSWORD, KEY_PASSWORD, pSettings));

	UINT uTimeout = 1;
    CComVar vTimeout;
	if (SUCCEEDED(pSettings->GetVariantValue(L"TimerInterval", &vTimeout)) && vTimeout.vt == VT_I4)
		uTimeout = vTimeout.intVal;

	CEdit wndTimeout = GetDlgItem(IDC_EDITCHECKINTERVAL);

	TCHAR buf[512] = { 0 };
	_itow_s(uTimeout, buf, 10);
	wndTimeout.SetWindowText(buf);
	return S_OK;
}

STDMETHODIMP CSettingsControl::Save(ISettings *pSettings)
{
	CHECK_E_POINTER(pSettings);
	RETURN_IF_FAILED(SaveEditBoxText(IDC_EDITSERVER, KEY_SERVER, pSettings));
	RETURN_IF_FAILED(SaveEditBoxText(IDC_EDITUSER, KEY_USER, pSettings));
	RETURN_IF_FAILED(SaveEditBoxText(IDC_EDITPASSWORD, KEY_PASSWORD, pSettings));

	CEdit wndTimeout = GetDlgItem(IDC_EDITCHECKINTERVAL);
	CComBSTR bstrTimeout;
	wndTimeout.GetWindowText(&bstrTimeout);

	if (bstrTimeout == NULL)
		bstrTimeout = "";

	int val = _wtoi(bstrTimeout);
	RETURN_IF_FAILED(pSettings->SetVariantValue(L"TimerInterval", &CComVar(val)));
	return S_OK;
}

STDMETHODIMP CSettingsControl::Reset(ISettings *pSettings)
{
	return E_NOTIMPL;
}

HRESULT CSettingsControl::SaveEditBoxText(int id, BSTR bstrKey, ISettings* pSettings)
{
	CEdit wndTextBox = GetDlgItem(id);
	CComBSTR bstr;
	wndTextBox.GetWindowText(&bstr);

	if (bstr == NULL)
		bstr = "";
	RETURN_IF_FAILED(pSettings->SetVariantValue(bstrKey, &CComVar(bstr)));
	return S_OK;
}

HRESULT CSettingsControl::LoadEditBoxText(int id, BSTR bstrKey, ISettings* pSettings)
{
	CHECK_E_POINTER(pSettings);
    CComVar vValue;
	CComBSTR bstr;
	if (SUCCEEDED(pSettings->GetVariantValue(bstrKey, &vValue) && vValue.vt == VT_BSTR))
		bstr = vValue.bstrVal;

	CEdit wndServerTextBox = GetDlgItem(id);
	wndServerTextBox.SetWindowText(bstr);
	return S_OK;
}
