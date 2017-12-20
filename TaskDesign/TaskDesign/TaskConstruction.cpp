#include "StdAfx.h"
#include "TaskConstruction.h"
#include "math.h"

TaskConstruction::TaskConstruction(void)
{
	m_pstTaskParsSingleMode=NULL;
	m_pstTaskParsMultipleMode=NULL;
}

TaskConstruction::~TaskConstruction(void)
{
	uninitialize();
	uninitializeMultMode();
}
//// construction of the virtual auditor signal
//struct TASKPARSSINGLEMODE *TaskConstruction::TaskSingleModeConstruction(void)
//{
//	short        snLengthDesignMatrix;
//	unsigned int unTotalLength,unLenBlock,n,j,m;
//	float f0,fa,fMinimum,fVal,fMax;
//	double x;
//	float *pfTBuf;
//	// define a hemodynamic model
//	double dbHRF[27]={ -0.0022,-0.0249,0.0193,0.1230,0.1913,0.2116,0.1954,0.1536,0.1025,0.0575,0.0257,0.0065,-0.0047,-0.0119,-0.0171,-0.0205,
//		-0.0217,-0.0205,-0.0174,-0.0132,-0.0089,-0.0054,-0.0029,-0.0014,-0.0006,-0.0002,-0.0001};
//
//	// standardize the hemodynamic function
//	for(n=0;n<27;n++)
//	{
//		dbHRF[n]=(dbHRF[n]+0.0249);
//	}
//	// obtain design matrix
//	snLengthDesignMatrix=m_pstTaskParsSingleMode->snLengthDesignMatrix;
//	memset(m_pstTaskParsSingleMode->lpDesignMatrix,BYTE(0),snLengthDesignMatrix);
//	srand(3000); 
//	m=0;
//	for(n=0;n<(unsigned short)snLengthDesignMatrix;n++)
//	{
//		fVal=(float)(rand()/(RAND_MAX+0.000000001));
//		if(fVal>0.5)
//		{
//			m++;
//			m_pstTaskParsSingleMode->lpDesignMatrix[n]=1;
//		}		
//	}
//	// allocate the buffer that stores the task signal
//	unTotalLength=m_pstTaskParsSingleMode->unTotalLength;
//	m_pstTaskParsSingleMode->snLengthTaskBlock=(unsigned int)(unTotalLength/(m+0.000000001));
//
//	pfTBuf=new float[unTotalLength];
//	f0=500;             // the lowest frequency of task signal (Hz)
//	fa=4000;           // the highest frequency of task signal (Hz)
//
//	//generate a synthetic auditory stimulating signal,
//	//which is composed of the sum of sines and cosines of various frequencies,
//	//this frequency range is from 500Hz to 2000Hz. 
//	for(n=1;n<=unTotalLength;n++)
//	{
//		fVal=0;
//		for(x=1;x<=4;x+=0.1)
//		{
//			fVal+=(float)sin((n*2*PI*x*f0)/fa);
//		}
//		m_pstTaskParsSingleMode->pfTask[n-1]=fVal;
//	}
//	//obtain the minimum
//	fMinimum=1000;
//	for(n=0;n<unTotalLength;n++)
//	{
//		fVal=m_pstTaskParsSingleMode->pfTask[n];
//		if(fVal<fMinimum)
//		{
//			fMinimum=fVal;
//		}
//	}
//	// To ensure every value of the task signal is positive, standardize the task signal
//	for(n=0;n<unTotalLength;n++)
//	{
//		fVal=m_pstTaskParsSingleMode->pfTask[n];
//		m_pstTaskParsSingleMode->pfTask[n]=fVal+fabs(fMinimum);	
//	}
//	/// filter using HRF
//	memcpy(pfTBuf,m_pstTaskParsSingleMode->pfTask,sizeof(float)*unTotalLength);
//	for(n=0;n<unTotalLength;n++)
//	{
//		fVal=0.0;
//		if(n<27)
//		{
//			for(j=0;j<=n;j++)
//			{
//				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
//			}
//			m_pstTaskParsSingleMode->pfTask[n]=fVal;
//		}
//		else 
//		{
//			for(j=n-26;j<=n;j++)
//			{
//				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
//			}
//			m_pstTaskParsSingleMode->pfTask[n]=fVal;
//		}	
//
//	}
//	// obtain the maximum
//	fMax=0.0;
//	for(n=0;n<unTotalLength;n++)
//	{
//		fVal=m_pstTaskParsSingleMode->pfTask[n];
//		if(fMax<fVal)
//		{
//			fMax=fVal;
//		}		
//	}
//
//	// modulate the task signal by using a random method
//	unLenBlock=unTotalLength/m;
//	memcpy(pfTBuf,m_pstTaskParsSingleMode->pfTask,sizeof(float)*unTotalLength);
//	srand(1000);
//	for(n=0;n<unTotalLength-unLenBlock;n+=unLenBlock)
//	{
//		fVal=(float)(rand()/(RAND_MAX+0.000000001));
//		for(j=n;j<n+unLenBlock;j++)
//		{
//			m_pstTaskParsSingleMode->pfTask[j]=(float)(0.9*fVal*(pfTBuf[j]/(fMax+0.00000001)));
//
//		}
//	}
//	for(n=j;n<unTotalLength;n++)
//	{
//		m_pstTaskParsSingleMode->pfTask[n]=(float)(0.9*fVal*(pfTBuf[n]/(fMax+0.00000001)));
//		
//	}
//
//	//delete temporal buffer
//	delete pfTBuf;
//	pfTBuf=NULL;
//	return m_pstTaskParsSingleMode;
//}

