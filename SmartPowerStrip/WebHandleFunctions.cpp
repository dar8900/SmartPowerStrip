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

#include "WebHandleFunctions.h"

extern RELE Rele[];
extern TIME_DATE_FORMAT PresentTime;
extern FLAGS Flag;
extern BAND_FORMAT Band;
ESP8266WebServer server(80);

extern String VersionValue;
extern String VersionDate;

const char WebPage[] PROGMEM = R"=====(

	<!DOCTYPE html>
<html>
    <head>
        <title>Smart Power-Strip</title>
		<meta charset="UTF-8">
        <script>
		// var DoOnce;
		// function GetReleStatus()
		// {
		//   if(!DoOnce)
		//   {
		// 	  var i;
		// 	  var ReleStatusId = "BAND_ACTIVE";
		// 	  var ReleStatusFunc = "BandStatus";
		// 	  var xhttp = new XMLHttpRequest();
		// 	  xhttp.onreadystatechange = function()
		// 	  {
		// 		if (this.readyState == 4 && this.status == 200)
		// 		{
		// 		  document.getElementById(ReleStatusId).innerHTML =
		// 		  this.responseText;
		// 		}
		// 	  };
		// 	  xhttp.open("GET", ReleStatusFunc, true);
		// 	  xhttp.send();
		// 	  DoOnce = true;
		//   }
	   // }


