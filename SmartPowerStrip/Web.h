#define REQ_BUF_SZ   60

typedef enum
{
	NO_CONN = 0,
	DARIO_CELL,
	NONNA,
	CAMERA_MIA,
	SALOTTO,
	CAMERA_GRANDE,
	MAX_WIFI_ITEM
}WIFI_LIST_ITEM;

typedef struct
{
	String Ssid;
	const char* Password;
	const char* RealSsid;
	int32_t SignalPower;
}WIFI_LIST;



void WifiInit(void);
void WifiDisconnect(void);
String GetWifiSignalPower(void);
String WifiIP(void);
void ShowWifiStatus(short Row, short Col, bool Status);
void ShowClientConnected(short Row, short Col, bool Status);
void WebServerInit(void);
void WebClient(void);
void WifiConnectionChoice(short *WifiListItem, String *NomeWifi);
void WifiScanForSignal(void);
void WifiRiconnect(void);
