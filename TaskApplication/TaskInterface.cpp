#include "TaskInterface.h"

int TaskInterface::getDaysLeft() const
{
    time_t now = time(NULL);
    double rep = std::difftime(date, now) / 86400;
    return static_cast<int>(rep);
}
