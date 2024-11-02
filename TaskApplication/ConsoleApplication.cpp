#include "ConsoleApplication.h"
#include <iomanip>
#include <conio.h>

std::string truncate(const std::string& text, size_t width) {
	if (text.length() > width - 1) {
		return text.substr(0, width - 4) + "...";
	}
	else {
		return text;
	}
}

void ConsoleApplication::createTask()
{
	system("cls");

	if (current_root->isLeaf())
		return;

	string title, description;
	std::tm date_input = {};
	char hasPriority;
	int level;
	TaskInitVisitor* options;
	TaskFactoryInterface* factory;

	
	std::cout << "Task creation started...\n";
	std::cin.ignore(100, '\n');

	while (true)
	{
		std::cout << "Enter the title: ";
		
		std::getline(std::cin, title);

		TaskScheduler* leaf = root->find(title);
		if (!leaf)
			break;
		std::cout << "A task with this title already exists.\n";
	}
	
	std::cout << "Enter the description: ";
	std::getline(std::cin, description);

	std::cout << "Enter the task date [day-month-year]: ";
	std::cin >> std::get_time(&date_input, "%d-%m-%Y");
	std::time_t date = std::mktime(&date_input);
	
	std::cin.ignore(100, '\n');
	std::cout << "Is there a priority level for this task? (y/n): ";
	std::cin >> hasPriority;

	if (hasPriority == 'y' || hasPriority == 'Y')
	{
		std::cout << "Enter the priority level (number): ";
		std::cin >> level;

		options = new TaskLevelInitVisitor(title.c_str(), description.c_str(), date, level);
		factory = new LevelTaskFactory();
	}
	else
	{
		options = new TaskInitVisitor(title.c_str(), description.c_str(), date);
		factory = new TaskFactory();
	}
	TaskLeaf* new_leaf = new TaskLeaf{ factory->create(options) };
	current_root->add(new_leaf);

	delete factory;
}

void ConsoleApplication::createList()
{
	if (current_root->isLeaf())
		return;

	system("cls");

	string title, description;
	std::cout << "Task list creation started...\n";
	std::cin.ignore(100, '\n');
	std::cout << "Enter the title: ";
	std::getline(std::cin, title);

	std::cout << "Enter the description: ";
	std::getline(std::cin, description);

	TaskComposide* new_list = new TaskComposide{ title.c_str(), description.c_str() };
	current_root->add(new_list);
}

void ConsoleApplication::move_up()
{
	if (!current_root->getParent())
		return;
	current_root = current_root->getParent();
	cursor_position = 0;
}

void ConsoleApplication::move_down()
{
	if (current_root->isLeaf())
		return;

	vector<TaskScheduler*> list = ((TaskComposide*)(current_root))->getChildes();
	if (cursor_position < 0 || cursor_position >= list.size())
		return;

	current_root = list[cursor_position];
	cursor_position = 0;
}

void ConsoleApplication::findTask()
{
	string title;
	std::cout << "Title: ";
	std::getline(std::cin, title);
	TaskScheduler* leaf = root->find(title);

	if (!leaf)
		return;

	current_root = leaf;
}

void ConsoleApplication::removeElement()
{
	if (current_root->isLeaf())
	{
		TaskScheduler* parent = current_root->getParent();
		root->remove(current_root);
		current_root = parent;
	}
	else {
		TaskComposide* composide = (TaskComposide*)current_root;
		
		if (cursor_position < 0 || cursor_position >= composide->getChildes().size())
			return;

		composide->remove(composide->getChildes()[cursor_position]);
	}
	cursor_position = 0;
}

void ConsoleApplication::displayMenu()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 11);
	std::cout << "MENU: arrows up/down) move cursore ";
	SetConsoleTextAttribute(handle, 15);
	std::cout << "1) New Task   ";
	SetConsoleTextAttribute(handle, 10);
	std::cout << "2) New List   ";
	SetConsoleTextAttribute(handle, 12);
	std::cout << "backspace) Delete   ";
	SetConsoleTextAttribute(handle, 14);
	std::cout << "esc) Close   ";
	SetConsoleTextAttribute(handle, 9);
	std::cout << "enter) Open   ";
	SetConsoleTextAttribute(handle, 8);
	std::cout << "f) Find   ";
	SetConsoleTextAttribute(handle, 13);
	std::cout << "q) Quit\n";
	SetConsoleTextAttribute(handle, 15);
}

void ConsoleApplication::printCurrentRoot()
{
	if (!current_root->isLeaf())
	{
		std::cout << std::endl << std::string(75, '=') << std::endl;
		std::cout << current_root->getTitle() << std::endl;
		std::cout << current_root->getDescription();
		std::cout << std::endl << std::string(75, '=') << std::endl;

		const size_t titleWidth = 20;
		const size_t descWidth = 30;

		std::cout << std::left << "  "
			<< std::setw(titleWidth) << "Task Title"
			<< std::setw(descWidth) << "Description"
			<< std::setw(15) << "Days Left"
			<< std::setw(10) << "Priority" << "\n";
		std::cout << std::string(titleWidth + descWidth + 25, '-') << "\n";

		const vector<TaskScheduler*> list = ((TaskComposide*)(current_root))->getChildes();
		int i = 0;
		for (auto it : list)
		{
			std::cout << std::left
				<< (i == cursor_position ? "> " : "  ")
				<< std::setw(titleWidth) << truncate(it->getTitle(), titleWidth)
				<< std::setw(descWidth) << truncate(it->getDescription(), descWidth)
				<< std::setw(15) << it->getDaysLeft()
				<< std::setw(10) << it->getLevel() << "\n";
			if (!it->isLeaf())
			{
				const vector<TaskScheduler*> sub_list = ((TaskComposide*)(it))->getChildes();
				for (auto sub_it : sub_list)
				{
					std::cout << "\t-" << truncate(sub_it->getTitle(), titleWidth) << std::endl;
				}
			}
			i++;
		}
	}
	else {
		std::cout << std::endl << std::string(75, '=') << std::endl;
		std::cout << "Title: " << std::endl;
		std::cout << current_root->getTitle() << std::endl;
		std::cout << "Description: " << std::endl;
		std::cout << current_root->getDescription() << std::endl;
		std::cout << "Days left: " << current_root->getDaysLeft() << std::endl;
		std::cout << "Priority level: " << current_root->getLevel() << std::endl;
		std::cout << std::string(75, '=') << std::endl;
	}
}

void ConsoleApplication::cursorUp()
{
	if (cursor_position > 0)
		cursor_position--;
}

void ConsoleApplication::cursorDown()
{
	if (current_root->isLeaf())
		return;

	int size = ((TaskComposide*)(current_root))->getChildes().size();
	if (cursor_position >= size - 1)
		return;

	cursor_position++;
}

void ConsoleApplication::menu()
{
	while (true)
	{
		system("cls");
		int choice;
		displayMenu();
		printCurrentRoot();
		choice = _getch();
		switch (choice)
		{
		case '1':
			createTask();
			saveManager->save(root);
			break;
		case '2':
			createList();
			saveManager->save(root);
			break;
		case 'f':
			findTask();
			break;
		case 8:
			removeElement();
			saveManager->save(root);
			break;
		case 27:
			move_up();
			break;
		case 13:
			move_down();
			break;
		case 'q':
			return;
		case 72:
			cursorUp();
			break;
		case 80:
			cursorDown();
			break;
		}
	}
}
