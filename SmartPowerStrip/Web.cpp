#include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
#include <Arduino.h>
#include "Web.h"
#include "LCDLib.h"
#include "SmartPowerStrip.h"
#include "Rele.h"
#include "Band.h"
#include "Menu.h"
#include "EEPROM_Ard.h"
#include "TimeLib.h"
#include "Buttons.h"

extern FLAGS Flag;
extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;

const char* Hostname = "cavestrip";
String HostName = "cavestrip";

char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

WIFI_LIST List[] = 
{
	{"No Conn."     , "password"					, "NoConn" 					},
	{"Dario Cell"	, "dari9299"					, "DEO DOOM"				},
	{"Wifi Nonna"	, "Kyr2FGdVynR9ejUE"			, "TP-LINK_Extender_2.4GHz" },
	{"Camera mia"	, "dariolinorobby198919611962"	, "ZIXEL"					},
	{"Salotto Casa"	, "Kyr2FGdVynR9ejUE"			, "TIM-56878495"			},
	{"Camera Grande", "Kyr2FGdVynR9ejUE"			, "TIM-56878495_EXT"		},
};

short WifiItemSsid;

WiFiServer server(80);
WiFiClient client;

void WifiInit()
{
	short NumbPoint = 0;
	short TimerNoConnection = 0;
	bool ExitWifiInit = false;
	short WifiListItem = 0;
	short ButtonPress = NO_PRESS;
	String HostnameExtended = "http://", NomeWifi;
	Flag.WifiActive = false;
	ClearLCD();
	WiFi.mode(WIFI_STA);
	while(!ExitWifiInit)
	{
		LCDPrintString(ONE, CENTER_ALIGN, "Scegli a quale rete");
		LCDPrintString(TWO, CENTER_ALIGN, "connettersi:");
		LCDPrintString(THREE, CENTER_ALIGN, List[WifiListItem].Ssid);
		ButtonPress = CheckButtons();
		switch(ButtonPress)
		{
			case BUTTON_UP:
				if(WifiListItem > 0)
					WifiListItem--;
				else
					WifiListItem = MAX_WIFI_ITEM - 1;
				LCDPrintLineVoid(THREE);
				break;
			case BUTTON_DOWN:
				if(WifiListItem < MAX_WIFI_ITEM - 1)
					WifiListItem++;
				else
					WifiListItem = 0;
				LCDPrintLineVoid(THREE);			
				break;
			case BUTTON_SET:
				NomeWifi = String(List[WifiListItem].RealSsid);
				WifiItemSsid = WifiListItem;
				ExitWifiInit = true;
				ClearLCD();
				break;
			case BUTTON_LEFT:
			default:
				break;
		}
		ButtonPress = NO_PRESS;
		delay(60);
	}
	if(NomeWifi != String(List[NO_CONN].RealSsid))
	{
		WiFi.hostname(Hostname);
		WiFi.begin(List[WifiListItem].RealSsid, List[WifiListItem].Password);	
		LCDPrintString(ONE, CENTER_ALIGN, "Connettendo a:");
		LCDPrintString(TWO, CENTER_ALIGN, List[WifiListItem].Ssid);
		while (WiFi.status() != WL_CONNECTED) 
		{
			delay(1000);
			if(NumbPoint > 19)
			{
				NumbPoint = 0;
				LCDPrintLineVoid(2);
			}
			LCDPrintString(2, 0 + NumbPoint, ".");
			NumbPoint++;
			TimerNoConnection++;
			if(TimerNoConnection == 25)
			{
				ClearLCD();
				LCDPrintString(ONE, CENTER_ALIGN, "Nessuna rete");
				LCDPrintString(TWO, CENTER_ALIGN, "rilevata.");
				LCDPrintString(THREE, CENTER_ALIGN, "Uscita...");
				delay(2000);
				ClearLCD();
				Flag.WifiActive = false;
				break;
			}
			Flag.WifiActive = true;
		}	
		if(Flag.WifiActive)
		{
			LCDShowPopUp("Connesso!");
			HostnameExtended += String(Hostname);
			LCDPrintString(TWO, CENTER_ALIGN, "Hostname: ");	
			LCDPrintString(THREE, CENTER_ALIGN, HostnameExtended);
			delay(3000);
			ClearLCD();		
		}
	}
	return;
}

String WifiIP()
{
	String IP;
	//IP = String(WiFi.localIP());
	IP = WiFi.localIP().toString();
	return IP;
}


