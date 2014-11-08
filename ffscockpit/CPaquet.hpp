/**
OpenWorlds Copyright (C) 2008 Neophile

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

// ****************************************************************************
// *                                                                          *
// *              CPAQUET.HPP : Classe de formatage des paquets tcp           *
// *                                                                          *
// *                      Neophile (c) 2006 : 09/02/2007                      *
// *      Rev B : 18/03/07                                                    *
// *                                                                          *
// ****************************************************************************
// * Rev A:Neophile:27/03/2007;Description :                                  *
// *                                Ajout du type APaquet pour la gestion     *
// *                                De la pile d'envoi des paquets            *
// ****************************************************************************

#ifndef CPAQUET_HPP
#define CPAQUET_HPP

#ifndef  WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wxprec.h>

#include <vector>

enum
{
    PQ_OK=0,
    PQ_EOF,
    PQ_BAD_LENGTH,
    PQ_TOO_LONG
};

class CPaquet
{
private:
	std::vector<unsigned char>				Tampon;
	std::vector<unsigned char>::iterator	Pointeur;
	bool									LEndian;
	int										GetVoid (unsigned char* Data, size_t Taille);
	int										GetVoid (size_t Taille);
	int										AddVoid (const unsigned char* Data, size_t Taille);
	int										AddVoid (size_t Taille);

public:
											CPaquet (bool pLEndian = true);
											CPaquet (unsigned char* Buffer, size_t Taille,bool pLEndian = true);
											~CPaquet ();
	void									Set (unsigned char* Buffer, size_t Taille);
	void									Get (unsigned char* Buffer, size_t Taille);
	size_t									GetSize ();
	int										GetByte (unsigned char& Byte);
	int										GetByte ();
	int										GetWord (unsigned short& Word);
	int										GetWord (short& Word);
	int										GetWord ();
	int										GetDWord (unsigned int& DWord);
	int										GetDWord (int& DWord);
	int										GetDWord ();
	int										GetFloat (float& Real);
	int										GetFloat ();
	int										GetString (wxString& Str);
	int										GetString ();
	int										GetLongString (wxString& Str);
	int										GetLongString ();
	int										GetStream (unsigned char* Buffer, size_t Taille);
	int										GetStream (size_t Taille);
	int										Begin ();
	int										End ();
	int										Clear ();
	size_t									GetPos ();
	int										AddByte (unsigned char Byte);
	int										AddByte ();
	int										AddWord (unsigned short Word);
	int										AddWord ();
	int										AddSWord (short Word);
	int										AddDWord (unsigned int DWord);
	int										AddDWord ();
	int										AddSDWord (int DWord);
	int										AddFloat (float Real);
	int										AddFloat ();
	int										AddString (wxString Str);
	int										AddString ();
	int										AddLongString (wxString Str);
	int										AddLongString ();
	int										AddStream (unsigned char* Buffer, size_t Taille);
	int										AddStream (wxString& Str);
};

#endif
