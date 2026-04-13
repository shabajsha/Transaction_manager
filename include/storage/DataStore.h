#ifndef DATA_STORE_H
#define DATA_STORE_H

#include <map>
#include <memory>
#include <cstdint>
#include "Record.h"
#include <unordered_map>
#include <Transaction.h>
#include <iostream>
// class DataStore {
// public:
//     DataStore();
//     ~DataStore();

//     void insertRecord(std::shared_ptr<Record> record);
//     std::shared_ptr<Record> getRecord(uint32_t record_id) const;
//     bool deleteRecord(uint32_t record_id);
//     bool updateRecord(uint32_t record_id, const std::vector<uint8_t>& data);

//     // TODO: Implement buffer page management

// private:
//     std::map<uint32_t, std::shared_ptr<Record>> records_;
// };

class DataStore
{
public:
    int read(std::string key);
    void write(std::string key);
    void actions(Operation Op);

private:
    std::unordered_map<std::string, int> _records;
};


#endif // DATA_STORE_H
