# Automatic-temperature-control-
Contrôler  automatiquement  la  température  
ce projet utulise un microcontroleur PIC  pour controler automatiquement la température d'une zone , cette zone pourrait etre une petite usine , une maison ou tout autre lieu qui nécessite une termpérature controlée comme un incubateur (oeufs ) par exemple .
 
 - entrer votre température desiree par le kepad (# = entrer , 0 = pour effacer et corriger votre nombre saisi , * = pour changer la valeur )
 - le capteur de température LM35 va mesurer la valeur de T 
 - le PIC va prendre la valeur de zone mesurée par le capteur et faire une comparaison et donner des comandes 
 - si T de zone > T desiree === ventilateur va se demarer 
 - si T de zone < T desiree === chauffage va se demarer
 - si T > 40 === buzzer et led rouge va se clignoter  


- SOFTWARE : 

           -MIKROC FOR PIC 
           -PORTEUS 8 PROFESSIONEL
           
- HARDWARE : 
               
           - PIC18F45K22
           - BUZZER
           - TEMPERATURE SENSOR 
           - LED RED 
           - LCD 
           - KEYBAD 9 BUTTON 
           - 2 RELAIS 
           - VENTLATEUR OU CHAUFFAGE 
             
- SCHEMA PROTEUSE :
![image](https://user-images.githubusercontent.com/80831555/115291937-df3b8500-a144-11eb-91cc-04a0451ff3fa.png)


- PCB LAYOUT : 
![image](https://user-images.githubusercontent.com/80831555/115292134-1c077c00-a145-11eb-88c5-654af9eef9b4.png)


- 3D VISUALISATION : 
![image](https://user-images.githubusercontent.com/80831555/115292370-64269e80-a145-11eb-98fc-7cc3b6985ad5.png)