// construction of the virtual visual signal
struct TASKPARSSINGLEMODE *TaskConstruction::TaskSingleModeConstruction(void)
{
	BOOL bIsOver;
	LPBITMAPINFOHEADER  lpBitmapInfoHeader;
	LPBITMAPFILEHEADER  lpBitmapFileHeader;
	LPBYTE              lpImage;
	short        snLengthDesignMatrix;
	unsigned int unTotalLength,n,j,m;
	int nWidth,nHeight,nSizeImage,nBytes,nBytesRow;

	float fVal,fMax;
	float *pfTBuf,*pfBuf;
	CString strVisualPath;
	CFile file;

	// define a hemodynamic model
	double dbHRF[27]={ -0.0022,-0.0249,0.0193,0.1230,0.1913,0.2116,0.1954,0.1536,0.1025,0.0575,0.0257,0.0065,-0.0047,-0.0119,-0.0171,-0.0205,
		-0.0217,-0.0205,-0.0174,-0.0132,-0.0089,-0.0054,-0.0029,-0.0014,-0.0006,-0.0002,-0.0001};

	// standardize the hemodynamic function
	for(n=0;n<27;n++)
	{
		dbHRF[n]=(dbHRF[n]+0.0249);
	}
	// obtain design matrix
	snLengthDesignMatrix=m_pstTaskParsSingleMode->snLengthDesignMatrix;
	memset(m_pstTaskParsSingleMode->lpDesignMatrix,BYTE(0),snLengthDesignMatrix);
	srand(3000); 
	m=0;
	for(n=0;n<(unsigned short)snLengthDesignMatrix;n++)
	{
		fVal=(float)(rand()/(RAND_MAX+0.000000001));
		if(fVal>0.5)
		{
			m++;
			m_pstTaskParsSingleMode->lpDesignMatrix[n]=1;
		}		
	}
	// allocate the buffer that stores the task signal
	unTotalLength=m_pstTaskParsSingleMode->unTotalLength;
	m_pstTaskParsSingleMode->snLengthTaskBlock=(unsigned int)(unTotalLength/(m+0.000000001));

