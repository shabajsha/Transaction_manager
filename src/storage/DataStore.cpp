#include "../../include/storage/DataStore.h"

DataStore::DataStore()
{
}

DataStore::~DataStore()
{
}

// void DataStore::insertRecord(std::shared_ptr<Record> record) {
//     records_[record->getRecordId()] = record;
// }

// std::shared_ptr<Record> DataStore::getRecord(uint32_t record_id) const {
//     auto it = records_.find(record_id);
//     if (it != records_.end()) {
//         return it->second;
//     }
//     return nullptr;
// }

// bool DataStore::deleteRecord(uint32_t record_id) {
//     auto it = records_.find(record_id);
//     if (it != records_.end()) {
//         records_.erase(it);
//         return true;
//     }
//     return false;
// }

// bool DataStore::updateRecord(uint32_t record_id, const std::vector<uint8_t>& data) {
//     auto it = records_.find(record_id);
//     if (it != records_.end()) {
//         it->second->setData(data);
//         return true;
//     }
//     return false;
// }

int DataStore::read(std::string key){
    if (_records.find(key) == _records.end())
        _records[key] = 0;

    return _records[key];
}

void DataStore::write(std::string key){
    _records[key]++;
}
