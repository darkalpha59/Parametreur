/*
* Nom fichier: commande.cpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Gere les fonctions relatives au deroulement general du programme
*				-affiche menu des chargement des parametres
*				-gerer les threads pour l'envoie des trame
*				-gerer l'ordre d'envoie des parametres
*				-gestion des differents types de trames
*/

//system header
#include <iostream>
#include <string>
#include <windows.h>
#include <pthread.h>

//extern header
#include "tinyxml2/tinyxml2.h"
#include "ftdi/ftd2xx.h"

//program header
#include "command.hpp"
#include "serial.hpp"
#include "serial_port.hpp"
#include "file.hpp"

using namespace tinyxml2;
using namespace std;

extern char DEBUG;
extern char LOCK;

/*++++++++Machine class++++++++*/

Machine::Machine(){}

Machine::Machine(string num_machine){

	argu->num_machine = num_machine;
	
}

Machine::Machine(string num_machine, char r_type){

	argu->num_machine = num_machine;
	argu->run_type = r_type;
}

	
Machine::~Machine(){
	//pthread_mutex_destroy(&(argu->mutex));
	delete argu;
}
	

	
int Machine::load_file(){
	char temp;
		
	cout << "MACHINE " << argu->num_machine << endl;
	
	
	
	cout << "Programmer param fonctionnels .paf (1/0): " << endl;
	cin >> temp;
	if(temp == '1'){
		argu->paf_file = open_xml_file("paf");
		cout << "Renseigner le crc32: " << endl;
		cin >> argu->crc_32_paf;
	}
	else{argu->paf_file = "";}
			
			
	cout << "Programmer param critique .pal (1/0): " << endl;
	cin >> temp;
	if(temp == '1'){
		argu->pal_file =open_xml_file("pal");
		cout << "Renseigner le crc32: " << endl;
		cin >> argu->crc_32_pal;
	}
	else{argu->pal_file != "";}
		
				
	cout << "Programmer param non critique .plo (1/0): " << endl;
	cin >> temp;
	if(temp == '1'){
		argu->plo_file = open_xml_file("plo");
		cout << "Renseigner le crc32: " << endl;
		cin >> argu->crc_32_plo;
	}
	else{argu->plo_file != "";}
	
	
	return 0;
}
	
int Machine::run(){
		
	if(pthread_create(&th_machine, NULL, thread_machine, (void *)argu) == -1){
		cerr << "erreur creation thread machine" << endl;
		ret = 1;
		return -1;
	}
	return 0;
}
	
	
int Machine::Display_result(){
	
	if(ret != 1){//si non echec du thread machine
	
		pthread_join(th_machine, (void **)&ret);
	}
	
	cout << "Machine " << argu->num_machine 
	<< " :" << dec << ret << " " ;
	
	switch(ret){
		
		case 0:
			cout << "Done";
			break;
		
		case 1:
			cout << "Erreur creation thread machine";
			break;
			
		case 2:
			cout << "Erreur ouverture port";
			break;
			
		case 3:
			cout << "Erreur trame connexion";
			break;
			
		case 4:
			cout << "Erreur trame passage mode degradé";
			break;
			
		case 5:
			cout << "Erreur trame passage operator";
			break;			
			
		case 6:
			cout << "Erreur trame parametrage";
			break;
			
		case 61:
			cout << "Erreur chargement parametres critique pal";
			break;	
			
		case 62:
			cout << "Erreur chargement parametres non critique plo";
			break;	
			
		case 63:
			cout << "Erreur chargement parametres fonctionnel paf";
			break;
				
		default:
			cout << "Erreur";
			break;
	}
	
	cout << endl;
	return 0;
}
	

/*++++++++command main function++++++++*/



