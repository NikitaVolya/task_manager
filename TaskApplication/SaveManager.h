#pragma once

#include "TaskScheduler.h"
#include <fstream>
#include <iostream>

class SaveManager
{
private:
	TaskScheduler** table;
	uint16_t size;

	uint16_t getSizeOfTree(TaskScheduler* root);
	void writeNodeToTable(TaskScheduler* node, uint16_t& slice);
	void genarateTable(TaskScheduler* root);
	int findInTable(TaskScheduler* node);

	TaskScheduler* loadNode(std::ifstream& file);
	void saveNode(std::ofstream& file, TaskScheduler* node);
public:
	SaveManager() : table{ nullptr }, size{0} {};

	TaskScheduler* load();
	void save(TaskScheduler* root);
};

