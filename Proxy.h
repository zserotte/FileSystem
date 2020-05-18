#pragma once
#include "AbstractFile.h"

class Proxy : public AbstractFile {
private:
	int* refCount;
	AbstractFile* connectedFile;
	std::string name;
	AbstractFile* parent;
public:
	Proxy(AbstractFile* original, std::string myName);
	Proxy(const Proxy& p2, std::string myName);
	virtual std::vector<char> read() override;
	virtual int write(std::vector<char> myVec) override;
	virtual int append(std::vector<char> myVec) override;
	virtual unsigned int getSize() override;
	virtual std::string getName() override;

	virtual ~Proxy();

	virtual void accept(AbstractFileVisitor* myAFV) override;

	virtual AbstractFile* clone() override;

protected:
	virtual int addChild(AbstractFile* myChild) override;
	virtual int removeChild(std::string name) override;
	virtual void setParent(AbstractFile* myParent) override;
	virtual AbstractFile* getChild(std::string name) override;
	virtual AbstractFile* getParent() override;
};