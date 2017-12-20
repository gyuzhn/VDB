// TaskDesignView.cpp : implementation of the CTaskDesignView class
//

#include "stdafx.h"
#include "TaskDesign.h"

#include "TaskDesignDoc.h"
#include "TaskDesignView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTaskDesignView

IMPLEMENT_DYNCREATE(CTaskDesignView, CScrollView)

BEGIN_MESSAGE_MAP(CTaskDesignView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTaskDesignView construction/destruction

CTaskDesignView::CTaskDesignView()
{
	// TODO: add construction code here
	m_pstTaskParsMultipleModes=NULL;
	m_pstTaskParsSingleMode=NULL;
	m_psnSingleTaskSignal=NULL;

}

CTaskDesignView::~CTaskDesignView()
{
	if(m_pstTaskParsMultipleModes)
	{
		if(m_pstTaskParsMultipleModes->lpDesignMatrix)
		{
			delete m_pstTaskParsMultipleModes->lpDesignMatrix;
			m_pstTaskParsMultipleModes->lpDesignMatrix=NULL;
		}
		if(m_pstTaskParsMultipleModes->pfAuditoryTask)
		{
			delete m_pstTaskParsMultipleModes->pfAuditoryTask;
			m_pstTaskParsMultipleModes->pfAuditoryTask=NULL;
		}
		if(m_pstTaskParsMultipleModes->pfVisualTask)
		{
			delete m_pstTaskParsMultipleModes->pfVisualTask;
			m_pstTaskParsMultipleModes->pfVisualTask=NULL;
		}
		if(m_pstTaskParsMultipleModes->pfSomatosensoryTask)
		{
			delete m_pstTaskParsMultipleModes->pfSomatosensoryTask;
			m_pstTaskParsMultipleModes->pfSomatosensoryTask=NULL;
		}

		delete m_pstTaskParsMultipleModes;
		m_pstTaskParsMultipleModes=NULL;
	}
	if(m_psnSingleTaskSignal)
	{
		delete m_psnSingleTaskSignal;
		m_psnSingleTaskSignal=NULL;
	}
	if(m_pstTaskParsSingleMode)
	{
		if(m_pstTaskParsSingleMode->lpDesignMatrix)
		{
			delete m_pstTaskParsSingleMode->lpDesignMatrix;
			m_pstTaskParsSingleMode->lpDesignMatrix=NULL;
		}
		if(m_pstTaskParsSingleMode->pfTask)
		{
			delete m_pstTaskParsSingleMode->pfTask;
			m_pstTaskParsSingleMode->pfTask=NULL;
		}
		delete m_pstTaskParsSingleMode;
		m_pstTaskParsSingleMode=NULL;
	}
}

BOOL CTaskDesignView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CTaskDesignView drawing

void CTaskDesignView::OnDraw(CDC* pDC)
{
	CTaskDesignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if(pDoc->m_bIsOpenSingleMode==TRUE)
	{
		DrawSingleTaskMode(pDC);
	}
	else if(pDoc->m_bIsOpenMultipleModes==TRUE)
	{
		DrawMultipleTasksMode(pDC);
	}
	


}

void CTaskDesignView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CTaskDesignView printing

BOOL CTaskDesignView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTaskDesignView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTaskDesignView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTaskDesignView diagnostics

#ifdef _DEBUG
void CTaskDesignView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTaskDesignView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTaskDesignDoc* CTaskDesignView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaskDesignDoc)));
	return (CTaskDesignDoc*)m_pDocument;
}
#endif //_DEBUG


// CTaskDesignView message handlers

