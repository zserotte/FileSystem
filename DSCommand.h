#pragma once

#include "AbstractCommand.h"

class DSCommand : public AbstractCommand {
public:
	DSCommand(AbstractFileSystem* fileSys);
	virtual int execute(std::string& CWD, std::string options) override;
	virtual void displayInfo() override;
	virtual ~DSCommand() = default;
private:
	AbstractFileSystem* fs;
};