void WebServerInit()
{
    server.begin();           // start to listen for clients
}

void WifiDisconnect()
{
	ClearLCD();
	LCDPrintString(ONE, CENTER_ALIGN, "Disconnesso");
	LCDPrintString(TWO, CENTER_ALIGN, "dalla rete");
	LCDPrintString(THREE, CENTER_ALIGN, List[WifiItemSsid].Ssid);
	WiFi.disconnect(); 
	Flag.WifiActive = false;
	delay(1500);
	ClearLCD();
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
short StrContains(String str, String sfind)
{
    short found = 0;
    short index = 0;
    short len;

    len = str.length();
    
    if (sfind.length() > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (sfind.length() == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}

// checks if received HTTP request is switching on/off LEDs
// also saves the state of the LEDs
void SetRele(void)
{
	short ReleIndx;
	String TagOn[RELE_MAX], TagOff[RELE_MAX];
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		TagOn[ReleIndx] = "RELE" + String(ReleIndx + 1) + "=1"; 
		TagOff[ReleIndx] = "RELE" + String(ReleIndx + 1) + "=0";
	}
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		if (StrContains(HTTP_req, TagOn[ReleIndx])) 
		{
			Rele[ReleIndx].IsActive = true;  // save RELE state
			ON(ReleIdx2Pin(ReleIndx));
			Rele[ReleIndx].TurnOnTime.day = PresentTime.day;
			Rele[ReleIndx].TurnOnTime.hour = PresentTime.hour;
			Rele[ReleIndx].TurnOnTime.minute = PresentTime.minute;
			Rele[ReleIndx].TurnOnTime.second = PresentTime.second;
		}
		else if (StrContains(HTTP_req, TagOff[ReleIndx])) 
		{
			Rele[ReleIndx].IsActive = false;  // save RELE state
			OFF(ReleIdx2Pin(ReleIndx));
			Rele[ReleIndx].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}		
	}
}

// send the XML file with LED status
void XML_response(WiFiClient cl)
{
    int analog_val;            // stores value read from analog inputs
    int count;                 // used by 'for' loops
    int sw_arr[] = {2, 3, 5};  // pins interfaced to switches
	short ReleIndx = 0;
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");

	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		cl.print("<LED>");
		if (Rele[ReleIndx].IsActive)
		{
			cl.print("on");
		}
		else 
		{
			cl.print("off");
		}
		cl.println("</LED>");		
	}
    
    cl.print("</inputs>");
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

String WebPage()
{
	String Page;
	String Tmp = ""; 
	short ReleIndx;
	Page = "<!DOCTYPE html> <html> <head>";
    Page += "     <title>Smart Power-Strip</title>";
    Page += "    <script>";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		Tmp += "Rele" + String(ReleIndx + 1) + "Str = \"\";";
	}
	Page += Tmp;
	Tmp = "";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		Tmp += "var RELE" + String(ReleIndx + 1) + "State = 0;";
	}
	Page += Tmp;
	Page += "	function GetArduinoIO()                                                                                ";
	Page += "	{                                                                                                      ";
	Page += "		nocache = \"&nocache=\" + Math.random() * 1000000;                                                 ";
	Page += "		var request = new XMLHttpRequest();                                                                ";
	Page += "		request.onreadystatechange = function()                                                            ";
	Page += "		{                                                                                                  ";
	Page += "			if (this.readyState == 4) {                                                                    ";
	Page += "				if (this.status == 200) {                                                                  ";
	Page += "					if (this.responseXML != null) {                                                        ";
	Page += "						// XML file received - contains analog values, switch values and LED states        ";                                                                       
	Tmp = "";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		Tmp += "if (this.responseXML.getElementsByTagName('RELE')[" + String(ReleIndx) + "].childNodes[0].nodeValue === \"{";
		Tmp += "document.getElementById(\"RELE" + String(ReleIndx + 1) + "\").innerHTML = \"Accesa\"; ";
		Tmp += "RELE" + String(ReleIndx + 1) + "State = 1;}";
		Tmp += "else{";
		Tmp += "document.getElementById(\"RELE" + String(ReleIndx + 1) + "\").innerHTML = \"Spenta\";";
		Tmp += "RELE" + String(ReleIndx + 1) + "State = 0;}";
	}
	Page += Tmp;
	Page += "					}                                                                                      ";
	Page += "				}                                                                                          ";
	Page += "			}                                                                                              ";
	Page += "		}                                                                                                  ";
	Page += "		// send HTTP GET request with LEDs to switch on/off if any                                         ";
	Page += "		request.open(\"GET\", \"ajax_inputs\" + Rele1Str + Rele2Str + Rele3Str + Rele4Str + Rele5Str +     ";
    Page += "            		Rele6Str + Rele7Str + Rele8Str + nocache, true);                                       ";
	Page += "		request.send(null);                                                                                ";
	Page += "		setTimeout('GetArduinoIO()', 500);                                                                 ";
	Tmp = "";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		Tmp += "Rele" + String(ReleIndx + 1) + "Str = \"\";";
	}
	Page += Tmp;
	Tmp = "";
	Page += "	}                                                                                                      ";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		Tmp += "function GetRele" + String(ReleIndx + 1) + "(){";
		Tmp += "if (RELE" + String(ReleIndx + 1) + "State === 1) {";
		Tmp += "RELE" + String(ReleIndx + 1) + "State = 0;";
		Tmp += "Rele" + String(ReleIndx + 1) + "Str = \"&RELE" + String(ReleIndx + 1) + "=0\"};";
		Tmp += "else {";
		Tmp += "RELE" + String(ReleIndx + 1) + "State = 1;";
		Tmp += "Rele" + String(ReleIndx + 1) + "Str = \"&RELE" + String(ReleIndx + 1) + "=1\";}";
	}
	Page += Tmp;
	Page += "</script>                                                                                                 ";
	Page += "<style>                                                                                                   ";
	Page += "table, th, td                                                                                           ";
	Page += "	{                                                                                                    ";
	Page += "		border: 1px solid black;                                                                         ";
	Page += "		border-collapse: collapse;                                                                       ";
	Page += "		background-color: lavander;                                                                      ";
	Page += "	}                                                                                                    ";
	Page += "	th                                                                                                   ";
	Page += "	{                                                                                                    ";
	Page += "		height: 50px;                                                                                    ";
	Page += "		border-bottom: 1px solid LightGrey;                                                              ";
	Page += "		font-family:Verdana;                                                                             ";
	Page += "		font-size:23px;                                                                                  ";
	Page += "	}                                                                                                    ";
	Page += "	td                                                                                                   ";
	Page += "	{                                                                                                    ";
	Page += "		padding: 10px;                                                                                   ";
	Page += "		text-align: center;                                                                              ";
	Page += "		border-bottom: 1px solid LightGrey;                                                              ";
	Page += "		font-size:28px;                                                                                  ";
	Page += "		font-family:Verdana;                                                                             ";
	Page += "	}                                                                                                    ";
	Page += "	tr:hover {background-color:#f5f5f5;}                                                                 ";
	Page += "	.button                                                                                              ";
	Page += "	{                                                                                                    ";
	Page += "		background-color: #6495ED;                                                                       ";
	Page += "		border: black;                                                                                   ";
	Page += "		padding: 20px 60px;                                                                              ";
	Page += "		text-align: center;                                                                              ";
	Page += "		text-decoration: none;                                                                           ";
	Page += "		display: inline-block;                                                                           ";
	Page += "		font-size: 24px;                                                                                 ";
	Page += "		margin: 4px 2px;                                                                                 ";
	Page += "		cursor: pointer;                                                                                 ";
	Page += "		border-radius: 8px;                                                                              ";
	Page += "		box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2), 0 6px 20px 0 rgba(0,0,0,0.19);                         ";
	Page += "		color:white;                                                                                     ";
	Page += "	}                                                                                                    ";
	Page += "	.button:active                                                                                       ";
	Page += "	{                                                                                                    ";
	Page += "		background-color: #8B0000;                                                                       ";
	Page += "		box-shadow: 0 5px #666;                                                                          ";
	Page += "		transform: translateY(3px);                                                                      ";
	Page += "	}                                                                                                    ";
	Page += "	h1 {                                                                                                 ";
	Page += "		font-size: 60px;                                                                                 ";
	Page += "		font-family:Verdana;                                                                           ";
	Page += "		color: blue;                                                                                     ";
	Page += "		text-align:center;                                                                               ";
	Page += "		text-shadow: 3px 2px red;                                                                        ";
	Page += "	}                                                                                                    ";
	Page += "	h2 {                                                                                                 ";
	Page += "		font-size: 45px;                                                                                 ";
	Page += "		font-family:Verdana;                                                                           ";
	Page += "		color: MediumAquaMarine ;                                                                        ";
	Page += "		text-align:center;                                                                               ";
	Page += "		text-decoration: underline;                                                                      ";
	Page += "	}                                                                                                    ";
	Page += "	p {                                                                                                  ";
	Page += "		font-size: 13px;                                                                                 ";
	Page += "		color: black;                                                                                    ";
	Page += "		text-align:center;                                                                               ";
	Page += "	}                                                                                                    ";
	Page += "	.small_text {                                                                                        ";
	Page += "		font-size: 70%;                                                                                  ";
	Page += "		color: #737373;                                                                                  ";
	Page += "	}                                                                                                    ";
	Page += " </style>                                                                                                  ";
    Page += " </head>                                                                                                   ";
    Page += " <body onload=\"GetArduinoIO()\"style=\"background: linear-gradient(to top, #ffffff 80%, #ccffff 100%);\">   ";
    Page += "    <h1>Smart Power-Strip</h1>                                                                             ";                                                                               
	Page += "	<h2>Gestione delle prese e controllo tempi</h2>                                                                       ";
	Page += "		<table style=\"width:100%;\" border=\"1px solid black;\">";
	Page += "			<tr style=\"background-color:LightSkyBlue ;\"><th>NOME PRESA</th><th>Accensione / Spegnimento</th><th>Tempo Accensione</th><th>Timer Presa</th></tr>";
	Tmp = "";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		Tmp += "<tr><th style=\"background-color:hsl(9, 100%, 80%);\">PRESA "+ String(ReleIndx+1) +"</th><td><button class=\"button\" id=\"RELE"+ String(ReleIndx+1) +"\" onclick=\"GetRele"+ String(ReleIndx+1) +"()\">Spenta</button></td><td>0g 0h 0m</td><td>0h 0m</td></tr>";
	}
	Page += Tmp;   
	Page += "	</table>                                                                                                                                                                                                                  ";
	if(!Flag.BandActive)
		Page += "	<p style=\"background-color: hsl(0, 0%, 85%); border-bottom: 6px solid DodgerBlue; color:blue; text-align:center; font-family:Verdana; font-size:30px;\">Banda Oraria: <b style=\"color:red;\">Disattivata</b> </p>   ";
	else
		Page += "	<p style=\"background-color: hsl(0, 0%, 85%); border-bottom: 6px solid DodgerBlue; color:blue; text-align:center; font-family:Verdana; font-size:30px;\">Banda Oraria: <b style=\"color:MediumSeaGreen;\">Attivata</b> </p>   ";
	Page += "	<br>                                                                                                                                                                                                                      ";
	Page += "	<br>                                                                                                                                                                                                                      ";
	Page += "	<footer style=\"text-align:center;\">&copy Dario Cavedale</footer>                                                                                                                                                         ";
    Page += "    </body>                                                                                               ";
    Page += "    </html>                                                                                               ";
	return Page;
}

