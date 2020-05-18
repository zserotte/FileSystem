#include "Proxy.h"
#include <iostream>
using namespace std;

Proxy::Proxy(AbstractFile* original, std::string myName) : refCount(new int(0)), connectedFile(original),
name(myName), parent(nullptr) {
	*refCount = *refCount + 1;
}

Proxy::Proxy(const Proxy& p2, std::string myName) : refCount(p2.refCount), connectedFile(p2.connectedFile), 
name(myName), parent(nullptr) {
	*refCount = *refCount + 1;
}

Proxy::~Proxy() {
	*refCount -= 1;
	if (*refCount == 0) {
		delete connectedFile;
	}
}

std::vector<char> Proxy::read()
{
	return connectedFile->read();
}

int Proxy::write(std::vector<char> myVec)
{
	return connectedFile->write(myVec);
}

int Proxy::append(std::vector<char> myVec)
{
	return connectedFile->append(myVec);
}

unsigned int Proxy::getSize()
{
	return connectedFile->getSize();
}

std::string Proxy::getName()
{
	return name;
}

void Proxy::accept(AbstractFileVisitor* myAFV) {
	connectedFile->accept(myAFV);
}

AbstractFile* Proxy::clone()
{
	return connectedFile->clone();
}

int Proxy::addChild(AbstractFile* myChild)
{
	return notacomposite;
}

int Proxy::removeChild(std::string name)
{
	return notacomposite;
}

void Proxy::setParent(AbstractFile* myParent) {
	parent = myParent;
}

AbstractFile* Proxy::getChild(std::string name)
{
	cout << "Image and text files do not have children" << endl;
	return nullptr;
}

AbstractFile* Proxy::getParent()
{
	return parent;
}
