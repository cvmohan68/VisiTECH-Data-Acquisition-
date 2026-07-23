#include "stdafx.h"
#include "ChannelInfo.h"

IMPLEMENT_SERIAL(CChannelInfo, CObject,0)

CChannelInfo::CChannelInfo()
{
	nAAddress  = 0;
	nTimeScale = TREND_5MIN_TIME;
	bYAutoScale = TRUE;
	nYMax = 100;
	nYMin = 0;
	bDisplaySP = FALSE;
	bDisplayHiAlarm = FALSE;
	bDisplayLoAlarm = FALSE;
	bDisplayHiHiAlarm = FALSE;
	bDisplayLoLoAlarm = FALSE;
	
	nSetPoint = 0;
	nHiAlarm = 0;
	nLoAlarm = 0;
	nHiHiAlarm= 0;
	nLoLoAlarm = 0;
	fRateOfChange = 0.0; 
	
	nSetPointAddr = 0;
	nHiAlarmAddr = 0;
	nLoAlarmAddr = 0;
	nHiHiAlarmAddr = 0;
	nLoLoAlarmAddr = 0;
	
	bSPBindedToDeviceAddr = FALSE;
	bHiAlarmBindedToDeviceAddr = FALSE;
	bLoAlarmBindedToDeviceAddr = FALSE;
	bHiHiAlarmBindedToDeviceAddr = FALSE;
	bLoLoAlarmBindedToDeviceAddr = FALSE;

	bValidSP = FALSE;
	bValidHiAlarm = FALSE;
	bValidLoAlarm = FALSE;
	bValidHiHiAlarm = FALSE;
	bValidLoLoAlarm = FALSE;
	bValidROCAlarm	= FALSE; 

	nSignalConditioning = 0;			//Linear Conversion
	bEndabledCustomScaling = FALSE;
	nRawYMax = 100;
	nRawYMin = 0;
}

CChannelInfo::CChannelInfo(const CChannelInfo& info)
{
	nAAddress = info.nAAddress;
	nTimeScale = info.nTimeScale;
	bYAutoScale = info.bYAutoScale;
	nYMax	= info.nYMax;
	nYMin	= info.nYMin;
	
	bDisplaySP = info.bDisplaySP;
	bDisplayLoAlarm = info.bDisplayLoAlarm;
	bDisplayHiAlarm = info.bDisplayHiAlarm;
	bDisplayLoLoAlarm = info.bDisplayLoLoAlarm;
	bDisplayHiHiAlarm = info.bDisplayHiHiAlarm;

	nSetPoint = info.nSetPoint;
	nHiAlarm = info.nHiAlarm;
	nLoAlarm = info.nLoAlarm;
	nHiHiAlarm = info.nHiHiAlarm;
	nLoLoAlarm = info.nLoLoAlarm;
	fRateOfChange = info.fRateOfChange; 
	
	nSetPointAddr = info.nSetPointAddr;
	nHiAlarmAddr = info.nHiAlarmAddr;
	nLoAlarmAddr = info.nLoAlarmAddr;
	nHiHiAlarmAddr = info.nHiHiAlarmAddr;
	nLoLoAlarmAddr = info.nLoLoAlarmAddr;

	bSPBindedToDeviceAddr = info.bSPBindedToDeviceAddr;
	bHiAlarmBindedToDeviceAddr = info.bHiAlarmBindedToDeviceAddr;
	bLoAlarmBindedToDeviceAddr = info.bLoAlarmBindedToDeviceAddr;
	bHiHiAlarmBindedToDeviceAddr = info.bHiHiAlarmBindedToDeviceAddr;
	bLoLoAlarmBindedToDeviceAddr = info.bLoLoAlarmBindedToDeviceAddr;


	bValidSP = info.bValidSP;
	bValidHiAlarm = info.bValidHiAlarm;
	bValidLoAlarm = info.bValidLoAlarm; 
	bValidHiHiAlarm = info.bValidHiHiAlarm;
	bValidLoLoAlarm = info.bValidLoLoAlarm; 
	bValidROCAlarm = info.bValidROCAlarm; 

	nSignalConditioning = info.nSignalConditioning;			//Linear Conversion
	bEndabledCustomScaling = info.bEndabledCustomScaling;
	nRawYMax = info.nRawYMax;
	nRawYMin = info.nRawYMin;
}
	
