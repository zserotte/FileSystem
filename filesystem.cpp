/*
Author: Jon Shidal
Purpose: CSE 332 lab 5
filesystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
*/

#include <iostream>
#include "SimpleFileSystem.h"
#include "SimpleFileFactory.h"
#include<algorithm>
#include<iterator>
#include "ReadFileVisitor.h"
#include "HierarchicalFileSystem.h"
#include "HierarchicalFileFactory.h"
#include "CommandPrompt.h"
#include "TouchCommand.h"
#include "CDCommand.h"
#include "LSCommand.h"
#include "RemoveCommand.h"
#include "CATCommand.h"
#include "DSCommand.h"
#include "CPCommand.h"
#include "symCommand.h"

using namespace std;

int main(int argc, char * arg[])
{
	// allocate all objects needed
	AbstractFileFactory* ff = new HierarchicalFileFactory();
	AbstractFileSystem *fs = new HierarchicalFileSystem();
	AbstractCommand* com = new TouchCommand(ff,fs);
	AbstractCommand* com1 = new CDCommand(fs);
	AbstractCommand* com2 = new LSCommand(fs);
	AbstractCommand* com3 = new RemoveCommand(fs);
	AbstractCommand* com4 = new CATCommand(fs);
	AbstractCommand* com5 = new DSCommand(fs);
	AbstractCommand* com6 = new CPCommand(fs);
	AbstractCommand* com7 = new symCommand(fs);

	// create command prompt and configure
	// NOTE: the above commands will not work with a SimpleFileSystem, it would need a new set of commands
	// or a second CommandPrompt class
	CommandPrompt cmd;
	cmd.setFileSystem(fs);
	cmd.setFileFactory(ff);
	cmd.addCommand("touch", com);
	cmd.addCommand("cd", com1);
	cmd.addCommand("ls", com2);
	cmd.addCommand("rm", com3);
	cmd.addCommand("cat", com4);
	cmd.addCommand("ds", com5);
	cmd.addCommand("cp", com6);
	cmd.addCommand("sym", com7);

	/*AbstractFile* myFile = ff->createFile("directory");
	AbstractFile* myFile1 = ff->createFile("you.txt");
	AbstractFile* myFile2 = ff->createFile("you.img");
	fs->addFile("root/directory", myFile);
	fs->addFile("root/directory/you.txt", myFile1);
	fs->addFile("root/directory/you.img", myFile2);
	fs->openFile("root/directory/you.txt"); */

	// run the command prompt
	cmd.run();

	// clean up dynamically allocated resources
	delete ff;
	delete fs; // all files are destroyed here (in the file system destructor)
	delete com;
	delete com1;
	delete com2;
	delete com3;
	delete com4;
	delete com5;
	delete com6;
	delete com7;

	return 0;
}

