/*
* Nom fichier: serial_port.cpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Gere les fonctions relatives à la laison série
*				-Afficher menu de gestion des numeros de serie
*				-configurer liaison série
*				-configurer les numeros de serie des machines
*				-
*/


#include <iostream> //std::cout
#include <fstream>	//std::ifstream std::ofstream
#include <unistd.h>
#include <windows.h>
#include <sys/fcntl.h>
#include <string>
#include "ftdi/ftd2xx.h"

#include "serial_port.hpp"

using namespace std;


extern char DEBUG;



FT_HANDLE open_port(string machine){
	FT_HANDLE port;
	//mise en forme du nom du port COM
	string tty("");
	tty = tty + get_serial_num(machine[0]-48);
	
	//Ouverture du port COM
	port = FT_W32_CreateFile(tty.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FT_OPEN_BY_SERIAL_NUMBER,
			0);
	
	//Verification de l'ouverture du port COM

	if(port ==INVALID_HANDLE_VALUE)//si echec
	{
		cerr << "erreur ouverture port machine '" 
		<< machine[0]-48 << "'"<< endl;
	   
	}
	else//si success
	{
		//Configuration du port COM
		config_serial(port);
		cout << "Machine '"<< machine[0]-48 << "' connectee" << endl;
	}
	return port;
}


void config_serial(FT_HANDLE port){

	//Baudrate à 921600
	FT_SetBaudRate(port, FT_BAUD_921600);
	//1 bit de stop, pas de parité, octet de 8 bits
	FT_SetDataCharacteristics(port, 
		FT_BITS_8,
		FT_STOP_BITS_1,
		FT_PARITY_NONE);
	//pas de contrôle de flux
	FT_SetFlowControl(port, FT_FLOW_NONE,0,0);
	
	FT_ClrDtr(port);
	FT_SetDtr(port);
}


int manage_serial_num(){
	char cpt;
	string choix("");
	
	//affichage menu
	cout <<endl<< "+++++++++PORT COM MANAGER+++++++++" << endl;
	cout << "1-Configurer tous les numeros de serie (autodetect)" << endl;
	cout << "2-Configurer un numero de serie (autodetect)" <<endl;
	cout << "3-Afficher tous les numero de serie" <<endl;
	cout << endl << "++++++++++++++++++++++++++++++++++" << endl;
	cin >>choix;
	
	
	if(choix == "1"){
		
		//pour les 6 machines
		for(cpt=1;cpt<7;cpt++){
			detect_num_serial(cpt);
		}
		return 0;
		
	}
	else if(choix == "2"){
		
		int machine;
		
		//choix de la machine
		cout << "Choisir machine a configurer:" << endl;
		cin >> machine;
		
		//detection du numero de serie de la machine
		detect_num_serial(machine);
		return 0;
		
	}
	else if(choix == "3"){
		
		string temp("");
		
		//pour les 6 machines
		for(cpt=1;cpt<7;cpt++){
			
			//recuperation du numero de serie
			temp =get_serial_num(cpt);
			if(temp == ""){
				return -1;
			}
			
			//affichage du numero de serie
			cout << "Numero serie machine " << (int)cpt << ": " 
				<< temp << endl;
		}
		return 0;
		
	}	
	else{
		
		cout << "Erreur saisie" << endl;
		return 0;
	}
}

int detect_num_serial(int machine){
	string s_buf("");
	char cpt =0;
	char buf[24];
	cout << "Brancher la machine "  << machine << " sur le pc, " 
		<< "puis appuyer sur entrer" << endl;
	getchar();
	getchar();
	Sleep(1000);
	
	//tant que la machine n'as pas été détectée
	while(FT_ListDevices(0,buf,FT_LIST_BY_INDEX) != FT_OK)
	{
		cpt++;
		//si plus de 5 echec on quitte
		if(cpt == 125){
			cout << "Erreur detection" << endl;
			return -1;
		}
		Sleep(40);
	}
	
	cout << "numero serie: "<< buf << endl;
	s_buf = buf;
	//enregistrement numero de serie
	set_serial_num(machine, s_buf);
	return 0;
	
}


