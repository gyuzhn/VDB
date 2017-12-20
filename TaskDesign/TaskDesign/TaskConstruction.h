//TaskConstruction.h
#pragma once
// define parameters of task
#define  PI 3.1415926
#define BYTESROW(n)    ((3*n+3)/4)*4


struct SINGLETASKPARSHDR
{
	short   snLengthDesignMatrix; // the length of design matrix
	short   snLengthTaskBlock;   // the length of every task block (the length of time series of every block)
	int   nSamplePoints;   //total number of sample points
	unsigned int   unTotalLength;    //total length of task signal
	short int snLengthBlock; // sample points of every block
	struct SINGLETASKPARSHDR()
	{
		snLengthDesignMatrix=250;
		snLengthTaskBlock=4;
		nSamplePoints=1000;
		unTotalLength=4000;
		snLengthBlock=4;
	}
};
struct TASKPARSSINGLEMODE
{
	short   snLengthDesignMatrix; // the length of design matrix
	short   snLengthTaskBlock;   // the length of every task block (the length of time series of every block)
	int   nSamplePoints;   //total number of sample points
	unsigned int   unTotalLength;    //total length of task signal
	short int snLengthBlock; // sample points of every block
	LPBYTE lpDesignMatrix;     // design matrix
	float *pfTask;	        // the pointer of buffer that stores the task signal 
	struct TASKPARSSINGLEMODE()
	{
		snLengthDesignMatrix=250;
		snLengthTaskBlock=4;
		nSamplePoints=1000;
		unTotalLength=4000;
		snLengthBlock=4;
		lpDesignMatrix=NULL;
		pfTask=NULL;
	}
};
struct MULTITASKPARSHDR
{
	BYTE  NumTasks; // total number of tasks (NumTasks>=2. "NumTasks=2" indicates that the stimulating signals contain auditory and visual signals,
	// and "NumTasks=3" indicates that the stimulating signals contain auditory,visual,and somatosensory signals)
	short   snLengthDesignMatrix; // the length of design matrix
	short   snLengthTaskBlock;   // the length of every task block (the length of time series of every block)
	int   nSamplePoints;   //total number of sample points
	unsigned int   unTotalLength;    //total length of task signal
	short int snLengthBlock; // sample points of every block
	struct MULTITASKPARSHDR()
	{
		NumTasks=3;
		snLengthDesignMatrix=250;
		snLengthTaskBlock=4;
		nSamplePoints=1000;
		unTotalLength=4000;
		snLengthBlock=4;
	}
};

struct TASKPARSMULTIPLEMODES
{
	BYTE  NumTasks; // total number of tasks (NumTasks>=2. "NumTasks=2" indicates that the stimulating signals contain auditory and visual signals,
	                // and "NumTasks=3" indicates that the stimulating signals contain auditory,visual,and somatosensory signals)
	short   snLengthDesignMatrix; // the length of design matrix
	short   snLengthTaskBlock;   // the length of every task block (the length of time series of every block)
	int   nSamplePoints;   //total number of sample points
	unsigned int   unTotalLength;    //total length of task signal
	short int snLengthBlock; // sample points of every block
	LPBYTE lpDesignMatrix;     // design matrix
	float *pfAuditoryTask;  // the pointer of buffer that stores the auditory task signal 
	float *pfVisualTask;   // the pointer of buffer that stores the visual task signal 
	float *pfSomatosensoryTask; // the pointer of buffer that stores the somatosensory task signal 	        
	struct TASKPARSMULTIPLEMODES()
	{
		NumTasks=3;
		snLengthDesignMatrix=250;
		snLengthTaskBlock=4;
		nSamplePoints=1000;
		unTotalLength=4000;
		snLengthBlock=4;
		lpDesignMatrix=NULL;
		pfAuditoryTask=NULL;
		pfVisualTask=NULL;
		pfSomatosensoryTask=NULL;
	}

};

class TaskConstruction
{
public:
	TaskConstruction(void);
	~TaskConstruction(void);
public:
	struct TASKPARSSINGLEMODE *m_pstTaskParsSingleMode; // the pointer of buffer that stores the task of single mode
	struct TASKPARSMULTIPLEMODES *m_pstTaskParsMultipleMode; // the pointer of buffer that stores the task of multiple modes

public:
	// construct task of single mode
	struct TASKPARSSINGLEMODE *TaskSingleModeConstruction(void);
	// construct task of multiple modes
	struct TASKPARSMULTIPLEMODES *TaskMultipleModesConstruction(void);
public:
	// initialize task parameters of single mode
	int InitializeTaskParsSingleMode(unsigned int unTotalLengthIn);
	// delete the buffers that were allocated to construct the task of single mode
	int uninitialize(void);
	// initialize task parameters of multiple modes
	int InitializeTaskParsMultipleMode(unsigned int unTotalLengthIn);
	// delete the buffers that were allocated to construct the task of multiple modes
	int uninitializeMultMode(void);
	// Read constructed task signals of multiple modes that were stored in the directory: D:\\VDB\\Task\\TaskSignalMultipleModes.dat
	int ReadTaskSignalsMultipleModes(void);
	// Read constructed task signal of single modes that was stored in the directory: D:\\VDB\\Task\\TTaskSignalSingleMode.dat
	int ReadTaskSignalSingleMode(void);

};