int load_all_machine(){
	char load_1 = 0, load_2 = 0, load_3 = 0;
	char load_4 = 0, load_5 = 0, load_6 = 0;
	char temp;
	uchar cpt;
	string question(""); 
	
	if(LOCK){question = "Deverouiller machine ";}
	else{question = "Programmer machine ";}
	
	char *addr[7] ={&load_1,&load_2,&load_3,&load_4,&load_5,&load_6};
	
	
	for(cpt =0; cpt <6 ;cpt++){
		cout << question << cpt+1 << " ?(1/0)" <<endl;
		cin >> temp;
		if(temp == '1'){*(addr[cpt]) = 1;}
	}
	
	//parametrage des actions sur les machines
	//et chargement des fichiers xml
	Machine machine1("1",load_1);
	Machine machine2("2",load_2);
	Machine machine3("3",load_3);
	Machine machine4("4",load_4);
	Machine machine5("5",load_5);
	Machine machine6("6",load_6);
	
	
	if(load_1 && !LOCK){machine1.load_file();}
	if(load_2 && !LOCK){machine2.load_file();}
	if(load_3 && !LOCK){machine3.load_file();}
	if(load_4 && !LOCK){machine4.load_file();}
	if(load_5 && !LOCK){machine5.load_file();}
	if(load_6 && !LOCK){machine6.load_file();}
	
	cout << "Mettre les machines sous tension" << endl;
	system("PAUSE");
	//lencement de la programmation des machines
	if(load_1){machine1.run();}
	
	if(load_2){machine2.run();}
	if(load_3){machine3.run();}
	if(load_4){machine4.run();}
	if(load_5){machine5.run();}
	if(load_6){machine6.run();}
	
	
	//system("CLS");
	//attente des retours
	if(load_1){machine1.Display_result();}
	
	if(load_2){machine2.Display_result();}
	if(load_3){machine3.Display_result();}
	if(load_4){machine4.Display_result();}
	if(load_5){machine5.Display_result();}
	if(load_6){machine6.Display_result();}
	
	
	return 0;
	
	
}
	
	
/*++++++++Main Threads++++++++*/

void* thread_machine(void* arg){
	Arg *argu = (Arg*)arg;
		
	//ouvrir port
	argu->port = open_port( argu->num_machine );
	if(argu->port==INVALID_HANDLE_VALUE){
		FT_W32_CloseHandle(argu->port);
		return (void *)2;
	}
	cout<< "port configure" << endl;
	
	
	//connexion
	
	if(set_connexion(argu->port) == -1){
		cerr << "Erreur: connexion" << endl;
		FT_W32_CloseHandle(argu->port);
		return (void *)3;
	}
	
	
	//operateur
	
	if(set_operator_mod(argu->port) == -1){
		cerr << "Erreur: passage mode opérateur" << endl;
		FT_W32_CloseHandle(argu->port);
		return (void *)5;
	}
		
		
	int ret=0;
	
	if(LOCK){
		if(set_clean_mod(argu->port) == -1){
			cerr << "Erreur: passage mode dégradé"<<endl;
			return (void *)4;
		}	
	}
	else{
		ret = set_machine(argu);
	}

	
		
	//fermeture port
	FT_W32_CloseHandle(argu->port);
		
	//fin thread_machine
	return (void *)ret;
}


/*++++++++command function++++++++*/

int set_machine(Arg *argu){
	//3 type de fichier
		
	//if(argu->type_c){
	if(argu->pal_file != ""){
		if(set_param(argu->port, argu->pal_file, argu->crc_32_pal, "pal") == -1){
			cerr << "Erreur: chargement parametres" <<endl;
			return 61;
		}
	}
	
	//if(argu->type_nc){
	if(argu->plo_file != ""){
		if(set_param(argu->port, argu->plo_file, argu->crc_32_plo, "plo") == -1){
			cerr << "Erreur: chargement parametres" <<endl;
			return 62;
		}
	}
	
	//if(argu->type_f){
	if(argu->paf_file != ""){
		if(set_param(argu->port, argu->paf_file, argu->crc_32_paf, "paf") == -1){
			cerr << "Erreur: chargement parametres" <<endl;
			return 63;
		}
	}
	cout << "chargement parametres effectue" << endl;
	
	return 0;
}
	

/*++++++++command sub_function++++++++*/

int set_connexion(FT_HANDLE port){
	//if(DEBUG){cout <<endl<< "++++++Connexion++++++" <<endl;}
	//envoie trame de connexion
	
	if(send_cmd(port,"M","IDEN","") == -1){
		cerr<<"nouvelle tentative connexion"<<endl;
		
		if(send_cmd(port,"M","IDEN","") == -1){
			cerr<<"erreur connexion"<<endl;
			return -1;
		}
	}
	
	
	return 0;
}

