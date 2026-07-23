#if !defined(_DACONSTANTS)
#define _DACONSTANTS

//Globally used. 
#define ID_DATIMER				1
#define ID_DAEVALUATION_TIMER	2
#define ID_SPLASH_TIMER			3
#define ID_ALMBEEP_TIMER		4

//defined in DA Document 
#define MAX_PORTS   4
#define MAX_CONFIGURATIONS 5

//Used in MainFrame 
#define MAX_CHILD_TRENDS 4

//Used in DAFormView 
#define WM_STARTSTOP	WM_USER+15
#define WM_NEWDATA		WM_USER+16
#define WM_NEWSCREEN	WM_USER+17

//For Alarm & Event View
#define WM_NEWALARM		WM_USER+18
#define WM_ALMRECOVERED	WM_USER+19

//For ReportView
#define WM_DISPLAYREPORT WM_USER+20


// Used in TrendView
#define IDC_BTN_MAINFORM		100
#define IDC_BTN_FILELOAD		101
#define IDC_BTN_TRENDSTARTSTOP	102
#define	IDC_BTN_PREV			103
#define	IDC_BTN_NEXT			104
#define IDC_EDIT_CHANNEL		105
#define	IDC_BTN_GPREV			106
#define	IDC_BTN_GNEXT			107
#define IDC_EDIT_TRENDPOS		108
//#define IDC_LIST_ADDRESS		109
#define IDC_BTN_TEXTVIEW		110
#define IDC_BTN_TRENDHDINFO		111
#define IDC_BTN_TRENDPRINT		112
#define IDC_BTN_TRENDCLOSE		113
#define IDC_BTN_TRENDCOLOR		114

//User defined Message to handle the enter key in edit box defined in Trend View. 
#define WM_ENTERPRESSED		WM_USER+5
#define WM_TRENDPOSENTER	WM_USER+6

// Used in TextView
#define IDC_BTN_SELECTCHANNEL	120
#define IDC_BTN_STARTSTOP		121
#define IDC_BTN_RTHISTORIC		122
#define IDC_BTN_FRONTPANEL		123
#define	IDC_BTN_TRENDVIEW		124
#define	IDC_BTN_DACLOSE			125
#define IDC_LIST_DATACTRL		126
#define IDC_BTN_DATAPRINT		127
#define IDC_BTN_EXPORTTOEXCEL   128 

// Used in Multi Channel Trend View
#define IDC_BTN_MT_SELECTTRENDPENS	130
#define IDC_BTN_MT_STARTSTOP		131
#define IDC_BTN_MT_SETTINGS			132
#define IDC_BTN_MT_TRENDPRINT		133
#define IDC_BTN_MT_FRONTPANEL		134
#define IDC_BTN_MT_LISTVIEW			135
#define IDC_BTN_MT_DACLOSE			136
#define	IDC_BTN_MCT_GPREV			137
#define IDC_BTN_MCT_GNEXT			138
#define IDC_CMB_MCT_CHANNELS		139


//Used in all files. 
enum eViewType 
{
	FRONTPANEL		= 1, 
	TRENDVIEW		= 2,
	TEXTVIEW		= 3, 
	MTRENDVIEW		= 4,			//Multi Trend View
	REPORTVIEW		= 5
};

enum eChannelType
{
	INPUT_REGISTER		= 0,
	HOLDING_REGISTER	= 1,
	INPUT_DISCRETE		= 2,
	OUTPUT_COIL			= 3,
	MISC_GROUP			= 4
};

enum eTransmissionMode
{
	RTU_MODE	= 0,
	ASCII_MODE	= 1
};

enum eCommunicationMode
{
	RS232_MODE = 0,
	TCPIP_MODE = 1,
	OPC_MODE   = 2,
	OTHER_MODE = 3
};

//Used in TrendView;
enum ScreenResolution
{
	HIGH_RESOLUTION		= 1,					//1024 x 768
	MEDIUM_RESOLUTION	= 2						// 800 x 600
};


//Trend Related constants
#define X_AXIS_SCALE_PIXELS		720
#define Y_AXIS_SCALE_PIXELS		400
#define MAX_CHILD_SCREEN_PIXELS 360

#define X_ORGIN				100
#define Y_ORGIN				500



#define	TREND_5MIN_SCALE	0
#define	TREND_15MIN_SCALE	1
#define	TREND_30MIN_SCALE	2
#define	TREND_1HOUR_SCALE	3
#define	TREND_6HOUR_SCALE	4

#define TREND_5MIN_TIME		5		//X-Axis scale in terms of minutes. 
#define TREND_15MIN_TIME	15
#define TREND_30MIN_TIME	30
#define TREND_1HOUR_TIME	60
#define TREND_6HOUR_TIME	360

