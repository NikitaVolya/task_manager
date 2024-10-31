#include "ConsoleApplication.h"
#include <iomanip>

void ConsoleApplication::createTask()
{
	string title, description;
	char level_option;
	int level;
	std::tm date_input = {};

	int year, month, day;
	std::cin.ignore(100, '\n');
	std::cout << "Task title: ";
	std::getline(std::cin, title);

	std::cout << "Task description: ";
	std::getline(std::cin, description);

	std::cout << "Task date to complete [year-month-day]: ";
	std::cin >> std::get_time(&date_input, "%Y-%m-%d");
	std::time_t date = std::mktime(&date_input);
	
	std::cin.ignore(100, '\n');
	std::cout << "Add level? [y/n]: ";
	std::cin >> level_option;

	TaskInitVisitor* options;
	TaskFactoryInterface* factory;
	if (level_option == 'y' || level_option == 'Y')
	{
		std::cout << "Task level: ";
		std::cin >> level;

		options = new TaskLevelInitVisitor(title.c_str(), description.c_str(), date, level);
		factory = new LevelTaskFactory();
	}
	else
	{
		options = new TaskInitVisitor(title.c_str(), description.c_str(), date);
		factory = new TaskFactory();
	}
	TaskInterface* task = factory->create(options);
	TaskLeaf* leaf = new TaskLeaf{ task };
	current_root->add(leaf);

	delete factory;
}

void ConsoleApplication::createList()
{
	string title, description;
	std::cin.ignore(100, '\n');
	std::cout << "Task list title: ";
	std::getline(std::cin, title);

	std::cout << "Task list description: ";
	std::getline(std::cin, description);

	TaskComposide* new_list = new TaskComposide{ title.c_str(), description.c_str() };
	current_root->add(new_list);
}

void ConsoleApplication::up()
{
	if (!current_root->getParent())
		return;
	current_root = current_root->getParent();
}

void ConsoleApplication::down()
{
	string title;
	std::cin.ignore(100, '\n');
	std::cout << "Title: ";
	std::getline(std::cin, title);
	TaskScheduler* node = current_root->find(title);
	if (!node)
		return;
	if (node->isLeaf())
		return;

	current_root = node;
}

void ConsoleApplication::findTask()
{
	string title;
	std::cin.ignore(100, '\n');
	std::cout << "Title: ";
	std::getline(std::cin, title);
	TaskScheduler* leaf = current_root->find(title);

	if (!leaf)
		return;
	
	if (!leaf->getParent())
		return;

	current_root = leaf->getParent();
}

void ConsoleApplication::removeElement()
{
	string title;
	std::cin.ignore(100, '\n');
	std::cout << "Title: ";
	std::getline(std::cin, title);
	TaskScheduler* node = current_root->find(title);
	if (!node)
		return;
	if (!node->getParent())
		return;
	node->getParent()->remove(node);
}

void ConsoleApplication::printCurrentRoot()
{
	if (!current_root->isLeaf())
	{
		std::cout << "\n=======================================\n";
		std::cout << current_root->getTitle();
		std::cout << "\n--------------------------\n";
		std::cout << current_root->getDescription();
		std::cout << "\n=======================================\n";

		const vector<TaskScheduler*> list = ((TaskComposide*)(current_root))->getChildes();
		for (auto it : list)
		{
			std::cout << it->getTitle() << " | " << it->getDescription() << " | Days left: " << it->getDaysLeft() << " | " << it->getLevel() << std::endl;

			if (!it->isLeaf())
			{
				const vector<TaskScheduler*> sub_list = ((TaskComposide*)(it))->getChildes();
				for (auto sub_it : sub_list)
				{
					std::cout << "\t-" << sub_it->getTitle() << std::endl;
				}
			}
		}
	}
}

void ConsoleApplication::menu()
{
	while (true)
	{
		system("cls");
		int choice;
		std::cout << "1. create task\n2. create list\n3. find task\n4. remove\n5. up\n6. down\n7. quit\n";
		printCurrentRoot();
		std::cin >> choice;

		switch (choice)
		{
		case 1:
			createTask();
			break;
		case 2:
			createList();
			break;
		case 3:
			findTask();
			break;
		case 4:
			removeElement();
			break;
		case 5:
			up();
			break;
		case 6:
			down();
			break;
		case 7:
			return;
		}
	}
}
