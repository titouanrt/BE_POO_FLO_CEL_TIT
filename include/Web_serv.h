#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <stdexcept>
#include "CustomExceptions.h"

    inline String BoolToString(bool b)
    {
      return b ? "true" : "false";
    }
class WebServ{
    private:
      static bool step_3;
      static bool step_4;
      static bool step_5;
      static String index_html;
      static String ouverture_html;
      static String solutions;
      static String style;
      static String script;
      static ESP8266WebServer server;
      static IPAddress myIP;
public:

    //Création du point d'accès wifi
    static void ConnectWiFi(String ssid, String passwd){
        Serial.println();
        Serial.print("Configuring access point...");
        if(!WiFi.softAP(ssid,passwd))
        {
          Serial.print("Could not start access point !");
          throw WiFiExceptions();
        }
        myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);

    }

    static void update(){
      server.handleClient();
    }
    
    //actualisation du code javascript pour mettre à jour les variables
    static void updateStates(){
      script = R"rawliteral(
var ouvert = 0;

function ChangeToGreen(cell) {
    cell.style.backgroundColor = "green";
}
function ChangeToRed(cell) {
    cell.style.backgroundColor = "red";
}

setInterval(CheckOpen, 500);

var CPP_FLAG_1 =)rawliteral"+ BoolToString(step_3) +R"rawliteral(;
var CPP_FLAG_2 =)rawliteral"+ BoolToString(step_4) +R"rawliteral(;
var CPP_FLAG_3 =)rawliteral"+ BoolToString(step_5) +R"rawliteral(;

function CheckOpen(){
    const Cell1 = document.querySelector(".cell1");
    const Cell2 = document.querySelector(".cell2");
    const Cell3 = document.querySelector(".cell3");
    const Cell4 = document.querySelector(".cell4");

    if(CPP_FLAG_1)
        ChangeToGreen(Cell1); 
    else
        ChangeToRed(Cell1); 
    if(CPP_FLAG_2)
        ChangeToGreen(Cell2); 
    else
        ChangeToRed(Cell2);
    if(CPP_FLAG_3)
        ChangeToGreen(Cell3); 
    else
        ChangeToRed(Cell3);
    
    //ouverture du coffre
    if(CPP_FLAG_1 && CPP_FLAG_2 && CPP_FLAG_3)
    {
        ouvert = 1;
        confetti({
            particleCount: 100,
            spread: 70,
            origin: { y: 0.6 },
          });
    }
}

const duration = 15 * 1000,
  animationEnd = Date.now() + duration,
  defaults = { startVelocity: 30, spread: 360, ticks: 60, zIndex: 0 };

function randomInRange(min, max) {
  return Math.random() * (max - min) + min;
}

function start_confetti(){
    const duration = 5000; // 5 seconds
    const animationEnd = Date.now() + duration;

const interval = setInterval(function() {
  const timeLeft = animationEnd - Date.now();

  if (timeLeft <= 0) {
    return clearInterval(interval);
  }

  const particleCount = 50 * (timeLeft / duration);

  // since particles fall down, start a bit higher than random
  confetti(
    Object.assign({}, defaults, {
      particleCount,
      origin: { x: randomInRange(0.1, 0.3), y: Math.random() - 0.2 },
    })
  );
  confetti(
    Object.assign({}, defaults, {
      particleCount,
      origin: { x: randomInRange(0.7, 0.9), y: Math.random() - 0.2 },
    })
  );
}, 250);
}
)rawliteral";
    }

    static void SetStep3(bool s3){
      step_3 = s3;
    }
    
    static void SetStep4(bool s4){
      step_4 = s4;
    }
    
    static void SetStep5(bool s5){
      step_5 = s5;
    }

    //envoi des pages web
    static void handleRoot() {
        server.send(200, "text/html", String(index_html.c_str()));
    }

    static void handleOuv() {
        server.send(200, "text/html", String(ouverture_html.c_str()));
    }

    static void handleSol() {
        server.send(200, "text/html", String(solutions.c_str()));
    }

    static void handleCSS() {
        server.send(200, "text/css", String(style.c_str()));
    }

    static void handleScr() {
        server.send(200, "text/script", String(script.c_str()));
    }

    static void begin(){
      server.send(200, "text/html", String(index_html.c_str()));
      server.send(200, "text/html", String(ouverture_html.c_str()));
      server.send(200, "text/html", String(solutions.c_str()));
      server.send(200, "text/css", String(style.c_str()));
      server.send(200, "text/script", String(script.c_str()));
      //définition de l'adresse des pages web
      server.on("/", handleRoot);
      server.on("/index.html", handleRoot);
      server.on("/ouverture.html", handleOuv);
      server.on("/solutions.html", handleSol);
      server.on("/style.css", handleCSS);
      server.on("/script.js", handleScr);
      server.begin(80);
    }
};