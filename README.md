<h1>Execution du projet</h1>
Afin de porter le programme sur l'ESP 8266, nous avons fait le choix d'utiliser Visual Studio Code au dépens de l'IDE Arduino.<br>
Cela, pour avoir un explorateur de fichiers directement intégré au logiciel de développement et une aide à la complétion plus efficace.<br>
L'utilisation de VSCode implique l'installation de l'extension <a href="https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide">platform.io</a>.<br>
Après avoir téléversé le projet sur l'ESP 8266, un réseau WIFI nommé <b>ESPap</b> est créé, il faut s'y connecter et entrer le mot de passe <b>thereisnospoon</b> et se rendre à l'adresse 192.168.4.1
<h1>Solution du projet</h1>
<h3>Capteur de toucher et bouton poussoir</h3>
La première sécurité combine 2 capteurs. En moins de 10 secondes vous devez réaliser la suite d'appuis suivante :<br>
<li> Maintenir son doigt sur le capteur de toucher<br></li>
<li> Appuyer sur le bouton poussoir 2 fois</li>

<h3>Potentiomètre</h3>
Tourner le potentiomètre jusqu'à atteindre une tension se situant entre 1.5V et 2V.

<h3>Capteur de température</h3>
La température autour du coffre doit atteindre les 30 degrés.<br><br>

A chaque fois qu'une sécurité a été passée, il faut actualiser la page <b>Statut du Coffre</b> pour visualiser les sécurités déverrouillées.<br>
Une fois toutes les sécurités passées, toutes les cases du tableau devraient passer au vert et afficher des confettis à l'écran.