//////////////////////////////////  FUNZIONI PER ACCENSIONE ///////////////////////////////////

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
			 xhttp.open("GET", "SetRele2?STATUS_RELE2="+Status, true);
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

		setInterval(function()
		{
		  GetBandStatus();
		}, 30000);

		function GetBandStatus()
		{
		  var i;
		  var BandStatusId = "BAND_ACTIVE";
		  var BandStatusFunc = "BandStatus";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(BandStatusId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", BandStatusFunc, true);
		  xhttp.send();
		}

		function GetFWInfo()
		{
		  var i;
		  var FWInfoId = "FW_INFO";
		  var FWInfoFunc = "GetFWInfo";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(FWInfoId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", FWInfoFunc, true);
		  xhttp.send();
		}

//////////////////////////////////  FUNZIONI PER TURN ON ///////////////////////////////////

		setInterval(function()
		{
		  GetTurnOnRele1();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele1()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE1";
		  var TurnOnReleFunc = "TurnOnRele1";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}

		setInterval(function()
		{
		  GetTurnOnRele2();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele2()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE2";
		  var TurnOnReleFunc = "TurnOnRele2";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}

	    setInterval(function()
		{
		  GetTurnOnRele3();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele3()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE3";
		  var TurnOnReleFunc = "TurnOnRele3";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}
				setInterval(function()
		{
		  GetTurnOnRele4();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele4()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE4";
		  var TurnOnReleFunc = "TurnOnRele4";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}
				setInterval(function()
		{
		  GetTurnOnRele5();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele5()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE5";
		  var TurnOnReleFunc = "TurnOnRele5";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}
				setInterval(function()
		{
		  GetTurnOnRele6();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele6()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE6";
		  var TurnOnReleFunc = "TurnOnRele6";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}
				setInterval(function()
		{
		  GetTurnOnRele7();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele7()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE7";
		  var TurnOnReleFunc = "TurnOnRele7";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}
				setInterval(function()
		{
		  GetTurnOnRele8();
		}, 8000); //30s Seconds update rate

		function GetTurnOnRele8()
		{
		  var i;
		  var TurnOnReleId = "TURN_ON_RELE8";
		  var TurnOnReleFunc = "TurnOnRele8";
		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			if (this.readyState == 4 && this.status == 200)
			{
			  document.getElementById(TurnOnReleId).innerHTML =
			  this.responseText;
			}
		  };
		  xhttp.open("GET", TurnOnReleFunc, true);
		  xhttp.send();
		}

//////////////////////////////////  FUNZIONI PER TIMER ///////////////////////////////////

		setInterval(function()
		{
		  GetTimerTimeRele1();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele1()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE1";
		  var TimeReleFunc = "TimerRele1";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

		}

		setInterval(function()
		{
		  GetTimerTimeRele2();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele2()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE2";
		  var TimeReleFunc = "TimerRele2";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

		}

				setInterval(function()
		{
		  GetTimerTimeRele3();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele3()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE3";
		  var TimeReleFunc = "TimerRele3";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

		}
				setInterval(function()
		{
		  GetTimerTimeRele4();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele4()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE4";
		  var TimeReleFunc = "TimerRele4";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

		}
				setInterval(function()
		{
		  GetTimerTimeRele5();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele5()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE5";
		  var TimeReleFunc = "TimerRele5";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

		}
				setInterval(function()
		{
		  GetTimerTimeRele6();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele6()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE6";
		  var TimeReleFunc = "TimerRele6";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

		}
				setInterval(function()
		{
		  GetTimerTimeRele7();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele7()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE7";
		  var TimeReleFunc = "TimerRele7";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

		}
				setInterval(function()
		{
		  GetTimerTimeRele8();
		}, 8000); // 60s Seconds update rate

		function GetTimerTimeRele8()
		{
		  var i;
		  var TimerReleId = "TIMER_RELE8";
		  var TimeReleFunc = "TimerRele8";

		  var xhttp = new XMLHttpRequest();
		  xhttp.onreadystatechange = function()
		  {
			  if (this.readyState == 4 && this.status == 200)

			   document.getElementById(TimerReleId).innerHTML =
			   this.responseText;

		  };
		  xhttp.open("GET", TimeReleFunc, true);
		  xhttp.send();

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
    <body
	style="background: linear-gradient(to top, #ffffff 80%, #ccffff 100%);">
        <h1>Smart Power-Strip</h1>
		<h2>Gestione delle prese e controllo tempi</h2>
		<table style="width:100%;" border="1px solid black;">
			<tr style="background-color:LightSkyBlue ;"><th>NOME PRESA</th><th>Accensione / Spegnimento</th><th>Stato Presa</th><th>Tempo Accensione</th><th>Orario Spegnimento Timer</th></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 1</th><td><button class="button" onclick="SetRele1(1)">Accesa</button><button class="button" onclick="SetRele1(0)">Spenta</button></td><td id="STATUS_RELE1" style = "text-align:center"></td><td id="TURN_ON_RELE1">Attesa Dati</td><td id="TIMER_RELE1">Attesa Dati</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 2</th><td><button class="button" onclick="SetRele2(1)">Accesa</button><button class="button" onclick="SetRele2(0)">Spenta</button></td><td id="STATUS_RELE2" style = "text-align:center"></td><td id="TURN_ON_RELE2">Attesa Dati</td><td id="TIMER_RELE2">Attesa Dati</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 3</th><td><button class="button" onclick="SetRele3(1)">Accesa</button><button class="button" onclick="SetRele3(0)">Spenta</button></td><td id="STATUS_RELE3" style = "text-align:center"></td><td id="TURN_ON_RELE3">Attesa Dati</td><td id="TIMER_RELE3">Attesa Dati</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 4</th><td><button class="button" onclick="SetRele4(1)">Accesa</button><button class="button" onclick="SetRele4(0)">Spenta</button></td><td id="STATUS_RELE4" style = "text-align:center"></td><td id="TURN_ON_RELE4">Attesa Dati</td><td id="TIMER_RELE4">Attesa Dati</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 5</th><td><button class="button" onclick="SetRele5(1)">Accesa</button><button class="button" onclick="SetRele5(0)">Spenta</button></td><td id="STATUS_RELE5" style = "text-align:center"></td><td id="TURN_ON_RELE5">Attesa Dati</td><td id="TIMER_RELE5">Attesa Dati</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 6</th><td><button class="button" onclick="SetRele6(1)">Accesa</button><button class="button" onclick="SetRele6(0)">Spenta</button></td><td id="STATUS_RELE6" style = "text-align:center"></td><td id="TURN_ON_RELE6">Attesa Dati</td><td id="TIMER_RELE6">Attesa Dati</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 7</th><td><button class="button" onclick="SetRele7(1)">Accesa</button><button class="button" onclick="SetRele7(0)">Spenta</button></td><td id="STATUS_RELE7" style = "text-align:center"></td><td id="TURN_ON_RELE7">Attesa Dati</td><td id="TIMER_RELE7">Attesa Dati</td></tr>
			<tr><th style="background-color:hsl(9, 100%, 80%);">PRESA 8</th><td><button class="button" onclick="SetRele8(1)">Accesa</button><button class="button" onclick="SetRele8(0)">Spenta</button></td><td id="STATUS_RELE8" style = "text-align:center"></td><td id="TURN_ON_RELE8">Attesa Dati</td><td id="TIMER_RELE8">Attesa Dati</td></tr>
		</table>
		<p style="color:blue; text-align:center; font-family:Verdana; font-size:30px;">Banda Oraria:</p> <p style="color:MediumSeaGreen;text-align:center; font-family:Verdana; font-size:40px;" id="BAND_ACTIVE">Controllo in corso...</p>
		<br>
		<br>
		<p style="color:blue; text-align:center; font-family:Verdana; font-size:20px;">Firmware Info: </p> <p style="color:MediumSeaGreen;text-align:center; font-family:Verdana; font-size:40px;" id="FW_INFO">Controllo in corso...</p>
		<br>
		<br>
		<footer style="text-align:center;">&copy Dario Cavedale</footer>
    </body>
</html>

)=====";

void HandleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void HandleHomePage()
{
	String Page(WebPage);
	Flag.ClientConnected = true;
	server.send(200, "text/html", Page); //Send web page
}


void HandleStatus1()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_1].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}

void HandleStatus2()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_2].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}
void HandleStatus3()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_3].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}
void HandleStatus4()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_4].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}
void HandleStatus5()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_5].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}
void HandleStatus6()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_6].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}
void HandleStatus7()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_7].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}
void HandleStatus8()
{
	CheckEvents();
	String StatusStr;
	if(Rele[RELE_8].IsActive)
		StatusStr = "ACCESA";
	else
		StatusStr = "SPENTA";

	server.send(200, "text/plane", StatusStr); //Send to client ajax request
}

void HandleBandStatus()
{
	String BandStatusStr;
	if(Flag.IsBandSetted)
	{
		BandStatusStr = "SETTATA ";
		BandStatusStr += "La fascia è dalle ";
		String BandTime1 = String(Band.InitHour) + ":";
		String BandTime2 = String(Band.EndHour) + ":";
		if(Band.InitMinute < 10)
			BandTime1 += "0" + String(Band.InitMinute);
		else
			BandTime1 += String(Band.InitMinute);
		if(Band.EndMinute < 10)
			BandTime2 += "0" + String(Band.EndMinute);
		else
			BandTime2 += String(Band.EndMinute);

		BandStatusStr += BandTime1 + " alle " + BandTime2;
	}

	else
		BandStatusStr = "NON SETTATA";

	server.send(200, "text/plane", BandStatusStr); //Send to client ajax request
}

void HandleFWInfo()
{
	String FWStr;
	FWStr = "Numero Versione: " + VersionValue + "   Data Rilascio: " + VersionDate;
	server.send(200, "text/plane", FWStr); //Send to client ajax request
}

void HandleTimer1()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_1].HaveTimer && Rele[RELE_1].IsActive)
		TimerStr = String(Rele[RELE_1].TimerTime.hour) + ":" + String(Rele[RELE_1].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer2()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_2].HaveTimer && Rele[RELE_2].IsActive)
		TimerStr = String(Rele[RELE_2].TimerTime.hour) + ":" + String(Rele[RELE_2].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer3()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_3].HaveTimer && Rele[RELE_3].IsActive)
		TimerStr = String(Rele[RELE_3].TimerTime.hour) + ":" + String(Rele[RELE_3].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer4()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_4].HaveTimer && Rele[RELE_4].IsActive)
		TimerStr = String(Rele[RELE_4].TimerTime.hour) + ":" + String(Rele[RELE_4].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer5()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_5].HaveTimer && Rele[RELE_5].IsActive)
		TimerStr = String(Rele[RELE_5].TimerTime.hour) + ":" + String(Rele[RELE_5].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer6()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_6].HaveTimer && Rele[RELE_6].IsActive)
		TimerStr = String(Rele[RELE_6].TimerTime.hour) + ":" + String(Rele[RELE_6].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer7()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_7].HaveTimer && Rele[RELE_7].IsActive)
		TimerStr = String(Rele[RELE_7].TimerTime.hour) + ":" + String(Rele[RELE_7].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTimer8()
{
	CheckEvents();
	String TimerStr;
	if(Rele[RELE_8].HaveTimer && Rele[RELE_8].IsActive)
		TimerStr = String(Rele[RELE_8].TimerTime.hour) + ":" + String(Rele[RELE_8].TimerTime.minute);
	else
		TimerStr = "NESSUN TIMER";

	server.send(200, "text/plane", TimerStr); //Send to client ajax request
}

void HandleTurnOn1()
{
	Flag.ClientConnected = true;
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_1].IsActive)
		TurnOnStr = String(Rele[RELE_1].ActiveTime.day) + "g " + String(Rele[RELE_1].ActiveTime.hour) + "h " + String(Rele[RELE_1].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleTurnOn2()
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_2].IsActive)
		TurnOnStr = String(Rele[RELE_2].ActiveTime.day) + "g " + String(Rele[RELE_2].ActiveTime.hour) + "h " + String(Rele[RELE_2].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn3()
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_3].IsActive)
		TurnOnStr = String(Rele[RELE_3].ActiveTime.day) + "g " + String(Rele[RELE_3].ActiveTime.hour) + "h " + String(Rele[RELE_3].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn4()
{
	Flag.ClientConnected = true;
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_4].IsActive)
		TurnOnStr = String(Rele[RELE_4].ActiveTime.day) + "g " + String(Rele[RELE_4].ActiveTime.hour) + "h " + String(Rele[RELE_4].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn5()
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_5].IsActive)
		TurnOnStr = String(Rele[RELE_5].ActiveTime.day) + "g " + String(Rele[RELE_5].ActiveTime.hour) + "h " + String(Rele[RELE_5].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn6()
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_6].IsActive)
		TurnOnStr = String(Rele[RELE_6].ActiveTime.day) + "g " + String(Rele[RELE_6].ActiveTime.hour) + "h " + String(Rele[RELE_6].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}