const CChannelInfo  CChannelInfo::operator =(const CChannelInfo& info)
{
	nAAddress = info.nAAddress;
	nTimeScale = info.nTimeScale;
	bYAutoScale = info.bYAutoScale;
	nYMax	= info.nYMax;
	nYMin	= info.nYMin;
	
	bDisplaySP = info.bDisplaySP;
	bDisplayLoAlarm = info.bDisplayLoAlarm;
	bDisplayHiAlarm = info.bDisplayHiAlarm;
	bDisplayLoLoAlarm = info.bDisplayLoLoAlarm;
	bDisplayHiHiAlarm = info.bDisplayHiHiAlarm;

	nSetPoint = info.nSetPoint;
	nHiAlarm = info.nHiAlarm;
	nLoAlarm = info.nLoAlarm;
	nHiHiAlarm = info.nHiHiAlarm;
	nLoLoAlarm = info.nLoLoAlarm;
	fRateOfChange = info.fRateOfChange; 
	
	nSetPointAddr = info.nSetPointAddr;
	nHiAlarmAddr = info.nHiAlarmAddr;
	nLoAlarmAddr = info.nLoAlarmAddr;
	nHiHiAlarmAddr = info.nHiHiAlarmAddr;
	nLoLoAlarmAddr = info.nLoLoAlarmAddr;

	bSPBindedToDeviceAddr = info.bSPBindedToDeviceAddr;
	bHiAlarmBindedToDeviceAddr = info.bHiAlarmBindedToDeviceAddr;
	bLoAlarmBindedToDeviceAddr = info.bLoAlarmBindedToDeviceAddr;
	bHiHiAlarmBindedToDeviceAddr = info.bHiHiAlarmBindedToDeviceAddr;
	bLoLoAlarmBindedToDeviceAddr = info.bLoLoAlarmBindedToDeviceAddr;


	bValidSP = info.bValidSP;
	bValidHiAlarm = info.bValidHiAlarm;
	bValidLoAlarm = info.bValidLoAlarm; 
	bValidHiHiAlarm = info.bValidHiHiAlarm;
	bValidLoLoAlarm = info.bValidLoLoAlarm; 
	bValidROCAlarm = info.bValidROCAlarm; 

	nSignalConditioning = info.nSignalConditioning;			//Linear Conversion
	bEndabledCustomScaling = info.bEndabledCustomScaling;
	nRawYMax = info.nRawYMax;
	nRawYMin = info.nRawYMin;

	return *this;
}

CChannelInfo::~CChannelInfo()
{
	
}

void CChannelInfo::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << nAAddress << nTimeScale << bYAutoScale << nYMax << nYMin;
		ar << bDisplaySP << bDisplayLoAlarm << bDisplayHiAlarm <<bDisplayLoLoAlarm << bDisplayHiHiAlarm; 
		ar << nSetPoint << nHiAlarm << nLoAlarm << nHiHiAlarm << nLoLoAlarm <<fRateOfChange;  		
		ar << nSetPointAddr << nHiAlarmAddr << nLoAlarmAddr << nHiHiAlarmAddr << nLoLoAlarmAddr; 
		ar << bSPBindedToDeviceAddr << bHiAlarmBindedToDeviceAddr << bLoAlarmBindedToDeviceAddr << bHiHiAlarmBindedToDeviceAddr << bLoLoAlarmBindedToDeviceAddr;
		ar << bValidSP << bValidHiAlarm << bValidLoAlarm << bValidHiHiAlarm << bValidLoLoAlarm << bValidROCAlarm; 
		ar << nSignalConditioning << bEndabledCustomScaling << nRawYMax << nRawYMin; 	
	}
	else
	{
		ar >> nAAddress >> nTimeScale >> bYAutoScale >> nYMax >> nYMin;
		ar >> bDisplaySP >> bDisplayLoAlarm >> bDisplayHiAlarm >> bDisplayLoLoAlarm >> bDisplayHiHiAlarm; 
		ar >> nSetPoint >> nHiAlarm >> nLoAlarm >> nHiHiAlarm >> nLoLoAlarm >> fRateOfChange; 		
		ar >> nSetPointAddr >> nHiAlarmAddr >> nLoAlarmAddr >> nHiHiAlarmAddr >> nLoLoAlarmAddr; 
		ar >> bSPBindedToDeviceAddr >> bHiAlarmBindedToDeviceAddr >> bLoAlarmBindedToDeviceAddr >> bHiHiAlarmBindedToDeviceAddr >> bLoLoAlarmBindedToDeviceAddr;
		ar >> bValidSP >> bValidHiAlarm >> bValidLoAlarm >> bValidHiHiAlarm >> bValidLoLoAlarm >> bValidROCAlarm;
		ar >> nSignalConditioning >> bEndabledCustomScaling >> nRawYMax >> nRawYMin; 	
	}
}

