/*
* Nom fichier: file.hpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Prototypes des fonctions de file.cpp:
*				
*/

#ifndef FILE_HPP
#define FILE_HPP


#include <iostream>
#include <string>


using namespace std;

struct Param{
		string addr = "";
		string data = "";
};

string explorateur(string type);
string open_xml_file(string extention);
Param* read_one_param_xml(string file, long index);
Param* read_one_info_xml(string file, string name, char index);
Param* read_pal_info_xml(string file, string name, char index);
Param* read_plo_info_xml(string file, string name, char index);
Param* read_paf_info_xml(string file, string name, char index);



#endif