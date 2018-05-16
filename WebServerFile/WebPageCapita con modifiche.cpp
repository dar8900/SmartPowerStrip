LATO WEB

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demo">
<h1>The ESP8266 NodeMCU Update web page without refresh</h1>
	<button type="button" onclick="sendData(1)">LED ON</button>
	<button type="button" onclick="sendData(0)">LED OFF</button><BR>
</div>

<div>
	ADC Value is : <span id="ADCValue">0</span><br>
    LED State is : <span id="LEDState">NA</span>
</div>
<script>
function sendData(led) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LEDState").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setLED?LEDstate="+led, true);
  xhttp.send();
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 2000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}
</script>
<br><br><a href="https://circuits4you.com">Circuits4you.com</a>
</body>
</html>
)=====";

///////////////////////////////// LATO ARDUINO ////////////////////////////////////////////////
/*
 * ESP8266 NodeMCU AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "index.h" //Our HTML webpage contents with javascripts

#define LED 2  //On board LED

//SSID and Password of your WiFi router
const char* ssid = "circuits4you.com";
const char* password = "123456789";

ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void HandleHomePage() {
String s = MAIN_page; //Read HTML contents
server.send(200, "text/html", s); //Send web page
}

void HandleTimer1() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_1].HaveTimer && Rele[RELE_1].IsActive)
		TimerStr = String(Rele[RELE_1].TimerTime.hour) + "h " + String(Rele[RELE_1].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer2() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_2].HaveTimer && Rele[RELE_2].IsActive)
		TimerStr = String(Rele[RELE_2].TimerTime.hour) + "h " + String(Rele[RELE_2].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer3() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_3].HaveTimer && Rele[RELE_3].IsActive)
		TimerStr = String(Rele[RELE_3].TimerTime.hour) + "h " + String(Rele[RELE_3].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer4() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_4].HaveTimer && Rele[RELE_4].IsActive)
		TimerStr = String(Rele[RELE_4].TimerTime.hour) + "h " + String(Rele[RELE_4].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer5() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_5].HaveTimer && Rele[RELE_5].IsActive)
		TimerStr = String(Rele[RELE_5].TimerTime.hour) + "h " + String(Rele[RELE_5].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer6() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_6].HaveTimer && Rele[RELE_6].IsActive)
		TimerStr = String(Rele[RELE_6].TimerTime.hour) + "h " + String(Rele[RELE_6].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer7() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_7].HaveTimer && Rele[RELE_7].IsActive)
		TimerStr = String(Rele[RELE_7].TimerTime.hour) + "h " + String(Rele[RELE_7].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer8() 
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_8].HaveTimer && Rele[RELE_8].IsActive)
		TimerStr = String(Rele[RELE_8].TimerTime.hour) + "h " + String(Rele[RELE_8].TimerTime.minute) + "m";
	else
		TimerStr = "NESSUN TIMER"; 
	
	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTurnOn1() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_1].IsActive)
		TurnOnStr = String(Rele[RELE_1].TurnOnTime.day) + "g " + String(Rele[RELE_1].TurnOnTime.hour) + "h " + String(Rele[RELE_1].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleTurnOn2() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_2].IsActive)
		TurnOnStr = String(Rele[RELE_2].TurnOnTime.day) + "g " + String(Rele[RELE_2].TurnOnTime.hour) + "h " + String(Rele[RELE_2].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn3() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_3].IsActive)
		TurnOnStr = String(Rele[RELE_3].TurnOnTime.day) + "g " + String(Rele[RELE_3].TurnOnTime.hour) + "h " + String(Rele[RELE_3].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn4() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_4].IsActive)
		TurnOnStr = String(Rele[RELE_4].TurnOnTime.day) + "g " + String(Rele[RELE_4].TurnOnTime.hour) + "h " + String(Rele[RELE_4].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA"; 
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn5() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_5].IsActive)
		TurnOnStr = String(Rele[RELE_5].TurnOnTime.day) + "g " + String(Rele[RELE_5].TurnOnTime.hour) + "h " + String(Rele[RELE_5].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn6() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_6].IsActive)
		TurnOnStr = String(Rele[RELE_6].TurnOnTime.day) + "g " + String(Rele[RELE_6].TurnOnTime.hour) + "h " + String(Rele[RELE_6].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA"; 
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn7() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_7].IsActive)
		TurnOnStr = String(Rele[RELE_7].TurnOnTime.day) + "g " + String(Rele[RELE_7].TurnOnTime.hour) + "h " + String(Rele[RELE_7].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleTurnOn8() 
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_8].IsActive)
		TurnOnStr = String(Rele[RELE_8].TurnOnTime.day) + "g " + String(Rele[RELE_8].TurnOnTime.hour) + "h " + String(Rele[RELE_8].TurnOnTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";
	
	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleRele1() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE1"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_1].IsActive)
		{
			ON(ReleIdx2Pin(RELE_1)); //LED ON
			Rele[RELE_1].IsActive = true;
			Rele[RELE_1].TurnOnTime.day = PresentTime.day;
			Rele[RELE_1].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_1].TurnOnTime.minute = PresentTime.minute;
			Rele[ReleIndx].ActiveTime.minute = 0;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_1].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_1)); //LED OFF
			Rele[RELE_1].IsActive = true;
			Rele[RELE_1].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_1].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele2() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE2"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_2].IsActive)
		{
			ON(ReleIdx2Pin(RELE_2)); //LED ON
			Rele[RELE_2].IsActive = true;
			Rele[RELE_2].TurnOnTime.day = PresentTime.day;
			Rele[RELE_2].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_2].TurnOnTime.minute = PresentTime.minute;
			Rele[ReleIndx].ActiveTime.minute = 0;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_2].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_2)); //LED OFF
			Rele[RELE_2].IsActive = true;
			Rele[RELE_2].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_2].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}
void HandleRele3() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE3"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_3].IsActive)
		{
			ON(ReleIdx2Pin(RELE_3)); //LED ON
			Rele[RELE_3].IsActive = true;
			Rele[RELE_3].TurnOnTime.day = PresentTime.day;
			Rele[RELE_3].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_3].TurnOnTime.minute = PresentTime.minute;	
			Rele[ReleIndx].ActiveTime.minute = 0;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_3].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_3)); //LED OFF
			Rele[RELE_3].IsActive = true;
			Rele[RELE_3].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_3].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele4() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE4"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_4].IsActive)
		{
			ON(ReleIdx2Pin(RELE_4)); //LED ON
			Rele[RELE_4].IsActive = true;
			Rele[RELE_4].TurnOnTime.day = PresentTime.day;
			Rele[RELE_4].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_4].TurnOnTime.minute = PresentTime.minute;	
			Rele[ReleIndx].ActiveTime.minute = 0;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_4].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_4)); //LED OFF
			Rele[RELE_4].IsActive = true;
			Rele[RELE_4].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_4].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele5() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE5"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_5].IsActive)
		{
			ON(ReleIdx2Pin(RELE_5)); //LED ON
			Rele[RELE_5].IsActive = true;
			Rele[RELE_5].TurnOnTime.day = PresentTime.day;
			Rele[RELE_5].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_5].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_5].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_5)); //LED OFF
			Rele[RELE_5].IsActive = true;
			Rele[RELE_5].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_5].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele6()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE6"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_6].IsActive)
		{
			ON(ReleIdx2Pin(RELE_6)); //LED ON
			Rele[RELE_6].IsActive = true;
			Rele[RELE_6].TurnOnTime.day = PresentTime.day;
			Rele[RELE_6].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_6].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_6].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_6)); //LED OFF
			Rele[RELE_6].IsActive = true;
			Rele[RELE_6].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_6].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele7() 
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE7"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_7].IsActive)
		{
			ON(ReleIdx2Pin(RELE_7)); //LED ON
			Rele[RELE_7].IsActive = true;
			Rele[RELE_7].TurnOnTime.day = PresentTime.day;
			Rele[RELE_7].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_7].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_7].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_7)); //LED OFF
			Rele[RELE_7].IsActive = true;
			Rele[RELE_7].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_7].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

void HandleRele8()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE8"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	if(t_state == "1")
	{
		if(!Rele[RELE_8].IsActive)
		{
			ON(ReleIdx2Pin(RELE_8)); //LED ON
			Rele[RELE_8].IsActive = true;
			Rele[RELE_8].TurnOnTime.day = PresentTime.day;
			Rele[RELE_8].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_8].TurnOnTime.minute = PresentTime.minute;			
		}
		LedStatus = "ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_8].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_8)); //LED OFF
			Rele[RELE_8].IsActive = true;
			Rele[RELE_8].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_8].TurnOnTime = SetTimeVarRele(0,0,0,0);
		}
		LedStatus = "SPENTA"; //Feedback parameter  
	}
	server.send(200, "text/plane", LedStatus); //Send web page	
	delay(200);
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", HandleHomePage);      //Which routine to handle at root location. This is display page
  server.on("/SetRele1", HandleRele1);
  server.on("/SetRele2", HandleRele2);
  server.on("/SetRele3", HandleRele3);
  server.on("/SetRele4", HandleRele4);
  server.on("/SetRele5", HandleRele5);
  server.on("/SetRele6", HandleRele6);
  server.on("/SetRele7", HandleRele7);
  server.on("/SetRele8", HandleRele8);
  server.on("/TurnOnRele1", HandleTurnOn1);
  server.on("/TurnOnRele2", HandleTurnOn2);
  server.on("/TurnOnRele3", HandleTurnOn3);
  server.on("/TurnOnRele4", HandleTurnOn4);
  server.on("/TurnOnRele5", HandleTurnOn5);
  server.on("/TurnOnRele6", HandleTurnOn6);
  server.on("/TurnOnRele7", HandleTurnOn7);
  server.on("/TurnOnRele8", HandleTurnOn8);
  server.on("/TimerRele1", HandleTimer1);
  server.on("/TimerRele2", HandleTimer2);
  server.on("/TimerRele3", HandleTimer3);
  server.on("/TimerRele4", HandleTimer4);
  server.on("/TimerRele5", HandleTimer5);
  server.on("/TimerRele6", HandleTimer6);
  server.on("/TimerRele7", HandleTimer7);
  server.on("/TimerRele8", HandleTimer8);
  
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}