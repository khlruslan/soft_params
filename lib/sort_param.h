#pragma once

#include "soft_param.h"
#include <iostream>
#include <optional>
#include <vector>

namespace sp {
struct KeyToIndex {
  sp::Key key_;
  size_t index_;
  auto operator<=>(const KeyToIndex &rhs) const { return key_ <=> rhs.key_; }
  auto operator==(const KeyToIndex &rhs) const { return key_ == rhs.key_; }
};

using IndexByKey = std::vector<sp::KeyToIndex>;

IndexByKey GetIndexByKey(const sp::Parameters &v);
IndexByKey GetCommonIndexByKey(const sp::IndexByKey &v1,
                               const sp::IndexByKey &v2);
void Print(std::ostream &os, const sp::IndexByKey &index);

using SearchResult = std::optional<size_t>;
using SearchResultList = std::vector<SearchResult>;
using FoundParameter = std::optional<Parameter>;

using FoundParameterList = std::vector<FoundParameter>;

SearchResult FindByIndex(const sp::IndexByKey &index, const sp::Key &key);
FoundParameter GetFoundParameter(const sp::Parameters &parameters,
                                 const SearchResult &item);
void FillEmptyParameter(FoundParameterList &l);
} // namespace sp