void CTaskDesignView::DrawSingleTaskMode(CDC *pDC)
{
	CTaskDesignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	BYTE Val;
	short int snLenDesignMatrix,i,j,k,m,x1,x2,y1,y2,snLenSample,snLenBlock;
	unsigned int unTotalLength,n;
	float fMax,fVal;
	CRect rc;
	CString str;
	CPen NewPen,NewPen1,NewPen2;
	struct TASKPARSSINGLEMODE *pstTaskParsSingleMode;
	pstTaskParsSingleMode=pDoc->m_pTaskConstruction->m_pstTaskParsSingleMode;


	str=_T("Design matrix");
	GetClientRect(&rc);
	SetTextColor(pDC->GetSafeHdc(),RGB(0,0,0));
	TextOut(pDC->GetSafeHdc(),rc.left+5,rc.top+5,str,str.GetLength());

	NewPen.CreatePen(PS_SOLID ,1,RGB(0,0,0)); 
	CPen* poldPen=pDC->SelectObject(&NewPen);
	pDC->MoveTo(rc.left+15,rc.top+20);
	pDC->LineTo(rc.left+15,rc.Height()/3);
	pDC->MoveTo(rc.left+15,rc.Height()/3);
	pDC->LineTo(rc.right-15,rc.Height()/3);
	pDC->SelectObject(poldPen);
	NewPen.DeleteObject();

	NewPen1.CreatePen(PS_SOLID ,1,RGB(0,0,255)); 
	CPen *pOldPen1=pDC->SelectObject(&NewPen1);
	snLenDesignMatrix=pstTaskParsSingleMode->snLengthDesignMatrix;
	k=(rc.Width()-50)/snLenDesignMatrix;
	n=rc.Height()/6;
	m=0;
	for(i=0;i<snLenDesignMatrix-1;i++)
	{
		Val=pstTaskParsSingleMode->lpDesignMatrix[i];
		if(Val==0)
		{
			pDC->MoveTo(rc.left+15+i*k,rc.Height()/3);
			pDC->LineTo(rc.left+15+(i+1)*k,rc.Height()/3);
		}
		else
		{
			m++;
			pDC->MoveTo(rc.left+15+i*k,rc.Height()/3);
			pDC->LineTo(rc.left+15+(i)*k,n);
			pDC->MoveTo(rc.left+15+i*k,n);
			pDC->LineTo(rc.left+15+(i+1)*k,n);
			pDC->MoveTo(rc.left+15+(i+1)*k,n);
			pDC->LineTo(rc.left+15+(i+1)*k,rc.Height()/3);

		}

	}
	str=_T("Task signal of single mode");
	SetTextColor(pDC->GetSafeHdc(),RGB(255,0,0));
	TextOut(pDC->GetSafeHdc(),rc.left+15,rc.Height()/3+20,str,str.GetLength());
	pDC->MoveTo(rc.left+15,rc.Height()/3+60);
	pDC->LineTo(rc.left+15,rc.Height()-20);
	pDC->MoveTo(rc.left+15,rc.Height()-20);
	pDC->LineTo(rc.right-15,rc.Height()-20);
	pDC->SelectObject(pOldPen1); 
	NewPen1.DeleteObject();
	// create pen
	NewPen2.CreatePen(PS_SOLID ,1,RGB(255,0,0)); 
	CPen *pOldPen2=pDC->SelectObject(&NewPen2);

	unTotalLength=pstTaskParsSingleMode->unTotalLength;
	snLenBlock=unTotalLength/m;         // the length of every block of task signal
	snLenSample=snLenBlock/pstTaskParsSingleMode->snLengthBlock; //the distance of task signal
	// obtain the maximum of task signal
	fMax=0;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsSingleMode->pfTask[n];
		if(fMax<fVal)
		{
			fMax=fVal;
		}
	}
	// standardize the task signal
	if(m_psnSingleTaskSignal)
	{
		delete m_psnSingleTaskSignal;
		m_psnSingleTaskSignal=NULL;
	}
	m_psnSingleTaskSignal=new short int[unTotalLength];
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsSingleMode->pfTask[n];
		m_psnSingleTaskSignal[n]=(short int)((fVal/(fMax+0.000000001))*(0.6*rc.Height()-100));		
	}
	// plot the waveform of the task signal	
	m=0;
	for(i=0;i<snLenDesignMatrix-1;i++)
	{
		Val=pstTaskParsSingleMode->lpDesignMatrix[i];
		if(Val==0)
		{
			pDC->MoveTo(rc.left+15+i*k,rc.Height()-20);
			pDC->LineTo(rc.left+15+(i+1)*k,rc.Height()-20);
		}
		else
		{
			for(j=0;j<pstTaskParsSingleMode->snLengthBlock-1;j++)
			{
				x1=(short)(rc.left+15+(i)*k+(j)*(k/(pstTaskParsSingleMode->snLengthBlock+0.000001)));
				x2=(short)(rc.left+15+(i)*k+(j+1)*(k/(pstTaskParsSingleMode->snLengthBlock+0.000001)));
				y1=rc.Height()-20-m_psnSingleTaskSignal[m*snLenBlock+j*snLenSample];
				y2=rc.Height()-20-m_psnSingleTaskSignal[m*snLenBlock+(j+1)*snLenSample];
				pDC->LineTo(x1,y1);
				pDC->MoveTo(x1,y1);
				pDC->LineTo(x2,y2);				
			}
			m++;
			pDC->MoveTo(x2,y2);
			pDC->LineTo(rc.left+15+(i+1)*k,rc.Height()-20);				


		}
	}
	pDC->SelectObject(pOldPen2);
	NewPen2.DeleteObject();
}
void CTaskDesignView::DrawMultipleTasksMode(CDC *pDC)
{
	CTaskDesignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	BYTE Val;
	short int snLenDesignMatrix,i,j,k,n1,n2,m,x1,x2,y1,y2,snLenSample,snLenBlock;
	unsigned int unTotalLength,n;
	float fMax,fVal;
	CRect rc;
	CString str;
	CPen NewPen,NewPen1,NewPen2,NewPen3,NewPen4;
	struct TASKPARSMULTIPLEMODES *pstTaskParsMultipleModes;
	pstTaskParsMultipleModes=pDoc->m_pTaskConstruction->m_pstTaskParsMultipleMode;
	str=_T("Design matrix");
	GetClientRect(&rc);
	SetTextColor(pDC->GetSafeHdc(),RGB(0,0,0));
	TextOut(pDC->GetSafeHdc(),rc.left+5,rc.top+5,str,str.GetLength());

	NewPen.CreatePen(PS_SOLID ,1,RGB(0,0,0)); 
	CPen* poldPen=pDC->SelectObject(&NewPen);
	pDC->MoveTo(rc.left+15,rc.top+20);
	pDC->LineTo(rc.left+15,rc.Height()/5);
	pDC->MoveTo(rc.left+15,rc.Height()/5);
	pDC->LineTo(rc.right-15,rc.Height()/5);
	pDC->SelectObject(poldPen);
	NewPen.DeleteObject();

	NewPen1.CreatePen(PS_SOLID ,1,RGB(0,0,255)); 
	CPen *pOldPen1=pDC->SelectObject(&NewPen1);
	snLenDesignMatrix=pstTaskParsMultipleModes->snLengthDesignMatrix;
	k=(rc.Width()-50)/snLenDesignMatrix;
	n=rc.Height()/15;
	m=0;
	for(i=0;i<snLenDesignMatrix-1;i++)
	{
		Val=pstTaskParsMultipleModes->lpDesignMatrix[i];
		if(Val==0)
		{
			pDC->MoveTo(rc.left+15+i*k,rc.Height()/5);
			pDC->LineTo(rc.left+15+(i+1)*k,rc.Height()/5);
		}
		else
		{
			m++;
			pDC->MoveTo(rc.left+15+i*k,rc.Height()/5);
			pDC->LineTo(rc.left+15+(i)*k,n);
			pDC->MoveTo(rc.left+15+i*k,n);
			pDC->LineTo(rc.left+15+(i+1)*k,n);
			pDC->MoveTo(rc.left+15+(i+1)*k,n);
			pDC->LineTo(rc.left+15+(i+1)*k,rc.Height()/5);

		}

	}
	//plot auditory task signal
	n1=(int)(0.4*rc.Height());
	str=_T("Auditory task signal");
	SetTextColor(pDC->GetSafeHdc(),RGB(255,0,0));
	TextOut(pDC->GetSafeHdc(),rc.left+15,rc.Height()/5+5,str,str.GetLength());
	pDC->MoveTo(rc.left+15,rc.Height()/5+20);
	pDC->LineTo(rc.left+15,n1);
	pDC->MoveTo(rc.left+15,n1);
	pDC->LineTo(rc.right-15,n1);

	// create pen
	NewPen2.CreatePen(PS_SOLID ,1,RGB(255,0,0)); 
	CPen *pOldPen2=pDC->SelectObject(&NewPen2);

	unTotalLength=pstTaskParsMultipleModes->unTotalLength;
	snLenBlock=unTotalLength/m;         // the length of every block of task signal
	snLenSample=snLenBlock/pstTaskParsMultipleModes->snLengthBlock; //the distance of task signal
	// obtain the maximum of task signal
	fMax=0;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsMultipleModes->pfAuditoryTask[n];
		if(fMax<fVal)
		{
			fMax=fVal;
		}
	}
	// standardize the task signal
	if(m_psnSingleTaskSignal)
	{
		delete m_psnSingleTaskSignal;
		m_psnSingleTaskSignal=NULL;
	}
	m_psnSingleTaskSignal=new short int[unTotalLength];
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsMultipleModes->pfAuditoryTask[n];
		m_psnSingleTaskSignal[n]=(short int)((fVal/(fMax+0.000000001))*((int)(0.16*rc.Height())));		
	}
	// plot the waveform of the task signal		
	m=0;
	for(i=0;i<snLenDesignMatrix-1;i++)
	{
		Val=pstTaskParsMultipleModes->lpDesignMatrix[i];
		if(Val==0)
		{
			pDC->MoveTo(rc.left+15+i*k,n1);
			pDC->LineTo(rc.left+15+(i+1)*k,n1);
		}
		else
		{
			for(j=0;j<pstTaskParsMultipleModes->snLengthBlock-1;j++)
			{
				x1=(short)(rc.left+15+(i)*k+(j)*(k/(pstTaskParsMultipleModes->snLengthBlock+0.000001)));
				x2=(short)(rc.left+15+(i)*k+(j+1)*(k/(pstTaskParsMultipleModes->snLengthBlock+0.000001)));
				y1=n1-m_psnSingleTaskSignal[m*snLenBlock+j*snLenSample];
				y2=n1-m_psnSingleTaskSignal[m*snLenBlock+(j+1)*snLenSample];
				pDC->LineTo(x1,y1);
				pDC->MoveTo(x1,y1);
				pDC->LineTo(x2,y2);				
			}
			m++;
			pDC->MoveTo(x2,y2);
			pDC->LineTo(rc.left+15+(i+1)*k,n1);				


		}
	}

	pDC->SelectObject(pOldPen2);
	NewPen2.DeleteObject();
	//plot visual task signal
	n2=(int)(0.6*rc.Height());
	str=_T("Visual task signal");
	SetTextColor(pDC->GetSafeHdc(),RGB(255,0,255));
	TextOut(pDC->GetSafeHdc(),rc.left+15,n1+5,str,str.GetLength());
	pDC->MoveTo(rc.left+15,n1+20);
	pDC->LineTo(rc.left+15,n2);
	pDC->MoveTo(rc.left+15,n2);
	pDC->LineTo(rc.right-15,n2);
	pDC->SelectObject(pOldPen1); 
	NewPen1.DeleteObject();
	// create pen
	NewPen3.CreatePen(PS_SOLID ,1,RGB(255,0,255)); 
	CPen *pOldPen3=pDC->SelectObject(&NewPen3);

	unTotalLength=pstTaskParsMultipleModes->unTotalLength;
	snLenBlock=unTotalLength/m;         // the length of every block of task signal
	snLenSample=snLenBlock/pstTaskParsMultipleModes->snLengthBlock; //the distance of task signal
	// obtain the maximum of task signal
	fMax=0;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsMultipleModes->pfVisualTask[n];
		if(fMax<fVal)
		{
			fMax=fVal;
		}
	}
	// standardize the task signal
	if(m_psnSingleTaskSignal)
	{
		delete m_psnSingleTaskSignal;
		m_psnSingleTaskSignal=NULL;
	}
	m_psnSingleTaskSignal=new short int[unTotalLength];
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsMultipleModes->pfVisualTask[n];
		m_psnSingleTaskSignal[n]=(short int)((fVal/(fMax+0.000000001))*(0.16*rc.Height()));		
	}
	// plot the waveform of the task signal	
	m=0;
	for(i=0;i<snLenDesignMatrix-1;i++)
	{
		Val=pstTaskParsMultipleModes->lpDesignMatrix[i];
		if(Val==0)
		{
			pDC->MoveTo(rc.left+15+i*k,n2);
			pDC->LineTo(rc.left+15+(i+1)*k,n2);
		}
		else
		{
			for(j=0;j<pstTaskParsMultipleModes->snLengthBlock-1;j++)
			{
				x1=(short)(rc.left+15+(i)*k+(j)*(k/(pstTaskParsMultipleModes->snLengthBlock+0.000001)));
				x2=(short)(rc.left+15+(i)*k+(j+1)*(k/(pstTaskParsMultipleModes->snLengthBlock+0.000001)));
				y1=n2-m_psnSingleTaskSignal[m*snLenBlock+j*snLenSample];
				y2=n2-m_psnSingleTaskSignal[m*snLenBlock+(j+1)*snLenSample];
				pDC->LineTo(x1,y1);
				pDC->MoveTo(x1,y1);
				pDC->LineTo(x2,y2);				
			}
			m++;
			pDC->MoveTo(x2,y2);
			pDC->LineTo(rc.left+15+(i+1)*k,n2);				


		}
	}
	//plot somatosensroy task signal
	n1=(int)(0.8*rc.Height());
	str=_T("Somatosensory task signal");
	SetTextColor(pDC->GetSafeHdc(),RGB(0,120,255));
	TextOut(pDC->GetSafeHdc(),rc.left+15,n2+5,str,str.GetLength());
	pDC->MoveTo(rc.left+15,n2+20);
	pDC->LineTo(rc.left+15,n1);
	pDC->MoveTo(rc.left+15,n1);
	pDC->LineTo(rc.right-15,n1);
	pDC->SelectObject(pOldPen3);
	NewPen3.DeleteObject();
	// create pen
	NewPen4.CreatePen(PS_SOLID ,1,RGB(0,120,255)); 
	CPen *pOldPen4=pDC->SelectObject(&NewPen4);

	unTotalLength=pstTaskParsMultipleModes->unTotalLength;
	snLenBlock=unTotalLength/m;         // the length of every block of task signal
	snLenSample=snLenBlock/pstTaskParsMultipleModes->snLengthBlock; //the distance of task signal
	// obtain the maximum of task signal
	fMax=0;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsMultipleModes->pfSomatosensoryTask[n];
		if(fMax<fVal)
		{
			fMax=fVal;
		}
	}
	// standardize the task signal
	if(m_psnSingleTaskSignal)
	{
		delete m_psnSingleTaskSignal;
		m_psnSingleTaskSignal=NULL;
	}
	m_psnSingleTaskSignal=new short int[unTotalLength];
	for(n=0;n<unTotalLength;n++)
	{
		fVal=pstTaskParsMultipleModes->pfSomatosensoryTask[n];
		m_psnSingleTaskSignal[n]=(short int)((fVal/(fMax+0.000000001))*((int)(0.16*rc.Height())));		
	}
	// plot the waveform of the task signal	
	m=0;
	for(i=0;i<snLenDesignMatrix-1;i++)
	{
		Val=pstTaskParsMultipleModes->lpDesignMatrix[i];
		if(Val==0)
		{
			pDC->MoveTo(rc.left+15+i*k,n1);
			pDC->LineTo(rc.left+15+(i+1)*k,n1);
		}
		else
		{
			for(j=0;j<pstTaskParsMultipleModes->snLengthBlock-1;j++)
			{
				x1=(short)(rc.left+15+(i)*k+(j)*(k/(pstTaskParsMultipleModes->snLengthBlock+0.000001)));
				x2=(short)(rc.left+15+(i)*k+(j+1)*(k/(pstTaskParsMultipleModes->snLengthBlock+0.000001)));
				y1=n1-m_psnSingleTaskSignal[m*snLenBlock+j*snLenSample];
				y2=n1-m_psnSingleTaskSignal[m*snLenBlock+(j+1)*snLenSample];
				pDC->LineTo(x1,y1);
				pDC->MoveTo(x1,y1);
				pDC->LineTo(x2,y2);				
			}
			m++;
			pDC->MoveTo(x2,y2);
			pDC->LineTo(rc.left+15+(i+1)*k,n1);				


		}
	}

	pDC->SelectObject(pOldPen4);
	NewPen4.DeleteObject();

}
