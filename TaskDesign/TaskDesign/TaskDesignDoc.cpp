// TaskDesignDoc.cpp : implementation of the CTaskDesignDoc class
//

#include "stdafx.h"
#include "TaskDesign.h"

#include "TaskDesignDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTaskDesignDoc

IMPLEMENT_DYNCREATE(CTaskDesignDoc, CDocument)

BEGIN_MESSAGE_MAP(CTaskDesignDoc, CDocument)
	ON_COMMAND(ID_TASKCONSTRUCTION_SINGLEMODE, &CTaskDesignDoc::OnTaskconstructionSinglemode)
	ON_COMMAND(ID_TASKCONSTRUCTION_MUTIPLEMODES, &CTaskDesignDoc::OnTaskConstructionMutipleModes)
	ON_COMMAND(ID_TASKCONSTRUCTION_MULTIPLETASKSDISPLAY, &CTaskDesignDoc::OnTaskConstructionMultipletasksDisplay)
	ON_COMMAND(ID_TASKCONSTRUCTION_SINGLETASKDISPLAY, &CTaskDesignDoc::OnTaskConstructionSingleTaskDisplay)
END_MESSAGE_MAP()


// CTaskDesignDoc construction/destruction

CTaskDesignDoc::CTaskDesignDoc()
{
	// TODO: add one-time construction code here
	m_pTaskConstruction=new class TaskConstruction;
	m_bIsOpenMultipleModes=FALSE;
	m_bIsOpenSingleMode=FALSE;


}

CTaskDesignDoc::~CTaskDesignDoc()
{
	if(m_pTaskConstruction)
	{
		delete m_pTaskConstruction;
		m_pTaskConstruction=NULL;
	}
	
}

BOOL CTaskDesignDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTaskDesignDoc serialization

void CTaskDesignDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTaskDesignDoc diagnostics

#ifdef _DEBUG
void CTaskDesignDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTaskDesignDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTaskDesignDoc commands

void CTaskDesignDoc::OnTaskconstructionSinglemode()
{
	// TODO: 在此添加命令处理程序代码
	unsigned int unLengthTimeSeries;
	CString  strTaskFilePath;
	CFile    file;
	struct SINGLETASKPARSHDR SingleTaskParsHdr;
	unLengthTimeSeries=4000;

	// define the directory that store the task signal of single mode
	strTaskFilePath=_T("D:\\VDB\\Task\\TaskSignalSingleMode.dat");
	// create the task signal file to write
	if(!file.Open(strTaskFilePath,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		AfxMessageBox(_T("Creating the task signal file failed"));
		return;
	}
	m_pTaskConstruction->InitializeTaskParsSingleMode(unLengthTimeSeries);
	m_pTaskConstruction->TaskSingleModeConstruction();
	SingleTaskParsHdr.nSamplePoints=m_pTaskConstruction->m_pstTaskParsSingleMode->nSamplePoints;
	SingleTaskParsHdr.snLengthBlock=m_pTaskConstruction->m_pstTaskParsSingleMode->snLengthBlock;
	SingleTaskParsHdr.snLengthDesignMatrix=m_pTaskConstruction->m_pstTaskParsSingleMode->snLengthDesignMatrix;
	SingleTaskParsHdr.snLengthTaskBlock=m_pTaskConstruction->m_pstTaskParsSingleMode->snLengthTaskBlock;
	SingleTaskParsHdr.unTotalLength=m_pTaskConstruction->m_pstTaskParsSingleMode->unTotalLength;
	file.Write(&SingleTaskParsHdr,sizeof(struct SINGLETASKPARSHDR));
	file.Write(m_pTaskConstruction->m_pstTaskParsSingleMode->lpDesignMatrix,m_pTaskConstruction->m_pstTaskParsSingleMode->snLengthDesignMatrix);
	file.Write(m_pTaskConstruction->m_pstTaskParsSingleMode->pfTask,sizeof(float)*m_pTaskConstruction->m_pstTaskParsSingleMode->unTotalLength);
	file.Close();
	m_pTaskConstruction->uninitialize();
	AfxMessageBox(_T("Task construction is complete"));
}

void CTaskDesignDoc::OnTaskConstructionMutipleModes()
{
	// TODO: 在此添加命令处理程序代码
	unsigned int unLengthTimeSeries;
	CString  strTaskFilePath;
	CFile    file;
	struct MULTITASKPARSHDR  stMultitaskParsHdr;

	unLengthTimeSeries=4000;

	// define the directory that store the task signal of single mode
	strTaskFilePath=_T("D:\\VDB\\Task\\TaskSignalMultipleModes.dat");
	// create the task signal file to write
	if(!file.Open(strTaskFilePath,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		AfxMessageBox(_T("Creating the task signal file failed"));
		return;
	}
	m_pTaskConstruction->InitializeTaskParsMultipleMode(unLengthTimeSeries);
	m_pTaskConstruction->TaskMultipleModesConstruction();
	stMultitaskParsHdr.nSamplePoints=m_pTaskConstruction->m_pstTaskParsMultipleMode->nSamplePoints;
	stMultitaskParsHdr.NumTasks=m_pTaskConstruction->m_pstTaskParsMultipleMode->NumTasks;
	stMultitaskParsHdr.snLengthBlock=m_pTaskConstruction->m_pstTaskParsMultipleMode->snLengthBlock;
	stMultitaskParsHdr.snLengthDesignMatrix=m_pTaskConstruction->m_pstTaskParsMultipleMode->snLengthDesignMatrix;
	stMultitaskParsHdr.snLengthTaskBlock=m_pTaskConstruction->m_pstTaskParsMultipleMode->snLengthTaskBlock;
	stMultitaskParsHdr.unTotalLength=m_pTaskConstruction->m_pstTaskParsMultipleMode->unTotalLength;
	file.Write(&stMultitaskParsHdr,sizeof(struct MULTITASKPARSHDR));
	file.Write(m_pTaskConstruction->m_pstTaskParsMultipleMode->lpDesignMatrix,m_pTaskConstruction->m_pstTaskParsMultipleMode->snLengthDesignMatrix);
	file.Write(m_pTaskConstruction->m_pstTaskParsMultipleMode->pfAuditoryTask,sizeof(float)*m_pTaskConstruction->m_pstTaskParsMultipleMode->unTotalLength);
	file.Write(m_pTaskConstruction->m_pstTaskParsMultipleMode->pfVisualTask,sizeof(float)*m_pTaskConstruction->m_pstTaskParsMultipleMode->unTotalLength);
	file.Write(m_pTaskConstruction->m_pstTaskParsMultipleMode->pfSomatosensoryTask,sizeof(float)*m_pTaskConstruction->m_pstTaskParsMultipleMode->unTotalLength);
	file.Close();
	m_pTaskConstruction->uninitializeMultMode();
	AfxMessageBox(_T("Task construction is complete"));
}

void CTaskDesignDoc::OnTaskConstructionMultipletasksDisplay()
{
	// TODO: 在此添加命令处理程序代码
	// Read task signals of multiple modes
	m_pTaskConstruction->ReadTaskSignalsMultipleModes();
	m_bIsOpenMultipleModes=TRUE;
	m_bIsOpenSingleMode=FALSE;
	UpdateAllViews(NULL);
}

void CTaskDesignDoc::OnTaskConstructionSingleTaskDisplay()
{
	// TODO: 在此添加命令处理程序代码
	// Read task signal of single mode
	m_pTaskConstruction->ReadTaskSignalSingleMode();
	m_bIsOpenMultipleModes=FALSE;
	m_bIsOpenSingleMode=TRUE;
	UpdateAllViews(NULL);

}