int set_serial_num(int machine, string num_serial){
	
	//Stockage des anciennes valeurs
	string port_ORDES1 = get_serial_num(1);
	string port_AG2    = get_serial_num(2);
	string port_ORDES3 = get_serial_num(3);
	string port_ORDES4 = get_serial_num(4);
	string port_AG5    = get_serial_num(5);
	string port_ORDES6 = get_serial_num(6);
		
	//Remplacement de la valeur choisie
	switch(machine){
		case 1:
			port_ORDES1=num_serial;
			break;
			
		case 2:
			port_AG2=num_serial;
			break;			
			
		case 3:
			port_ORDES3=num_serial;
			break;			
			
		case 4:
			port_ORDES4=num_serial;
			break;

		case 5:
			port_AG5=num_serial;
			break;

		case 6:
			port_ORDES6=num_serial;
			break;			
	}
	
	//ouverture fichier en ecriture
	ofstream fichier("config.txt",  ios::out | ios::trunc);
	if(!fichier){
		cerr << "Erreur ouverture config.txt en ecriture" << endl;
		return -1;
	}
	
	//Ecriture des nouvelles valeurs
	fichier << port_ORDES1 <<" "<<port_AG2 <<" "<< port_ORDES3
		<<" "<< port_ORDES4 <<" "<< port_AG5 <<" "<< port_ORDES6;
	fichier.clear();
	fichier.close();
	return 0;
}

string get_serial_num(int machine){
	
	
	string port_ORDES1("");
	string port_AG2("");
	string port_ORDES3("");
	string port_ORDES4("");
	string port_AG5("");
	string port_ORDES6("");
	
	//ouverture fichier en lecture
	ifstream fichier("config.txt", ios::in );
	
	if(!fichier){
		
		cerr << "Erreur ouverture config.txt en lecture" << endl;
		return "";
	}
	

	
	//Recuperation des valeurs
	fichier >> port_ORDES1 >> port_AG2 >> port_ORDES3
		>> port_ORDES4 >> port_AG5 >> port_ORDES6;
	fichier.clear();
	fichier.close();

	//Revoie de la valeur choisie
	switch(machine){
		case 1:
			return port_ORDES1;
			break;
	
		case 2:
			return port_AG2;
			break;
			
		case 3:
			return port_ORDES3;
			break;
			
		case 4:
			return port_ORDES4;
			break;
			
		case 5:
			return port_AG5;
			break;
			
		case 6:
			return port_ORDES6;
			break;
			
		default:
			return "";
			break;
	}
	
}

string get_port_com(FT_HANDLE ft_handle){
	long port;
	if(FT_GetComPortNumber(ft_handle,&port) == FT_OK){
		return to_string(port);
	}
	return "";

}






/*
int detect_all(){
	
	FT_STATUS ftStatus; 
	DWORD numDevs;
	
	
	ftStatus = FT_ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY); 
	if (ftStatus == FT_OK) { 
		// FT_ListDevices OK, number of devices connected is in numDevs 
		cout << "Device connected: " << numDevs << endl;
	} 
	else { 
		// FT_ListDevices failed 
		cout << "Device connected: ERROR"  << endl;
	}
	
	
	DWORD devIndex = 0; // first device 
	char Buffer[64]; // more than enough room!
	
	ftStatus = FT_ListDevices((PVOID)devIndex,Buffer,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER); 
	if (ftStatus == FT_OK) { 
		// FT_ListDevices OK, serial number is in Buffer 
		cout << "serial number: " << Buffer << endl;
	} 
	else { 
		// FT_ListDevices failed 
		cout << "serial number: ERROR" << endl;
	}
	
	
	char *BufPtrs[3]; // pointer to array of 3 pointers 
	char Buffer1[64]; // buffer for description of first device 
	char Buffer2[64]; // buffer for description of second device 
	// initialize the array of pointers 
	BufPtrs[0] = Buffer1; 
	BufPtrs[1] = Buffer2; 
	BufPtrs[2] = NULL; 
	
	ftStatus = FT_ListDevices(BufPtrs,&numDevs,FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION); 
	if (ftStatus == FT_OK) { 
		// FT_ListDevices OK, product descriptions are in Buffer1 and Buffer2, and 
		// numDevs contains the number of devices connected 
		cout << "Description1: " << Buffer1 << endl;
		cout << "Description2: " << Buffer2 << endl;
		cout << "Device connected: " << numDevs << endl;
	} 
	else { 
		// FT_ListDevices failed 
		cout << "Description1: ERROR" << endl;
		cout << "Description1: ERROR" << endl;
		cout << "Device connected: ERROR"  << endl;
	}
	
	return 0;
}
*/