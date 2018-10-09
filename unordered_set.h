#pragma once

#include <functional>
#include <algorithm>
#include "vector.h"

template<typename T>
class unordered_set {
public:

    class const_iterator {
    public:

        typedef typename vector<T>::const_iterator vec_const_it;

        const_iterator();

        const_iterator(const vector<vector<T>> &, const vec_const_it &, const size_t, const size_t);

        const_iterator &operator++();

        operator const T *() const;

    private:
        const vector<vector<T>> &elemHashesForIters;
        vec_const_it it;
        size_t index;
        const size_t lastIndex; // index of chunk with global end()
    };

    class iterator {
    public:

        typedef typename vector<T>::iterator vec_it;

        iterator();

        iterator(vector<vector<T>> &hashes, const vec_it &_it, const size_t _index, const size_t last_index);

        iterator &operator++();

        operator const_iterator();

        operator T *();

    private:
        vector<vector<T>> &elemHashesForIters;
        vec_it it;
        size_t index;
        const size_t lastIndex; // index of chunk with global end()
    };

    unordered_set();

    void insert(const T &k);

    bool erase(const T &k);

    bool find(const T &k) const;

    void clear();

    bool empty() const;

    size_t size() const;

    const_iterator begin() const;

    const_iterator end() const;

    iterator begin();

    iterator end();

private:
    vector<vector<T>> elemHashes;
    size_t elemCount;                            // sum of elements in all buckets
    myHash <T> hasher;                            // standart stl hasher

    size_t currPowerOf2;                         // count of buckets

    static size_t fastMod(size_t number, size_t powerOf2);

    static size_t enlargeFunc(size_t elemCount);

    vector<vector<T>> createRemappedTable() const;
};

template<typename T>
inline unordered_set<T>::unordered_set() {
    /*
    * the largest this power the less we have to reallocate
    * but more memory will be used
    */
    elemCount = 0;
    int power = 2;
    currPowerOf2 = 1 << power;
    elemHashes.resize(currPowerOf2);
}

template<typename T>
inline void unordered_set<T>::insert(const T &k) {
    if (!find(k)) {
        elemCount++;
        size_t expectedSize = enlargeFunc(elemCount);
        if (expectedSize > elemHashes.size()) {
            currPowerOf2 *= 2;
            elemHashes = createRemappedTable();                // complex operation
        }
        size_t hashIndex = fastMod(hasher(k), currPowerOf2);
        elemHashes[hashIndex].push_back(k);
    }
}

template<typename T>
inline bool unordered_set<T>::erase(const T &k) {
    if (find(k)) {
        elemCount--;
        size_t hashIndex = fastMod(hasher(k), currPowerOf2);
        return elemHashes[hashIndex].erase(k);
    } else return false;
}

template<typename T>
inline bool unordered_set<T>::find(const T &k) const {
    size_t hashIndex = fastMod(hasher(k), currPowerOf2);
    const vector<T> &bucket = elemHashes[hashIndex];

    for (auto it : bucket) {
        if (it == k) {
            return true;
        }
    }
    return false;
}

template<typename T>
inline void unordered_set<T>::clear() {
    for (auto i : elemHashes) {
        i.clear();
    }
    elemCount = 0;
}

template<typename T>
inline bool unordered_set<T>::empty() const {
    return elemCount == 0;
}

template<typename T>
inline size_t unordered_set<T>::size() const {
    return elemCount;
}

template<typename T>
inline typename unordered_set<T>::const_iterator unordered_set<T>::begin() const {
    if (elemCount > 0) {
        size_t firstInd = 0;
        while ((firstInd + 1) < elemHashes.size() && elemHashes[firstInd].empty()) {
            firstInd++;
        }
        size_t lastInd = currPowerOf2 - 1;
        while (lastInd > 0 && elemHashes[lastInd].empty()) {
            lastInd--;
        }
        return const_iterator(elemHashes, elemHashes[firstInd].begin(), firstInd, lastInd);
    } else return const_iterator(elemHashes, elemHashes[0].begin(), 0, 0);
}

template<typename T>
inline typename unordered_set<T>::const_iterator unordered_set<T>::end() const {
    if (elemCount > 0) {
        size_t lastInd = currPowerOf2 - 1;
        while (lastInd > 0 && elemHashes[lastInd].empty()) {
            lastInd--;
        }
        return const_iterator(elemHashes, elemHashes[lastInd].end(), lastInd, lastInd);
    } else return const_iterator(elemHashes, elemHashes[0].end(), 0, 0);
}

