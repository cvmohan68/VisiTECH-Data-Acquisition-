#ifndef _ALMEVTCONFIGINFO
#define _ALMEVTCONFIGINFO

class CAlmEvtConfigInfo : public CObject
{
	DECLARE_SERIAL(CAlmEvtConfigInfo)

public:
	short	nAlmCode;
	short	nAlmType;
	CString	strAlmMsg;
	
	eChannelType	eChType;
	unsigned long	nAddress;

	BOOL	bIsAnalogAction;   //TRUE means action on Holding Reg FALSE means on Output Coil. 
	int		nActionRegister;		// 0 means not valid action.  
	short	nActionValue;	 //In the case of discrete action it will be 0 or 1. 	

public:
	CAlmEvtConfigInfo();
	~CAlmEvtConfigInfo();
	CAlmEvtConfigInfo(eChannelType eCh, long nAddr, short nAlmType, short nAlmCode, CString strAlmMsg); 
	CAlmEvtConfigInfo(const CAlmEvtConfigInfo& info);
	const CAlmEvtConfigInfo operator =(const CAlmEvtConfigInfo& info);
	virtual void Serialize(CArchive& ar);
};

class CAlmEvtInfo : public CObject
{
	DECLARE_SERIAL(CAlmEvtInfo)

public:
	CTime almInTime;
	CAlmEvtConfigInfo almConfigInfo;

	BOOL	bAcknowledged; 
	CTime	almOutTime;
	CTime	almAckTime; 
	CString	strValue;
	CString strTagName; 
	BOOL	bStatus;			//Still in Alarm mode or recoverd ? 

public:
	CAlmEvtInfo();
	~CAlmEvtInfo();
	CAlmEvtInfo(CAlmEvtConfigInfo almConfigInfo, CTime almInTime, CString strVal);
	CAlmEvtInfo(const CAlmEvtInfo& info);
	const CAlmEvtInfo operator =(const CAlmEvtInfo& info);
	virtual void Serialize(CArchive& ar);
};

#endif //ALMEVTCONFIGINFO

typedef CTypedPtrList<CObList, CAlmEvtConfigInfo*> CAlmEvtConfigList;
typedef CTypedPtrList<CObList, CAlmEvtInfo*> CAlmEvtList;