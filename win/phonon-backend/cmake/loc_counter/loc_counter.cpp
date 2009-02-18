/*
 * Copyright (C) 2008  Tanguy Krotoff <tkrotoff@gmail.com>
 *
 * Redistribution and use is allowed according to the terms of the BSD license.
 * For details see the accompanying COPYING file.
 */

#include <fstream>
#include <list>
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
	#include <windows.h>
#else
	#include <dirent.h>
#endif	//WIN32

#include <cstdlib>

typedef std::list<std::string> StringList;

StringList sourceCodeExtensions() {
	StringList extensions;

	extensions.push_back("cpp");
	extensions.push_back("cxx");
	extensions.push_back("c++");
	extensions.push_back("c");

	extensions.push_back("h");
	extensions.push_back("hxx");
	extensions.push_back("hpp");
	extensions.push_back("h++");

	return extensions;
}

bool filter(const std::string & filename) {
	static StringList extensions(sourceCodeExtensions());

	for (StringList::iterator it = extensions.begin(); it != extensions.end(); it++) {
		int ok = filename.compare(filename.size() - (*it).size(), (*it).size(), *it);
		if (ok == 0) {
			return true;
		}
	}

	return false;
}

int locFile(const std::string & filename) {
	int loc = 0;

	if (filter(filename)) {
		std::ifstream file(filename.c_str());
		std::string line;
		while (getline(file, line)) {
			loc++;
		}
		file.close();
	}

	return loc;
}

#ifndef S_ISDIR
	#ifdef S_IFDIR
		#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
	#else
		#define S_ISDIR(mode) 0
	#endif
#endif	//!S_ISDIR

bool isDirectory(const std::string & path) {
	struct stat statbuf;

	stat(path.c_str(), &statbuf);
	return S_ISDIR(statbuf.st_mode);
}

int _totalLoc = 0;

void findAllFilesUNIX(const std::string & path, bool recursive) {
#ifndef WIN32
	//http://www.commentcamarche.net/forum/affich-1699952-langage-c-recuperer-un-dir

	//Warning: opendir() is limited to PATH_MAX
	//See http://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
	DIR * dir = opendir(path.c_str());
	if (!dir) {
		std::cerr << __FUNCTION__ << "Error: opendir() failed" << std::endl;
		perror(path.c_str());
	} else {
		struct dirent * entry = NULL;
		while (entry = readdir(dir)) {
			std::string name(entry->d_name);

			//Avoid '.', '..' and other hidden files
			if (name[0] != '.') {

				std::string filename(path + '/' + name);

				if (recursive && isDirectory(filename)) {
					//Recurse
					findAllFilesUNIX(filename, recursive);
				}

				else {
					_totalLoc += locFile(filename);
				}
			}
		}

		int ret = closedir(dir);
		if (ret != 0) {
			std::cerr << __FUNCTION__ << "Error: closedir() failed" << std::endl;
			perror(path.c_str());
		}
	}
#endif	//WIN32
}

void replace(std::string & str, const std::string & before, const std::string & after) {
	std::string::size_type pos = 0;
	while ((pos = str.find(before, pos)) != std::string::npos) {
		str.replace(pos, before.length(), after);
		pos = pos + after.length();
	}
}

void findAllFilesWin32(const std::string & path, bool recursive) {
#ifdef WIN32
	//See http://msdn.microsoft.com/en-us/library/ms811896.aspx
	//See http://msdn.microsoft.com/en-us/library/aa364418.aspx
	//See http://msdn.microsoft.com/en-us/library/aa365247.aspx

	std::string longPath("\\\\?\\" + path + "\\*");
	replace(longPath, "/", "\\");

	WIN32_FIND_DATAW fileData;
	//LPCWSTR = wchar_t *
	//LPCSTR = char *
	//TCHAR = char or wchar_t
	//WCHAR = wchar_t

	//Get the first file
	std::wstring wideLongPath(longPath.begin(), longPath.end());
	HANDLE hList = FindFirstFileW((TCHAR *) wideLongPath.c_str(), &fileData);
	if (hList == INVALID_HANDLE_VALUE) {
		std::cerr << __FUNCTION__ << "Error: no files found, error code: " << GetLastError() << std::endl;
	}

	else {
		//Traverse through the directory structure
		bool finished = false;
		while (!finished) {

			std::wstring wideName(fileData.cFileName);
			std::string name(wideName.begin(), wideName.end());
			std::string filename(path + '\\' + name);

			//Check if the object is a directory or not
			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

				//Avoid '.', '..' and other hidden files
				if (name[0] != '.') {
					//Filter directory matching the given pattern
					if (recursive) {
						findAllFilesWin32(filename, recursive);
					}
				}
			}

			else {
				_totalLoc += locFile(filename);
			}

			if (!FindNextFileW(hList, &fileData)) {
				if (GetLastError() == ERROR_NO_MORE_FILES) {
					finished = true;
				}
			}
		}
	}

	FindClose(hList);
#endif	//WIN32
}

int main(int argc, char * argv[]) {
	std::string rootPath;
	bool recursive = true;

	if (argc > 1) {
		rootPath = std::string(argv[1]);
		bool exists = isDirectory(rootPath);
		if (!exists) {
			std::cerr << "Error: path does not exist:" << rootPath << std::endl;
			return EXIT_FAILURE;
		}
	}
	if (argc > 2) {
		recursive = atoi(argv[2]) != 0;
	}

#ifdef WIN32
	findAllFilesWin32(rootPath, recursive);
#else
	findAllFilesUNIX(rootPath, recursive);
#endif	//WIN32

	std::cout << _totalLoc;

	return EXIT_SUCCESS;
}
