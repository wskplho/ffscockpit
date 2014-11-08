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
// *                 CPAQUET.H : Classe de formatage des paquets tcp          *
// *                                                                          *
// *                      Neophile (c) 2006 : 09/02/2007                      *
// *      Rev B : 18/03/07                                                    *
// *                                                                          *
// ****************************************************************************
// * Rev A:Neophile:27/03/2007;Description :                                  *
// *                                Ajout du type APaquet pour la gestion     *
// *                                De la pile d'envoi des paquets            *
// ****************************************************************************

#include "CPaquet.h"

//-----------------------------------------------------------------------------
// Constructeur vierge

CPaquet::CPaquet (bool pLEndian)
{
	Tampon.reserve(1024);
	Pointeur=Tampon.begin();
	LEndian = pLEndian;
}

//-----------------------------------------------------------------------------
// Constructeur avec initialisation

CPaquet::CPaquet (UINT8* Buffer, size_t Taille, bool pLEndian)
{
	LEndian = pLEndian;
	Pointeur=Tampon.begin();
	Set (Buffer,Taille);
}

//-----------------------------------------------------------------------------
// Destructeur

CPaquet::~CPaquet ()
{
}

//-----------------------------------------------------------------------------
// Retourne des données de tous type en fonction du type endian de la plateforme