	pfTBuf=new float[unTotalLength];
	// obtain visual task signal
	strVisualPath=_T("D:\\VDB\\Task\\visual.bmp");
	if(!file.Open(strVisualPath,CFile::modeRead,NULL))
	{
		AfxMessageBox(_T("Opening file failed"));
		return NULL;
	}
	lpBitmapFileHeader=new BITMAPFILEHEADER[1];
	file.Read(lpBitmapFileHeader,sizeof(BITMAPFILEHEADER));
	lpBitmapInfoHeader=new BITMAPINFOHEADER[1];
	file.Read(lpBitmapInfoHeader,sizeof(BITMAPINFOHEADER));
	nWidth=lpBitmapInfoHeader->biWidth;nHeight=lpBitmapInfoHeader->biHeight;
	nBytesRow=BYTESROW(nWidth);
	nSizeImage=nHeight*nBytesRow;
	lpImage=new BYTE[nSizeImage];
	file.Read(lpImage,nSizeImage);
	file.Close();
	pfBuf=m_pstTaskParsSingleMode->pfTask;
	m=0;
	bIsOver=FALSE;
	for(n=0;n<(unsigned int)nHeight;n++)
	{
		nBytes=n*nBytesRow;
		for(j=0;j<(unsigned int)nWidth;j++)
		{
			*(pfBuf)=lpImage[3*j+nBytes];
			pfBuf++;
			m++;
			if(m>=unTotalLength)
			{
				bIsOver=TRUE;
				break;
			}
		}
		if(bIsOver==TRUE)
		{
			break;
		}
	}	
	/// filter using HRF
	memcpy(pfTBuf,m_pstTaskParsSingleMode->pfTask,sizeof(float)*unTotalLength);
	for(n=0;n<unTotalLength;n++)
	{
		fVal=0.0;
		if(n<27)
		{
			for(j=0;j<=n;j++)
			{
				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
			}
			m_pstTaskParsSingleMode->pfTask[n]=fVal;
		}
		else 
		{
			for(j=n-26;j<=n;j++)
			{
				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
			}
			m_pstTaskParsSingleMode->pfTask[n]=fVal;
		}	

	}
	// obtain the maximum
	fMax=0.0;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=m_pstTaskParsSingleMode->pfTask[n];
		if(fMax<fVal)
		{
			fMax=fVal;
		}		
	}

	// standardize the visual task signal 
	memcpy(pfTBuf,m_pstTaskParsSingleMode->pfTask,sizeof(float)*unTotalLength);
	for(n=0;n<unTotalLength;n++)
	{
		m_pstTaskParsSingleMode->pfTask[n]=(float)(5*(pfTBuf[n]/(fMax+0.00000001)));		
	}

	//delete temporal buffer
	delete lpImage;
	lpImage=NULL;
	delete lpBitmapInfoHeader;
	lpBitmapInfoHeader=NULL;
	delete lpBitmapFileHeader;
	lpBitmapFileHeader=NULL;
	delete pfTBuf;
	pfTBuf=NULL;
	return m_pstTaskParsSingleMode;
}

int TaskConstruction::InitializeTaskParsSingleMode(unsigned int unTotalLengthIn )
{
	short snLengthDesignMatrix;
	if(m_pstTaskParsSingleMode==NULL)
	{
		m_pstTaskParsSingleMode=new struct TASKPARSSINGLEMODE[1];
		m_pstTaskParsSingleMode->snLengthBlock=4;
		m_pstTaskParsSingleMode->nSamplePoints=400;
		m_pstTaskParsSingleMode->unTotalLength=unTotalLengthIn;
		snLengthDesignMatrix=m_pstTaskParsSingleMode->nSamplePoints/m_pstTaskParsSingleMode->snLengthBlock;
		m_pstTaskParsSingleMode->snLengthDesignMatrix=snLengthDesignMatrix;
		if(m_pstTaskParsSingleMode->lpDesignMatrix==NULL)
		{
			m_pstTaskParsSingleMode->lpDesignMatrix=new BYTE [snLengthDesignMatrix];
		}
		if(m_pstTaskParsSingleMode->pfTask==NULL)
		{
			m_pstTaskParsSingleMode->pfTask=new float[m_pstTaskParsSingleMode->unTotalLength];
		}
	}
	return TRUE;
}

int TaskConstruction::uninitialize(void)
{
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
	return TRUE;
}



