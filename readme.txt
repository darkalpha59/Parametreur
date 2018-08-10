V1.0.1
10/08/2018
Compatibilité Windows7 pro SP1
g++ v6.3.0
tinyxml2 v6.0.0

compiler:
g++ -o prog *.o -ltinyxml2 -lComdlg32 -lftd2xx -lpthread
g++ -o X.o -c X.c -Wextra -Wall -Werror



Changement:

-utilisation fihcier .ini
-amelioration



amelioration à prévoir:
-Verification du type d'erreur de la trame
-verification crc16 de la trame
-verification de la date de la trame
