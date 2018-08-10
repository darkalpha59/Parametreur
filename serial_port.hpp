/*
* Nom fichier: serial_port.hpp
* Date: 26/07/2018
* Auteur: KOENIG Julien
* Description: Prototypes des fonctions de serial_port.cpp:
*				
*/

#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include <iostream>
#include <string>
//#include <windows.h>
#include "ftdi/ftd2xx.h"

using namespace std;



FT_HANDLE open_port(string num_port);
void config_serial(FT_HANDLE port);
int manage_serial_num();
int detect_num_serial(int machine);
int set_serial_num(int machine, string num_serial);
string get_serial_num(int machine);
string get_port_com(FT_HANDLE ft_handle);



#endif