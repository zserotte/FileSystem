/*
Author: Jon Shidal
Purpose: CSE 332 lab5 - defines the hierarchical file system class
*/

#include "HierarchicalFileSystem.h"
#include <iostream>
#include "DirectoryFile.h"

using namespace std;

HierarchicalFileSystem::HierarchicalFileSystem() : rootDirectory(nullptr){
	rootDirectory = new DirectoryFile("root");
	rootDirectory->setParent(rootDirectory);  // root, so parent is itself
}

int HierarchicalFileSystem::addFile(std::string path, AbstractFile* fp) {
	size_t endOfDirectories = path.find_last_of('/');
	if (endOfDirectories == string::npos) { // not '/' so not a valid path
		return invalidpath;
	}
	string filename = path.substr(endOfDirectories + 1, string::npos);
	string directoryPath = path.substr(0, endOfDirectories);
	AbstractFile* toAddTo = parsePath(directoryPath); 
	if (toAddTo == nullptr) {  // directory doesn't exist
		return locationdoesnotexist;
	}
	if (fp == nullptr) {
		return nullfile;
	}
	else {
		if (fp->getParent() != nullptr) {   // file already exist elsewhere in the filesystem
			return filealreadyadded;
		}
		int result = toAddTo->addChild(fp);
		if ( result == AbstractFile::notacomposite || result == AbstractFile::filealreadyexists) { // unable to addChild, must not be a directoryFile
			return result;
		}
		fp->setParent(toAddTo);
		return success;
	}
}

int HierarchicalFileSystem::deleteFileRecursively(string path) {
	AbstractFile* fileToDelete = parsePath(path);
	if (fileToDelete == nullptr) {
		return filedoesnotexist; // unable to find file
	}
	if (openFiles.find(fileToDelete) != openFiles.end()) {
		return fileisopen; // file in use
	}
	AbstractFile* parent = fileToDelete->getParent();
	if (parent == fileToDelete) {  // in case it is the root directory
		return cantdeleteroot;
	}
	// Figure out if it is a directory through dynamic casting
	DirectoryFile* der = dynamic_cast<DirectoryFile*>(fileToDelete);
	// Image or text file
	if (der == nullptr) {
		parent->removeChild(fileToDelete->getName());
		delete fileToDelete;
		return success;
	} // Directory
	else {
		int overallResult = success;
		if (der->children.empty()) {
			parent->removeChild(fileToDelete->getName()); // remove from parent, this rewrites parent's contents also
			delete fileToDelete;
			return success;
		} // Recursively delete all of its contents
		else {
			map<string, AbstractFile*> myChildren = der->children;
			auto it = myChildren.begin();
			for (it; it != myChildren.end(); it++) {
				string childName = it->second->getName();
				string fullPath = path + "/" + childName;
				int tempResult;
				tempResult = deleteFileRecursively(fullPath);
				if (tempResult != success) {
					overallResult = tempResult;
				}
			}
			if (der->children.empty()) {
				parent->removeChild(fileToDelete->getName()); // remove from parent, this rewrites parent's contents also
				delete fileToDelete;
				return overallResult;
			}
			return overallResult;
		}
	}
}

int HierarchicalFileSystem::deleteFile(std::string path) {
	AbstractFile* fileToDelete = parsePath(path);
	if (fileToDelete == nullptr) {
		return filedoesnotexist; // unable to find file
	}
	if (openFiles.find(fileToDelete) != openFiles.end()) {
		return fileisopen; // file in use
	}
	AbstractFile* parent = fileToDelete->getParent();
	if (parent == fileToDelete) {  // in case it is the root directory
		return cantdeleteroot;
	}
	// Find out of it is a directory via dynamic casting
	DirectoryFile* der = dynamic_cast<DirectoryFile*>(fileToDelete);
	// Image or text file
	if (der == nullptr) {
		parent->removeChild(fileToDelete->getName()); // remove from parent, this rewrites parent's contents also
		delete fileToDelete;
		return success;
	} // Directory
	else {
		if (der->children.empty()) {
			parent->removeChild(fileToDelete->getName()); // remove from parent, this rewrites parent's contents also
			delete fileToDelete;
			return success;
		} // Throw an error for trying to delete a non empty directory
		else {
			return directorynotempty;
		}
	}

	/* parent->removeChild(fileToDelete->getName()); // remove from parent, this rewrites parent's contents also
	delete fileToDelete;
	return success; */
}

AbstractFile* HierarchicalFileSystem::openFile(std::string path) {
	AbstractFile* fileToOpen = parsePath(path);
	if (fileToOpen == nullptr) {
		return nullptr;
	}
	if (openFiles.find(fileToOpen) == openFiles.end()) {
		openFiles.insert(fileToOpen);
		return fileToOpen;
	}
	else {
		return nullptr;
	}
}

int HierarchicalFileSystem::closeFile(AbstractFile* file) {
	auto it = openFiles.find(file);
	if (it != openFiles.end()) { // file is currently open
		openFiles.erase(it);
		return success;
	}
	else { // file not open
		return filenotopen;
	}
}

AbstractFile* HierarchicalFileSystem::parsePath(string path) {
	size_t startIndex = 0;
	size_t endIndex = path.find_first_of('/', startIndex);
	AbstractFile* current;
	string directory;
	if (endIndex != string::npos) {
		directory = path.substr(startIndex, endIndex - startIndex);
		if (directory == "root") {
			current = rootDirectory;
			startIndex = endIndex + 1;
		}
		else {
			return nullptr;
		}
	}
	else {
		if (path.substr(startIndex, string::npos) == "root") {
			return rootDirectory;
		}
		else {
			return nullptr;
		}
	}
	// root handled, traverse the rest of the path
	while (startIndex != string::npos) {
		endIndex = path.find_first_of('/',startIndex);
		if (endIndex == string::npos) {
			directory = path.substr(startIndex, string::npos);
			startIndex = string::npos;
		}
		else {
			directory = path.substr(startIndex, endIndex - startIndex);
			startIndex = endIndex + 1;
		}
		current = current->getChild(directory);
		if (current == nullptr) {
			return nullptr;
		}
	}
	return current;
}

HierarchicalFileSystem::~HierarchicalFileSystem() {
	for (auto it = rootDirectory->children.begin(); it != rootDirectory->children.end(); ++it) {
		delete it->second;
	}
}
