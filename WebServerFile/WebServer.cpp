#define REQ_BUF_SZ   60
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer
boolean LED_state[4] = {0}; // stores the states of the LEDs

void WebServerInit()
{
    server.begin();           // start to listen for clients
}

void GesClient()
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
                    if (StrContains(HTTP_req, "ajax_inputs")) 
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

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
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

String WebPage()
{
	String Page;
	Page = "<!DOCTYPE html> <html> <head>";
    Page += "     <title>Smart Power-Strip</title>";
    Page += "    <script>"
	Page += "	Rele1Str = \"\";   ";
	Page += "	Rele2Str = \"\";   ";
	Page += "	Rele3Str = \"\";   ";
	Page += "	Rele4Str = \"\";   ";
	Page += "	Rele5Str = \"\";   ";
	Page += "	Rele6Str = \"\";   ";
	Page += "	Rele7Str = \"\";   ";
	Page += "	Rele8Str = \"\";   ";
	Page += "	var RELE1State = 0; ";
	Page += "	var RELE2State = 0; ";
	Page += "	var RELE3State = 0; ";
	Page += "	var RELE4State = 0; ";
	Page += "	var RELE5State = 0; ";
	Page += "	var RELE6State = 0; ";
	Page += "	var RELE7State = 0; ";
	Page += "	var RELE8State = 0; ";
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
	Page += "						var count;                                                                         ";
	Page += "						// RELE 1                                                                          ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[0].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE1\").innerHTML = \"RELE 1 is ON\";               ";
	Page += "							RELE1State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE1\").innerHTML = \"RELE 1 is OFF\";              ";
	Page += "							RELE2State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						// RELE 2                                                                          ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[1].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE2\").innerHTML = \"RELE 2 is ON\";               ";
	Page += "							RELE2State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE2\").innerHTML = \"RELE 2 is OFF\";              ";
	Page += "							RELE1State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						 // RELE 3                                                                         ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[2].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE3\").innerHTML = \"RELE 3 is ON\";               ";
	Page += "							RELE3State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE3\").innerHTML = \"RELE 3 is OFF\";              ";
	Page += "							RELE3State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						  // RELE 4                                                                        ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[3].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE4\").innerHTML = \"RELE 4 is ON\";               ";
	Page += "							RELE4State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE4\").innerHTML = \"RELE 4 is OFF\";              ";
	Page += "							RELE4State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						 // RELE 5                                                                         ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[4].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE5\").innerHTML = \"RELE 5 is ON\";               ";
	Page += "							RELE5State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE5\").innerHTML = \"RELE 5 is OFF\";              ";
	Page += "							RELE5State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						 // RELE 6                                                                         ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[5].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE6\").innerHTML = \"RELE 6 is ON\";               ";
	Page += "							RELE6State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE6\").innerHTML = \"RELE 6 is OFF\";              ";
	Page += "							RELE6State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						 // RELE 7                                                                         ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[6].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE7\").innerHTML = \"RELE 7 is ON\";               ";
	Page += "							RELE7State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE7\").innerHTML = \"RELE 7 is OFF\";              ";
	Page += "							RELE7State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						 // RELE 8                                                                         ";
	Page += "						if (this.responseXML.getElementsByTagName('RELE')[7].childNodes[0].nodeValue === \"";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE8\").innerHTML = \"RELE 8 is ON\";               ";
	Page += "							RELE8State = 1;                                                                ";
	Page += "						}                                                                                  ";
	Page += "						else                                                                               ";
	Page += "						{                                                                                  ";
	Page += "							document.getElementById(\"RELE8\").innerHTML = \"RELE 8 is OFF\";              ";
	Page += "							RELE8State = 0;                                                                ";
	Page += "						}                                                                                  ";
	Page += "					}                                                                                      ";
	Page += "				}                                                                                          ";
	Page += "			}                                                                                              ";
	Page += "		}                                                                                                  ";
	Page += "		// send HTTP GET request with LEDs to switch on/off if any                                         ";
	Page += "		request.open(\"GET\", \"ajax_inputs\" + Rele1Str + Rele2Str + Rele3Str + Rele4Str + Rele5Str +     ";
    Page += "            		Rele6Str + Rele7Str + Rele8Str + nocache, true);                                       ";
	Page += "		request.send(null);                                                                                ";
	Page += "		setTimeout('GetArduinoIO()', 500);                                                                 ";
	Page += "		Rele1Str = \"\";                                                                                   ";
	Page += "		Rele2Str = \"\";			                                                                       ";
	Page += "		Rele3Str = \"\";			                                                                       ";
	Page += "		Rele4Str = \"\";			                                                                       ";
	Page += "		Rele5Str = \"\";			                                                                       ";
	Page += "		Rele6Str = \"\";			                                                                       ";
	Page += "		Rele7Str = \"\";			                                                                       ";
	Page += "		Rele8Str = \"\";			                                                                       ";
	Page += "	}                                                                                                      ";
	Page += "	                                                                                                       ";
	Page += "	function GetRele1()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE1State === 1) {                                                                            ";
	Page += "			RELE1State = 0;                                                                                ";
	Page += "			Rele1Str = \"&RELE1=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE1State = 1;                                                                                ";
	Page += "			Rele1Str = \"&RELE1=1\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
	Page += "	function GetRele2()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE2State === 1) {                                                                            ";
	Page += "			RELE2State = 0;                                                                                ";
	Page += "			Rele2Str = \"&RELE2=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE2State = 1;                                                                                ";
	Page += "			Rele2Str = \"&RELE2=1\";                                                                         ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
	Page += "	function GetRele3()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE3State === 1) {                                                                            ";
	Page += "			RELE3State = 0;                                                                                ";
	Page += "			Rele3Str = \"&RELE3=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE3State = 1;                                                                                ";
	Page += "			Rele3Str = \"&RELE3=1\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
	Page += "	function GetRele4()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE4State === 1) {                                                                            ";
	Page += "			RELE4State = 0;                                                                                ";
	Page += "			Rele4Str = \"&RELE4=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE4State = 1;                                                                                ";
	Page += "			Rele4Str = \"&RELE4=1\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
	Page += "	function GetRele5()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE5State === 1) {                                                                            ";
	Page += "			RELE5State = 0;                                                                                ";
	Page += "			Rele5Str = \"&RELE5=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE5State = 1;                                                                                ";
	Page += "			Rele5Str = \"&RELE5=1\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
	Page += "	function GetRele6()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE6State === 1) {                                                                            ";
	Page += "			RELE6State = 0;                                                                                ";
	Page += "			Rele6Str = \"&RELE6=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE6State = 1;                                                                                ";
	Page += "			Rele6Str = \"&RELE6=1\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
	Page += "	function GetRele7()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE7State === 1) {                                                                            ";
	Page += "			RELE7State = 0;                                                                                ";
	Page += "			Rele7Str = \"&RELE7=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE7State = 1;                                                                                ";
	Page += "			Rele7Str = \"&RELE7=1\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
	Page += "	function GetRele8()                                                                                    ";
	Page += "	{                                                                                                      ";
	Page += "		if (RELE8State === 1) {                                                                            ";
	Page += "			RELE8State = 0;                                                                                ";
	Page += "			Rele8Str = \"&RELE8=0\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "		else {                                                                                             ";
	Page += "			RELE8State = 1;                                                                                ";
	Page += "			Rele8Str = \"&RELE8=1\";                                                                       ";
	Page += "		}                                                                                                  ";
	Page += "	}                                                                                                      ";
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
	Page += "		<button type=\"button\" id=\"RELE1\" onclick=\"GetRele1()\">RELE 1 is OFF</button><br /><br />     ";
	Page += "		<button type=\"button\" id=\"RELE2\" onclick=\"GetRele2()\">RELE 2 is OFF</button><br /><br />     ";
	Page += "		<button type=\"button\" id=\"RELE3\" onclick=\"GetRele3()\">RELE 3 is OFF</button><br /><br />     ";
	Page += "		<button type=\"button\" id=\"RELE4\" onclick=\"GetRele4()\">RELE 4 is OFF</button><br /><br />     ";
	Page += "		<button type=\"button\" id=\"RELE5\" onclick=\"GetRele5()\">RELE 5 is OFF</button><br /><br />     ";
	Page += "		<button type=\"button\" id=\"RELE6\" onclick=\"GetRele6()\">RELE 6 is OFF</button><br /><br />     ";
	Page += "		<button type=\"button\" id=\"RELE7\" onclick=\"GetRele7()\">RELE 7 is OFF</button><br /><br />     ";
	Page += "		<button type=\"button\" id=\"RELE8\" onclick=\"GetRele8()\">RELE 8 is OFF</button><br /><br />     ";
	Page += "	</div>                                                                                                 ";
    Page += "    </body>                                                                                               ";
    Page += "    </html>                                                                                               ";
	return Page;
}