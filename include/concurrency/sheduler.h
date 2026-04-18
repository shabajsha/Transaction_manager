#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "Transaction.h"
#include "DataStore.h"
using namespace std;

class Scheduler {
public:
    vector<Operation> createSchedule(vector<shared_ptr<Transaction>> & txns);
    void executeSchedule(vector<Operation> &schedule, DataStore& store);
};

#endif