#ifndef _CHANNELINFO
#define _CHANNELINFO

class CChannelInfo:public CObject
{
	DECLARE_SERIAL(CChannelInfo)

public:
	long	nAAddress;
	short   nTimeScale;				//Now it is holding in terms of mins of X-axis 
	int		nYMax;
	int		nYMin;
	
	BOOL	bYAutoScale;
	BOOL	bDisplaySP;
	BOOL	bDisplayLoAlarm;
	BOOL	bDisplayHiAlarm;
	BOOL	bDisplayLoLoAlarm;
	BOOL	bDisplayHiHiAlarm;	

	int		nSetPoint;
	int		nHiAlarm;
	int		nLoAlarm;
	int		nHiHiAlarm; 
	int		nLoLoAlarm; 
	float   fRateOfChange; 

	int		nSetPointAddr;
	int		nHiAlarmAddr;
	int		nLoAlarmAddr;
	int		nHiHiAlarmAddr;
	int		nLoLoAlarmAddr; 

	BOOL	bSPBindedToDeviceAddr;
	BOOL	bHiAlarmBindedToDeviceAddr;
	BOOL	bLoAlarmBindedToDeviceAddr;
	BOOL	bHiHiAlarmBindedToDeviceAddr;
	BOOL	bLoLoAlarmBindedToDeviceAddr;
	
	BOOL	bValidSP;
	BOOL	bValidHiAlarm;	
	BOOL	bValidLoAlarm;
	BOOL	bValidHiHiAlarm;	
	BOOL	bValidLoLoAlarm;
	BOOL	bValidROCAlarm; 

	short	nSignalConditioning;
	BOOL	bEndabledCustomScaling;
	int		nRawYMax;
	int		nRawYMin;

public:
	CChannelInfo();
	~CChannelInfo();
	CChannelInfo(const CChannelInfo& info);
	const CChannelInfo operator =(const CChannelInfo& info);
	virtual void Serialize(CArchive& ar);
};

#endif //CHANNELINFO

typedef CTypedPtrList<CObList, CChannelInfo*> CChannelInfoList;