#define TREND_5MIN_PIXELS	2.4
#define TREND_15MIN_PIXELS	0.8
#define TREND_30MIN_PIXELS	0.4
#define TREND_1HOUR_PIXELS	0.2
#define TREND_6HOUR_PIXELS	0.03333


//Alarm and Event related constants. 
#define MAX_ALM_CODES			13
#define MAX_ALM_TYPES			 3

//Alarm types. 
#define ALM_TYPE_ALARM			0
#define ALM_TYPE_EVENT			1
#define ALM_TYPE_MESSAGE		2

//Alarm Codes
#define MISC_ALM_CODE						0
#define ON_TO_OFF							1
#define OFF_TO_ON							2	 
#define LTHIHIALMVALUE_TO_GTHIHIALMVALUE	3
#define GTHIHIALMVALUE_TO_LTHIHIALMVALUE	4
#define LTHIALMVALUE_TO_GTHIALMVALUE		5
#define GTHIALMVALUE_TO_LTHIALMVALUE		6
#define GTLOALMVALUE_TO_LTLOALMVALUE	    7
#define LTLOALMVALUE_TO_GTLOALMVALUE		8
#define GTLOLOALMVALUE_TO_LTLOLOALMVALUE	9
#define LTLOLOALMVALUE_TO_GTLOLOALMVALUE	10
#define PV_RATE_OF_CHNGE					11
#define HR_VALUE_CHANGE						12 

/* Alredy used in SpAlarmConfig.cpp
#define HIGH_HIGH_ALARM			 3 
#define HIGH_HIGH_EVENT			 4 
#define HIGH_ALARM				 5	
#define HIGH_EVENT				 6 
#define LOW_ALARM				 7
#define LOW_EVENT				 8	
#define LOW_LOW_ALARM			 9
#define LOW_LOW_EVENT			 10
#define RATE_OF_CHANGE			 11	
*/


//Settings Related Stuff. 
#define DSTIME_6HOURS		0
#define DSTIME_8HOURS		1
#define DSTIME_12HOURS		2
#define DSTIME_24HOURS		3

#define ID_SC_LINEAR		0
#define ID_SC_SQRROOT		1
#define ID_SC_LOG			2



#define FRONTPANEL_BKG_COLOR	RGB(255, 255, 240)
#define LOGIN_DLG_BKG_COLOR		RGB(255,247, 210)	//RGB(255,243,196)  bit thick color
#define LY1_DLG_BKG_COLOR		RGB(240,240, 200)
#define LY2_DLG_BKG_COLOR		RGB(255,240, 200)

//MCT Related constants
#define MAX_MCT_TRENDS   4

//Multi Channel Trend, Trend Screen Page Colors. 
#define MCT_TSP_WHITE		16777215		//long value for white color
#define MCT_TSP_LYELLOW		15794175
#define MCT_TSP_BTNFACE		14215660
#define	MCT_TSP_GRAY		14803425
#define MCT_TSP_THICKGRAY	12632256

#define MCT_TS_BLACK		 1245234
#define MCT_TS_WHITE		16777215
#define MCT_TS_LYELLOW		15794175
#define MCT_TS_CYAN			16777124
#define MCT_TS_BTNFACE		14215660


#define MAX_MCT_TRENDPAGE_COLORS	5 
#define MAX_MCT_TRENDSCREEN_COLORS	5 

#define MCT_TRENDS_SUPERIMPOSEDTRACES		0
#define MCT_TRENDS_SEPERATETRACES			1

#define MCT_TRENDS_GRIDOFF	0
#define MCT_TRENDS_GRIDON	1

#define MCT_MAX_TRENDSCREEN_HEIGHT	500
#define MCT_MAX_TRENDSCREEN_WIDTH	820

//Custom MessageBox related. 
#define MSGBOX_BMP_ABOUT		0
#define MSGBOX_BMP_INFORMATION	1
#define MSGBOX_BMP_QUESTION		2
#define MSGBOX_BMP_ERROR		3

#define MSGBOX_BTN_OKCANCEL		0
#define MSGBOX_BTN_OKONLY		1


#define ID_PRINTR_COL80			0 
#define ID_PRINTR_COL132		1

#define ID_ALM_BEEPMODE_MOMENTARY 0
#define ID_ALM_BEEPMODE_LATCH	  1

#define ID_BEEP_ALMONLY			0
#define ID_BEEP_EVTONLY			1
#define ID_BEEP_ALMEVT			2

#endif //_DACONSTANTS
