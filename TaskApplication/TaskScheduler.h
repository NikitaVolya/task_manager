#pragma once
#include <string>
#include <ctime>
#include <vector>
#include <stdexcept>
#include "TaskInterface.h"

using std::string;
using std::time_t;
using std::vector;


class TaskScheduler
{
private:
	TaskScheduler* parent;

public:
	TaskScheduler() : parent{ nullptr } {};
	TaskScheduler(TaskScheduler* parentP) : parent{ parentP } {};
	~TaskScheduler() { if (parent) parent->remove(this); }

	virtual const string& getTitle() const = 0;
	virtual const string& getDescription() const = 0;
	virtual int getDaysLeft() const = 0;
	virtual time_t getDate() const = 0;
	virtual int getLevel() const = 0;

	virtual void add(TaskScheduler* value) = 0;
	virtual TaskScheduler* remove(TaskScheduler* value) = 0;
	virtual TaskScheduler* find(string title) = 0;

	virtual const TaskScheduler* getParent() const { return parent; }
	virtual TaskScheduler* getParent() { return parent; }
	virtual void setParent(TaskScheduler* value) { parent = value; }

	virtual bool isLeaf() const = 0;
};


class TaskComposide : public TaskScheduler
{
private:
	string title;
	string description;
	vector<TaskScheduler*> childes;

public:
	TaskComposide(const char* titleP, const char* descriptionP, TaskScheduler* parent) : 
		title{ titleP }, description{ descriptionP }, childes{}, TaskScheduler{parent} {};
	TaskComposide(const char* titleP, const char* descriptionP) : title{ titleP }, description{ descriptionP }, childes{} {};
	~TaskComposide();

	const string& getTitle() const override { return title; };
	const string& getDescription() const override { return description; };
	time_t getDate() const override { return 0; }

	virtual int getDaysLeft() const;
	virtual int getLevel() const;

	void add(TaskScheduler* value) override;
	TaskScheduler* remove(TaskScheduler* value) override;
	const vector<TaskScheduler*>& getChildes() const { return childes; }
	virtual TaskScheduler* find(string title) override;

	bool isLeaf() const override { return false; };
};


class TaskLeaf : public TaskScheduler
{
private:
	TaskInterface* task;

public:
	TaskLeaf(TaskInterface* taskP) : task{ taskP } {};
	TaskLeaf(TaskScheduler* parentP, TaskInterface* taskP) : task{ taskP }, TaskScheduler{ parentP } {};
	~TaskLeaf() { delete task; }

	const string& getTitle() const override { return task->getTitle(); };
	const string& getDescription() const override { return task->getDescription(); };
	int getDaysLeft() const override { return task->getDaysLeft(); };
	time_t getDate() const override { return task->getDate(); }

	virtual int getLevel() const { return task->getLevel(); };

	void add(TaskScheduler* value) override { throw std::invalid_argument("TaskLeaf is leaf without childes"); };
	TaskScheduler* remove(TaskScheduler* value) override { throw std::invalid_argument("TaskLeaf is leaf without childes"); };
	virtual TaskScheduler* find(string title) override;

	bool isLeaf() const override { return true; };
};
