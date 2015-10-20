/*Copyright © October 2015, Psykauze

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
The Software is provided “as is”, without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement. In no event shall the authors or copyright holders X be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or the use or other dealings in the Software.
Except as contained in this notice, the name of Psykauze shall not be used in advertising or otherwise to promote the sale, use or other dealings in this Software without prior written authorization from the <copyright holders>.*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "libvpk.h"

bool di_is_read = 0;
bool de_is_read = 0;
vpkfile::~vpkfile() {}
vpkfile::vpkfile(std::ifstream &TheFile) : File(TheFile){}
DirInfo vpkfile::getDirInfo() {
if (!di_is_read)
{
	File.seekg(-9, std::ios::end);
	if (File.rdstate() != 0)
	{
		std::cout << "Cannot move to the DirInfo. State : " << File.rdstate() << std::endl;
		File.clear();
	}
	else
	{
		File.read( (char*)( &di) , sizeof( di ) );
		if (File.rdstate() != 0)
		{
			std::cout << "Cannot read the DirInfo. State : " << File.rdstate() << std::endl;
			File.clear();
		}
	}
	std::cout << "File count: " << di.m_uFileCount << ", Directory Offset: " << di.m_uDirectoryOffset << ", Type: " << (int) di.m_uType << " sizeof(DirInfo) == " << sizeof(DirInfo) << std::endl;
	di_is_read = 1;
}
	return di;
}

DirEntry vpkfile::getDirEntry() {
	DirEntry ADirEntry;
	File.read(( char* )(&ADirEntry.m_uNameLength), 4 );
	ADirEntry.m_szFileName = new char[ADirEntry.m_uNameLength+1];
	File.read((char*)( ADirEntry.m_szFileName), ADirEntry.m_uNameLength );
	// Ensure we have '\0' at the end of szFileName
	ADirEntry.m_szFileName[ADirEntry.m_uNameLength] = '\0';
	File.read( (char*)( &ADirEntry.m_uFileOffset), 4 );
	File.read( (char*)( &ADirEntry.m_uFileLength), 4 );
	File.clear();
	return ADirEntry;
}

DirEntry * vpkfile::getListEntry() {
if (!de_is_read)
{
	de = new DirEntry[di.m_uFileCount];
	File.seekg(di.m_uDirectoryOffset, std::ios::beg);
	if (File.rdstate() != 0)
	{
		std::cout << "Cannot move at the Header offset " << di.m_uDirectoryOffset << " State : " << File.rdstate() << std::endl;
		File.clear();
	}
	for(unsigned int i = 0; i < di.m_uFileCount; i++)
	{
		de[i] = getDirEntry();
	}
	de_is_read = 1;
}
	return de;
}

DirInfo getDirInfo (std::ifstream &TheFile)
{
	DirInfo di;
	TheFile.seekg(-9, std::ios::end);
	if (TheFile.rdstate() != 0)
	{
		std::cout << "Cannot move to the DirInfo. State : " << TheFile.rdstate() << std::endl;
		TheFile.clear();
	}
	else
	{
		TheFile.read( (char*)( &di) , sizeof( di ) );
		if (TheFile.rdstate() != 0)
		{
			std::cout << "Cannot read the DirInfo. State : " << TheFile.rdstate() << std::endl;
			TheFile.clear();
		}
	}
	return di;
}

DirEntry readDirEntry (std::ifstream &TheFile)
{
	DirEntry de;
	TheFile.read(( char* )(&de.m_uNameLength), 4 );
	de.m_szFileName = new char[de.m_uNameLength+1];
	TheFile.read((char*)( de.m_szFileName), de.m_uNameLength );
	// Ensure we have '\0' at the end of szFileName
	de.m_szFileName[de.m_uNameLength] = '\0';
	TheFile.read( (char*)( &de.m_uFileOffset), 4 );
	TheFile.read( (char*)( &de.m_uFileLength), 4 );
	TheFile.clear();
	return de;
}

char * getFile (std::ifstream &TheFile, DirEntry TheEntry, std::ofstream &TheLogFile)
{
	char * buffer = new char[TheEntry.m_uFileLength];
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	TheFile.seekg(TheEntry.m_uFileOffset, std::ios::beg);
	if (TheFile.rdstate() != 0)
	{
		std::cout << "Cannot move at the offset " << TheEntry.m_uFileOffset << " File : " << TheEntry.m_szFileName << " State : " << TheFile.rdstate() << std::endl;
		TheLogFile << " : Error";
		TheFile.clear();
	}
	else
	{
		TheFile.read( (char*) buffer, TheEntry.m_uFileLength );
		if (TheFile.rdstate() != 0)
		{
			std::cout << "Cannot read " << TheEntry.m_uFileLength << " from : " << TheEntry.m_szFileName << " State : " << TheFile.rdstate() << std::endl;
			TheLogFile << " : Error";
			TheFile.clear();
		}
		else
		{
			TheLogFile << " : Ok";
		}
	}
//	TheFile.get(buffer, TheEntry.m_uFileLength );
	TheFile.clear();
	return buffer;
}

DirEntry * readIndexFile (std::ifstream &TheFile, DirInfo Header)
{
	DirEntry * IndexEntry = new DirEntry[Header.m_uFileCount];
	char * buffer;
	TheFile.seekg(Header.m_uDirectoryOffset, std::ios::beg);
	if (TheFile.rdstate() != 0)
	{
		std::cout << "Cannot move at the Header offset " << Header.m_uDirectoryOffset << " State : " << TheFile.rdstate() << std::endl;
		TheFile.clear();
	}
	for(unsigned int i = 0; i < Header.m_uFileCount; i++)
	{
		IndexEntry[i] = readDirEntry (TheFile);
	}
	return IndexEntry;
}

