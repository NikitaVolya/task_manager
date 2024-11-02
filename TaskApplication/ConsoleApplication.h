#pragma once

#include "TaskScheduler.h"
#include "TaskFactory.h"
#include "SaveManager.h"
#include <iostream>
#include <windows.h>


class ConsoleApplication
{
private:
	TaskScheduler* root;
	TaskScheduler* current_root;
	SaveManager* saveManager;

	int cursor_position;

	void createTask();
	void createList();

	void move_up();
	void move_down();
	
	void findTask();
	void removeElement();

	void displayMenu();
	void printCurrentRoot();

	void cursorUp();
	void cursorDown();
public:
	ConsoleApplication() {

		saveManager = new SaveManager;
		root = saveManager->load();

		if (!root)
			root = new TaskComposide{ "main", "list of all tasks" };

		current_root = root;
		cursor_position = 0;
	}
	~ConsoleApplication() {
		delete root;
		delete saveManager;
	}

	void menu();
};