int TaskConstruction::InitializeTaskParsMultipleMode(unsigned int unTotalLengthIn )
{
	short snLengthDesignMatrix;
	if(m_pstTaskParsMultipleMode==NULL)
	{
		m_pstTaskParsMultipleMode=new struct TASKPARSMULTIPLEMODES[1];
		m_pstTaskParsMultipleMode->snLengthBlock=4;
		m_pstTaskParsMultipleMode->nSamplePoints=400;
		m_pstTaskParsMultipleMode->unTotalLength=unTotalLengthIn;
		snLengthDesignMatrix=m_pstTaskParsMultipleMode->nSamplePoints/m_pstTaskParsMultipleMode->snLengthBlock;
		m_pstTaskParsMultipleMode->snLengthDesignMatrix=snLengthDesignMatrix;
		if(m_pstTaskParsMultipleMode->lpDesignMatrix==NULL)
		{
			m_pstTaskParsMultipleMode->lpDesignMatrix=new BYTE [snLengthDesignMatrix];
		}
		if(m_pstTaskParsMultipleMode->pfAuditoryTask==NULL)
		{
			m_pstTaskParsMultipleMode->pfAuditoryTask=new float[m_pstTaskParsMultipleMode->unTotalLength];
		}
		if(m_pstTaskParsMultipleMode->pfVisualTask==NULL)
		{
			m_pstTaskParsMultipleMode->pfVisualTask=new float[m_pstTaskParsMultipleMode->unTotalLength];
		}
		if(m_pstTaskParsMultipleMode->pfSomatosensoryTask==NULL)
		{
			m_pstTaskParsMultipleMode->pfSomatosensoryTask=new float[m_pstTaskParsMultipleMode->unTotalLength];
		}

	}
	return TRUE;
}
int TaskConstruction::uninitializeMultMode(void)
{
	if(m_pstTaskParsMultipleMode)
	{
		if(m_pstTaskParsMultipleMode->lpDesignMatrix)
		{
			delete m_pstTaskParsMultipleMode->lpDesignMatrix;
			m_pstTaskParsMultipleMode->lpDesignMatrix=NULL;
		}
		if(m_pstTaskParsMultipleMode->pfAuditoryTask)
		{
			delete m_pstTaskParsMultipleMode->pfAuditoryTask;
			m_pstTaskParsMultipleMode->pfAuditoryTask=NULL;
		}
		if(m_pstTaskParsMultipleMode->pfVisualTask)
		{
			delete m_pstTaskParsMultipleMode->pfVisualTask;
			m_pstTaskParsMultipleMode->pfVisualTask=NULL;
		}
		if(m_pstTaskParsMultipleMode->pfSomatosensoryTask)
		{
			delete m_pstTaskParsMultipleMode->pfSomatosensoryTask;
			m_pstTaskParsMultipleMode->pfSomatosensoryTask=NULL;
		}
		delete m_pstTaskParsMultipleMode;
		m_pstTaskParsMultipleMode=NULL;
	}
	return TRUE;
}

struct TASKPARSMULTIPLEMODES *TaskConstruction::TaskMultipleModesConstruction(void)
{
	BOOL bIsOver;
	LPBITMAPINFOHEADER  lpBitmapInfoHeader;
	LPBITMAPFILEHEADER  lpBitmapFileHeader;
	LPBYTE              lpImage;
	short        snLengthDesignMatrix;
	int          nSizeImage,nBytesRow,nBytes;
	unsigned int unTotalLength,unLenBlock,n,j,m,nWidth,nHeight;
	float f0,fa,fMinimum,fVal,fMax;
	double x;
	float *pfTBuf,*pfBuf;
	CString strVisualPath;
	CFile file;
	// define a hemodynamic model
	double dbHRF[27]={ -0.0022,-0.0249,0.0193,0.1230,0.1913,0.2116,0.1954,0.1536,0.1025,0.0575,0.0257,0.0065,-0.0047,-0.0119,-0.0171,-0.0205,
		-0.0217,-0.0205,-0.0174,-0.0132,-0.0089,-0.0054,-0.0029,-0.0014,-0.0006,-0.0002,-0.0001};

	// standardize the hemodynamic function
	for(n=0;n<27;n++)
	{
		dbHRF[n]=(dbHRF[n]+0.0249);
	}
	// obtain design matrix
	snLengthDesignMatrix=m_pstTaskParsMultipleMode->snLengthDesignMatrix;
	memset(m_pstTaskParsMultipleMode->lpDesignMatrix,BYTE(0),snLengthDesignMatrix);
	srand(3000); 
	m=0;
	for(n=0;n<(unsigned short)snLengthDesignMatrix;n++)
	{
		fVal=(float)(rand()/(RAND_MAX+0.000000001));
		if(fVal>0.5)
		{
			m++;
			m_pstTaskParsMultipleMode->lpDesignMatrix[n]=1;
		}		
	}
	// allocate the buffer that stores the auditory task signal
	unTotalLength=m_pstTaskParsMultipleMode->unTotalLength;
	m_pstTaskParsMultipleMode->snLengthTaskBlock=(unsigned int)(unTotalLength/(m+0.000000001));

