#include "DSCommand.h"
#include <iostream>
#include <sstream>
#include "ReadFileVisitor.h"

using namespace std;

DSCommand::DSCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

int DSCommand::execute(std::string& CWD, std::string options) {
	AbstractFile* top = fs->openFile(CWD);
	if (top == nullptr) { // file creation failed
		cout << "failed to open directory" << endl;
		return directorydoesnotexist;
	}
	stringstream before(options);
	string fileName;
	before >> fileName;
	fileName = CWD + '/' + fileName;
	AbstractFile* sub = fs->openFile(fileName);
	if (sub == nullptr) { // file creation failed
		cout << "failed to open directory" << endl;
		fs->closeFile(top);
		return directorydoesnotexist;
	}
	string type;
	// ds -d
	if (before >> type) {
		if (type == "-d") {
			vector<char> contents = sub->read();
			for (size_t i = 0; i < contents.size(); i++) {
				cout << contents[i];
			}
			cout << endl;
			fs->closeFile(sub);
			fs->closeFile(top);
			return success;
		}
		else {
			cout << "The user entered an unspecified type of ds" << endl;
			fs->closeFile(sub);
			fs->closeFile(top);
			dsSpecificationError;
		}
	} // Regular ds
	else {
		ReadFileVisitor reader;
		sub->accept(&reader);
		fs->closeFile(top);
		fs->closeFile(sub);
		return success;
	}
}
void DSCommand::displayInfo() {
	cout << "Display the contents of a file" << endl;
	cout << "Usage: 'ds <filename>' for a formatted display or 'ds <filename> -d' for an unformatted display" << endl;
}