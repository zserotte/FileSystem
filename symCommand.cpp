#include "symCommand.h"
#include <iostream>
#include <sstream>
#include "DirectoryFile.h"
#include "Proxy.h"
// #include "HierarchicalFileSystem.h"

using namespace std;

symCommand::symCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

int symCommand::execute(std::string& CWD, std::string options)
{
	stringstream before(options);
	string fileName;
	before >> fileName;
	// If they didn't type anything after sym 
	if (fileName.size() == 0) {
		cout << "The user did not specify a file to copy" << endl;
		return badInput;
	}
	string stringToCopy = CWD + '/' + fileName;
	AbstractFile* from = fs->openFile(stringToCopy);
	if (from == nullptr) { // file creation failed
		cout << "failed to open directory" << endl;
		return directorydoesnotexist;
	}
	// Check if the file we opened is a directory
	DirectoryFile* fromCheck = dynamic_cast<DirectoryFile*>(from);
	if (fromCheck != nullptr) {
		cout << "The user attempted to create a symbolic link to a directory" << endl;
		//delete fromCheck;
		return symToDirectory;
	}
	//delete fromCheck; // Deleting nullptr
	string locationToCopyTo;
	before >> locationToCopyTo;
	// If they did not give us a location to copy the file to
	if (locationToCopyTo.size() == 0) {
		cout << "The user did not specify a path to symbolically link the file to" << endl;
		return noPath;
	}
	AbstractFile* to = fs->openFile(locationToCopyTo);
	if (to == nullptr) { // file creation failed
		fs->closeFile(from);
		cout << "failed to open directory" << endl;
		return directorydoesnotexist;
	}
	DirectoryFile* toCheck = dynamic_cast<DirectoryFile*>(to);
	// The file is an image or text file, throw an error
	if (toCheck == nullptr) {
		fs->closeFile(to);
		fs->closeFile(from);
		//delete toCheck;
		cout << "The user attempted to symbollicaly link to an image or text file" << endl;
		return copyTOiORt;
	}
	//delete toCheck;
	// Check if the file that we opened is a proxy object
	Proxy* fileToCopy = dynamic_cast<Proxy*>(from);
	locationToCopyTo = locationToCopyTo + '/' + fileName;
	// The user is trying to create a symoblic link to a proxy object

	if (fileToCopy != nullptr) { // All other cases
		// size_t index = CWD.find_last_of('/');
		// string myName = CWD.substr(index, CWD.size() - 1);
		Proxy* myProxy = new Proxy(*fileToCopy, fileName);
		fs->addFile(locationToCopyTo, myProxy);
		fs->closeFile(from);
		fs->closeFile(to);
		return success;
	} // The user is creating a symbolic link to the original file
	else { // Case 1
		//delete fileToCopy;
		// Clone the original file
		auto realFile = from->clone();
		Proxy* myProxy = new Proxy(realFile, fileName);
		fs->closeFile(from);
		// Delete the original file
		fs->deleteFile(stringToCopy);
		// Add our proxy object to where the original file was
		fs->addFile(stringToCopy, myProxy);
		Proxy* myProxy2 = new Proxy(*myProxy, fileName);
		fs->addFile(locationToCopyTo, myProxy2);
		fs->closeFile(to);
		return success;
	}

	/* auto newFile = from->clone();

	// Add the fileName onto the location to copy to
	locationToCopyTo = locationToCopyTo + '/' + fileName;
	return 0; */
}

void symCommand::displayInfo() {
	cout << "Create a symbolic link to an exsiting file at a specified location on the system" << endl;
	cout << "Usage: 'sym <file_to_copy> <full_path_to_destination>'" << endl;
}
