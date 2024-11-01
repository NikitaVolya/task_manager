#pragma once

#include "TaskScheduler.h"
#include "TaskFactory.h"
#include <iostream>


class ConsoleApplication
{
private:
	TaskScheduler* root;
	TaskScheduler* current_root;

	int cursor_position;

	void createTask();
	void createList();

	void move_up();
	void move_down();
	
	void findTask();
	void removeElement();

	void printCurrentRoot();

	void cursorUp();
	void cursorDown();
public:
	ConsoleApplication() {
		root = new TaskComposide{ "main", "list of all tasks" };
		current_root = root;
		cursor_position = 0;
	}
	~ConsoleApplication() {
		delete root;
	}

	void menu();
};