	pfTBuf=new float[unTotalLength];
	f0=500;             // the lowest frequency of task signal (Hz)
	fa=4000;           // the highest frequency of task signal (Hz)

	//generate a synthetic auditory stimulating signal,
	//which is composed of the sum of sines and cosines of various frequencies,
	//this frequency range is from 500Hz to 2000Hz. 
	for(n=1;n<=unTotalLength;n++)
	{
		fVal=0;
		for(x=1;x<=4;x+=0.1)
		{
			fVal+=(float)sin((n*2*PI*x*f0)/fa);
		}
		m_pstTaskParsMultipleMode->pfAuditoryTask[n-1]=fVal;
	}
	//obtain the minimum
	fMinimum=1000;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=m_pstTaskParsMultipleMode->pfAuditoryTask[n];
		if(fVal<fMinimum)
		{
			fMinimum=fVal;
		}
	}
	// To ensure every value of the auditory task signal is positive, standardize the auditory task signal
	for(n=0;n<unTotalLength;n++)
	{
		fVal=m_pstTaskParsMultipleMode->pfAuditoryTask[n];
		m_pstTaskParsMultipleMode->pfAuditoryTask[n]=fVal+fabs(fMinimum);	
	}
	/// filter using HRF
	memcpy(pfTBuf,m_pstTaskParsMultipleMode->pfAuditoryTask,sizeof(float)*unTotalLength);
	for(n=0;n<unTotalLength;n++)
	{
		fVal=0.0;
		if(n<27)
		{
			for(j=0;j<=n;j++)
			{
				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
			}
			m_pstTaskParsMultipleMode->pfAuditoryTask[n]=fVal;
		}
		else 
		{
			for(j=n-26;j<=n;j++)
			{
				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
			}
			m_pstTaskParsMultipleMode->pfAuditoryTask[n]=fVal;
		}	

	}
	// obtain the maximum
	fMax=0.0;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=m_pstTaskParsMultipleMode->pfAuditoryTask[n];
		if(fMax<fVal)
		{
			fMax=fVal;
		}		
	}

	// modulate the auditory task signal by using a random method
	unLenBlock=unTotalLength/m;
	memcpy(pfTBuf,m_pstTaskParsMultipleMode->pfAuditoryTask,sizeof(float)*unTotalLength);
	srand(1000);
	for(n=0;n<unTotalLength-unLenBlock;n+=unLenBlock)
	{
		fVal=(float)(rand()/(RAND_MAX+0.000000001));
		for(j=n;j<n+unLenBlock;j++)
		{
			m_pstTaskParsMultipleMode->pfAuditoryTask[j]=(float)(5*fVal*(pfTBuf[j]/(fMax+0.00000001)));

		}
	}
	for(n=j;n<unTotalLength;n++)
	{
		m_pstTaskParsMultipleMode->pfAuditoryTask[n]=(float)(5*fVal*(pfTBuf[n]/(fMax+0.00000001)));

	}
	// obtain visual task signal
	strVisualPath=_T("D:\\VDB\\Task\\visual.bmp");
	if(!file.Open(strVisualPath,CFile::modeRead,NULL))
	{
		AfxMessageBox(_T("Opening file failed"));
		return NULL;
	}
	lpBitmapFileHeader=new BITMAPFILEHEADER[1];
	file.Read(lpBitmapFileHeader,sizeof(BITMAPFILEHEADER));
	lpBitmapInfoHeader=new BITMAPINFOHEADER[1];
	file.Read(lpBitmapInfoHeader,sizeof(BITMAPINFOHEADER));
	nWidth=lpBitmapInfoHeader->biWidth;nHeight=lpBitmapInfoHeader->biHeight;
	nBytesRow=BYTESROW(nWidth);
	nSizeImage=nHeight*nBytesRow;
	lpImage=new BYTE[nSizeImage];
	file.Read(lpImage,nSizeImage);
	file.Close();
	pfBuf=m_pstTaskParsMultipleMode->pfVisualTask;
	m=0;
	bIsOver=FALSE;
	for(n=0;n<nHeight;n++)
	{
		nBytes=n*nBytesRow;
		for(j=0;j<nWidth;j++)
		{
			*(pfBuf)=lpImage[3*j+nBytes];
			pfBuf++;
			m++;
			if(m>=unTotalLength)
			{
				bIsOver=TRUE;
				break;
			}
		}
		if(bIsOver==TRUE)
		{
			break;
		}
	}	
	/// filter using HRF
	memcpy(pfTBuf,m_pstTaskParsMultipleMode->pfVisualTask,sizeof(float)*unTotalLength);
	for(n=0;n<unTotalLength;n++)
	{
		fVal=0.0;
		if(n<27)
		{
			for(j=0;j<=n;j++)
			{
				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
			}
			m_pstTaskParsMultipleMode->pfVisualTask[n]=fVal;
		}
		else 
		{
			for(j=n-26;j<=n;j++)
			{
				fVal+=(float)(dbHRF[n-j]*pfTBuf[j]);
			}
			m_pstTaskParsMultipleMode->pfVisualTask[n]=fVal;
		}	

	}
	// obtain the maximum
	fMax=0.0;
	for(n=0;n<unTotalLength;n++)
	{
		fVal=m_pstTaskParsMultipleMode->pfVisualTask[n];
		if(fMax<fVal)
		{
			fMax=fVal;
		}		
	}

	// standardize the visual task signal 
	memcpy(pfTBuf,m_pstTaskParsMultipleMode->pfVisualTask,sizeof(float)*unTotalLength);
	for(n=0;n<unTotalLength;n++)
	{
		m_pstTaskParsMultipleMode->pfVisualTask[n]=(float)(5*(pfTBuf[n]/(fMax+0.00000001)));		
	}
	// obtain somatosensory signal
	memcpy(m_pstTaskParsMultipleMode->pfSomatosensoryTask,m_pstTaskParsMultipleMode->pfVisualTask,sizeof(float)*unTotalLength);

	//delete temporal buffer
	delete lpImage;
	lpImage=NULL;
	delete lpBitmapInfoHeader;
	lpBitmapInfoHeader=NULL;
	delete lpBitmapFileHeader;
	lpBitmapFileHeader=NULL;
	delete pfTBuf;
	pfTBuf=NULL;
	return m_pstTaskParsMultipleMode;
}

