// TaskDesignView.h : interface of the CTaskDesignView class
//


#pragma once


class CTaskDesignView : public CScrollView
{
protected: // create from serialization only
	CTaskDesignView();
	DECLARE_DYNCREATE(CTaskDesignView)

// Attributes
public:
	CTaskDesignDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTaskDesignView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
private:
	struct TASKPARSSINGLEMODE *m_pstTaskParsSingleMode;
	struct TASKPARSMULTIPLEMODES *m_pstTaskParsMultipleModes;
	short int *m_psnSingleTaskSignal;


public:
	void DrawSingleTaskMode(CDC *pDC);
	void DrawMultipleTasksMode(CDC *pDC);
};

#ifndef _DEBUG  // debug version in TaskDesignView.cpp
inline CTaskDesignDoc* CTaskDesignView::GetDocument() const
   { return reinterpret_cast<CTaskDesignDoc*>(m_pDocument); }
#endif

