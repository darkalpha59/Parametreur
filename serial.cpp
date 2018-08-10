/*
* Nom fichier: serial.cpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Gere les fonctions relatives aux trames
*				-envoyer une trame
*				-recevoir une trame et la verifier
*/


#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <sys/fcntl.h>
#include <string>
#include "ftdi/ftd2xx.h"

#include "serial.hpp"
#include "crc.hpp"

using namespace std;


extern char DEBUG;

uchar date_ent1 = 1;//%
uchar date_ent2 = 1;//&
uchar date_snmt = 0;
uchar date_ihm = 0;



int send_cmd(FT_HANDLE port,string recept,string cmd,string data){
	string ret= "DONE";
	
	if( send_trame(port, recept, cmd, data) == -1){return -1;}
	
	
	ret = verify_trame(receive_trame(port));
	if(ret == "DONE"){return 0;}
	else if (ret == "TIMEOUT"){return -1;}//aucune reponse
	else if (ret == "ERROR"){return -1;}//si autre erreur 
	
	//si erreur de date
	else if (ret == "DATE"){
		
		while(1){
			//renvoie de la trame
			if( send_trame(port, recept, cmd, data) == -1){return -1;}
			ret = verify_trame(receive_trame(port));
			
			if(ret == "DONE"){return 0;}//si ACK!
			else if(ret != "DATE"){return -1;}//si autre erreur
			//else if(cpt == 254){return -1;}
			//else{cpt++;}
		}
	}
	
	//si erreur de...
	//else if(){}
	
	else{ return 0;}
}

int send_trame(FT_HANDLE port,string recept,string cmd,string data){
	
	uchar* u_trame;
	string trame("");
	uchar date;
	string crc_16;
	DWORD BytesWritten = 0;
	
	//recuperation de la date correspondante
	if(recept == "M"){date = date_snmt;}
	if(recept == "%"){date = date_ent1;}
	if(recept == "&"){date = date_ent2;}
	
	//assemblage des elements de la trame
	trame += "I"+recept+(char)0+ cmd +data;
	
	//Convertion pour calcul crc16
	u_trame = (uchar*)trame.c_str();
	//ajout de la date
	u_trame[2] = date;
	//calcul crc16
	crc_16 = crc16( u_trame );
	
	//ajout du crc16 et de 2 emplacement pour les octets de fin
	trame = trame + crc_16 + "ff";
	
	//conversion pour trame finale
	u_trame = (uchar*)trame.c_str();
	//ajout de la date
	u_trame[2] = date;
	//ajout des octets de fin
	u_trame[trame.size()-2] = 10;
	u_trame[trame.size()-1] = 13;
	
	if(DEBUG){
		if(cmd != "IDEN"){
			display_trame(u_trame);
		}
	}

	//Envoie de la trame
	if(FT_W32_WriteFile(port,u_trame,trame.size(),&BytesWritten,NULL) ){
		if (trame.size() != BytesWritten){ 
			cerr << "Erreur ecriture port COM: Timeout" << endl;
			return -1;
		} 
	} 
	else{ 
		cerr << "Erreur ecriture port COM" << endl;
		return -1;
	} 
	
	
	//incrementation date
	if(recept == "M"){
		if(date_snmt == 255){date_snmt =0;}
		else{date_snmt++;}
	}
	else if(recept == "%"){
		if(date_ent1 == 255){date_ent1 =0;}
		else{date_ent1++;}
	}
	else if(recept == "&"){
		if(date_ent2 == 255){date_ent2 =0;}
		else{date_ent2++;}
	}
	
	
	return 0;
}



void display_trame(uchar* tram){
	//stringstream stream;
	unsigned int i;
	unsigned int taille = get_uchar_lenght(tram);

	cout << "trame: " << (int)tram[2] << endl;
	
	//affichage destinataire
	if(tram[1] == '%'){
		cout << "to entite 1 " << tram[1] << endl;
	}
	else if(tram[1] == 'M'){
		cout << "to snmt " << tram[1] << endl;
	}
	else if(tram[1] == '&'){
		cout << "to entite 2 " << tram[1] << endl;
	}
	
	//affichage en ascii
	
	cout<<"ascii: ";

    for(i = 0; i<taille; i++)
    {
        cout <<"_" << tram[i];  
    }
	
	//affichage en hexadecimal
	cout << endl << "ascii (hex):";
	for(i=0;i<taille; i++){
			
		cout << "_" << hex << (int)tram[i];	

	}
	
	cout << endl <<"___________________"<<endl;

}



