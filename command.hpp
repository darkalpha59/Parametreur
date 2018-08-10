/*
* Nom fichier: commande.hpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Prototypes des fonctions de commande.cpp:
*				
*/

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <windows.h>
#include <pthread.h>
#include "ftdi/ftd2xx.h"

typedef unsigned char uchar;


using namespace std;

struct Arg{
	
		string num_machine;
		FT_HANDLE port;
		//char type_f = 1;
		//char type_c = 1;
		//char type_nc = 1;
		string paf_file ="";
		string plo_file ="";
		string pal_file ="";
		char run_type = 0;// 0:unused 1:used
		string crc_32_paf = "";
		string crc_32_plo = "";
		string crc_32_pal = "";
		

};



class Machine{
	
	private:
		
	Arg *argu = new Arg();;
	pthread_t th_machine;	
	int ret = (int)malloc(sizeof(int));

		
	public:
	
	Machine();
	Machine(string num_machine);
	Machine(string num_machine, char lck);
	~Machine();
	int load_file();
	int run();
	int Display_result();
	
	
};


int load_all_machine();

void *thread_machine(void* arg);

int set_machine(Arg *argu);


int set_connexion(FT_HANDLE port);
int set_clean_mod(FT_HANDLE port);
int set_operator_mod(FT_HANDLE port);
int set_param(FT_HANDLE port,string file, string crc32, string type);
int send_entete(FT_HANDLE port,string file, int entity, string crc32);
int send_param(FT_HANDLE port,string file, int entity);

#endif
