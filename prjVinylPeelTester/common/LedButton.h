#pragma once
#include "pch.h"
// LedButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLedButton window

#define DEFAULT_MARGIN	20

/*
  class CLedButton : public CButton (version 1.0)

  Description:
    class that displays a LED that has 2 states: ON or OFF.
    The LED can have some text on its right.
    The LED is read-only (by user) and can not have focus (you should disable "tab stop" in the resource),
    The disabled state is not implemented: it is like the enable state.

  How to use:
    To your dialog add either a button, a checkbox or a radio button,
    bind it to a member variable (of type Control) using ClassWizard,
    then change in the header file the class type to CLedButton.

    In InitDialog bind your bitmap resource to the button:
    m_greenLed.SetImage( IDB_LEDBUTTON_GREEN, 15 ); //15 is the width of one image in the bitmap

    //You can set or retreive the state of the LED (ON or OFF) using
    m_greenLed.Depress(TRUE);  //ON  (second image in bitmap)
    m_greenLed.Depress(FALSE); //OFF (first image in bitmap)

    These bitmaps are given with this source code:
    IDB_LEDBUTTON_GREEN.bmp
    IDB_LEDBUTTON_RED.bmp

  Copyright:
    Completely Free (as long as this header remains like this) !
    Please visit http://welcome.to/softbird for updates, other sources and more !
    (c)Benjamin Mayrargue 2000



Example of use:

in OnInitDialog():
  //Set images
  m_greenLed.SetImage( IDB_GREENBUTTON, 15 );
  m_redLed.SetImage( IDB_REDBUTTON, 15 );
  m_redLed2.SetImage( IDB_REDBUTTON, 15 );

  // set the initial state of buttons
  m_greenLed.Depress(true);
  m_redLed.Depress(true);

//Add a handler to a switch button (for example)
  void CExampleDlg::OnClikSwitchRedLed()
  {
    m_redLed.Depress(!m_redLed.IsDepressed());
  }
*/


class CRGBImageList
{
protected:
  CDC  m_dcMem;
  HBITMAP m_hImage;
  CBitmap m_bmp;
  UINT m_width, m_height;
  CGdiObject* m_old;
  BITMAP m_bmpInfo;


public:
  CRGBImageList():m_bmpInfo(), m_hImage(), m_width(0), m_height(0) {
    m_dcMem.CreateCompatibleDC(NULL);
    m_old = NULL;
  }
  virtual ~CRGBImageList()  {
    if (m_hImage)
    {
      m_dcMem.SelectObject(m_old);
      m_old = NULL;
      m_bmp.Detach();
      ::DeleteObject(m_hImage);
    }
    m_dcMem.DeleteDC();
  }


  BOOL Create(UINT idBitmap, UINT nWidth)  {
    m_hImage = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(idBitmap), IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS);
    if (!m_hImage) return FALSE;
    m_bmp.Attach(m_hImage);
    m_width = nWidth;

    //CSize size = m_bmp.GetBitmapDimension(); //Must be set by SetBitmapDimension: unusable here.
    m_bmp.GetObject(sizeof(BITMAP), &m_bmpInfo);
    m_height = m_bmpInfo.bmHeight;

    m_old = m_dcMem.SelectObject(&m_bmp);
    return TRUE;
  }

  BOOL Draw(CDC* pDC, int nImage, POINT pt) {
    return  pDC->BitBlt(pt.x, pt.y, m_width, m_height, &m_dcMem, nImage * m_width, 0, SRCCOPY);
  }

  CSize GetImageDimension() { CSize s(m_width, m_bmpInfo.bmHeight); return s; }


};


class CLedButton : public CButton
{
  /****************************************************
   *	data
   ****************************************************/
protected:
  int m_nMargin;
  int m_nRadius;
  CPoint m_ptCenter;
  CRect  m_textRect;
  BOOL   m_bDepressed, m_bCenterAlign, m_bReadOnly; //m_bReadOnly does not work

  CRGBImageList m_imgList;
  CSize m_rcImage;


  /****************************************************
   *	Constructor
   ****************************************************/
public:
  //ReadOnly does not work: it is ALWAYS read-only.
  CLedButton(BOOL bReadOnly = TRUE, BOOL bDepressed = FALSE, BOOL bCenterAlign = TRUE) { 
    Init(bReadOnly, bDepressed, bCenterAlign); 
  }
  virtual ~CLedButton() {}


  /****************************************************
   *	func
   ****************************************************/

