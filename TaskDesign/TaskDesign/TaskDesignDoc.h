// TaskDesignDoc.h : interface of the CTaskDesignDoc class
//


#pragma once

#include "TaskConstruction.h"


class CTaskDesignDoc : public CDocument
{
protected: // create from serialization only
	CTaskDesignDoc();
	DECLARE_DYNCREATE(CTaskDesignDoc)

// Attributes
public:

// Operations
public:
	class TaskConstruction   *m_pTaskConstruction;

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CTaskDesignDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTaskconstructionSinglemode();
	afx_msg void OnTaskConstructionMutipleModes();
	afx_msg void OnTaskConstructionMultipletasksDisplay();
public:
	BOOL m_bIsOpenMultipleModes;
	BOOL m_bIsOpenSingleMode;

	afx_msg void OnTaskConstructionSingleTaskDisplay();
};


