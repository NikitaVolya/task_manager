#pragma once

#include "TaskInterface.h"
#include "TaskInitVisitor.h"

using std::string;
using std::time_t;


class TaskFactoryInterface
{
public:
	virtual TaskInterface* create(const TaskInitVisitor* visitor) const = 0;
	virtual void remove(TaskInterface* task);
};


class TaskFactory : public TaskFactoryInterface
{
public:
	TaskInterface* create(const TaskInitVisitor* visitor) const override;
};


class LevelTaskFactory : public TaskFactoryInterface
{
public:
	TaskInterface* create(const TaskInitVisitor* visitor) const override;
};