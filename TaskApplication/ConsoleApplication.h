#pragma once

#include "TaskScheduler.h"
#include "TaskFactory.h"
#include <iostream>


class ConsoleApplication
{
private:
	TaskScheduler* root;
	TaskScheduler* current_root;

	void createTask();
	void createList();
	void up();
	void down();
	void findTask();
	void removeElement();
	void printCurrentRoot();
public:
	ConsoleApplication() {
		root = new TaskComposide{ "main", "list of all tasks" };
		current_root = root;
	}
	~ConsoleApplication() {
		delete root;
	}

	void menu();
};

