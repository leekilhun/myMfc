// tab_io.cpp: 구현 파일
//

#include "pch.h"
#include "fastechModule.h"
#include "tab_io.h"
#include "afxdialogex.h"


#include "..\CGridListCtrlEx\CGridColumnTraitDateTime.h"
#include "..\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "..\CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "..\CGridListCtrlEx\CGridColumnTraitHyperLink.h"
#include "..\CGridListCtrlEx\CGridRowTraitXP.h"
#include "..\CGridListCtrlEx\ViewConfigSection.h"


// tab_io 대화 상자

IMPLEMENT_DYNAMIC(tab_io, CDialogEx)

tab_io::tab_io(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_IO, pParent)
{

}

tab_io::~tab_io()
{
}

void tab_io::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}


BEGIN_MESSAGE_MAP(tab_io, CDialogEx)
END_MESSAGE_MAP()


// tab_io 메시지 처리기


BOOL tab_io::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

  m_listLog.SetCellMargin(1.5);
  CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;
  m_listLog.SetDefaultRowTrait(pRowTrait);
  m_listLog.EnableVisualStyles(true);

  // Create Columns
  m_listLog.InsertHiddenLabelColumn();	// Requires one never uses column 0

	TRACE("GetColCount = %d\n", m_LogData.GetColCount());
	for (int col = 0; col < m_LogData.GetColCount(); ++col)
	{
		const CString& title = m_LogData.GetColTitle(col);
		TRACE(L"title = %s\n", m_LogData.GetColTitle(col));
		CGridColumnTrait* pTrait = NULL;
		if (col == 0)	// Item
		{
			//pTrait = new CGridColumnTraitText;
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
			pComboTrait->SetShowDropDown(TRUE);//REX
			//pComboTrait->SetStyle(10);
			//pComboTrait->SetSingleClickEdit(TRUE);
			//pComboTrait->SetStyle(2);
			const vector<CString>& countries = m_LogData.GetLogMenu();
			for (size_t i = 0; i < countries.size(); ++i) {
				pComboTrait->AddItem((DWORD_PTR)i, countries[i]);
				TRACE(L"i = %d, countries[i] = %s\n", i, countries[i]);
			}
			pTrait = pComboTrait;
		}
		if (col == 1)	// Context
		{
			pTrait = new CGridColumnTraitEdit;
			//CGridColumnTraitCombo* pComboTrait1 = new CGridColumnTraitCombo;
			//pTrait = pComboTrait1;
		}

		m_listLog.InsertColumnTrait(col + 1, title, LVCFMT_LEFT, 150, col, pTrait);
		//m_ListCtrl.InsertColumnTrait()
		//m_ListCtrl.InsertItem()
}

	// Insert data into list-control by copying from datamodel
	TRACE("GetRowIds = %d\n", m_LogData.GetRowIds());

	int nItem = 0;
	for (size_t rowId = 0; rowId < m_LogData.GetRowIds(); ++rowId)
	{
		TRACE(L"rowId= %d, InsertItem = %s\n", rowId, m_LogData.GetCellText(rowId, 0));
		nItem = m_listLog.InsertItem(++nItem, m_LogData.GetCellText(rowId, 0));
		m_listLog.SetItemData(nItem, rowId);
		for (int col = 0; col < m_LogData.GetColCount(); ++col)
		{
			int nCellCol = col + 1;	// +1 because of hidden column
			const CString& strCellText = m_LogData.GetCellText(rowId, col);
			m_listLog.SetItemText(nItem, nCellCol, strCellText);
			TRACE(L"col = %d, nItem = %d, nCellCol = %d, strCellText = %s\n", col, nItem, nCellCol, strCellText);
		}
	}

	CViewConfigSectionWinApp* pColumnProfile = new CViewConfigSectionWinApp(_T("Sample List"));
	pColumnProfile->AddProfile(_T("Default"));
	pColumnProfile->AddProfile(_T("Special"));
	m_listLog.SetupColumnConfig(pColumnProfile);


	TRACE("ShowAllDataModel go\n");
	m_LogData.ShowAllDataModel();

#if 0

  LPWSTR list[3] = { L"date-time",L"level", L"context" };
  int list_width[3] = { 150,100,255 };

  for (int i = 0; i < 3; i++)
  {
    m_listLog.InsertColumn(i, list[i], LVCFMT_CENTER, list_width[i]);
  }


  m_listLog.SetExtendedStyle(m_listLog.GetExStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
  CString str_no;
  int line_no = 0;
  str_no.Format(L"No %d", line_no + 1);
  m_listLog.InsertItem(line_no, str_no);
  m_listLog.SetItem(line_no, 0, LVIF_TEXT, L"first line!", 0, 0, 0, 0);
  m_listLog.SetItem(line_no, 1, LVIF_TEXT, L"first col 2!", 0, 0, 0, 0);
  m_listLog.SetItem(line_no, 2, LVIF_TEXT, L"first context", 0, 0, 0, 0);

#endif // 0



  return TRUE;  // return TRUE unless you set the focus to a control
                // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
