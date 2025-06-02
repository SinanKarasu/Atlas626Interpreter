#pragma once

#include <iterator>

template<typename Container>
class ResettableIterator {
public:
    using Iterator = typename Container::iterator;
    using T = typename Container::value_type;

    ResettableIterator(Container& container)
        : container_(container), current_(container.begin()) {}
        
    ResettableIterator(Container& container, typename Container::iterator start)
    	: container_(container), current_(start) {}


    // Pre-increment
    ResettableIterator& operator++() {
        ++current_;
        return *this;
    }

    // Post-increment
    ResettableIterator operator++(int) {
        ResettableIterator tmp = *this;
        ++current_;
        return tmp;
    }

    // Dereference
    T& operator*() {
        return *current_;
    }

    // Arrow
    T* operator->() {
        return &(*current_);
    }

	bool operator!=(const typename Container::iterator& other) const {
  		return current_ != other;
	}

    // Inequality
    bool operator!=(const ResettableIterator& other) const {
        return current_ != other.current_;
    }

    // Equality
    bool operator==(const ResettableIterator& other) const {
        return current_ == other.current_;
    }

    // Reset to beginning
    void reset() {
        current_ = container_.begin();
    }

    // End accessor
    Iterator end() const {
        return container_.end();
    }

private:
    Container& container_;
    Iterator current_;
};
