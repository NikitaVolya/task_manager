#include "SaveManager.h"


uint16_t SaveManager::getSizeOfTree(TaskScheduler* root)
{
	if (root->isLeaf())
		return 1;

	uint16_t rep = 1;
	TaskComposide* composite = (TaskComposide*)root;

	for (auto it : composite->getChildes())
	{
		rep += getSizeOfTree(it);
	}

	return rep;
}

void SaveManager::writeNodeToTable(TaskScheduler* node, uint16_t& slice)
{
	table[slice++] = node;
	if (node->isLeaf())
		return;

	TaskComposide* composite = (TaskComposide*)node;
	for (auto it : composite->getChildes())
		writeNodeToTable(it, slice);
}

void SaveManager::genarateTable(TaskScheduler* root)
{
	size = getSizeOfTree(root);
	table = new TaskScheduler * [size];

	uint16_t index = 0;
	writeNodeToTable(root, index);
}

int SaveManager::findInTable(TaskScheduler* node)
{
	for (int i = 0; i < size; i++)
		if (table[i] == node)
			return i;
	return -1;
}

TaskScheduler* SaveManager::loadNode(std::ifstream& file)
{
	TaskScheduler* node;
	bool isLeaf;
	int parent;
	int level;
	time_t date;
	char* title = nullptr;
	char* description = nullptr;

	std::cout << "Read node\n";

	file.read((char*)&parent, sizeof(parent));
	file.read((char*)&isLeaf, sizeof(isLeaf));

	std::cout << "Is leaf: " << isLeaf << "  Parent: " << parent << std::endl;

	if (isLeaf)
	{
		file.read((char*)&level, sizeof(level));
		file.read((char*)&date, sizeof(date));
	}

	size_t titleSize;
	file.read((char*)&titleSize, sizeof(titleSize));
	title = new char[titleSize + 1];
	title[titleSize] = '\0';
	file.read(title, titleSize);

	size_t descriptionSize;
	file.read((char*)&descriptionSize, sizeof(descriptionSize));
	description = new char[descriptionSize + 1];
	description[descriptionSize] = '\0';
	file.read(description, descriptionSize);

	std::cout << title << " " << description << std::endl;

	if (isLeaf)
	{
		LevelTask* task = new LevelTask(title, description, date, level);
		{
			node = new TaskLeaf(table[parent], task);
			
		}
	}
	else
		if (parent == -1)
			node = new TaskComposide{ title, description };
		else
			node = new TaskComposide{ title, description, table[parent] };

	if (parent != -1)
	{
		TaskComposide* parentNode = (TaskComposide*)table[parent];
		parentNode->add(node);
	}

	delete[] title;
	delete[] description;

	return node;
}

void SaveManager::saveNode(std::ofstream& file, TaskScheduler* node)
{
	bool isLeaf = node->isLeaf();
	int parent = findInTable(node->getParent());

	
	file.write((char*)&parent, sizeof(int));
	file.write((char*)&isLeaf, sizeof(bool));
	if (isLeaf)
	{
		int level = node->getLevel();
		file.write((char*)&level, sizeof(int));
		time_t date = node->getDate();
		file.write((char*)&date, sizeof(time_t));
	}

	size_t titleSize = node->getTitle().size();
	const char* titleText = node->getTitle().c_str();
	file.write((char*)&titleSize, sizeof(size_t));
	file.write(titleText, titleSize);

	size_t descriptionSize = node->getDescription().size();
	const char* descriptionText = node->getDescription().c_str();
	file.write((char*)&descriptionSize, sizeof(size_t));
	file.write(descriptionText, descriptionSize);
}


TaskScheduler* SaveManager::load()
{
	std::ifstream file("data", std::ios::binary);
	if (!file.is_open())
		return nullptr;

	file.read((char*) & size, sizeof(size));
	std::cout << size << std::endl;

	table = new TaskScheduler * [size];
	std::cout << "Table is create. Start read nodes\n";
	for (int i = 0; i < size; i++)
		table[i] = loadNode(file);


	return table[0];
}

void SaveManager::save(TaskScheduler* root)
{
	genarateTable(root);

	std::ofstream file("data", std::ios::binary | std::fstream::out);
	if (!file.is_open())
		return;

	file.write((char*)&size, sizeof(uint16_t));

	for (int i = 0; i < size; i++)
		saveNode(file, table[i]);

	file.close();
	delete[] table;
	table = nullptr;
	size = 0;
}