void HandleTurnOn7()
{
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_7].IsActive)
		TurnOnStr = String(Rele[RELE_7].ActiveTime.day) + "g " + String(Rele[RELE_7].ActiveTime.hour) + "h " + String(Rele[RELE_7].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleTurnOn8()
{
	Flag.ClientConnected = true;
	CheckEvents();
	String TurnOnStr;
	if(Rele[RELE_8].IsActive)
		TurnOnStr = String(Rele[RELE_8].ActiveTime.day) + "g " + String(Rele[RELE_8].ActiveTime.hour) + "h " + String(Rele[RELE_8].ActiveTime.minute) + "m";
	else
		TurnOnStr = "SPENTA";

	server.send(200, "text/plane", TurnOnStr); //Send to client ajax request
}

void HandleRele1()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE1"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_1].IsActive)
		{
			ON(ReleIdx2Pin(RELE_1)); //LED ON
			Rele[RELE_1].IsActive = true;
			Rele[RELE_1].TurnOnTime.day = PresentTime.day;
			Rele[RELE_1].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_1].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_1].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_1, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_1].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_1)); //LED OFF
			Rele[RELE_1].IsActive = false;
			Rele[RELE_1].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_1].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_1, STATUS_OFF);
		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}

void HandleRele2()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE2"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_2].IsActive)
		{
			ON(ReleIdx2Pin(RELE_2)); //LED ON
			Rele[RELE_2].IsActive = true;
			Rele[RELE_2].TurnOnTime.day = PresentTime.day;
			Rele[RELE_2].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_2].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_2].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_2, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_2].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_2)); //LED OFF
			Rele[RELE_2].IsActive = false;
			Rele[RELE_2].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_2].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_2, STATUS_OFF);
		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}