string receive_trame(FT_HANDLE port){
	
	
	FT_STATUS ftstatus;
	string msg("");
	DWORD dwRead;
	DWORD dwWaiting;
	uchar cpt=0;
	char* temp= NULL;
	
	Sleep(10);// voir temps de reponse moyen
	while(1){
		FT_GetQueueStatus(port, &dwWaiting);
		if(dwWaiting>0){break;}
		else{
			if( cpt == 250){return "timeout";};

			cpt++;
			Sleep(20);
		}
		
	}
	
	cpt =0;
	temp = (char*)calloc(2,sizeof(char));
	
	while(1){
		//on lit 1 caractere de la trame recue
		ftstatus = FT_W32_ReadFile(port,temp,1,&dwRead,NULL);
		cpt++;
		msg=msg +temp[0];
		//if(DEBUG){cout << temp;}
		
		//si erreur de reception
		if(ftstatus == 0){ 
			cerr << "Erreur lecture port COM: " 
			<< ftstatus << endl;
			free (temp);
			return "";
		}
		
		//si on atteint la fin de la trame
		else if(temp[0] == 0x0D && cpt > 7){//fin de trame
			if(DEBUG){
				cout << endl << "msg: '" << msg << "'"<< endl;
			}
			free (temp);
			return msg;
		}
	
	}
	return "";
	
}

/*uchar* receive_trame(FT_HANDLE port){
	
	
	FT_STATUS ftstatus;
	uchar* msg = (uchar*)malloc(250*sizeof(uchar));
	memset(msg,0,200);
	DWORD dwRead;
	uchar cpt=0;
	char* temp= (char*)malloc(2*sizeof(char));
	memset(temp,0,2);
	
	while(1){
		ftstatus = FT_W32_ReadFile(port,temp,1,&dwRead,NULL);
		msg[cpt]=temp[0];
		if(DEBUG){cout << temp;}
		
		if(ftstatus == 0){ //si erreur
			cerr << "Erreur lecture port COM: " 
			<< ftstatus << endl;
			free (temp);
			return NULL;
		}
		
		else if(msg[cpt] == 0x0D && cpt != 2){//fin de trame
			if(DEBUG){cout << endl << "msg: " << msg << endl;}
			free (temp);
			return msg;
		}
		cpt++;
	
	}
	
}
*/


string verify_trame(string trame){
	//-1 si crc mauvais
	//-1 si crc bon et NACK
	// 0 si crc bon et ACK
	
	//retourne "DONE" si ok ou le messagte d'erreur
	if(trame == "" || trame == "timeout"){return "ERROR";}
	else if(verify_ACK(trame)==-1){
		return tell_error(trame);
	}
	else{ return "DONE";}
	

}


/*int verify_trame(uchar* trame){
	//-1 si crc mauvais
	//-1 si crc bon et NACK
	// 0 si crc bon et ACK
	
	if(verify_crc(trame) == -1){
		return -1;
	}
	
	string tram = (char*)trame;
	free(trame);
	if(verify_ACK(tram)==-1){
		if(VERIF){tell_error(tram);}
		return -1;
	}
	else if(trame == NULL){
		return -1;
	}
	else{ return 0;}

}
*/

/*int verify_crc(string trame){
	string trame_begin;
	string crc_16;
	string new_crc_16;
	stringstream stream;
	vector<uchar> vec;

	//recuperer trame sans crc et bits de fin
	long taille = trame.size() -12;//taille sans crc et octets de fin
	stream.clear();
	stream.str("");
	stream << trame.substr(0,taille);
	stream >> trame_begin;



	//calculer crc
	new_crc_16 = crc16( trame_begin );

	//recuperer crc transmit
	stream.clear();
	stream.str("");
	stream << trame.substr(taille,8);
	stream >> crc_16;


	if(new_crc_16 != crc_16){
		if(DEBUG){
			cerr << "crc non verifié" <<endl;
		}
		return -1;//erreur crc16
	}
	if(DEBUG){
		cout << "crc verifié" <<endl;
	}
	return 0;

}
*/

int verify_ACK(string trame){
	uchar pos;
		
	pos = trame.find("I",0);//trouver 'I' / 0x49
	
	if(trame.substr(pos+2,4) != "ACK!"){
		cerr << "NACK: " << trame.substr(pos+2,8) <<endl;
		return -1;
	}
	if(DEBUG){
		//cout << "ack verifie" <<endl;
	}
	return 0;
	
}

string tell_error(string trame){

	return trame.substr( (trame.find("I",0)+5),4) ;
	
	
}


