#include "CATCommand.h"
#include "AbstractFile.h"
#include <iostream>
#include <sstream>

using namespace std;

CATCommand::CATCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

int CATCommand::execute(std::string& CWD, std::string options) {
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
	// Cat -a
	if (before >> type) {
		if (type == "-a") {
			cout << fileName << endl;
			cout << "Enter the data you would like to append to the exisiting file. ";
			cout << "Enter ':wq' to save the file and exit, enter ':q' to exit without saving" << endl;
			string extension = fileName.substr(fileName.size() - 4, fileName.size() - 1);
			vector<char> contents = sub->read();
			for (auto i = 0; i < contents.size(); i++) {
				cout << contents[i];
			}
			if (contents.size() > 0) {
				cout << endl;
			}
			string input;
			string text;
			while (getline(cin, input)) {
				if (input == ":q") {
					fs->closeFile(sub);
					fs->closeFile(top);
					return success;
				}
				else if (input == ":wq") {
					vector<char> adding(text.begin(), text.end());
					if (extension == ".img") {
						fs->closeFile(sub);
						fs->closeFile(top);
						cout << "You are not allowed to append to an image file" << endl;
						return AbstractFile::returnvalues::unabletoappend;
					}
					sub->append(adding);
					fs->closeFile(sub);
					fs->closeFile(top);
					return success;
				}
				else {
					text = text + input + '\n';
				}
			}
		}
		else {
			cout << "The user entered an unspecified type of CAT" << endl;
			fs->closeFile(sub);
			fs->closeFile(top);
			CATSpecificationError;
		}
	} // Regular cat
	else {
		cout << fileName << endl;
		cout << "Enter the data you would like to write to the file. ";
		cout << "Enter ':wq' to save the file and exit, enter ':q' to exit without saving" << endl;
		string input;
		string text;
		while (getline(cin, input)) {
			if (input == ":q") {
				fs->closeFile(sub);
				fs->closeFile(top);
				return success;
			}
			else if (input == ":wq") {
				vector<char> adding(text.begin(), text.end());
				string extension = fileName.substr(fileName.size() - 4, fileName.size() - 1);
				if (extension == ".img") {
					adding.pop_back();
				}
				sub->write(adding);
				fs->closeFile(sub);
				fs->closeFile(top);
				return success;
			}
			else {
				text = text + input + '\n';
			}
		}
	}
}
void CATCommand::displayInfo() {
	cout << "Update file contents" << endl;
	cout << "Usage: 'cat <filename>' to overwrite file contents or 'cat <filename> -a' to diplay files contents and append to file" << endl;
}