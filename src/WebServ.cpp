#include <WString.h>>
#include "Web_serv.h"

bool WebServ::step_3 = false;
bool WebServ::step_4 = false;
bool WebServ::step_5 = false;
ESP8266WebServer WebServ::server = ESP8266WebServer();
IPAddress WebServ::myIP = IPAddress();
String WebServ::index_html = R"rawliteral(
<!DOCTYPE html>
<html>
 <head>
    <title>Coffre fort</title>
<link href="style.css" rel="stylesheet"/>
    <script src="script.js" defer></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js@2.9.3/dist/Chart.min.js"></script>;
    <script src="https://cdn.jsdelivr.net/npm/@tsparticles/confetti@3.0.3/tsparticles.confetti.bundle.min.js"></script>;

    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/devibeans.min.css">;
    <script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>;
    
    <script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/languages/go.min.js"></script>;
    
    <script>hljs.highlightAll();</script>
 </head>
 <body>
    <ul class="menu">  
        <nav>
            <a href="index.html">Structure du projet</a>
            <a href="ouverture.html">Statut du coffre</a>
            <a href="solutions.html">Solutions</a>
            <div class="animation"></div>
        </nav>
    </ul>
    <div class="titre">Présentation du projet</div>
    <br><br>
    <div class="texte">
        Cette page montre les choix de développement du projet.<br>
        Le schéma ci-dessous illustre la structure adoptée pour la rédaction de notre librairie personnalisée<br>
  <h3><u>Classe Device</u></h3>
  <pre><code class="language-cpp">  
  class Device
  {
    protected:
      int8_t pin;
    public:
      Device(int8_t pin):pin(pin){};
      ~Device(){};
  };  
  </code></pre>
    <h3><u>Classe Sensor</u></h3>
    <pre><code class="language-cpp">
  class Sensor: public Device
  {
    public:
      Sensor(int8_t pin):Device(pin)
      {
        pinMode(pin, INPUT);
      };
      ~Sensor(){};
  };
    </code></pre>

    <h3><u>Classe Actuator</u></h3>
    <pre><code class="language-cpp">
  class Actuator: public Device
  {
    public:
      Actuator(int8_t pin):Device(pin)
      {
        pinMode(pin, OUTPUT);
      };
      ~Actuator(){};
  };
        </code></pre>

    </div>
 </body>
</html>
)rawliteral";

String WebServ::ouverture_html = R"rawliteral(
<!DOCTYPE html>
<html>
 <head>
    <title>Coffre fort</title>
    <link href="style.css" rel="stylesheet"/>
    <script src="script.js" defer></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js@2.9.3/dist/Chart.min.js"></script>;
    <script src="https://cdn.jsdelivr.net/npm/@tsparticles/confetti@3.0.3/tsparticles.confetti.bundle.min.js"></script>;
 </head>
  <body>
    <ul class="menu">  
        <nav>
    
            <a href="index.html">Structure du projet</a>
            <a href="ouverture.html">Statut du coffre</a>
            <a href="solutions.html">Solutions</a>
            <div class="animation"></div>
          </nav>
    </ul>

  <br>
<div class="texte">
<h2><u>Visualisation des sécurités désactivées</u></h2>
<table>
    <thead>
      <tr>
        <th scope="col">Sécurité</th>
        <th scope="col">Statut</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <th scope="row">secu 1</th>
        <td class="cell1"></td>
      </tr>
      <tr>
        <th scope="row">secu 2</th>
        <td class="cell2"></td>
      </tr>
      <tr>
        <th scope="row">secu 3</th>
        <td class="cell3"></td>
      </tr>
    </tbody>
  </table>
  

</div>

  </body>
</html>
  )rawliteral";
String WebServ::solutions = R"rawliteral(
<!DOCTYPE html>
<html>
 <head>
    <title>Coffre fort</title>
    <link href="style.css" rel="stylesheet"/>
    <script src="script.js" defer></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js@2.9.3/dist/Chart.min.js"></script>;
    <script src="https://cdn.jsdelivr.net/npm/@tsparticles/confetti@3.0.3/tsparticles.confetti.bundle.min.js"></script>;
 </head>
  <body>
    <ul class="menu">  
        <nav>
    
            <a href="index.html">Structure du projet</a>
            <a href="ouverture.html">Statut du coffre</a>
            <a href="solutions.html">Solutions</a>
            <div class="animation"></div>
          </nav>
    </ul>

  <br>
