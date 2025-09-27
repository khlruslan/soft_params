#pragma once

#include <iostream>
#include <memory>

#include "print.h"
#include "soft_param.h"

namespace model {
class model {
public:
  template <typename T>
  model(T t) : data_(std::make_unique<impl<T>>(std::move(t))) {}

  model(model &&m) noexcept = default;
  model &operator=(model &&m) noexcept = default;

  model(const model &m) : data_(m.data_->copy()) {}
  model &operator=(const model &m) {
    if (this != &m) {
      model temp(m);
      swap(temp);
    }
    return *this;
  }

  void Print(std::ostream &os) const { return data_->Print(os); }
  model GetEmptyParameter() const { return model(data_->GetEmptyParameter()); }
  sp::Parameter GetParameter() const { return data_->GetParameter(); }

private:
  void swap(model &m) {
    auto temp = std::move(data_);
    data_ = std::move(m.data_);
    m.data_ = std::move(temp);
  }

  struct concept_t {
    virtual ~concept_t() = default;
    virtual void Print(std::ostream &os) const = 0;
    virtual std::unique_ptr<concept_t> GetEmptyParameter() const = 0;
    virtual std::unique_ptr<concept_t> copy() const = 0;
    virtual sp::Parameter GetParameter() const = 0;
  };

  template <typename T> struct impl : concept_t {
    T data_;
    impl(T &&t) : data_(std::move(t)) {}

    void Print(std::ostream &os) const override {
      print::Print(os, data_);
      return;
    }

    std::unique_ptr<concept_t> GetEmptyParameter() const override {
      return std::make_unique<impl<T>>(sp::GetEmptyParameter(data_));
    }

    std::unique_ptr<concept_t> copy() const override {
      return std::make_unique<impl<T>>(*this);
    }

    sp::Parameter GetParameter() const override { return data_; }
  };

  std::unique_ptr<concept_t> data_;

private:
  model(std::unique_ptr<concept_t> &&d) : data_(std::move(d)) {}
}; // class model
} // namespace model
