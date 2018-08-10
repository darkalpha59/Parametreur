/*
* Nom fichier: serial.hpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Prototypes des fonctions de serial.cpp:
*				
*/

#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <iostream>
#include <string>
//#include <windows.h>
#include "ftdi/ftd2xx.h"

using namespace std;

typedef unsigned char uchar;


int send_cmd(FT_HANDLE port,string recept,string cmd,string data);

int send_trame(FT_HANDLE port , string recept,string cmd,string data);
void display_trame(uchar* tram);

string receive_trame(FT_HANDLE port);
//uchar* receive_trame(FT_HANDLE port);
string verify_trame(string trame);
//int verify_trame(uchar* trame);
//int verify_crc(string trame);
int verify_ACK(string trame);
string tell_error(string trame);


#endif
