#include "stdafx.h"
#include "AlmEvtConfigInfo.h"
#include "MainFrm.h"
#include "DADoc.h"

IMPLEMENT_SERIAL(CAlmEvtConfigInfo, CObject,0)

CAlmEvtConfigInfo::CAlmEvtConfigInfo()
{
	nAlmCode = MISC_ALM_CODE;
	nAlmType = ALM_TYPE_MESSAGE;
	strAlmMsg = _T("");
	eChType = MISC_GROUP;
	nAddress = 0;
	bIsAnalogAction = FALSE;
	nActionRegister = 0; 
	nActionValue = 0; 
}

CAlmEvtConfigInfo::CAlmEvtConfigInfo(eChannelType eChType, long nAddress, short nAlmType, short nAlmCode, CString strAlmMsg)
{
	this->nAlmCode = nAlmCode;
	this->nAlmType = nAlmType;
	this->strAlmMsg = strAlmMsg;
	this->eChType = eChType;
	this->nAddress = nAddress;
	this->bIsAnalogAction = FALSE; 
	this->nActionRegister = 0;
	this->nActionValue = 0; 
}

CAlmEvtConfigInfo::CAlmEvtConfigInfo(const CAlmEvtConfigInfo& info)
{
	nAlmCode = info.nAlmCode;
	nAlmType = info.nAlmType;
	strAlmMsg = info.strAlmMsg;
	eChType = info.eChType;
	nAddress = info.nAddress;
	bIsAnalogAction = info.bIsAnalogAction;
	nActionRegister = info.nActionRegister;
	nActionValue = info.nActionValue; 

}
	
const CAlmEvtConfigInfo  CAlmEvtConfigInfo::operator =(const CAlmEvtConfigInfo& info)
{
	nAlmCode = info.nAlmCode;
	nAlmType = info.nAlmType;
	strAlmMsg = info.strAlmMsg;
	eChType = info.eChType;
	nAddress = info.nAddress;
	bIsAnalogAction = info.bIsAnalogAction;
	nActionRegister = info.nActionRegister;
	nActionValue = info.nActionValue; 
	return *this;
}

CAlmEvtConfigInfo::~CAlmEvtConfigInfo()
{

}
void CAlmEvtConfigInfo::Serialize(CArchive& ar)
{
	CObject::Serialize( ar );
	if(ar.IsStoring())
	{
		ar << (int) eChType << nAddress << nAlmCode << nAlmType << strAlmMsg;
		ar << bIsAnalogAction << nActionRegister << nActionValue; 
	}
	else
	{
		ar >> (int&) eChType >> nAddress >> nAlmCode >> nAlmType >> strAlmMsg;
		ar >> bIsAnalogAction >> nActionRegister >> nActionValue; 
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////Now CAlmEvtInfo Class Related Stuff

IMPLEMENT_SERIAL(CAlmEvtInfo, CObject,0)

CAlmEvtInfo::CAlmEvtInfo()
{
	almInTime = CTime::GetCurrentTime();
	almOutTime =  -1; 
	almAckTime =  -1; 
	bAcknowledged = FALSE;
	strValue = _T("");
	strTagName = _T("");
	bStatus = FALSE;
}

CAlmEvtInfo::CAlmEvtInfo(CAlmEvtConfigInfo almConfigInfo, CTime almInTime, CString strVal)
{
	CString strChType[] = { _T("AI"), _T("HR"), _T("DI"), _T("OC"), _T("MISC_GROUP") };
	CDADoc*	pDoc = (CDADoc*) ( (CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	ASSERT_VALID(pDoc); 
	CString strTmp, strBuf; 
	
	this->almConfigInfo = almConfigInfo;
	this->almInTime = almInTime;
	this->strValue = strVal; 

	strBuf = 	pDoc->GetTagString(&pDoc->tagInfoList, almConfigInfo.eChType, almConfigInfo.nAddress); 
	if(strBuf.IsEmpty())
	{
		strTmp = strChType[almConfigInfo.eChType]; 
		strBuf.Format(_T("%s:%d"), strTmp, almConfigInfo.nAddress); 
	}

	this->strTagName = strBuf; 
	
	//defaults. 
	this->almOutTime =  -1; 
	this->almAckTime =  -1; 
	this->bAcknowledged = FALSE;
	this->bStatus = FALSE;
}

CAlmEvtInfo::CAlmEvtInfo(const CAlmEvtInfo& info)
{
	almConfigInfo = info.almConfigInfo;
	almInTime = info.almInTime;
	almOutTime = info.almOutTime;
	almAckTime = info.almAckTime;
	bAcknowledged = info.bAcknowledged;
	strValue = info.strValue;
	strTagName = info.strTagName;
	bStatus = info.bStatus; 
}
	
const CAlmEvtInfo  CAlmEvtInfo::operator =(const CAlmEvtInfo& info)
{
	almConfigInfo = info.almConfigInfo;
	almInTime = info.almInTime;
	almOutTime = info.almOutTime;
	almAckTime = info.almAckTime;
	bAcknowledged = info.bAcknowledged;
	strValue = info.strValue;
	strTagName = info.strTagName;
	bStatus = info.bStatus; 
	
	return *this;
}

CAlmEvtInfo::~CAlmEvtInfo()
{

}
void CAlmEvtInfo::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << almInTime << almOutTime << almAckTime << bAcknowledged << strValue << strTagName << bStatus; 
	}
	else
	{
		ar >> almInTime >> almOutTime >> almAckTime >> bAcknowledged >> strValue >> strTagName >> bStatus; 
	}
	almConfigInfo.Serialize(ar);
}