PQ_RESULT CPaquet::GetVoid (unsigned char* Data, size_t Taille)
{
	if (!Taille) return PQ_BAD_LENGTH;
	size_t MemPtr;
	if (LEndian) MemPtr=Taille-1;
	else MemPtr=0;
	for (size_t i = 0; i < Taille; ++i)
	{
		if (Pointeur == Tampon.end()) return PQ_EOF;
		Data[MemPtr] = *(Pointeur++);
		if (LEndian) --MemPtr;
		else ++MemPtr;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Incrémente l'iterateur pour une donnée type

PQ_RESULT CPaquet::GetVoid(size_t Taille)
{
	if (!Taille) return PQ_BAD_LENGTH;
	if (Pointeur>=(Tampon.end()-(Taille-1)))
	{
		return PQ_EOF;
	}
	Pointeur+= Taille;
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajoute  une données de tout type en fonction du type endian de la plateforme

PQ_RESULT	CPaquet::AddVoid (const unsigned char* Data, size_t Taille)
{
	size_t PtrMem;
	if (LEndian) PtrMem = Taille -1;
	else PtrMem = 0;
	for (size_t i = 0; i < Taille; ++i)
	{
		Tampon.push_back(Data[PtrMem]);
		if (LEndian) --PtrMem;
		else ++PtrMem;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajoute des données nulle pour tout type

PQ_RESULT	CPaquet::AddVoid(size_t Taille)
{
	for (size_t i = 0; i < Taille; ++i)
	{
		Tampon.push_back(0);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Remplie le buffer avec un autre buffer simple

PQ_RESULT CPaquet::Set (unsigned char* Buffer, size_t Taille)
{
	Tampon.assign(Buffer, Buffer + Taille);
	Pointeur = Tampon.begin();
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Renvoie le buffer complet avec sa taille

PQ_RESULT CPaquet::Get (unsigned char* Buffer, size_t Taille)
{
	if (!Taille) return PQ_BAD_LENGTH;
	if (Taille > Tampon.size()) Taille= Tampon.size();
	copy ( Tampon.begin(), Tampon.begin()+Taille, Buffer);
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Renvoie le buffer complet avec sa taille
PQ_RESULT CPaquet::Get(wxString& Buffer)
{
	for (vector<BYTE>::iterator i = Tampon.begin(); i != Tampon.end(); ++i)
	{
		Buffer << wxString::Format(_T("%02X:"),*i);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Renvoie la taille du buffer

size_t CPaquet::GetSize ()
{
	return Tampon.size();
}

//-----------------------------------------------------------------------------
// Renvoie un octet appellé aussi CHAR

PQ_RESULT CPaquet::GetByte(INT8& Byte)
{
	return GetVoid ((unsigned char*)(&Byte), sizeof(Byte));
}

//-----------------------------------------------------------------------------
// Renvoie un octet appellé aussi CHAR

PQ_RESULT CPaquet::GetByte(UINT8& Byte)
{
	return GetVoid ((unsigned char*)(&Byte), sizeof(Byte));
}

//-----------------------------------------------------------------------------
// Saute un Octet

PQ_RESULT CPaquet::GetByte()
{
	return GetVoid(sizeof(INT8));
}

//-----------------------------------------------------------------------------
// Renvoie un mot (2 octets) appellé aussi SHORT

PQ_RESULT	CPaquet::GetWord(INT16& Word)
{
	return GetVoid ((unsigned char*)(&Word), sizeof(Word));
}

//-----------------------------------------------------------------------------
// Renvoie un mot (2 octets) appellé aussi SHORT

PQ_RESULT	CPaquet::GetWord (UINT16& Word)
{
	return GetVoid ((unsigned char*)(&Word), sizeof(Word));
}

//-----------------------------------------------------------------------------
// Saute un Mot

PQ_RESULT CPaquet::GetWord()
{
	return GetVoid(sizeof(INT16));
}

//-----------------------------------------------------------------------------
// Renvoie un mot double (4 octets) appellé aussi INT

PQ_RESULT	CPaquet::GetDWord(INT32& DWord)
{
	return GetVoid ((unsigned char*)(&DWord), sizeof(DWord));
}

//-----------------------------------------------------------------------------
// Renvoie un mot double (4 octets) appellé aussi INT

PQ_RESULT	CPaquet::GetDWord (UINT32& DWord)
{
	return GetVoid ((unsigned char*)(&DWord), sizeof(DWord));
}

//-----------------------------------------------------------------------------
// Saute un Mot Double

PQ_RESULT CPaquet::GetDWord()
{
	return GetVoid(sizeof(INT32));
}

//-----------------------------------------------------------------------------
// Renvoie un nombre flottant

PQ_RESULT	CPaquet::GetFloat(float& Real)
{
	return GetVoid((BYTE*)(&Real), sizeof(float));
}

//-----------------------------------------------------------------------------
// Saute un Flottant

PQ_RESULT CPaquet::GetFloat()
{
	return GetVoid(sizeof(float));
}

//-----------------------------------------------------------------------------
// Renvoie une chaine de charactère depuis le format AW

PQ_RESULT CPaquet::GetString(wxString& Str)
{
	BYTE Taille;
	Str=_T("");
	if (GetByte(Taille)) return PQ_EOF;
	if (Pointeur >(Tampon.end()-Taille)) return PQ_BAD_LENGTH;
	for (size_t i=0; i<Taille; i++)
	{
		Str << (wxChar)*(Pointeur++);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saute une chaine de charactères

PQ_RESULT CPaquet::GetString()
{
	BYTE Taille;
	if (GetByte(Taille)) return PQ_EOF;
	return GetStream(Taille);
}

//-----------------------------------------------------------------------------
// Renvoie une chaine de charactere de grande longueur

PQ_RESULT CPaquet::GetLongString(wxString& Str)
{
	WORD Taille;
	Str=_T("");
	if (GetWord (Taille)) return PQ_EOF;
	for (WORD i=0; i<Taille; ++i)
	{
		if (Pointeur==Tampon.end()) return PQ_EOF;
		Str << (wxChar)*(Pointeur++);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saute une chaine de charactere de grande longueur

PQ_RESULT CPaquet::GetLongString()
{
	WORD Taille;
	if (GetWord (Taille)) return PQ_EOF;
	return GetStream(Taille);
}

//-----------------------------------------------------------------------------
// Renvoie une suite d'octets

PQ_RESULT CPaquet::GetStream(unsigned char* Buffer, size_t Taille)
{
	for (size_t i=0; i<Taille ; ++i)
	{
		if (Pointeur==Tampon.end()) return PQ_EOF;
		Buffer[i]=*(Pointeur++);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saute une suite d'octets

PQ_RESULT CPaquet::GetStream(size_t Taille)
{
	for (size_t i=0; i<Taille ; ++i)
	{
		if (Pointeur==Tampon.end()) return PQ_EOF;
		++Pointeur;
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Déplace le pointeur au début du buffer

PQ_RESULT CPaquet::Begin()
{
	Pointeur=Tampon.begin();
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Déplace le pointeur à la fin du buffer

PQ_RESULT CPaquet::End()
{
	Pointeur=Tampon.end();
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Efface et réinitialise le buffer

PQ_RESULT	CPaquet::Clear()
{
	Tampon.clear();
	return CPaquet::Begin ();
}

//-----------------------------------------------------------------------------
// Donne la position du pointeur dans le buffer

size_t CPaquet::GetPos ()
{
	return Pointeur - Tampon.begin();
}

//-----------------------------------------------------------------------------
// Encapsulation d'un autre paquet

PQ_RESULT CPaquet::Add(CPaquet& Paquet)
{
	for (vector<BYTE>::iterator i = Paquet.Tampon.begin(); i != Paquet.Tampon.end(); ++i)
	{
		Tampon.push_back(*i);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'un Octet appellé aussi char

PQ_RESULT CPaquet::AddByte(INT8 Byte)
{
	return AddVoid ((const unsigned char*)(&Byte), sizeof(Byte));
}

//-----------------------------------------------------------------------------
// Ajout d'un Octet appellé aussi char

PQ_RESULT CPaquet::AddByte (UINT8 Byte)
{
	return AddVoid ((const unsigned char*)(&Byte), sizeof(Byte));
}

//-----------------------------------------------------------------------------
// Ajout d'un Octet appellé aussi char

PQ_RESULT CPaquet::AddByte()
{
	return AddVoid (sizeof(INT8));
}

//-----------------------------------------------------------------------------
// Ajout d'un mot (2 octets) appellé aussi short

PQ_RESULT CPaquet::AddWord(INT16 Word)
{
	return AddVoid ((const unsigned char*)(&Word), sizeof (Word));
}

//-----------------------------------------------------------------------------
// Ajout d'un mot (2 octets) appellé aussi short

PQ_RESULT CPaquet::AddWord (UINT16 Word)
{
	return AddVoid ((const unsigned char*)(&Word), sizeof (Word));
}

//-----------------------------------------------------------------------------
// Ajout d'un mot (2 octets) appellé aussi short

PQ_RESULT CPaquet::AddWord()
{
	return AddVoid (sizeof(INT16));
}

//-----------------------------------------------------------------------------
// Ajout d'un double mot (4 octets) appellé aussi int

PQ_RESULT CPaquet::AddDWord(const INT32 Dword)
{
	return AddVoid ((const unsigned char*)(&Dword), sizeof(Dword));
}

//-----------------------------------------------------------------------------
// Ajout d'un double mot (4 octets) appellé aussi int

PQ_RESULT CPaquet::AddDWord (const UINT32 Dword)
{
	return AddVoid ((const unsigned char*)(&Dword), sizeof(Dword));
}

//-----------------------------------------------------------------------------
// Ajout d'un double mot vide

PQ_RESULT	CPaquet::AddDWord()
{
	return AddVoid (sizeof(INT32));
}

//-----------------------------------------------------------------------------
// Ajout d'un nombre flotant

PQ_RESULT	CPaquet::AddFloat(float Real)
{
	return AddVoid ((const unsigned char*)(&Real), sizeof (Real));
}


//-----------------------------------------------------------------------------
// Ajout d'un flottant vide

PQ_RESULT	CPaquet::AddFloat()
{
	return AddVoid (sizeof(float));
}

//-----------------------------------------------------------------------------
// Ajout d'une chaine de charactères au format PROTO AW

PQ_RESULT CPaquet::AddString(wxString Str)
{
	if (Str.Length () >255) return PQ_TOO_LONG;
	AddByte ((BYTE)Str.Length ());
	for (size_t i=0; i<Str.Length(); ++i)
	{
		Tampon.push_back(Str[i]);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'une chaine de charactères au format PROTO AW

PQ_RESULT CPaquet::AddString()
{
	AddByte ();
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'une chaine longue de charactere

PQ_RESULT CPaquet::AddLongString(wxString Str)
{
	WORD Taille = Str.Length ();
	if (Taille > 1000) Taille=1000;
	AddWord (Taille);
	for (size_t i=0; i<Taille; ++i)
	{
		Tampon.push_back (Str[i]);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Saut d'une chaine longue de charactere

PQ_RESULT CPaquet::AddLongString()
{
	WORD Word=0;
	AddWord(Word);
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'une suite d'octets

PQ_RESULT CPaquet::AddStream(BYTE* Buffer, size_t Taille)
{
	for (size_t i=0; i < Taille; ++i)
	{
		AddByte(Buffer[i]);
	}
	return PQ_OK;
}

//-----------------------------------------------------------------------------
// Ajout d'une suite d'octets

PQ_RESULT CPaquet::AddStream(wxString& Str)
{
	for (size_t i=0; i < Str.Length(); ++i)
	{
		AddByte(Str[i]);
	}
	return PQ_OK;
}
