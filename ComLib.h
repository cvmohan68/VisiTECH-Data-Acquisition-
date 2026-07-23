//////////////////////////////////////////////////////////////////////
// ComLib.h: interface for the CComLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMLIB_H__94DA81C3_B492_43FD_BE47_D4CAD5BED693__INCLUDED_)
#define AFX_COMLIB_H__94DA81C3_B492_43FD_BE47_D4CAD5BED693__INCLUDED_

#define  COM_SERIAL	0
#define  COM_TCPIP	1
#define  COM_OPC	2
#define  COM_OTHER	3


#define DllExport    __declspec(dllexport)
#define DllImport    __declspec(dllimport)


#include <tchar.h>


class CComLib
{

public:
virtual BOOL  SetCommunicationMedia(short nType) = 0;

virtual BOOL  SetTcpIpSettings(DWORD dwIpAddress, int nIpPortNo) = 0;
virtual BOOL  SetSerialSettings(short nPortNo, long nBaudRate, short nDataBits, short nStopBits, short nParity) = 0;
virtual BOOL  SetOpcSettings(COpcItemInfoList* opcList, TCHAR* szMachineName, TCHAR* szSrvName, TCHAR* szGrName, int nScanTime, float fDBPercent, int nOpcReadType) = 0;
	
virtual BOOL Connect() = 0;
virtual BOOL DisConnect() = 0;
virtual BOOL IsConnected() = 0;
virtual BOOL EnableRs485Mode() = 0;
	
virtual BOOL ReadAnalogROMultipleRegisters(short nSlaveAddr, int iStartAddr, short regAData[], short nNoRegisters) = 0;
virtual BOOL ReadAnalogROSingleRegister(short nSlaveAddr, int iAddr, short& regAData) = 0;
virtual BOOL ReadAnalogRWMultipleRegisters(short nSlaveAddr, int iStartAddr, short regAData[], short nNoRegisters) = 0;
virtual BOOL ReadAnalogRWSingleRegister(short nSlaveAddr, int iAddr, short& regAData) = 0;
	
virtual BOOL ReadDiscreteROMultipleRegisters(short nSlaveAddr, int iStartAddr, BOOL bitDData[], short nNoRegisters) = 0;
virtual BOOL ReadDiscreteROSingleRegister(short nSlaveAddr, int iAddr, BOOL& bitDData) = 0;
virtual BOOL ReadDiscreteRWMultipleRegisters(short nSlaveAddr, int iStartAddr, BOOL bitDData[], short nNoRegisters) = 0;
virtual BOOL ReadDiscreteRWSingleRegister(short nSlaveAddr, int iAddr, BOOL& bitDData) = 0;
	
virtual BOOL WriteAnalogRWMultipleRegisters(short nSlaveAddr, int iStartAddr, short regAData[], short nNoRegisters) = 0;
virtual BOOL WriteAnalogRWSingleRegister(short nSlaveAddr, int iAddr, short regAData) = 0;
virtual BOOL WriteDiscreteRWMultipleRegisters(short nSlaveAddr, int iStartAddr, BOOL bitDData[], short nNoRegisters) = 0;
virtual BOOL WriteDiscreteRWSingleRegister(short nSlaveAddr, int iAddr, BOOL bitDData) = 0;
	
virtual void SetTimeOut(int nTimeout) = 0;
virtual int GetTimeOut(void) = 0; 
virtual void SetPollDelay(int nPolldelay) = 0;
virtual int GetPollDelay(void) = 0;
virtual void SetRetryCount(short nRetrycount) = 0;
virtual short GetRetryCount(void) = 0; 
virtual unsigned long GetTransactionCount(void) = 0;
virtual CString GetLastProtocolError() = 0;
};

extern "C" DllImport CComLib* CreateComLibrary();
typedef CComLib* (*CREATECOMLIB)();  

extern "C" DllImport VOID DestroyComLibrary();
typedef VOID (*DESTROYCOMLIB)();  

#endif  //(AFX_COMLIB_H__94DA81C3_B492_43FD_BE47_D4CAD5BED693__INCLUDED_)