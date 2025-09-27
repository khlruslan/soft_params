
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <optional>
#include <variant>
#include <vector>

#include "soft_param.h"
#include "sort_param.h"

namespace sp {

IndexByKey GetIndexByKey(const sp::Parameters &v) {
  IndexByKey index;
  index.reserve(v.size());

  std::vector<size_t> numbers(v.size(), 0u);
  std::iota(numbers.begin(), numbers.end(), 0ull);

  std::transform(
      v.begin(), v.end(), numbers.begin(), std::back_inserter(index),
      [](const auto &item, const auto &n) {
        return sp::KeyToIndex{
            std::visit([](const auto &a) { return sp::GetKey(a); }, item), n};
      });

  std::sort(index.begin(), index.end());

  return index;
}

IndexByKey GetCommonIndexByKey(const sp::IndexByKey &v1,
                               const sp::IndexByKey &v2) {
  IndexByKey index;
  index.reserve(v1.size() + v2.size());
  std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(),
             std::back_inserter(index));
  index.erase(std::unique(index.begin(), index.end()), index.end());
  return index;
}

void Print(std::ostream &os, const sp::IndexByKey &index) {
  for (const auto &a : index) {
    os << a.key_.order_ << " " << static_cast<uint32_t>(a.key_.number_) << " "
       << a.index_ << '\n';
  }
}

SearchResult FindByIndex(const sp::IndexByKey &index, const sp::Key &key) {
  sp::KeyToIndex item{key, 0ull};
  auto search = std::lower_bound(index.begin(), index.end(), item);
  if (search != index.end() && search->key_ == key) {
    return search->index_;
  } else {
    return std::nullopt;
  }
}

FoundParameter GetFoundParameter(const sp::Parameters &parameters,
                                 const sp::SearchResult &item) {
  if (item) {
    return FoundParameter{parameters[*item]};
  }
  return FoundParameter{};
}

void FillEmptyParameter(FoundParameterList &l) {
  bool found = false;
  size_t empty;
  size_t full;
  auto empty_visitor = [](const auto &item) {
    return sp::GetEmptyParameter(item);
  };

  if (!l[0]) {
    empty = 0;
    full = 1;
    found = true;
  } else if (!l[1]) {
    empty = 1;
    full = 0;
    found = true;
  }
  if (found) {
    l[empty] = std::visit(empty_visitor, *l[full]);
  }
}

} // namespace sp