  inline void Init(BOOL bReadOnly = TRUE, BOOL bDepressed = FALSE, BOOL bCenterAlign = TRUE) {
    m_bDepressed = bDepressed;
    m_bCenterAlign = bCenterAlign;
    m_nMargin = DEFAULT_MARGIN;
    m_bReadOnly = bReadOnly;
  }

  //Call "SetImage" once before displaying the control.
  inline void SetImage(UINT idBmp, UINT nWidthOfOneImage) {
    m_imgList.Create(idBmp, nWidthOfOneImage);
    m_rcImage = m_imgList.GetImageDimension();
    m_ptCenter.x = 1; //m_ptCenter.x - rcImage.cx/2;
    m_ptCenter.y = m_ptCenter.y - m_rcImage.cy / 2;

    if (m_nMargin < m_rcImage.cx)
      m_nMargin = 5 + m_rcImage.cx;
  }
  //Does not work// void SetReadOnly(BOOL bReadOnly=TRUE) { m_bReadOnly = bReadOnly; }

  //Set button state (TRUE=pushed, FALSE=depressed)
  inline BOOL Depress(BOOL bDown) {
    //if( !m_bReadOnly )
    //  m_bDepressed = GetCheck();
    if (bDown != m_bDepressed)
    {
      m_bDepressed = bDown;
      //SetCheck(bDown);
      //PostMessage(BM_SETCHECK, bDown, 0);
      RedrawWindow();
    }

    return m_bDepressed;
  }
  
  //Get button state
  inline BOOL IsDepressed() {
    if (!m_bReadOnly)
      m_bDepressed = GetCheck();
    return m_bDepressed;
  }


public:
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {
    ASSERT(lpDrawItemStruct != NULL);
    if (lpDrawItemStruct->itemAction != ODA_DRAWENTIRE)
      return;

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;

    int nSavedDC = pDC->SaveDC();

    pDC->SelectStockObject(NULL_BRUSH);
    pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));

    //if( GetStyle()&BS_AUTOCHECKBOX && lpDrawItemStruct->itemState&ODS_SELECTED)
  //    SetCheck(GetCheck()?0:1);
    //if( !m_bReadOnly )
      //m_bDepressed = lpDrawItemStruct->itemState&ODS_SELECTED;
      //m_bDepressed = GetCheck();
      //Does not work as an auto checkbox !!! Why ???

    if (m_bDepressed) //Button down
      m_imgList.Draw(pDC, 1, m_ptCenter);
    else	//Button up
      m_imgList.Draw(pDC, 0, m_ptCenter);

    // draw the text if there is any
    CString strText;
    GetWindowText(strText);
    CSize Extent;
    CPoint pt;

    CRect textRect, clientRect;
    GetClientRect(clientRect);
    textRect = clientRect;
    textRect.left += m_nMargin;
    textRect.right -= 2;
    if (!strText.IsEmpty())
    {

      pDC->SetBkMode(TRANSPARENT);

      /*if (state & ODS_DISABLED)
      {
        Extent = pDC->GetTextExtent(strText);
        pt.x = rect.left + m_nMargin;
        pt.y = (int)((rect.Height() - Extent.cy)*0.5);
        pDC->DrawState(pt, Extent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
        }
      else*/
      {
        pDC->DrawText(strText, textRect, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);
        int h = textRect.Height();
        textRect.top = clientRect.top + (int)((clientRect.Height() - textRect.Height()) * 0.5 + 0.5);
        textRect.bottom = textRect.top + h;
        pDC->DrawText(strText, textRect, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
        m_textRect = textRect;
      }
    }
    textRect.right += 2;

    pDC->RestoreDC(nSavedDC);
  }

protected:
  virtual void PreSubclassWindow() {
    CButton::PreSubclassWindow();

    // make sure we are an owner draw button
    ModifyStyle(0, BS_OWNERDRAW);

    CRect rect;
    GetClientRect(rect);

    // setup the button metrics
    LOGFONT lf;
    GetFont()->GetLogFont(&lf);
    m_nRadius = lf.lfHeight;
    if (m_nRadius == 0)
      m_nRadius = 15;
    if (m_nRadius < 0)
      m_nRadius = (-1) * m_nRadius;
    m_nRadius = (int)(rect.bottom * 0.5) - 5;

    // don't let the button get above this maximum radius
    // user can reset m_nRadius later, if desired
    if (m_nRadius > 6)
      m_nRadius = 6;

    m_ptCenter.x = rect.left + m_nRadius + 1;
    if (m_bCenterAlign)
      m_ptCenter.y = rect.top + (int)(rect.Height() * 0.5 + 0.5);
    else
      m_ptCenter.y = rect.top + m_nRadius + 1;
  }
};

