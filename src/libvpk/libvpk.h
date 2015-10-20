/*Copyright © October 2015, Psykauze

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
The Software is provided “as is”, without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement. In no event shall the authors or copyright holders X be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or the use or other dealings in the Software.
Except as contained in this notice, the name of Psykauze shall not be used in advertising or otherwise to promote the sale, use or other dealings in this Software without prior written authorization from the <copyright holders>.*/

#include <iostream>
#include <fstream>
#include <sstream>

struct DirInfo {
  unsigned int m_uFileCount; // The number of files in the archive
  unsigned int m_uDirectoryOffset; // The position of the first DirEntry structure
  unsigned char m_uType; // Archive type; not important
};
DirInfo getDirInfo (std::ifstream &);
struct DirEntry {
  unsigned int  m_uNameLength; // Length of the filename
  char * m_szFileName; // The filename; m_uNameLength bytes long
  unsigned int  m_uFileOffset; // Position of the file
  unsigned int  m_uFileLength; // Length of the file
};
DirEntry readDirEntry (std::ifstream &);
char * getFile (std::ifstream &, DirEntry, std::ofstream &);
DirEntry * readIndexFile (std::ifstream &,DirInfo);

class vpkfile {
private :
	bool di_is_read;
	bool de_is_read;
	DirInfo di;
	DirEntry * de;
	std::ifstream &File;
	int jour;
	DirEntry getDirEntry();
//	std::ofstream &LogFile;
//	size_t Error;
public :
	~vpkfile();
	vpkfile(std::ifstream &);
	DirInfo getDirInfo();
	DirEntry* getListEntry();
};


