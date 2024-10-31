#include "TaskFactory.h"

void TaskFactoryInterface::remove(TaskInterface* task)
{
	delete task;
	task = nullptr;
}

TaskInterface* TaskFactory::create(const TaskInitVisitor* visitor) const
{
	TaskInterface* rep = new Task{ visitor->getTitle().c_str(), visitor->getDescription().c_str(), visitor->getDate()};
	delete visitor;
	visitor = nullptr;
	return rep;
}

TaskInterface* LevelTaskFactory::create(const TaskInitVisitor* visitor) const
{
	TaskInterface* rep = new LevelTask{ visitor->getTitle().c_str(), visitor->getDescription().c_str(), visitor->getDate(), visitor->getLevel()};
	delete visitor;
	visitor = nullptr;
	return rep;
}
