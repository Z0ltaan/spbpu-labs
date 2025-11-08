// // #include "buffer.hpp"
// #include <algorithm>
// #include "priority.hpp"
// #include "request.hpp"
//
// course::Buffer::Buffer(size_t size) : buffer_() {}
//
// const course::SimpleRequest &course::Buffer::operator[](size_t index) const noexcept { return buffer_[index]; }
//
// course::SimpleRequest &course::Buffer::operator[](size_t index) noexcept
// {
//   return const_cast< SimpleRequest & >(static_cast< const Buffer & >(*this)[index]);
// }
//
// const course::SimpleRequest &course::Buffer::at(size_t index) const { return buffer_.at(index); }
// course::SimpleRequest &course::Buffer::at(size_t index)
//
// {
//   return const_cast< SimpleRequest & >(static_cast< const Buffer & >(*this).at(index));
// }
//
// std::vector< course::SimpleRequest >::const_iterator course::Buffer::begin() const noexcept { return buffer_.begin();
// }
//
// std::vector< course::SimpleRequest >::const_iterator course::Buffer::end() const noexcept { return buffer_.end(); }
//
// std::vector< course::SimpleRequest >::iterator course::Buffer::begin() noexcept
// {
//   return std::vector< course::SimpleRequest >::iterator(this->begin());
// }
//
// std::vector< course::SimpleRequest >::iterator course::Buffer::end() noexcept
// {
//   return std::vector< course::SimpleRequest >::iterator(this->end());
// }
//
// size_t course::Buffer::size() const noexcept { return buffer_.size(); }
//
// bool isEmptySlot(const course::SimpleRequest &item) { return item.getData().first == EMPTY; }
//
// bool course::Buffer::isEmpty() const noexcept { return std::all_of(buffer_.begin(), buffer_.end(), isEmptySlot); }
//
// bool course::Buffer::isFull() const noexcept { return std::none_of(buffer_.begin(), buffer_.end(), isEmptySlot); }
