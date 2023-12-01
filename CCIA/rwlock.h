//
// Created by just do it on 2023/12/1.
//

#ifndef CODE_SNIP_RWLOCK_H
#define CODE_SNIP_RWLOCK_H

#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>

class dns_entry {

};

class dns_cache {
private:
    std::map<std::string, dns_entry> entries_;
    mutable std::shared_timed_mutex entry_mutex_;
public:
    dns_entry find_entry(const std::string& domain) const {
        std::shared_lock<std::shared_timed_mutex> lk(entry_mutex_);
        const auto it = entries_.find(domain);
        return (it == entries_.end()) ? dns_entry{}: it->second;
    }
    void update_or_add_entry(const std::string& domain, const dns_entry& entry) {
        std::lock_guard<std::shared_timed_mutex> lg(entry_mutex_);
        entries_[domain] = entry;
    }
};

#endif //CODE_SNIP_RWLOCK_H