int TaskConstruction::ReadTaskSignalsMultipleModes(void)
{	
	CString  strTaskFilePath;
	CFile    file;
	struct MULTITASKPARSHDR stMultitaskParsHdr;
	if(m_pstTaskParsMultipleMode==NULL)
	{
		m_pstTaskParsMultipleMode=new struct TASKPARSMULTIPLEMODES[1];
	}
	// define the directory that store the task signal of single mode
	strTaskFilePath=_T("D:\\VDB\\Task\\TaskSignalMultipleModes.dat");
	// create the task signal file to write
	if(!file.Open(strTaskFilePath,CFile::modeRead,NULL))
	{
		AfxMessageBox(_T("opening the task signal file failed"));
		return FALSE;
	}
	file.Read(&stMultitaskParsHdr,sizeof(struct MULTITASKPARSHDR));
	m_pstTaskParsMultipleMode->nSamplePoints=stMultitaskParsHdr.nSamplePoints;
	m_pstTaskParsMultipleMode->NumTasks=stMultitaskParsHdr.NumTasks;
	m_pstTaskParsMultipleMode->snLengthBlock=stMultitaskParsHdr.snLengthBlock;
	m_pstTaskParsMultipleMode->snLengthDesignMatrix=stMultitaskParsHdr.snLengthDesignMatrix;
	m_pstTaskParsMultipleMode->snLengthTaskBlock=stMultitaskParsHdr.snLengthTaskBlock;
	m_pstTaskParsMultipleMode->unTotalLength=stMultitaskParsHdr.unTotalLength;
	if(m_pstTaskParsMultipleMode->lpDesignMatrix)
	{
		delete m_pstTaskParsMultipleMode->lpDesignMatrix;
		m_pstTaskParsMultipleMode->lpDesignMatrix=NULL;
	}
	m_pstTaskParsMultipleMode->lpDesignMatrix=new BYTE[m_pstTaskParsMultipleMode->snLengthDesignMatrix];
	file.Read(m_pstTaskParsMultipleMode->lpDesignMatrix,m_pstTaskParsMultipleMode->snLengthDesignMatrix);
	if(m_pstTaskParsMultipleMode->pfAuditoryTask)
	{
		delete m_pstTaskParsMultipleMode->pfAuditoryTask;
		m_pstTaskParsMultipleMode->pfAuditoryTask=NULL;
	}
	m_pstTaskParsMultipleMode->pfAuditoryTask=new float[m_pstTaskParsMultipleMode->unTotalLength];
	file.Read(m_pstTaskParsMultipleMode->pfAuditoryTask,sizeof(float)*m_pstTaskParsMultipleMode->unTotalLength);
	if(m_pstTaskParsMultipleMode->pfVisualTask)
	{
		delete m_pstTaskParsMultipleMode->pfVisualTask;
		m_pstTaskParsMultipleMode->pfVisualTask=NULL;
	}
	m_pstTaskParsMultipleMode->pfVisualTask=new float[m_pstTaskParsMultipleMode->unTotalLength];
	file.Read(m_pstTaskParsMultipleMode->pfVisualTask,sizeof(float)*m_pstTaskParsMultipleMode->unTotalLength);
	if(m_pstTaskParsMultipleMode->pfSomatosensoryTask)
	{
		delete m_pstTaskParsMultipleMode->pfSomatosensoryTask;
		m_pstTaskParsMultipleMode->pfSomatosensoryTask=NULL;
	}
	m_pstTaskParsMultipleMode->pfSomatosensoryTask=new float[m_pstTaskParsMultipleMode->unTotalLength];
	file.Read(m_pstTaskParsMultipleMode->pfSomatosensoryTask,sizeof(float)*m_pstTaskParsMultipleMode->unTotalLength);
	file.Close();	
	return TRUE;
}
int TaskConstruction::ReadTaskSignalSingleMode(void)
{	
	CString  strTaskFilePath;
	CFile    file;
	struct SINGLETASKPARSHDR stSingleTaskParsHdr; 
	if(m_pstTaskParsSingleMode==NULL)
	{
		m_pstTaskParsSingleMode=new struct TASKPARSSINGLEMODE[1];
	}
	// define the directory that store the task signal of single mode
	strTaskFilePath=_T("D:\\VDB\\Task\\TaskSignalSingleMode.dat");
	// create the task signal file to write
	if(!file.Open(strTaskFilePath,CFile::modeRead,NULL))
	{
		AfxMessageBox(_T("opening the task signal file failed"));
		return FALSE;
	}
	file.Read(&stSingleTaskParsHdr,sizeof(struct SINGLETASKPARSHDR));
	m_pstTaskParsSingleMode->nSamplePoints=stSingleTaskParsHdr.nSamplePoints;
	m_pstTaskParsSingleMode->snLengthBlock=stSingleTaskParsHdr.snLengthBlock;
	m_pstTaskParsSingleMode->snLengthDesignMatrix=stSingleTaskParsHdr.snLengthDesignMatrix;
	m_pstTaskParsSingleMode->snLengthTaskBlock=stSingleTaskParsHdr.snLengthTaskBlock;
	m_pstTaskParsSingleMode->unTotalLength=stSingleTaskParsHdr.unTotalLength;
	if(m_pstTaskParsSingleMode->lpDesignMatrix)
	{
		delete m_pstTaskParsSingleMode->lpDesignMatrix;
		m_pstTaskParsSingleMode->lpDesignMatrix=NULL;
	}
	m_pstTaskParsSingleMode->lpDesignMatrix=new BYTE[m_pstTaskParsSingleMode->snLengthDesignMatrix];
	file.Read(m_pstTaskParsSingleMode->lpDesignMatrix,m_pstTaskParsSingleMode->snLengthDesignMatrix);
	if(m_pstTaskParsSingleMode->pfTask)
	{
		delete m_pstTaskParsSingleMode->pfTask;
		m_pstTaskParsSingleMode->pfTask=NULL;
	}
	m_pstTaskParsSingleMode->pfTask=new float[m_pstTaskParsSingleMode->unTotalLength];
	file.Read(m_pstTaskParsSingleMode->pfTask,sizeof(float)*m_pstTaskParsSingleMode->unTotalLength);
	file.Close();
	return TRUE;
}

