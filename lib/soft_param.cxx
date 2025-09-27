#include <algorithm>
#include <bitset>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "bit_utils.h"
#include "format_utils.h"
#include "mml_utils.h"
#include "soft_param.h"

namespace sp {
std::ostream &operator<<(std::ostream &os, const DifferenceInfo &list) {
  for (auto line : list) {
    os << line << '\n';
  }
  return os;
}

ParameterInfo GetParameterInfo(const mml::MapStringString &description,
                               const mml::ConvertInfo &ci) {
  ParameterInfo param;

  param.type_ = mml::GetItemByKey(description, ci.type_key_);
  std::string value_name = mml::GetItemByKey(ci.type_to_value_, param.type_);
  std::string number_name = mml::GetItemByKey(ci.type_to_number_, param.type_);
  std::string number = mml::GetItemByKey(description, number_name);
  param.id_ = std::stoul(number);
  param.value_ = mml::GetItemByKey(description, value_name);

  return param;
}

ParameterInfoList Convert(const mml::VectorMapStringString &mml_dict,
                          const mml::ConvertInfo &ci) {
  ParameterInfoList pil;
  pil.reserve(mml_dict.data_.size());

  std::transform(mml_dict.data_.cbegin(), mml_dict.data_.cend(),
                 std::back_inserter(pil),
                 [&ci](const mml::MapStringString &item) {
                   return GetParameterInfo(item, ci);
                 });

  return pil;
}

ParameterInfoList Load(const std::string &file, const std::string &prefix,
                       const mml::ConvertInfo &ci) {

  mml::VectorMapStringString params = mml::Load(file, prefix);
  return Convert(params, ci);
}

// Create Epmty paramerer
Parameter GetEmptyParameter(const BitSoftParameter &a) {
  return BitSoftParameter(false, a.number_);
}

Parameter GetEmptyParameter(const ByteSoftParameter &a) {
  return ByteSoftParameter(0, a.number_);
}

Parameter GetEmptyParameter(const DwordSoftParameter &a) {
  return DwordSoftParameter(0, a.number_);
}

Parameter GetEmptyParameter(const StringSoftParameter &a) {
  return StringSoftParameter("", a.number_);
}

Parameter GetEmptyParameter(const BitExSoftParameter &a) {
  return BitExSoftParameter(false, a.number_);
}

Parameter GetEmptyParameter(const ByteExSoftParameter &a) {
  return ByteExSoftParameter(0, a.number_);
}

Parameter GetEmptyParameter(const DwordExSoftParameter &a) {
  return DwordExSoftParameter(0, a.number_);
}

Parameter GetEmptyParameter(const StringExSoftParameter &a) {
  return StringExSoftParameter("", a.number_);
}

Parameter GetEmptyParameter(const BitExBSoftParameter &a) {
  return BitExBSoftParameter(false, a.number_);
}

Parameter GetEmptyParameter(const ByteExBSoftParameter &a) {
  return ByteExBSoftParameter(0, a.number_);
}

Parameter GetEmptyParameter(const DwordExBSoftParameter &a) {
  return DwordExBSoftParameter(0, a.number_);
}

Parameter GetEmptyParameter(const StringExBSoftParameter &a) {
  return StringExBSoftParameter("", a.number_);
}

// Get Difference
DifferenceInfo GetDifference(const BitSoftParameter &lhs,
                             const BitSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  if (lhs.value_ != rhs.value_) {
    os << "BIT" << lhs.number_;
    result.emplace_back(os.str());
  }
  return result;
}

DifferenceInfo GetDifference(const BitExSoftParameter &lhs,
                             const BitExSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  if (lhs.value_ != rhs.value_) {
    os << "BIT_EX" << lhs.number_;
    result.emplace_back(os.str());
  }
  return result;
}

DifferenceInfo GetDifference(const BitExBSoftParameter &lhs,
                             const BitExBSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  if (lhs.value_ != rhs.value_) {
    os << "BIT_EX_B" << lhs.number_;
    result.emplace_back(os.str());
  }
  return result;
}

DifferenceInfo GetDifference(const ByteSoftParameter &lhs,
                             const ByteSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  std::string item;
  if (lhs.value_ != rhs.value_) {
    auto bits_list = util::GetDifferentBits(lhs.value_, rhs.value_);
    std::transform(
        std::begin(bits_list), std::end(bits_list), std::back_inserter(result),
        [&os, &lhs, &item](const auto &i) {
          // i + 1 - bit number starts from 1
          os << "BIT" << i + 1 << " of " << GetName(lhs) << lhs.number_;
          item = os.str();

          os.str("");
          os.clear();
          return item;
        });
  }
  return result;
}

DifferenceInfo GetDifference(const ByteExSoftParameter &lhs,
                             const ByteExSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  std::string item;
  if (lhs.value_ != rhs.value_) {
    auto bits_list = util::GetDifferentBits(lhs.value_, rhs.value_);
    std::transform(
        std::begin(bits_list), std::end(bits_list), std::back_inserter(result),
        [&os, &lhs, &item](const auto &i) {
          // i + 1 - bit number starts from 1
          os << "BIT" << i + 1 << " of " << GetName(lhs) << lhs.number_;
          item = os.str();

          os.str("");
          os.clear();
          return item;
        });
  }
  return result;
}

DifferenceInfo GetDifference(const ByteExBSoftParameter &lhs,
                             const ByteExBSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  std::string item;
  if (lhs.value_ != rhs.value_) {
    auto bits_list = util::GetDifferentBits(lhs.value_, rhs.value_);
    std::transform(
        std::begin(bits_list), std::end(bits_list), std::back_inserter(result),
        [&os, &lhs, &item](const auto &i) {
          // i + 1 - bit number starts from 1
          os << "BIT" << i + 1 << " of " << GetName(lhs) << lhs.number_;
          item = os.str();

          os.str("");
          os.clear();
          return item;
        });
  }
  return result;
}

DifferenceInfo GetDifference(const DwordSoftParameter &lhs,
                             const DwordSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  std::string item;
  if (lhs.value_ != rhs.value_) {
    auto bits_list = util::GetDifferentBits(lhs.value_, rhs.value_);
    std::transform(
        std::begin(bits_list), std::end(bits_list), std::back_inserter(result),
        [&os, &lhs, &item](const auto &i) {
          // i + 1 - bit number starts from 1
          os << "BIT" << i + 1 << " of " << GetName(lhs) << lhs.number_;
          item = os.str();

          os.str("");
          os.clear();
          return item;
        });
  }
  return result;
}

DifferenceInfo GetDifference(const DwordExSoftParameter &lhs,
                             const DwordExSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  std::string item;
  if (lhs.value_ != rhs.value_) {
    auto bits_list = util::GetDifferentBits(lhs.value_, rhs.value_);
    std::transform(
        std::begin(bits_list), std::end(bits_list), std::back_inserter(result),
        [&os, &lhs, &item](const auto &i) {
          // i + 1 - bit number starts from 1
          os << "BIT" << i + 1 << " of " << GetName(lhs) << lhs.number_;
          item = os.str();

          os.str("");
          os.clear();
          return item;
        });
  }
  return result;
}

DifferenceInfo GetDifference(const DwordExBSoftParameter &lhs,
                             const DwordExBSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  std::string item;
  if (lhs.value_ != rhs.value_) {
    auto bits_list = util::GetDifferentBits(lhs.value_, rhs.value_);
    std::transform(
        std::begin(bits_list), std::end(bits_list), std::back_inserter(result),
        [&os, &lhs, &item](const auto &i) {
          // i + 1 - bit number starts from 1
          os << "BIT" << i + 1 << " of " << GetName(lhs) << lhs.number_;
          item = os.str();

          os.str("");
          os.clear();
          return item;
        });
  }
  return result;
}

DifferenceInfo GetDifference(const StringSoftParameter &lhs,
                             const StringSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  if (lhs.value_ != rhs.value_) {
    os << "Strings are different";
    result.emplace_back(os.str());
  }
  return result;
}

DifferenceInfo GetDifference(const StringExSoftParameter &lhs,
                             const StringExSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  if (lhs.value_ != rhs.value_) {
    os << "Strings are different";
    result.emplace_back(os.str());
  }
  return result;
}

DifferenceInfo GetDifference(const StringExBSoftParameter &lhs,
                             const StringExBSoftParameter &rhs) {
  std::ostringstream os;
  DifferenceInfo result;
  if (lhs.value_ != rhs.value_) {
    os << "Strings are different";
    result.emplace_back(os.str());
  }
  return result;
}

std::string GetShortView(const BitSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const ByteSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const DwordSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const StringSoftParameter &p) { return p.value_; }

std::string GetShortView(const BitExSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const ByteExSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const DwordExSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const StringExSoftParameter &p) { return p.value_; }

std::string GetShortView(const BitExBSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const ByteExBSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const DwordExBSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetShortView(const StringExBSoftParameter &p) { return p.value_; }

std::string GetFullView(const BitSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetFullView(const ByteSoftParameter &p) {
  return fmt::insert_spaces(std::bitset<8>{p.value_}.to_string(), 4);
}
std::string GetFullView(const DwordSoftParameter &p) {
  return fmt::insert_spaces(std::bitset<32>{p.value_}.to_string(), 4);
}
std::string GetFullView(const StringSoftParameter &p) { return p.value_; }

std::string GetFullView(const BitExSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetFullView(const ByteExSoftParameter &p) {
  return fmt::insert_spaces(std::bitset<8>{p.value_}.to_string(), 4);
}
std::string GetFullView(const DwordExSoftParameter &p) {
  return fmt::insert_spaces(std::bitset<32>{p.value_}.to_string(), 4);
}
std::string GetFullView(const StringExSoftParameter &p) { return p.value_; }

std::string GetFullView(const BitExBSoftParameter &p) {
  return std::to_string(p.value_);
}
std::string GetFullView(const ByteExBSoftParameter &p) {
  return fmt::insert_spaces(std::bitset<8>{p.value_}.to_string(), 4);
}
std::string GetFullView(const DwordExBSoftParameter &p) {
  return fmt::insert_spaces(std::bitset<32>{p.value_}.to_string(), 4);
}
std::string GetFullView(const StringExBSoftParameter &p) { return p.value_; }

std::string GetName(const BitSoftParameter &) { return "BIT"; }
std::string GetName(const ByteSoftParameter &) { return "BYTE"; }
std::string GetName(const DwordSoftParameter &) { return "DWORD"; }
std::string GetName(const StringSoftParameter &) { return "STRING"; }
std::string GetName(const BitExSoftParameter &) { return "BIT_EX"; }
std::string GetName(const ByteExSoftParameter &) { return "BYTE_EX"; }
std::string GetName(const DwordExSoftParameter &) { return "DWORD_EX"; }
std::string GetName(const StringExSoftParameter &) { return "STRING_EX"; }
std::string GetName(const BitExBSoftParameter &) { return "BIT_EX_B"; }
std::string GetName(const ByteExBSoftParameter &) { return "BYTE_EX_B"; }
std::string GetName(const DwordExBSoftParameter &) { return "DWORD_EX_B"; }
std::string GetName(const StringExBSoftParameter &) { return "STRING_EX_B"; }

int GetOrder(const BitSoftParameter &) { return 100; }
int GetOrder(const BitExSoftParameter &) { return 101; }
int GetOrder(const BitExBSoftParameter &) { return 102; }
int GetOrder(const ByteSoftParameter &) { return 200; }
int GetOrder(const ByteExSoftParameter &) { return 201; }
int GetOrder(const ByteExBSoftParameter &) { return 202; }
int GetOrder(const DwordSoftParameter &) { return 300; }
int GetOrder(const DwordExSoftParameter &) { return 301; }
int GetOrder(const DwordExBSoftParameter &) { return 302; }
int GetOrder(const StringSoftParameter &) { return 400; }
int GetOrder(const StringExSoftParameter &) { return 401; }
int GetOrder(const StringExBSoftParameter &) { return 402; }
} // namespace sp