int set_clean_mod(FT_HANDLE port){
	if(DEBUG){cout <<endl<< "++++++Deverrouillage mode degrade++++++" <<endl;}
	
	//envoie trame de deverouillage
		
	if(send_cmd(port,"%","DEVE","00009696") == -1){
		return -1;
	}
	
	if(send_cmd(port,"&","DEVE","00009696") == -1){
		return -1;
	}
	return 0;

}


int set_operator_mod(FT_HANDLE port){
	if(DEBUG){cout <<endl<< "++++++Passage mode operateur++++++" <<endl;}
	//envoie trame de passage en opérateur
	
	if(send_cmd(port,"M","DMOP","") == -1){
		return -1;
	}
	if(verify_trame(receive_trame(port)) != "DONE"){
		//return -1;
	}
	
	return 0;
	
}



int set_param(FT_HANDLE port,string file, string crc32, string type){
	//les 2 entite
	
	
	/********entite 1*****/
	
	if(send_cmd(port,"%","PARA","DEB") == -1){
		return -1;
	}
		
	if(send_entete(port, file,1, crc32) ){
		return -1;
	}	
	
	if(send_param(port, file,1) ){
		return -1;
	}
		
	if(type == "paf"){
		if(send_cmd(port,"%","PARA","FINF") == -1){return -1;}
	}	
	else if(type == "pal"){
		if(send_cmd(port,"%","PARA","FINC") == -1){return -1;}
	}
	else if(type == "plo"){
		if(send_cmd(port,"%","PARA","FINN") == -1){return -1;}
	}
	
	
	
	/********entite 2*****/
	
	if(send_cmd(port,"&","PARA","DEB") == -1){
		return -1;
	}
		
	if(send_entete(port, file,2,crc32) ){
		return -1;
	}
	
	if(send_param(port, file,2) ){
		return -1;
	}
		
		
	if(type == "paf"){
		if(send_cmd(port,"&","PARA","FINF") == -1){return -1;}
	}	
	else if(type == "pal"){
		if(send_cmd(port,"&","PARA","FINC") == -1){return -1;}
	}
	else if(type == "plo"){
		if(send_cmd(port,"&","PARA","FINN") == -1){return -1;}
	}
			
	
	return 0;
}


int send_entete(FT_HANDLE port,string file, int entity, string crc32){
	
	string id_ent[3];
	id_ent[1]= "%";
	id_ent[2]= "&";
	
	Param *para = NULL;
	
	
	
	
	//<checksum>
		
	para = read_one_info_xml(file,"checksum",0);
	
	if(send_cmd(port,id_ent[entity],"PARA",(para->addr)+crc32) == -1){
		return -1;
	}
	
	
	//<version>
	para = read_one_info_xml(file,"version",0);
					
	if(send_cmd(port,id_ent[entity],"PARA",(para->addr)+(para->data)) == -1){
		return -1;
	}
	
	//<fichier>
	for(uchar cpt =0; cpt<4; cpt++){
		
		para = read_one_info_xml(file,"fichier",cpt);
		
		
				
		if(send_cmd(port,id_ent[entity],"PARA",(para->addr)+(para->data)) == -1){
			return -1;
		}
	
	}
	
	//<date>
	for(uchar cpt =0; cpt<2; cpt++){
		
		para = read_one_info_xml(file,"date",cpt);
						
		if(send_cmd(port,id_ent[entity],"PARA",(para->addr)+(para->data)) == -1){
			return -1;
		}
	
	}
	
	//<application>
	for(uchar cpt =0; cpt<2; cpt++){
		
		para = read_one_info_xml(file,"application",cpt);
						
		if(send_cmd(port,id_ent[entity],"PARA",(para->addr)+(para->data)) == -1){
			return -1;
		}
	
	}
	
	return 0;
	
}



int send_param(FT_HANDLE port,string file, int entity){
	Param *para = NULL;
	long cpt =1;
	string id_ent[3];
	id_ent[1]= "%";
	id_ent[2]= "&";
	cout << endl;

		while(1){
			para = read_one_param_xml(file,cpt);
			cpt++;
			cout << "|" ;
			if(para == NULL){ 
				if(DEBUG){cout << endl << "fin fichier" << endl;}
				break;
			}
			
				
			if(send_cmd(port,id_ent[entity],"PARA",(para->addr)+(para->data)) == -1){
				return -1;
			}
			
		}
		//delete(para);

		return 0;
}

