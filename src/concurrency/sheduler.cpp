#include "Transaction_manager/include/concurrency/sheduler.h"

vector<Operation> Scheduler::createSchedule(vector<shared_ptr<Transaction>> &txns)
{
    vector<Operation> schedule;
    vector<int> index(txns.size(), 0);

    bool remaining = true;

    while (remaining)
    {
        remaining = false;
        for (int i = 0; i < txns.size(); i++)
        {
            if(index[i] < txns[i]->operations.size()){
                schedule.push_back(txns[i]->operations[index[i]]);
                index[i]++;
                remaining = true;
            }
        }
    }
    return schedule;
}

void Scheduler::executeSchedule(vector<Operation>& schedule, DataStore& store){

    for(Operation Op : schedule){
        if(Op.type == OperationType::READ){
            int val = store.read(Op.key);
            std::cout << "READ " << Op.key << " = " << val << std::endl;
        }
        else{
            store.write(Op.key);
            std::cout << "WRITE " << Op.key << std::endl;

        }
    }
}