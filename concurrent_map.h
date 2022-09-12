#pragma once

#include <map>
#include <vector>
#include <mutex>
#include <thread>

size_t NUMBER_OF_THREADS = std::thread::hardware_concurrency();

template <typename Key, typename Value>
class concurrent_map {
public:
    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys");

    struct Access {
        std::lock_guard<std::mutex> guard;
        Value& ref_to_value;
    };

    struct Part {
        std::mutex part_m_;
        std::map<Key, Value> part_data_;
    };

    explicit concurrent_map(size_t bucket_count = NUMBER_OF_THREADS)
        :number_of_parts_(bucket_count), data_(bucket_count)
    {
    }

    Access operator[](const Key& key) {
        size_t index = FindIndex(key);
        return { std::lock_guard(data_[index].part_m_), data_[index].part_data_[key] };
    }

    const Value& at(const Key& key) {
    	size_t index = FindIndex(key);
    	return data_[index].part_data_.at(key);
    }

    bool count(const Key& key) {
    	size_t index = FindIndex(key);
    	return data_[index].part_data_.count(key);
    }

    std::map<Key, Value> BuildOrdinaryMap() {
        std::map<Key, Value> result;

        for (size_t i = 0; i < number_of_parts_; ++i) {
            std::lock_guard c(data_[i].part_m_);
            result.insert(data_[i].part_data_.begin(), data_[i].part_data_.end());
        }

        return result;
    }

private:
    size_t number_of_parts_ = 4u;
    std::vector<Part> data_;

    size_t FindIndex(const Key& key) const {
    	return key % number_of_parts_;
    }
};