<div class="texte">
<h2><u>Solutions à chaque sécurité du coffre</u></h2>
Après la validation de chaque étape, le tableau de la page <b><a href="ouverture.html" id="ref_ouverture">statut du coffre</a></b> doit s'actualiser en passant les rectangles correspondant en vert.
<h3>Capteur de toucher et bouton poussoir</h3>
La première sécurité combine 2 capteurs. En moins de 10 secondes vous devez réaliser la suite d'appuis suivante :<br>
<li> Maintenir son doigt sur le capteur de toucher<br></li>
<li> Appuyer sur le bouton poussoir 2 fois</li>
<li> Retirer son doigt du capteur de toucher</li>
<li> Appuyer sur le bouton poussoir 1 fois</li>

<h3>Potentiomètre</h3>
Tourner le potentiomètre jusqu'à atteindre la valeur désirée et appuyer sur le bouton poussoir , cette dernière est affichée sur l'écran LCD.<br>
Le code de cette sécurité est 4925. (possibilité de mettre un systeme pour le générer aléatoirement)

<h3>Capteur de température</h3>
La température autour du coffre doit atteindre les X degrés. Dès qu'elle est atteinte la LED s'allume.
</div>

  </body>
</html>
)rawliteral";

String WebServ::style = R"rawliteral(

body{
  font-family: Geneva, sans-serif;
  color:rgb(0, 0, 0);
  background-color: rgb(230, 197, 238);
}
nav {
    margin: 0px auto 0;
    position: relative;
    width: 900px;
    height: 50px;
    border-radius: 8px;
    font-size: 0;
    text-align: center;
    background-color: rgba(213, 37, 37, 0.845) ;
  }
  .menu
  {
    width: 100%;
    background-color: rgb(212, 40, 44) ;
  }
  nav a {
    line-height: 50px;
    height: 100%;
    font-size: 15px;
    display: inline-block;
    position: relative;
    z-index: 1;
    text-decoration: none;
    text-transform: uppercase;
    text-align: center;
    color: white;
    cursor: pointer;
  }
  nav .animation {
    position: absolute;
    height: 100%;
    top: 0;
    z-index: 0;
    transition: all .5s ease 0s;
    border-radius: 8px;
  }
  a:nth-child(1) {
    width: 300px;
  }
  a:nth-child(2) {
    width: 300px;
  }
  a:nth-child(3) {
    width: 300px;
  }

  nav .start-home, a:nth-child(1):hover~.animation {
    width: 300px;
    left: 0;
    background-color: #e34d4d;
  }
  nav .start-about, a:nth-child(2):hover~.animation {
    width: 300px;
    left: 300px;
    background-color: #e34d4d;
  }
  nav .start-blog, a:nth-child(3):hover~.animation {
    width: 300px;
    left: 600px;
    background-color: #e34d4d;
  }

  

  
  p {
      position: absolute;
      bottom: 20px;
      width: 100%;
      text-align: center;
      color: #FFFFFF;
      font-family: 'Cherry Swash',cursive;
      font-size: 16px;
  }
  


h1{
  text-align: center;
}
h2{
  font-size: 130%;
  font-weight: bold;
}

.titre{
  font-size: 150%;
  text-align: center;
  font-weight:500 ;
  color: #F00;
}

  .texte{
    text-decoration: none;
    margin-left:7%;
    margin-right:7%;

    font-size: 90%;
    display: block;
    color: #000;
  }

code{
  display:block;
  background-color:black;
  margin-left:3%;
  margin-right:3%;
  font-size:100%;
}

table {
    border-collapse: collapse;
    border: 2px solid rgb(0, 0, 0);
    font-family: sans-serif;
    font-size: 1.2rem;
    letter-spacing: 1px;
    margin-left: auto;
    margin-right: auto;
  }
  
  thead {
    background-color: rgb(172, 172, 172);
  }
  
  th,
  td {
    border: 2px solid rgb(0, 0, 0);
    padding: 40px 200px;
  }
  
  
#ref_ouverture{
  color:black;
  text-decoration:none;
  
}
)rawliteral";

String WebServ::script = R"rawliteral(
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