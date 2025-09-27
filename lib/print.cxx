#include <algorithm>
#include <iostream>

#include "print.h"
#include "soft_param.h"

namespace print {
void Print(std::ostream &os, const sp::BitSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " " << lhs.value_;
}

void Print(std::ostream &os, const sp::ByteSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " "
     << static_cast<uint32_t>(lhs.value_);
}

void Print(std::ostream &os, const sp::DwordSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " " << lhs.value_;
}

void Print(std::ostream &os, const sp::StringSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " '" << lhs.value_ << '\'';
}

void Print(std::ostream &os, const sp::BitExSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " " << lhs.value_;
}

void Print(std::ostream &os, const sp::ByteExSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " "
     << static_cast<uint32_t>(lhs.value_);
}

void Print(std::ostream &os, const sp::DwordExSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " " << lhs.value_;
}

void Print(std::ostream &os, const sp::StringExSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " '" << lhs.value_ << '\'';
}

void Print(std::ostream &os, const sp::BitExBSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " " << lhs.value_;
}

void Print(std::ostream &os, const sp::ByteExBSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " "
     << static_cast<uint32_t>(lhs.value_);
}

void Print(std::ostream &os, const sp::DwordExBSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " " << lhs.value_;
}

void Print(std::ostream &os, const sp::StringExBSoftParameter &lhs) {
  os << sp::GetName(lhs) << lhs.number_ << " '" << lhs.value_ << '\'';
}

std::ostream &operator<<(std::ostream &os, const sp::DifferenceInfo &list) {
  std::for_each(std::begin(list), std::end(list),
                [&os](const auto &item) { os << item << '\n'; });
  return os;
}

void Print(std::ostream &os, const sp::DifferenceInfo &list) { os << list; }

} // namespace print
