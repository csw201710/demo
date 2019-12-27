#pragma once

#include "ResizableDialog.h"

class BaseDialog :
	public CResizableDialog
{
public:
	BaseDialog();
	BaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) :
		CResizableDialog(nIDTemplate, pParentWnd)
	{

	}

	~BaseDialog();
};

