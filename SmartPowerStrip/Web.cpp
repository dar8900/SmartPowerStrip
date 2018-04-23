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

extern FLAGS Flag;
extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
extern BAND_FORMAT Band;

const char* ssid = "ssid";  // Enter SSID here
const char* password = "password";  //Enter Password here
const char* Hostname = "cavestrip";

char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

String HostName = "cavestrip";

WiFiServer server(80);

void WifiInit()
{
	short NumbPoint = 0;
	String HostnameExtended = "http://";
	Flag.WifiActive = false;
	ClearLCD();
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);	
	WiFi.hostname(Hostname);
	LCDPrintString(0, CENTER_ALIGN, "Connettendo a:");
	LCDPrintString(1, CENTER_ALIGN, String(ssid));
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(1000);
		if(NumbPoint > 20)
		{
			NumbPoint = 0;
			LCDPrintLineVoid(2);
			delay(1000);
		}
		LCDPrintString(2, 0 + NumbPoint, ".");
		NumbPoint++;
			
	}
	LCDPrintString(3, CENTER_ALIGN, "Connesso");
	delay(2000);
	ClearLCD();
	HostnameExtended += String(Hostname);
	LCDPrintString(0, CENTER_ALIGN, "Hostname: ");	
	LCDPrintString(1, CENTER_ALIGN, HostnameExtended);
	delay(3000);
	ClearLCD();
	Flag.WifiActive = true;
	return;
}

String WifiIP()
{
	String IP;
	IP = String(WiFi.localIP());
	return IP;
}


void WebServerInit()
{
    server.begin();           // start to listen for clients
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
		}
		else if (StrContains(HTTP_req, TagOff[ReleIndx])) 
		{
			Rele[ReleIndx].IsActive = false;  // save RELE state
			OFF(ReleIdx2Pin(ReleIndx));
		}		
	}
}

// send the XML file with analog values, switch status
//  and LED status
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
		Tmp += "document.getElementById(\"RELE" + String(ReleIndx + 1) + "\").innerHTML = \"RELE " + String(ReleIndx + 1) + " is ON\"; ";
		Tmp += "RELE" + String(ReleIndx + 1) + "State = 1;}";
		Tmp += "else{";
		Tmp += "document.getElementById(\"RELE" + String(ReleIndx + 1) + "\").innerHTML = \"RELE " + String(ReleIndx + 1) + " is OFF\";";
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
	Page += "	.IO_box {                                                                                              ";
	Page += "		float: left;                                                                                       ";
	Page += "		margin: 0 20px 20px 0;                                                                             ";
	Page += "		border: 1px solid blue;                                                                            ";
	Page += "		padding: 0 5px 0 5px;                                                                              ";
	Page += "		width: 120px;                                                                                      ";
	Page += "	}                                                                                                      ";
	Page += "	h1 {                                                                                                   ";
	Page += "		font-size: 120%;                                                                                   ";
	Page += "		color: blue;                                                                                       ";
	Page += "		margin: 0 0 10px 0;                                                                                ";
	Page += "	}                                                                                                      ";
	Page += "	h2 {                                                                                                   ";
	Page += "		font-size: 85%;                                                                                    ";
	Page += "		color: #5734E6;                                                                                    ";
	Page += "		margin: 5px 0 5px 0;                                                                               ";
	Page += "	}                                                                                                      ";
	Page += "	p, form, button {                                                                                      ";
	Page += "		font-size: 80%;                                                                                    ";
	Page += "		color: #252525;                                                                                    ";
	Page += "	}                                                                                                      ";
	Page += "	.small_text {                                                                                          ";
	Page += "		font-size: 70%;                                                                                    ";
	Page += "		color: #737373;                                                                                    ";
	Page += "	}                                                                                                      ";
	Page += " </style>                                                                                                  ";
    Page += " </head>                                                                                                   ";
    Page += " <body onload=\"GetArduinoIO()\">                                                                          ";
    Page += "    <h1>Arduino Ajax I/O</h1>                                                                             ";
	Page += "	<div class=\"IO_box\">                                                                                 ";
	Page += "		<h2>RELEs Using Buttons</h2>                                                                       ";
	Tmp = "";
	for(ReleIndx = RELE_1; ReleIndx < RELE_MAX; ReleIndx++)
	{
		Tmp += "<button type=\"button\" id=\"RELE" + String(ReleIndx + 1) + "\" onclick=\"GetRele" + String(ReleIndx + 1) + "()\">RELE " + String(ReleIndx + 1) + " is OFF</button><br /><br /> ";
	}
	Page += Tmp;
	Page += "	</div>                                                                                                 ";
    Page += "    </body>                                                                                               ";
    Page += "    </html>                                                                                               ";
	return Page;
}

void WebClient()
{
    WiFiClient client = server.available();

    if (client) 
	{  // got client?
        bool currentLineIsBlank = true;
        while (client.connected()) 
		{
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
						client.println(WebPage());
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
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}