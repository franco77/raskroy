// Sheet.cpp : Implementation of CSheet
#include "stdafx.h"
#include "COMRaskroy.h"
#include "Sheet.h"

/////////////////////////////////////////////////////////////////////////////
// CSheet


STDMETHODIMP CSheet::get_Length(CURRENCY *pVal)
{
	*pVal = length;

	return S_OK;
}

STDMETHODIMP CSheet::put_Length(CURRENCY newVal)
{
	length = newVal;

	return S_OK;
}

STDMETHODIMP CSheet::get_Width(CURRENCY *pVal)
{
	*pVal = width;

	return S_OK;
}

STDMETHODIMP CSheet::put_Width(CURRENCY newVal)
{
	width = newVal;

	return S_OK;
}

STDMETHODIMP CSheet::get_Amount(long *pVal)
{
	*pVal = amount;

	return S_OK;
}

STDMETHODIMP CSheet::put_Amount(long newVal)
{
	amount = newVal;

	return S_OK;
}

STDMETHODIMP CSheet::get_Rotate(BOOL *pVal)
{
	*pVal = rotate;

	return S_OK;
}

STDMETHODIMP CSheet::put_Rotate(BOOL newVal)
{
	rotate = newVal;

	return S_OK;
}