template<typename T>
inline typename unordered_set<T>::iterator unordered_set<T>::begin() {
    if (elemCount > 0) {
        size_t firstInd = 0;
        while ((firstInd + 1) < elemHashes.size() && elemHashes[firstInd].empty()) {
            firstInd++;
        }
        size_t lastInd = currPowerOf2 - 1;
        while (lastInd > 0 && elemHashes[lastInd].empty()) {
            lastInd--;
        }
        return iterator(elemHashes, elemHashes[firstInd].begin(), firstInd, lastInd);
    } else return iterator(elemHashes, elemHashes[0].begin(), 0, 0);
}

template<typename T>
inline typename unordered_set<T>::iterator unordered_set<T>::end() {
    if (elemCount > 0) {
        size_t lastInd = currPowerOf2 - 1;
        while (lastInd > 0 && elemHashes[lastInd].empty()) {
            lastInd--;
        }
        return iterator(elemHashes, elemHashes[lastInd].end(), lastInd, lastInd);
    } else return iterator(elemHashes, elemHashes[0].end(), 0, 0);
}

template<typename T>
inline size_t unordered_set<T>::fastMod(size_t number, size_t powerOf2) {
    return number & (powerOf2 - 1);
}

template<typename T>
inline size_t unordered_set<T>::enlargeFunc(size_t elemCount) {
    return elemCount / 2 + 1;
}

template<typename T>
inline vector<vector<T>> unordered_set<T>::createRemappedTable() const {
    vector<vector<T>> newElemHashes;
    newElemHashes.resize(currPowerOf2);
    for (auto i : elemHashes) {
        for (auto j : i) {
            size_t hashIndex = fastMod(hasher(j), currPowerOf2);
            newElemHashes[hashIndex].push_back(j);
        }
    }
    return newElemHashes;
}

template<typename T>
inline unordered_set<T>::const_iterator::const_iterator() {}

template<typename T>
inline unordered_set<T>::const_iterator::const_iterator(const vector<vector<T>> &hashes, const vec_const_it &_it,
                                                        const size_t _index, const size_t last_index)
        : elemHashesForIters(hashes), it(_it), index(_index), lastIndex(last_index) {}

template<typename T>
inline typename unordered_set<T>::const_iterator &unordered_set<T>::const_iterator::operator++() {
    if (it != elemHashesForIters[index].end()) {
        ++it;
        // skip end()s of buckets except of last one;
        if (it == elemHashesForIters[index].end()) {
            if (it != elemHashesForIters[lastIndex].end()) {
                do {
                    index++;
                    it = elemHashesForIters[index].begin();
                } while (elemHashesForIters[index].empty());
            }
        }
    } else {
        do {
            index++;
            it = elemHashesForIters[index].begin();
        } while (elemHashesForIters[index].empty());
    }
    return *this;
}

template<typename T>
inline unordered_set<T>::const_iterator::operator const T *() const {
    return it;
}

template<typename T>
inline unordered_set<T>::iterator::iterator() {}

template<typename T>
inline unordered_set<T>::iterator::iterator(vector<vector<T>> &hashes, const vec_it &_it, const size_t _index,
                                            const size_t last_index)
        : elemHashesForIters(hashes), it(_it), index(_index), lastIndex(last_index) {}

template<typename T>
inline typename unordered_set<T>::iterator &unordered_set<T>::iterator::operator++() {
    if (it != elemHashesForIters[index].end()) {
        ++it;
        // skip end()s of buckets except of last one;
        if (it == elemHashesForIters[index].end()) {
            if (it != elemHashesForIters[lastIndex].end()) {
                do {
                    index++;
                    it = elemHashesForIters[index].begin();
                } while (elemHashesForIters[index].empty());
            }
        }
    } else {
        do {
            index++;
            it = elemHashesForIters[index].begin();
        } while (elemHashesForIters[index].empty());
    }
    return *this;
}

template<typename T>
inline unordered_set<T>::iterator::operator typename unordered_set<T>::const_iterator() {
    return const_iterator(elemHashesForIters, it, index, lastIndex);
}

template<typename T>
inline unordered_set<T>::iterator::operator T *() {
    return it;
}
