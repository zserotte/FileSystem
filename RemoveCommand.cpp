/*
Author: Jon Shidal
Purpose: CSE 332 lab 5
*/

#include "RemoveCommand.h"
#include<iostream>

using namespace std;
#include <sstream>

RemoveCommand::RemoveCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

// removes a file from the current working directory
int RemoveCommand::execute(std::string& CWD, std::string options) {
	
	stringstream before(options);
	string fileName;
	before >> fileName;
	fileName = CWD + '/' + fileName;
	string type;
	bool recursive = false;
	if (before >> type && type == "-r") {
		recursive = true;
	}
	
	// Edit this
	int result;
	if (recursive == true) {
		result = fs->deleteFileRecursively(fileName);
	}
	else {
		result = fs->deleteFile(fileName);
	}
	if (result == AbstractFileSystem::filedoesnotexist) {
		cout << "File does not exist" << endl;
		return filedoesnotexist;
	}
	else if (result == AbstractFileSystem::fileisopen) {
		cout << "File is currently in use" << endl;
		return success;
	}
	else if (result == AbstractFileSystem::directorynotempty) {
		cout << "You cannot delete a directory that has contents inside of it" << endl;
		return directoryNotEmpty;
	}
	return success;
}
void RemoveCommand::displayInfo() {
	cout << "touch removes a file from the file system and then deletes the file" << endl;
	cout << "Usage: rm <filename>" << endl;
}