#pragma once

#include <iostream>

#include "soft_param.h"

namespace print {

void Print(std::ostream &os, const sp::BitSoftParameter &lhs);
void Print(std::ostream &os, const sp::ByteSoftParameter &lhs);
void Print(std::ostream &os, const sp::DwordSoftParameter &lhs);
void Print(std::ostream &os, const sp::StringSoftParameter &lhs);
void Print(std::ostream &os, const sp::BitExSoftParameter &lhs);
void Print(std::ostream &os, const sp::ByteExSoftParameter &lhs);
void Print(std::ostream &os, const sp::DwordExSoftParameter &lhs);
void Print(std::ostream &os, const sp::StringExSoftParameter &lhs);
void Print(std::ostream &os, const sp::BitExBSoftParameter &lhs);
void Print(std::ostream &os, const sp::ByteExBSoftParameter &lhs);
void Print(std::ostream &os, const sp::DwordExBSoftParameter &lhs);
void Print(std::ostream &os, const sp::StringExBSoftParameter &lhs);

void Print(std::ostream &os, const sp::DifferenceInfo &list);
std::ostream &operator<<(std::ostream &os, const sp::DifferenceInfo &list);

} // namespace print
