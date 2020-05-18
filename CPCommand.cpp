#include "CPCommand.h"
#include <iostream>
#include <sstream>
#include "DirectoryFile.h"


using namespace std;

CPCommand::CPCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

int CPCommand::execute(std::string& CWD, std::string options) {
	stringstream before(options);
	string fileName;
	before >> fileName;
	// If they didn't type anything after cp 
	if (fileName.size() == 0) {
		cout << "The user did not specify a file to copy" << endl;
		return badInput;
	}
	string fileToCopy = CWD + '/' + fileName;
	AbstractFile* from = fs->openFile(fileToCopy);
	if (from == nullptr) { // file creation failed
		cout << "failed to open directory" << endl;
		return directorydoesnotexist;
	}
	// DirectoryFile* der = dynamic_cast<DirectoryFile*>(from);
	auto newFile = from->clone();
	fs->closeFile(from);
	// The file is a directory, throw an error
	if (newFile == nullptr) {
		cout << "The user attempted to copy from a directory" << endl;
		return copyFromDirectory;
	}
	string locationToCopyTo;
	before >> locationToCopyTo;
	// If they did not give us a location to copy the file to
	if (locationToCopyTo.size() == 0) {
		cout << "The user did not specify a path to copy the file to" << endl;
		return noPath;
	}
	AbstractFile* to = fs->openFile(locationToCopyTo);
	if (to == nullptr) { // file creation failed
		cout << "failed to open directory" << endl;
		return directorydoesnotexist;
	}
	DirectoryFile* der1 = dynamic_cast<DirectoryFile*>(to);
	// The file is an image or text file, throw an error
	if (der1 == nullptr) {
		fs->closeFile(to);
		cout << "The user attempted to copy to an image or text file" << endl;
		return copyTOiORt;
	}
	// delete der1;
	// Add the fileName onto the location to copy to
	locationToCopyTo = locationToCopyTo + '/' + fileName;
	fs->addFile(locationToCopyTo, newFile);
	fs->closeFile(to);
	return success;
}
void CPCommand::displayInfo() {
	cout << "Copy an exisiting file to another location on the system" << endl;
	cout << "Usage: 'cp <file_to_copy> <full_path_to_destination>'" << endl;
}