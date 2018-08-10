/*
* Nom fichier: file.cpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Gere les fonctions relatives au fichier xml
*				-ouvrir les fichier paf, plo, pal en graphique
*				-lire le fichier xml
*/

#include <iostream>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <commdlg.h>
#include "tinyxml2/tinyxml2.h"
#include "file.hpp"


using namespace tinyxml2;
using namespace std;

extern char DEBUG;



string open_xml_file(string extention){
	
	
	cout<<"Choisir fichier ." << extention <<":" << endl;
	string chemin = explorateur(extention);
	
	//Chargement du fichier
	
	XMLDocument file;
	
	
	if(file.LoadFile( chemin.c_str() ) == 0){
		cout << "fichier: " << chemin << " trouve" << endl;
		return chemin;
	}
	else{
		cerr << "fichier introuvable" << endl;
		return "";
	}
	
}

string explorateur(string type){
	char dossier[255];
	string retour("");
	string titre = "Choisir fichier .";
	titre = titre + type;
	
	
	GetCurrentDirectoryA(255, dossier);
    OPENFILENAME ofn;
    ZeroMemory ( &ofn , sizeof ( OPENFILENAMEW ) );
    ofn.lStructSize = sizeof ( OPENFILENAMEW );
    //ofn.lpstrFile = chemin;
	ofn.lpstrFile = (char*)malloc(1024* sizeof(char));
	ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 1024;
    ofn.lpstrTitle = _T(titre.c_str());
	
	if(type == "pal"){
		 ofn.lpstrFilter = _T(".pal\0*.pal*\0\0");
	}
	else if(type == "plo"){
		 ofn.lpstrFilter = _T(".plo\0*.plo*\0\0");
	}
	else if(type == "paf"){
		 ofn.lpstrFilter = _T(".paf\0*.paf*\0\0");
	}
    ofn.Flags = OFN_LONGNAMES | OFN_EXPLORER;
    
	GetOpenFileName(&ofn); 
	SetCurrentDirectory(dossier);
 
	retour = ofn.lpstrFile;
	free(ofn.lpstrFile);
	return retour;
}


Param* read_one_param_xml(string file_chemin, long index){
	int cpt=0;
	XMLDocument file;
	XMLNode* root_node;
	XMLElement* param_elem;
	XMLElement* addr_elem = NULL; 
	XMLElement* data_elem = NULL;
	Param *para = new Param();
	string temp("");
	
	file.LoadFile( file_chemin.c_str() );
	
	//Création d'un node pointant sur <parametrage>
	root_node = file.FirstChild();
	if(root_node == NULL){
		cerr << "erreur lecture fichier" << endl;
		return NULL;
	}
	
	//Création d'un element pointant sur <parametres>
	param_elem = root_node->FirstChildElement("parametres");
	
	//Création d'un element pointant sur le premier <param>
	param_elem = param_elem->FirstChildElement("param");
	
	//Création de la structure pour stocker les données du parametre
	para->addr = "";
	para->data = "";
	
	//Parcours du fichier jusqu'a l'element <param> d'index 'cpt'
	for(cpt=0; cpt<index; cpt++){
		param_elem = param_elem->NextSiblingElement("param");
		if(param_elem == NULL){
			return NULL;
		}
	}
	//stockage de la donnee <adresse> et <hexa> 
	addr_elem = param_elem->FirstChildElement("adresse");
	para->addr = addr_elem->GetText();
		
	data_elem = param_elem->FirstChildElement("hexa");
	temp = data_elem->GetText();
	
	for(char cpt = temp.size() ; cpt < 8; cpt++){
		para->data = para->data + "0";
	}
	
	
	para->data = para->data + temp;

	if(DEBUG){
		cout << "addr=" << para->addr<< endl;
		cout << "hexa=" << para->data<< endl;
	}
	return para;
	
}

/*Param* read_one_info_xml(string file_chemin, string name, char index){

	
	string type = file_chemin.substr(file_chemin.size()-3,3);
	if(type == "pal"){
		return ( read_pal_info_xml(file_chemin, name, index) );
	}
	else if(type == "plo"){
		return ( read_plo_info_xml( file_chemin, name, index) );
	}
	else if(type == "paf"){
		return ( read_paf_info_xml( file_chemin, name, index) );
	}
	else{
		return NULL;
	}
	
}*/



