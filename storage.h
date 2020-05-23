#ifndef _OPENCOLORIGO_STORAGE_H
#define _OPENCOLORIGO_STORAGE_H

#include <atomic>
#include <map>
#include <mutex>

namespace ocigo {

typedef uint64_t Id;

/*
class IndexMap
A class template for indexing values by an incrementing index
in a thread-safe map.
*/
template<class T>
class IndexMap {
public:
    IndexMap() : m_nextId(0) {}
    virtual ~IndexMap() = default;

    // Add a value to the map and return a reference id
    Id add(T value);

    // Get a value by its reference id.
    // Returns a default constructed value if it does
    // not exist.
    T get(Id id);

    // Remove a value by its reference id and return
    // true if it was removed.
    // Only erases the value from the map.
    bool remove(Id id);

private:
    std::atomic<Id> m_nextId;
    std::map<Id, T> m_map;
    std::mutex m_lock;
};

template<class T>
Id IndexMap<T>::add(T value) {
    Id id = ++m_nextId;
    std::lock_guard<std::mutex> lock(m_lock);
    m_map[id] = value;
    return id;
}

template<class T>
bool IndexMap<T>::remove(Id id) {
    std::lock_guard<std::mutex> lock(m_lock);
    return m_map.erase(id) > 0;
}

template<class T>
T IndexMap<T>::get(Id id)  {
    std::lock_guard<std::mutex> lock(m_lock);
    auto search = m_map.find(id);
    if (search == m_map.end()) {
        return T();
    }
    return search->second;
}

} // ocigo

#endif //_OPENCOLORIGO_STORAGE_H
