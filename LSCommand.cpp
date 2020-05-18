/*
Author: Jon Shidal
Purpose: CSE 332 lab 5
*/

#include "LSCommand.h"
#include "ReadFileVisitor.h"
#include "ReadMetadataFileVisitor.h"
#include <iostream>
#include <sstream>

using namespace std;

LSCommand::LSCommand(AbstractFileSystem * fileSys) : fs(fileSys) {}

int LSCommand::execute(std::string& CWD, std::string options) {
	AbstractFile* top = fs->openFile(CWD);
	if (top == nullptr) { // file creation failed
		cout << "failed to open directory" << endl;
		return directorydoesnotexist;
	}
	if (options == "") {
		ReadFileVisitor reader;
		top->accept(&reader);
		fs->closeFile(top);
		return success;
	} 
	else if (options == "-l") {
		ReadMetadataFileVisitor reader;
		vector<char> contents = top->read();
		stringstream before;
		stringstream after;
		for (size_t i = 0; i < contents.size(); i++) {
			before << contents[i];
		}

		string fileName;
		while (before >> fileName) {
			fileName = CWD + "/" + fileName;
			AbstractFile* sub = fs->openFile(fileName);
			sub->accept(&reader);
			fs->closeFile(sub);
			fileName = "";
		}
		fs->closeFile(top);
		return success;
	}
	else {
		fs->closeFile(top);
		return listFilesError;
	}
}
void LSCommand::displayInfo() {
	cout << "lists the contents of the current directory" << endl;
	cout << "Usage: 'ls' for simplified version or 'ls -l' for long version" << endl;
}