void WebClient()
{
    if (client) 
	{  // got client?
        bool currentLineIsBlank = true;
		Flag.ClientConnected = true;
		ClearLCD();
		LCDShowPopUp("Client CONNESSO");
		LCDPrintString(THREE, CENTER_ALIGN, "Client CONNESSO");
		String PaginaWeb = WebPage();
        while (client.connected()) 
		{
			TakeReleTime();
            if (client.available()) 
			{   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // limit the size of the stored received HTTP request
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (REQ_BUF_SZ - 1)) 
				{
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) 
				{
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    // remainder of header follows below, depending on if
                    // web page or XML page is requested
                    // Ajax request - send XML file
                    if (StrContains((String)HTTP_req, "ajax_inputs")) 
					{
                        // send rest of HTTP header
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        SetRele();
                        // send XML file containing input states
                        XML_response(client);
                    }
                    else 
					{  // web page request
                        // send rest of HTTP header
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();
                        // send web page
						client.println(PaginaWeb);
                    }
                    // display received HTTP request on serial port
                    Serial.print(HTTP_req);
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n')
					{
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') 
				{
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
			delay(60);
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
		LCDShowPopUp("Client DISCONNESSO");
		delay(1000);
		ClearLCD();
		CheckReleStatus();
		Flag.ClientConnected = false;
    } // end if (client)
}
