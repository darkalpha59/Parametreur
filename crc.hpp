/*
* Nom fichier: crc.hpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Prototypes des fonctions de crc.cpp:
*				
*/

#ifndef CRC_HPP
#define CRC_HPP

#include <iostream>
#include <string>

using namespace std;
typedef unsigned char uchar;

string crc16(uchar* Tab);
int get_uchar_lenght(uchar* tab);


#endif