void HandleRele3()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE3"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_3].IsActive)
		{
			ON(ReleIdx2Pin(RELE_3)); //LED ON
			Rele[RELE_3].IsActive = true;
			Rele[RELE_3].TurnOnTime.day = PresentTime.day;
			Rele[RELE_3].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_3].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_3].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_3, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_3].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_3)); //LED OFF
			Rele[RELE_3].IsActive = false;
			Rele[RELE_3].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_3].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_3, STATUS_OFF);
		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}

void HandleRele4()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE4"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_4].IsActive)
		{
			ON(ReleIdx2Pin(RELE_4)); //LED ON
			Rele[RELE_4].IsActive = true;
			Rele[RELE_4].TurnOnTime.day = PresentTime.day;
			Rele[RELE_4].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_4].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_4].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_4, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_4].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_4)); //LED OFF
			Rele[RELE_4].IsActive = false;
			Rele[RELE_4].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_4].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_4, STATUS_OFF);
		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}

void HandleRele5()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE5"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_5].IsActive)
		{
			ON(ReleIdx2Pin(RELE_5)); //LED ON
			Rele[RELE_5].IsActive = true;
			Rele[RELE_5].TurnOnTime.day = PresentTime.day;
			Rele[RELE_5].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_5].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_5].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_5, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_5].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_5)); //LED OFF
			Rele[RELE_5].IsActive = false;
			Rele[RELE_5].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_5].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_5, STATUS_OFF);

		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}

