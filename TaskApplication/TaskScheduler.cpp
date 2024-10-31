#include "TaskScheduler.h"

TaskComposide::~TaskComposide()
{
    for (TaskScheduler* i : childes)
    {
        delete i;
    }
}

int TaskComposide::getDaysLeft() const
{
    if (childes.size() == 0)
        return 0;

    int rep = childes[0]->getDaysLeft();

    for (const TaskScheduler* i : childes)
    {
        int tmp = i->getDaysLeft();
        if (tmp > rep)
            rep = tmp;
    }

    return rep;
}

int TaskComposide::getLevel() const
{
    if (childes.size() == 0)
        return 0;

    int rep = childes[0]->getLevel();

    for (const TaskScheduler* i : childes)
    {
        int tmp = i->getLevel();
        if (tmp > rep)
            rep = tmp;
    }

    return rep;
}

void TaskComposide::add(TaskScheduler* value)
{
    value->setParent(this);
    childes.push_back(value);
}

TaskScheduler* TaskComposide::remove(TaskScheduler* value)
{
    auto it = std::find(childes.begin(), childes.end(), value);

    // If element was found
    if (it == childes.end())
        return nullptr;

    childes.erase(it);
    value->setParent(nullptr);
    return value;
}

TaskScheduler* TaskComposide::find(string titleP)
{
    if (title == titleP)
        return this;
    for (TaskScheduler* i : childes)
    {
        TaskScheduler* rep = i->find(titleP);
        if (rep)
            return rep;
    }
    return nullptr;
}

TaskScheduler* TaskLeaf::find(string title)
{
    if (task->getTitle() == title)
        return this;
    return nullptr;
}
