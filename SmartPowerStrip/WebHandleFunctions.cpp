#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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

#include "WebPage.h"

extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
ESP8266WebServer server(80);


const char WebPage[] PROGMEM = R"=====( 

	<!DOCTYPE html>
<html>
    <head>
        <title>Smart Power-Strip</title>
		<meta charset="UTF-8">
        <script>		
		function SetRele1(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE1").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele1?STATUS_RELE1="+Status, true);
			 xhttp.send();
		}
		
		function SetRele2(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE2").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele2?STATUS_RELE2="+Status", true);
			 xhttp.send();
		}
		
		function SetRele3(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE3").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele3?STATUS_RELE3="+Status, true);
			 xhttp.send();
		}
		
		function SetRele4(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE4").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele4?STATUS_RELE4="+Status, true);
			 xhttp.send();
		}
		
		function SetRele5(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE5").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele5?STATUS_RELE5="+Status, true);
			 xhttp.send();
		}
		
		function SetRele6(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE6").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele6?STATUS_RELE6="+Status, true);
			 xhttp.send();
		}
		
		function SetRele7(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE7").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele7?STATUS_RELE7="+Status, true);
			 xhttp.send();
		}
		function SetRele8(Status)
		{
			 var xhttp = new XMLHttpRequest();
			 xhttp.onreadystatechange = function() 
			 {
				if (this.readyState == 4 && this.status == 200) 
				{
				  document.getElementById("STATUS_RELE8").innerHTML =
				  this.responseText;
				}
			  };
			 xhttp.open("GET", "SetRele8?STATUS_RELE8="+Status, true);
			 xhttp.send();
		}
		
		function vuota()
		{
		
		}
		
		setInterval(function() 
		{
		  GetTurnOnRele();
		}, 30000); //30s Seconds update rate
		 
		function GetTurnOnRele()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE";
		  var TurnOnReleFunc = "TurnOnRele";
		  for (i = 1; i < 9; i++)
		  { 
		    var xhttp = new XMLHttpRequest();
		    xhttp.onreadystatechange = function() 
		    {
			  if (this.readyState == 4 && this.status == 200) 
			  {
			    document.getElementById(TurnOnReleId+i).innerHTML =
			    this.responseText;
			  }
		    };
		    xhttp.open("GET", TurnOnReleFunc+i, true);
		    xhttp.send();
			setTimeout(vuota, 1000);
		  }		
		}
		
		setInterval(function() 
		{
		  GetTimerTimeRele();
		}, 60000); // 60s Seconds update rate
		 		
		function GetTimerTimeRele()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE";
		  var TimeReleFunc = "TimerRele";
		  for (i = 1; i < 9; i++)
		  { 
		    var xhttp = new XMLHttpRequest();
		    xhttp.onreadystatechange = function() 
		    {
			  if (this.readyState == 4 && this.status == 200) 
			  {
			    document.getElementById(TimerReleId+i).innerHTML =
			    this.responseText;
			  }
		    };
		    xhttp.open("GET", TimeReleFunc+i, true);
		    xhttp.send();
			setTimeout(vuota, 1000);
		  }
		}
	
	</script>
	<style>
	table, th, td 
		{
			border: 1px solid black;
			border-collapse: collapse;
			background-color: lavander;
		}
		th
		{
			height: 50px;
			border-bottom: 1px solid LightGrey;
			font-family:"Verdana";
			font-size:23px;
		}
		td
		{
			padding: 10px;
			text-align: center;
			border-bottom: 1px solid LightGrey;
			font-size:28px;
			font-family:Verdana;
		}
		tr:hover {background-color:#f5f5f5;}
		.button 
		{
			background-color: #6495ED;
			border: black;
			padding: 20px 60px;
			text-align: center;
			text-decoration: none;
			display: inline-block;
			font-size: 24px;
			margin: 4px 2px;
			cursor: pointer;
			border-radius: 8px;
			box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2), 0 6px 20px 0 rgba(0,0,0,0.19);
			color:white;
		}
		.button:active 
		{
			background-color: #8B0000;
			box-shadow: 0 5px #666;
			transform: translateY(3px);
		}
		h1 {
			font-size: 60px;
			font-family:"Verdana";
			color: blue;
			text-align:center;
			text-shadow: 3px 2px red;
		}
		h2 {
			font-size: 45px;
			font-family:"Verdana";
			color: MediumAquaMarine ;
			text-align:center;
			text-decoration: underline;
		}
		p {
			font-size: 13px;
			color: black;
			text-align:center;
		}
		.small_text {
			font-size: 70%;
			color: #737373;
		}
	</style>
    </head>
    <body onload="GetArduinoIO();" style="background: linear-gradient(to top, #ffffff 80%, #ccffff 100%);">
        <h1>Smart Power-Strip</h1>
		<h2>Gestione delle prese e controllo tempi</h2>
		<table style="width:100%;" border="1px solid black;">
			<tr style="background-color:LightSkyBlue ;"><th>NOME PRESA</th><th>Accensione / Spegnimento</th><th>Stato Presa</th><th>Tempo Accensione</th><th>Tempo Spegnimento Timer</th></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 1</th><td><button class="button" onclick="SetRele1(1)">Accesa</button><button class="button" onclick="SetRele1(0)">Spenta</button></td><td id="STATUS_RELE1" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE1">0g 0h 0m</td><td id="TIMER_RELE1">0h 0m</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 2</th><td><button class="button" onclick="SetRele2(1)">Accesa</button><button class="button" onclick="SetRele2(0)">Spenta</button></td><td id="STATUS_RELE2" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE2">0g 0h 0m</td><td id="TIMER_RELE2">0h 0m</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 3</th><td><button class="button" onclick="SetRele3(1)">Accesa</button><button class="button" onclick="SetRele3(0)">Spenta</button></td><td id="STATUS_RELE3" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE3">0g 0h 0m</td><td id="TIMER_RELE3">0h 0m</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 4</th><td><button class="button" onclick="SetRele4(1)">Accesa</button><button class="button" onclick="SetRele4(0)">Spenta</button></td><td id="STATUS_RELE4" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE4">0g 0h 0m</td><td id="TIMER_RELE4">0h 0m</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 5</th><td><button class="button" onclick="SetRele5(1)">Accesa</button><button class="button" onclick="SetRele5(0)">Spenta</button></td><td id="STATUS_RELE5" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE5">0g 0h 0m</td><td id="TIMER_RELE5">0h 0m</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 6</th><td><button class="button" onclick="SetRele6(1)">Accesa</button><button class="button" onclick="SetRele6(0)">Spenta</button></td><td id="STATUS_RELE6" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE6">0g 0h 0m</td><td id="TIMER_RELE6">0h 0m</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 7</th><td><button class="button" onclick="SetRele7(1)">Accesa</button><button class="button" onclick="SetRele7(0)">Spenta</button></td><td id="STATUS_RELE7" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE7">0g 0h 0m</td><td id="TIMER_RELE7">0h 0m</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 8</th><td><button class="button" onclick="SetRele8(1)">Accesa</button><button class="button" onclick="SetRele8(0)">Spenta</button></td><td id="STATUS_RELE8" style = "text-align:center">SPENTA</td><td id="TURN_ON_RELE8">0g 0h 0m</td><td id="TIMER_RELE8">0h 0m</td></tr>
		</table>
		<p style="color:blue; text-align:center; font-family:Verdana; font-size:30px;">Banda Oraria:</p> <p id="BAND_ACTIVE"> <b style="color:MediumSeaGreen;text-align:center; font-family:Verdana; font-size:35px;">Disattivata</b> </p>
		<br>
		<br>
		<footer style="text-align:center;">&copy Dario Cavedale</footer>
    </body>
</html>

)=====";

void HandleHomePage()
{
	String Page(WebPage);
	server.send(200, "text/html", Page); //Send web page
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