/*Copyright © October 2015, Psykauze

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
The Software is provided “as is”, without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement. In no event shall the authors or copyright holders X be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or the use or other dealings in the Software.
Except as contained in this notice, the name of Psykauze shall not be used in advertising or otherwise to promote the sale, use or other dealings in this Software without prior written authorization from the <copyright holders>.*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "../libvpk/libvpk.h"

std::string getTempDirectory(void)
{
	#ifdef WIN32
		std::string tmpdir = getenv("TEMP");
	#else
		std::string tmpdir = "/tmp";
	#endif
	return tmpdir;
}

void extractListFile(DirEntry * ListEntry, DirInfo TheHead, std::ifstream &TheFile, std::string directory, std::ofstream &TheLogFile)
{
	// Extract files
	char * buffer;
	std::string FileDirectory;
	std::ofstream TempFile;
	std::string filename;
	std::ostringstream TestI;
	size_t found;
	for(unsigned int i = 0; i < TheHead.m_uFileCount; i++)
	{
		filename = ListEntry[i].m_szFileName;
		TestI.str("");
		found=filename.find_last_of("/\\");
		TestI << i << filename.substr(found+1);
//		std::cout << "Folder : " << filename.substr(0,found) << " Name : " << filename.substr(found+1) << std::endl;
//		FileDirectory = directory + "/new/" + filename.substr(found+1);
		FileDirectory = directory + "/new/" + TestI.str();
		TheLogFile << i << "," << ListEntry[i].m_uNameLength << "," << ListEntry[i].m_szFileName << "," << ListEntry[i].m_uFileOffset << "," << ListEntry[i].m_uFileLength;
		buffer = getFile(TheFile,ListEntry[i],TheLogFile);
		TempFile.open(FileDirectory.c_str(),std::ios::out);
		if (TempFile.rdstate() != 0)
		{
			std::cout << "Cannot open in write mode the " << i << " entry. Its name : " << filename.substr(found+1) << " State : " << TempFile.rdstate() << std::endl;
			TheLogFile << " : Error" << std::endl;
			TempFile.clear();
		}
		else
		{
			TempFile.write(buffer,ListEntry[i].m_uFileLength);
			if (TempFile.rdstate() != 0)
			{
				std::cout << "Cannot write in " << filename.substr(found+1) << " State : " << TempFile.rdstate() << std::endl;
				TheLogFile << " : Error" << std::endl;
				TempFile.clear();
			}
			else
			{
				TheLogFile << " : Ok" << std::endl;
			}
		}
		TempFile.close();
		free(buffer);
	}
	TheFile.clear();
	TheLogFile.clear();
	return;
}

int main(int argc, char** argv)
{
	std::ifstream hFile;
std::cout << argv[1] << " : " << argc << std::endl;
	hFile.open(argv[1]);
	vpkfile Testconstruc(hFile);
	DirInfo New = Testconstruc.getDirInfo();
	DirEntry * Test = Testconstruc.getListEntry();
	std::cout << New.m_uFileCount << " ; " << sizeof(&Test) << std::endl;
	for(unsigned int i = 0; i < New.m_uFileCount; i++)
	{
		std::cout << Test[i].m_uNameLength << "," << Test[i].m_szFileName << "," << Test[i].m_uFileOffset << "," << Test[i].m_uFileLength << std::endl;
	}
	// Creating a logFile
	std::ofstream logFile;
	std::string logFilefile;
	logFilefile=getTempDirectory() + "/vpklog.txt";
	logFile.open(logFilefile.c_str(),std::ios::out);
	extractListFile(Test,New,hFile,getTempDirectory() + "/Activision",logFile);
/*	// Creating a logFile
	std::ofstream logFile;
	std::string logFilefile;
	logFilefile=getTempDirectory() + "/vpklog.txt";
	logFile.open(logFilefile.c_str(),std::ios::out);


	std::ifstream hFile;

	// Get file
	hFile.open(argv[1]);

	// Get Info header
	DirInfo Header = getDirInfo(hFile);
	logFile << "File count: " << Header.m_uFileCount << ", Directory Offset: " << Header.m_uDirectoryOffset << ", Type: " << (int) Header.m_uType << " sizeof(DirInfo) == " << sizeof(DirInfo) << std::endl;

	// Get Index Entry
	DirEntry * IndexEntry = new DirEntry[Header.m_uFileCount];
	IndexEntry = readIndexFile(hFile,Header);

	// Extract files
//	extractListFile(IndexEntry,Header,hFile,getTempDirectory(),logFile);
	extractListFile(IndexEntry,Header,hFile,"/media/stockage/Activision",logFile);
/*	char * buffer;
	std::ofstream TempFile;
	std::string OutFile;
	for(unsigned int i = 0; i < Header.m_uFileCount; i++)
	{
		TempFile.open("/tmp/vpktesttwo.wav",std::ios::out);
		logFile << IndexEntry[i].m_uNameLength << "," << IndexEntry[i].m_szFileName << "," << IndexEntry[i].m_uFileOffset << "," << IndexEntry[i].m_uFileLength << std::endl;
		buffer = getFile(hFile,IndexEntry[i]);
		TempFile.write(buffer,IndexEntry[i].m_uFileLength);
		TempFile.close();
		free(buffer);
	}
*/
/*	// Close file
	hFile.close();
	logFile.close();
*/
}
