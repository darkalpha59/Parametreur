/*
* Nom programme: Programmateur_BGS
* Version: V0.1.2
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Programme les parametres du BGS
* Sources S:\50-R&D\0-Gestion\14-Echange\23-JKO\Parametreur_BGS\Codes
*/

/*
* Nom fichier: Prog.cpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Fichier principal du programme
*				-Menu principal du programme
*/


#include <iostream>
#include "command.hpp"
#include "serial_port.hpp"


using namespace std;


char DEBUG =1;
char LOCK = 0;



int main(){
	string rep("");
	
	
	
	system("CLS");
	

	while(1){
		if(!DEBUG){system("CLS");}
		//affichage menu
		cout << endl << "+++++++++MENU+++++++++" << endl;
		cout << "1- Charger parametres" << endl;
		cout << "2- Deverouiller" << endl;
		cout << "3- Configurer numero de serie" << endl;
		cout << "4- Activer/desactiver DEBUG" << endl;
		cout << "5- quitter" << endl;
		cout << endl << "++++++++++++++++++++++" << endl;
		
		cin >> rep;
		
		system("CLS");
		if(rep == "1"){
			//chargement des parametres
			load_all_machine();
		}
		else if(rep == "2"){
			//chargement des parametres
			LOCK = 1;
			load_all_machine();
			LOCK =0;
		}
		else if(rep == "3"){
			//gestion des numeros de serie
			manage_serial_num();
		}
		else if(rep == "4"){
			//activation du mode debug
			DEBUG = !DEBUG;
			cout << "DEBUG = " << (int)DEBUG << endl;
		}
		else if(rep == "5"){
			//quitter
			break;
		}
		
		
	}
	
	
	return 0;
}