void HandleRele6()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE6"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_6].IsActive)
		{
			ON(ReleIdx2Pin(RELE_6)); //LED ON
			Rele[RELE_6].IsActive = true;
			Rele[RELE_6].TurnOnTime.day = PresentTime.day;
			Rele[RELE_6].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_6].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_6].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_6, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_6].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_6)); //LED OFF
			Rele[RELE_6].IsActive = false;
			Rele[RELE_6].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_6].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_6, STATUS_OFF);
		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}

void HandleRele7()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE7"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_7].IsActive)
		{
			ON(ReleIdx2Pin(RELE_7)); //LED ON
			Rele[RELE_7].IsActive = true;
			Rele[RELE_7].TurnOnTime.day = PresentTime.day;
			Rele[RELE_7].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_7].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_7].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_7, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_7].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_7)); //LED OFF
			Rele[RELE_7].IsActive = false;
			Rele[RELE_7].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_7].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_7, STATUS_OFF);
		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}

void HandleRele8()
{
	String LedStatus = "OFF";
	String t_state = server.arg("STATUS_RELE8"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	Flag.ClientConnected = true;
	if(t_state == "1")
	{
		if(!Rele[RELE_8].IsActive)
		{
			ON(ReleIdx2Pin(RELE_8)); //LED ON
			Rele[RELE_8].IsActive = true;
			Rele[RELE_8].TurnOnTime.day = PresentTime.day;
			Rele[RELE_8].TurnOnTime.hour = PresentTime.hour;
			Rele[RELE_8].TurnOnTime.minute = PresentTime.minute;
			Rele[RELE_8].ActiveTime.minute = 0;
			LedStatus = "ACCESA"; //Feedback parameter
			SaveReleStatus(RELE_8, STATUS_ON);
		}
		else
			LedStatus = "GIA ACCESA"; //Feedback parameter
	}
	else
	{
		if(Rele[RELE_8].IsActive)
		{
			OFF(ReleIdx2Pin(RELE_8)); //LED OFF
			Rele[RELE_8].IsActive = false;
			Rele[RELE_8].ActiveTime = SetTimeVarRele(0,0,0,0);
			Rele[RELE_8].TurnOnTime = SetTimeVarRele(0,0,0,0);
			LedStatus = "SPENTA"; //Feedback parameter
			SaveReleStatus(RELE_8, STATUS_OFF);
		}
		else
			LedStatus = "GIA SPENTA"; //Feedback parameter
	}
	server.send(200, "text/plane", LedStatus); //Send web page
}
