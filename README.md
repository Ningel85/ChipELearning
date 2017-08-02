# ChipELearning
Projet ChipELearning



* Download & Install Arduino IDE: https://www.arduino.cc/en/Main/Software 
* Download & Install Teensy Driver : https://www.pjrc.com/teensy/td_download.html
* Copy all libraries to C:\Users\user\Documents\Arduino\libraries (or wherever your library folder is installed)
* Arduino IDE : "Board: Teensy 3.2" 

ChipELearning is a robot able to learn movements, which makes every ChipELearning unique !

Bill of material :
* 1 x [Teensy 3.2](http://www.francerobotique.com/electronique/457-module-teensy-32.html) 
* 1 x Teensy Shield	
* 1 x [Maestro Micro](http://www.francerobotique.com/contr%C3%B4leurs-interfaces/138-contr%C3%B4leur-usb-micro-maestro-6.html)	
* 2 x [Servomotor FeedBack Small](https://www.adafruit.com/product/1449)	
* 4 x [Servomotor FeedBack Medium](https://www.adafruit.com/product/1404)
* 1 x [Accelerometer LSM303D](http://www.francerobotique.com/orientation/244-acc%C3%A9l%C3%A9rom%C3%A8tre-3d-lsm303d.html?search_query=LSM303D%09&results=2)	
* 1 x [IR sensor TSOP38238](http://www.francerobotique.com/luminosit%C3%A9-couleur/420-r%C3%A9cepteur-ir-tsop38238.html?search_query=TSOP38238%09&results=1)	
* 1 x [Remote control](http://www.francerobotique.com/t%C3%A9l%C3%A9commande/426-mini-t%C3%A9l%C3%A9commande.html)	
* 1 x [I2C display 128x32](https://www.adafruit.com/product/931)	
* 1 x [Regulator step-down 5V/3A](https://www.adafruit.com/product/1385)	
* 2 x [Battery 1S](http://www.francerobotique.com/batterie/302-batterie-li-ion-37v-1300mah-lb-040.html)	
* 2 x [Battery Charger](http://www.francerobotique.com/chargeur/303-chargeur-de-batterie-li-ion-lbb-040.html)

- The Teensy controller has various I/Os : I²C, UART, CAN, Touch, ADC, PWM.
It has an EEPROM big enough for the robot and is smaller than Arduino Uno and Arduino Mega.
- The Infrared sensor allows me to control the robot with a remote controller.
- The display is small but still has a decent resolution to write text and display drawings, logos, etc. Wiring the display is very simple as it uses an I²C bus to communicate.
- Touch I/Os from the Teensy board allows me to interact with the robot without the use of a push button. On ChipELearning, there is a paper fastener but anything else that conducts electricity is suitable.
- Servomotors send back their position, which allows the robot to memorize sequences of movements and play them back indefinitely.
- The accelerometer is used for the "Balance" mode. But it could also detect when it's falling or if it is upside down.
- The robot is powered by 2 Robotis batteries made of one cell each, connected in series. They could be replaced by any power supply up to 16V.
A power adapter allows to generate a 5V supply for the Teensy controller and the servos.


Functionalities :
- Remote Controller :
ChipELearning can be controlled using a remote controller.
- Walking/Motions :
The robot can move forwards, backwards and can rotate. 3 sequences of movements are predefined, and binded to keys 1, 2 and 3.
- Motion learning :
Motion learning allows the robot to do anything. Jumping, Skateboarding, etc. This feature makes any robot unique.
- Balance :
The robot keeps it's balance on it's foot.


Programme TeensyChipE V1.0  TeensyChipE.ino

=================================================================
==============================================================================


ChipELearning est un robot capable d'apprendre des mouvements ce qui rend chaque ChipELearning unique! 

Bill of material :
* 1 x [Teensy 3.2](http://www.francerobotique.com/electronique/457-module-teensy-32.html) 
* 1 x Teensy Shield	
* 1 x [Maestro Micro](http://www.francerobotique.com/contr%C3%B4leurs-interfaces/138-contr%C3%B4leur-usb-micro-maestro-6.html)	
* 2 x [Servomotor FeedBack Small](https://www.adafruit.com/product/1449)	
* 4 x [Servomotor FeedBack Medium](https://www.adafruit.com/product/1404)
* 1 x [Accelerometer LSM303D](http://www.francerobotique.com/orientation/244-acc%C3%A9l%C3%A9rom%C3%A8tre-3d-lsm303d.html?search_query=LSM303D%09&results=2)	
* 1 x [IR sensor TSOP38238](http://www.francerobotique.com/luminosit%C3%A9-couleur/420-r%C3%A9cepteur-ir-tsop38238.html?search_query=TSOP38238%09&results=1)	
* 1 x [Remote control](http://www.francerobotique.com/t%C3%A9l%C3%A9commande/426-mini-t%C3%A9l%C3%A9commande.html)	
* 1 x [I2C display 128x32](https://www.adafruit.com/product/931)	
* 1 x [Regulator step-down 5V/3A](https://www.adafruit.com/product/1385)	
* 2 x [Battery 1S](http://www.francerobotique.com/batterie/302-batterie-li-ion-37v-1300mah-lb-040.html)	
* 2 x [Battery Charger](http://www.francerobotique.com/chargeur/303-chargeur-de-batterie-li-ion-lbb-040.html)

- Le contrôleur Teensy possède des entrées sorties très variées, I2C, UART, CAN, Touch, ADC, PWM, possède une EEPROM suffisante pour le robot et est plus petite que les arduino uno et mega.
- Le capteur Infrarouge me permet de contrôler le robot avec une télécommande
- L'afficheur est petit mais a une bonne résolution pour écrire beaucoup de texte et afficher des dessins, logos... Le câblage est simple et le module communique en I2C, ce qui est très facile à utiliser
- Les I/O touch de la carte Teensy me permet d’interagir avec le robot sans bouton poussoir. Sur ChipELearning, il y a une attache parisienne mais on peut y mettre tout type d'objet conducteur. 
- Le feedback des servomoteurs retourne leur information de position. Cette option permet au robot de mémoriser les séquences de mouvement pour les refaire indéfiniment.
- L'accéléromètre est utilisé pour le mode équilibre. Mais il pourrait aussi savoir si il tombe, si il est à l'envers...
- Le robot est alimenté par 2 batteries Robotis d'une cellule chacune. Elles sont branchées en série. Un régulateur permet d'abaisser la tension à 5V pour le contrôleur Teensy et les servomoteurs. On peut y connecter une batterie de 16V maximum.


Fonctionnalités:
- Télécommande
ChipELearning peut-être contrôlé grâce à une télécommande. Avec les flèches directionnelles, des modes de jeux et d'apprentissage ainsi que 9 mouvements.
- Déplacement/Mouvement
Le robot peut se déplacer en avant, en arrière et tourner sur lui même. 3 séquences de mouvements sont pré-enregistrées et associées aux touches 1, 2 et 3.
- Apprentissage de mouvement
L'apprentissage des mouvements permet au robot de réaliser n'importe quelle action. Sauter, Faire du skate... Cette fonction rend le robot unique!
- Équilibre
Le robot reste en équilibre sur son pied. Exemple d'utilisation de l'accéléromètre


Programme TeensyChipE V1.0  TeensyChipE.ino