Param* read_one_info_xml(string file_chemin, string name, char index){
	
	string type = file_chemin.substr(file_chemin.size()-3, 3);
	
	string* adresses = NULL;
	adresses = (string*) calloc(10,sizeof(string));
	
	if(type == "pal"){
		//crc32
		addresses[0]= "80000000";	
		//nom fichier
		addresses[1]= "80000014";	
		addresses[2]= "8000001E";	
		addresses[3]= "80000028";	
		addresses[4]= "80000032";	
		//version
		addresses[5]= "8000003C";	
		//date
		addresses[6]= "80000046";	
		addresses[7]= "80000050";	
		//application
		addresses[8]= "800000A0";	
		addresses[9]= "800000AA";	
		
	}
	else if(type == "plo"){
		//crc32
		addresses[0]= "8000000A";	
		//nom fichier
		addresses[1]= "8000005A";	
		addresses[2]= "80000064";	
		addresses[3]= "8000006E";	
		addresses[4]= "80000078";	
		//version
		addresses[5]= "80000082";	
		//date
		addresses[6]= "8000008C";	
		addresses[7]= "80000096";	
		//application
		addresses[8]= "800000B4";	
		addresses[9]= "800000BE";
	}
	else if(type == "paf"){
		//crc32
		addresses[0]= "8000000A";	
		//nom fichier
		addresses[1]= "8000005A";	
		addresses[2]= "80000064";	
		addresses[3]= "8000006E";	
		addresses[4]= "80000078";	
		//version
		addresses[5]= "80000082";	
		//date
		addresses[6]= "8000008C";	
		addresses[7]= "80000096";	
		//application
		addresses[8]= "800000B4";	
		addresses[9]= "800000BE";
	}
	
	XMLDocument file;
	XMLNode* root_node;
	XMLElement* elem;
	Param *para = new Param();
	string data_temp("");
	
	para->addr = "";
	para->data = "";
	file.LoadFile( file_chemin.c_str() );
	root_node = file.FirstChild();
	
	//crc 32
	if(name == "checksum"){
		//elem = root_node->FirstChildElement("checksumfichier");
		//data_temp = elem->GetText();
		
		para->addr = adresses[0];
		para->data = "";
	}
	
	//nom fichier
	else if(name == "fichier"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("fichier");
		data_temp = elem->GetText();
		
		for(char cpt = (data_temp.size()-1); cpt <16;cpt++){
			data_temp = data_temp + " ";
		}
		
		if(index == 0){
			para->addr = adresses[1];
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = adresses[2];
			para->data = "0000" + data_temp.substr(4,4);
		}
		else if(index == 2){
			para->addr = adresses[3];
			para->data = "0000" + data_temp.substr(8,4);
		}
		else if(index == 3){
			para->addr = adresses[4];
			para->data = "0000" + data_temp.substr(12,4);
		}
	}
	
	//version
	else if(name == "version"){
		elem = root_node->FirstChildElement("versionparametres");
		data_temp = elem->GetText();
		
		para->addr = adresses[5];
		para->data = "0000" + data_temp;
	}
	
	//date
	else if(name == "date"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("datecreation");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = adresses[6];

			para->data = "0000" + data_temp.substr(0,4);
		}
		if(index == 1){
			para->addr = adresses[7];
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
		
	//application
	else if(name == "application"){
		elem = root_node->FirstChildElement("parametres");
		elem = elem->FirstChildElement("param");
		elem = elem->FirstChildElement("ascii");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = adresses[8];
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = adresses[9];
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
	
	if(DEBUG){
		cout << "addr=" << para->addr<< endl;
		cout << "hexa=" << para->data<< endl;
	}
	
	free(adresses);
	return para;
	
}




/*Param* read_pal_info_xml(string file_chemin, string name, char index){
	
	//crc32
	//"80000000";	
	//nom fichier
	//"80000014";	
	//"8000001E";	
	//"80000028";	
	//"80000032";	
	//version
	//"8000003C";	
	//date
	//"80000046";	
	//"80000050";	
	//application
	//"800000A0";	
	//"800000AA";
	
	XMLDocument file;
	XMLNode* root_node;
	XMLElement* elem;
	Param *para = new Param();
	string data_temp("");
	
	para->addr = "";
	para->data = "";
	file.LoadFile( file_chemin.c_str() );
	root_node = file.FirstChild();
	
	//crc 32
	if(name == "checksum"){
		//elem = root_node->FirstChildElement("checksumfichier");
		//data_temp = elem->GetText();
		
		para->addr = "80000000";
		para->data = "";
	}
	
	//nom fichier
	else if(name == "fichier"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("fichier");
		data_temp = elem->GetText();
		
		for(char cpt = (data_temp.size()-1); cpt <16;cpt++){
			data_temp = data_temp + " ";
		}
		
		if(index == 0){
			para->addr = "80000014";
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = "8000001E";
			para->data = "0000" + data_temp.substr(4,4);
		}
		else if(index == 2){
			para->addr = "80000028";
			para->data = "0000" + data_temp.substr(8,4);
		}
		else if(index == 3){
			para->addr = "80000032";
			para->data = "0000" + data_temp.substr(12,4);
		}
	}
	
	//version
	else if(name == "version"){
		elem = root_node->FirstChildElement("versionparametres");
		data_temp = elem->GetText();
		
		para->addr = "8000003C";
		para->data = "0000" + data_temp;
	}
	
	//date
	else if(name == "date"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("datecreation");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = "80000046";

			para->data = "0000" + data_temp.substr(0,4);
		}
		if(index == 1){
			para->addr = "80000050";
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
		
	//application
	else if(name == "application"){
		elem = root_node->FirstChildElement("parametres");
		elem = elem->FirstChildElement("param");
		elem = elem->FirstChildElement("ascii");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = "800000A0";
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = "800000AA";
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
	
	if(DEBUG){
		cout << "addr=" << para->addr<< endl;
		cout << "hexa=" << para->data<< endl;
	}
	return para;
	
}*/



/*Param* read_plo_info_xml(string file_chemin, string name, char index){
	
	//crc32
	//"8000000A";	
	//nom fichier
	//"8000005A";	
	//"80000064";	
	//"8000006E";	
	//"80000078";	
	//version
	//"80000082";	
	//date
	//"8000008C";	
	//"80000096";	
	//application
	//"800000B4";	
	//"800000BE";	

	
	XMLDocument file;
	XMLNode* root_node;
	XMLElement* elem;
	Param *para = new Param();
	string data_temp("");
	
	para->addr = "";
	para->data = "";
	file.LoadFile( file_chemin.c_str() );
	root_node = file.FirstChild();
	
	//crc 32
	if(name == "checksum"){
		elem = root_node->FirstChildElement("checksumfichier");
		data_temp = elem->GetText();
		
		para->addr = "8000000A";
		para->data = data_temp;
	}
	
	//nom fichier
	else if(name == "fichier"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("fichier");
		data_temp = elem->GetText();
		
		for(char cpt = (data_temp.size()-1); cpt <16;cpt++){
			data_temp = data_temp + " ";
		}
		
		if(index == 0){
			para->addr = "8000005A";
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = "80000064";
			para->data = "0000" + data_temp.substr(4,4);
		}
		else if(index == 2){
			para->addr = "8000006E";
			para->data = "0000" + data_temp.substr(8,4);
		}
		else if(index == 3){
			para->addr = "80000078";
			para->data = "0000" + data_temp.substr(12,4);
		}
	}
	
	//version
	else if(name == "version"){
		elem = root_node->FirstChildElement("versionparametres");
		data_temp = elem->GetText();
		
		para->addr = "80000082";
		para->data = "0000" + data_temp;
	}
	
	//date
	else if(name == "date"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("datecreation");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = "8000008C";

			para->data = "0000" + data_temp.substr(0,4);
		}
		if(index == 1){
			para->addr = "80000096";
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
		
	//application
	else if(name == "application"){
		elem = root_node->FirstChildElement("parametres");
		elem = elem->FirstChildElement("param");
		elem = elem->FirstChildElement("ascii");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = "800000B4";
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = "800000BE";
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
	
	if(DEBUG){
		cout << "addr=" << para->addr<< endl;
		cout << "hexa=" << para->data<< endl;
	}
	return para;
	
}


/*Param* read_paf_info_xml(string file_chemin, string name, char index){
	
	//crc32
	//"80000000";
	//nom fichier
	//"80000000";
	//"80000000";
	//"80000000";
	//"80000000";
	//version
	//"80000000";
	//date
	//"80000000";
	//"80000000";
	//application
	//"80000000";
	//"80000000";
	
	
	XMLDocument file;
	XMLNode* root_node;
	XMLElement* elem;
	Param *para = new Param();
	string data_temp("");
	
	para->addr = "";
	para->data = "";
	file.LoadFile( file_chemin.c_str() );
	root_node = file.FirstChild();
	
	//crc 32
	if(name == "checksum"){
		elem = root_node->FirstChildElement("checksumfichier");
		data_temp = elem->GetText();
		
		para->addr = "80000000";
		para->data = data_temp;
	}
	
	//nom fichier
	else if(name == "fichier"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("fichier");
		data_temp = elem->GetText();
		
		for(char cpt = (data_temp.size()-1); cpt <16;cpt++){
			data_temp = data_temp + " ";
		}
		
		if(index == 0){
			para->addr = "80000014";
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = "8000001E";
			para->data = "0000" + data_temp.substr(4,4);
		}
		else if(index == 2){
			para->addr = "80000028";
			para->data = "0000" + data_temp.substr(8,4);
		}
		else if(index == 3){
			para->addr = "80000032";
			para->data = "0000" + data_temp.substr(12,4);
		}
	}
	
	//version
	else if(name == "version"){
		elem = root_node->FirstChildElement("versionparametres");
		data_temp = elem->GetText();
		
		para->addr = "8000003C";
		para->data = "0000" + data_temp;
	}
	
	//date
	else if(name == "date"){
		elem = root_node->FirstChildElement("identification");
		elem = elem->FirstChildElement("datecreation");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = "80000046";

			para->data = "0000" + data_temp.substr(0,4);
		}
		if(index == 1){
			para->addr = "80000050";
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
		
	//application
	else if(name == "application"){
		elem = root_node->FirstChildElement("parametres");
		elem = elem->FirstChildElement("param");
		elem = elem->FirstChildElement("ascii");
		data_temp = elem->GetText();
		
		if(index == 0){
			para->addr = "800000A0";
			para->data = "0000" + data_temp.substr(0,4);
		}
		else if(index == 1){
			para->addr = "800000AA";
			para->data = "0000" + data_temp.substr(4,4);
		}
	}
	
	if(DEBUG){
		cout << "addr=" << para->addr<< endl;
		cout << "hexa=" << para->data<< endl;
	}
	return para;
	
}
*/