#pragma once
#include <string>
#include <ctime>

using std::string;
using std::time_t;

class TaskInitVisitor
{
private:
	string title;
	string description;
	time_t date;

public:
	TaskInitVisitor(const char* titleP, const char* descriptionP, time_t dateP) :
		title{ titleP }, description{ descriptionP }, date{ dateP } {}

	const string& getTitle() const { return title; }
	const string& getDescription() const { return description; }
	const time_t& getDate() const { return date; }
	virtual int getLevel() const { return 0; };

	void setTitle(const string& value) { title = value; }
	void setDescription(const string& value) { description = value; }
	void setDate(const time_t& value) { date = value; }
	virtual void setLevel(int value) {};
};


class TaskLevelInitVisitor : public TaskInitVisitor
{
private:
	int level;
public:
	TaskLevelInitVisitor(const char* titleP, const char* descriptionP, time_t dateP, int levelP) :
		TaskInitVisitor{ titleP, descriptionP, dateP }, level{levelP} {}

	int getLevel() const { return level; }
	void setLevel(int value) { level = value; }
};