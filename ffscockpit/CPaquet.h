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

using namespace std;


enum PQ_RESULT
{
	PQ_OK=0,
	PQ_EOF,
	PQ_BAD_LENGTH,
	PQ_TOO_LONG
};

class CPaquet
{
	private:
		vector<byte>	Tampon;
		vector<BYTE>::iterator	Pointeur;
		bool			LEndian;
		PQ_RESULT		GetVoid			(unsigned char* Data, size_t Taille);
		PQ_RESULT		GetVoid			(size_t Taille);
		PQ_RESULT		AddVoid			(const unsigned char* Data, size_t Taille);
		PQ_RESULT		AddVoid			(size_t Taille);

	public:
						CPaquet			(bool Endian = true);
						CPaquet			(unsigned char* Buffer, size_t Taille, bool Endian = true);
						~CPaquet		();
		PQ_RESULT		Set				(unsigned char* Buffer, size_t Taille);
		PQ_RESULT		Get				(unsigned char* Buffer, size_t Taille);
		PQ_RESULT		Get				(wxString& Buffer);
		size_t			GetSize			();
		PQ_RESULT		GetByte			(INT8& Byte);
		PQ_RESULT		GetByte			(UINT8& Byte);
		PQ_RESULT		GetByte			();
		PQ_RESULT		GetWord			(INT16& Word);
		PQ_RESULT		GetWord			(UINT16& Word);
		PQ_RESULT		GetWord			();
		PQ_RESULT		GetDWord		(INT32& DWord);
		PQ_RESULT		GetDWord		(UINT32& DWord);
		PQ_RESULT		GetDWord		();
		PQ_RESULT		GetFloat		(float& Real);
		PQ_RESULT		GetFloat		();
		PQ_RESULT		GetString		(wxString& Str);
		PQ_RESULT		GetString		();
		PQ_RESULT		GetLongString	(wxString& Str);
		PQ_RESULT		GetLongString	();
		PQ_RESULT		GetStream		(unsigned char* Buffer, size_t Taille);
		PQ_RESULT		GetStream		(size_t Taille);
		PQ_RESULT		Begin			();
		PQ_RESULT		End				();
		PQ_RESULT		Clear			();
		size_t			GetPos			();
		PQ_RESULT		Add				(CPaquet& Paquet);
		PQ_RESULT		AddByte			(INT8 Byte);
		PQ_RESULT		AddByte			(UINT8 Byte);
		PQ_RESULT		AddByte			();
		PQ_RESULT		AddWord			(INT16 Word);
		PQ_RESULT		AddWord			(UINT16 Word);
		PQ_RESULT		AddWord			();
		PQ_RESULT		AddDWord		(INT32 DWord);
		PQ_RESULT		AddDWord		(UINT32 DWord);
		PQ_RESULT		AddDWord		();
		PQ_RESULT		AddFloat		(float Real);
		PQ_RESULT		AddFloat		();
		PQ_RESULT		AddString		(wxString Str);
		PQ_RESULT		AddString		();
		PQ_RESULT		AddLongString	(wxString Str);
		PQ_RESULT		AddLongString	();
		PQ_RESULT		AddStream		(unsigned char* Buffer, size_t Taille);
		PQ_RESULT		AddStream		(wxString& Str);
};

#endif
