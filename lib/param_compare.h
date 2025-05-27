#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "params.h"

namespace sft {

struct KeyTypeId {
  std::string type_;
  uint32_t id_ = 0u;
  auto operator<=>(const KeyTypeId &) const = default;
};

std::ostream &operator<<(std::ostream &, const KeyTypeId &item);

using KeysVector = std::vector<KeyTypeId>;

KeysVector CreateCommonIndex(const sft::VectorParameterInfo &v1,
                             const sft::VectorParameterInfo &v2);

struct DifferenceInfo {
  std::string type_;
  uint32_t id_ = 0;
  std::string value1_;
  std::string value2_;
};

using DifferenceDetails = std::vector<std::string>;

class IDifference {
public:
  virtual DifferenceDetails GetDetails() const = 0;
  virtual bool IsSignificant() const = 0;

  virtual ~IDifference() = default;

private:
};

std::vector<int> GetBitsNumbers(uint32_t n, int bits = 8);

class BitDifference : public IDifference {
public:
  explicit BitDifference(const DifferenceInfo &data) { Init(data); }
  DifferenceDetails GetDetails() const override { return details_; };
  bool IsSignificant() const override { return !details_.empty(); };

  void Init(const DifferenceInfo &info);

private:
  DifferenceDetails details_;
};

class ByteDifference : public IDifference {
public:
  explicit ByteDifference(const DifferenceInfo &data) { Init(data); }
  DifferenceDetails GetDetails() const override { return details_; };
  bool IsSignificant() const override { return !details_.empty(); };

  void Init(const DifferenceInfo &info);

private:
  DifferenceDetails details_;
};

class DwordDifference : public IDifference {
public:
  explicit DwordDifference(const DifferenceInfo &data) { Init(data); }
  DifferenceDetails GetDetails() const override { return details_; };
  bool IsSignificant() const override { return !details_.empty(); };

  void Init(const DifferenceInfo &info);

private:
  DifferenceDetails details_;
};

class StringDifference : public IDifference {
public:
  explicit StringDifference(const DifferenceInfo &data) { Init(data); }
  DifferenceDetails GetDetails() const override { return details_; };
  bool IsSignificant() const override { return !details_.empty(); };

  void Init(const DifferenceInfo &info);

private:
  DifferenceDetails details_;
};

} // namespace sft
