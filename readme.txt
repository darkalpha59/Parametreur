V0.0.4
6/08/2018
Compatibilité Windows7 pro SP1
g++ v6.3.0
tinyxml2 v6.0.0

compiler:
g++ -o prog *.o -ltinyxml2 -lComdlg32 -lftd2xx -lpthread
g++ -o X.o -c X.c -Wextra -Wall -Werror



Changement:

-implementation adresse info different fichier



Fait:
-Menu
-liaison serie 921600 baud/s, 1 stopbit, no flow control, no parity 
-Stockage des port COM dans un fichier
-Modification des port COM dans le programme
-Selection fichier en mode graphique
-lecture fichier xml (pal,plo,paf)
-mise en forme et envoie des trames
-incrementation de la date
-calcul crc16 pour la trame
-reception de la trame
-verification ACK!
-threads pour envoie et reception des trames
-stockage machine avec numero de serie

A verifier:


amelioration à prévoir:
-Verification du type d'erreur de la trame
-verification crc16 de la trame
-verification de la date de la trame
