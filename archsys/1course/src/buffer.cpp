#include "buffer.hpp"
#include <algorithm>
#include "priority.hpp"

course::Buffer::Buffer(size_t size) : buffer_() {}

void course::Buffer::put(const SimpleRequest &request) {}

course::SimpleRequest course::Buffer::pop() { return SimpleRequest(EMPTY, 0); }
// const course::SimpleRequest &get() {}

const course::SimpleRequest &course::Buffer::operator[](size_t index) const noexcept { return buffer_[index]; }

const course::SimpleRequest &course::Buffer::at(size_t index) const { return buffer_.at(index); }

size_t course::Buffer::size() const noexcept { return buffer_.size(); }

bool isEmptySlot(const course::SimpleRequest &item) { return item.getData().first == EMPTY; }

bool course::Buffer::isEmpty() const noexcept { return std::all_of(buffer_.begin(), buffer_.end(), isEmptySlot); }

bool course::Buffer::isFull() const noexcept { return std::none_of(buffer_.begin(), buffer_.end(), isEmptySlot); }
