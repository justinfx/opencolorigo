#ifndef _OPENCOLORIGO_SHARED_PTR_MAP_H
#define _OPENCOLORIGO_SHARED_PTR_MAP_H

#include <map>
#include <atomic>
#include <mutex>

namespace ocigo {

typedef uint64_t Id;

template<class SharedPtrT>
class SharedPtrMap {
public:
    SharedPtrMap() : m_nextId(0) {}

    ~SharedPtrMap() = default;

    Id add(SharedPtrT ptr);

    SharedPtrT get(Id id);

    bool remove(Id id);

private:
    std::atomic<Id> m_nextId;
    std::map<Id, SharedPtrT> m_map;
    std::mutex m_lock;
};

template<class SharedPtrT>
Id SharedPtrMap<SharedPtrT>::add(SharedPtrT ptr) {
    Id id = ++m_nextId;
    std::lock_guard<std::mutex> lock(m_lock);
    m_map[id] = ptr;
    return id;
}

template<class SharedPtrT>
bool SharedPtrMap<SharedPtrT>::remove(Id id) {
    std::lock_guard<std::mutex> lock(m_lock);
    return m_map.erase(id) > 0;
}

template<class SharedPtrT>
SharedPtrT SharedPtrMap<SharedPtrT>::get(Id id)  {
    std::lock_guard<std::mutex> lock(m_lock);
    auto search = m_map.find(id);
    if (search == m_map.end()) {
        return SharedPtrT();
    }
    return search->second;
}

} // ocigo

#endif // _OPENCOLORIGO_SHARED_PTR_MAP_H
