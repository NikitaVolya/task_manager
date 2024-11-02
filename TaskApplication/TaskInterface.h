#pragma once
#include <string>
#include <ctime>

using std::string;
using std::time_t;

class TaskInterface
{
private:
	string title;
	string descripton;
	time_t date;

public:
	TaskInterface(const char* titleP, const char* descriptionP, time_t dateP) :
		title{ titleP }, descripton{ descriptionP }, date{ dateP } {}

	const string& getTitle() const { return title; }
	const string& getDescription() const { return descripton; }
	time_t getDate() const { return date; }
	int getDaysLeft() const;
	virtual int getLevel() const = 0;

	void setTitle(const string& value) { title = value; }
	void setDescription(const string& value) { descripton = value; }
	void setDate(const time_t& value) { date = value; }
	virtual void setLevel(int value) = 0;
	
};


class Task : public TaskInterface
{
public:
	Task(const char* titleP, const char* descriptionP, time_t dateP) : TaskInterface {titleP, descriptionP, dateP} {}
	int getLevel() const override { return 0; }
	void setLevel(int value) {}
};


class LevelTask : public TaskInterface
{
private:
	int level;
public:
	LevelTask(const char* titleP, const char* descriptionP, time_t dateP, int levelP) : TaskInterface{ titleP, descriptionP, dateP }, level{levelP} {}
	int getLevel() const override { return level; }
	void setLevel(int value) { level = value; }
};
