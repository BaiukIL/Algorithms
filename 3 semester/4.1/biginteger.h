#include <iostream>
#include <vector>
#include <string>

class BigInteger {

  private:

    bool signed_;
    std::vector<uint8_t> data_;
    static constexpr uint8_t MAX_VALUE_ = 99;

  public:
    // Constructors.
    // not explicit to allow constructions: BigInteger a = 5.
    BigInteger();
    BigInteger(int integer);
    BigInteger(const BigInteger& other);
    BigInteger(BigInteger&& other) noexcept;
    ~BigInteger() = default;
    // Assignment operators.
    BigInteger& operator=(const BigInteger& other) &;
    BigInteger& operator=(BigInteger&& other) & noexcept;
    // Unary operators.
    BigInteger  operator- () const;
    BigInteger& operator++()    &;
    BigInteger& operator--()    &;
    BigInteger  operator++(int) &;
    BigInteger  operator--(int) &;
    // Binary operators.
    BigInteger& operator+=(const BigInteger& other) &;
    BigInteger& operator-=(const BigInteger& other) &;
    BigInteger& operator*=(const BigInteger& other) &;
    BigInteger& operator/=(const BigInteger& other) &;
    BigInteger& operator%=(const BigInteger& other) &;
    // friend to allow constructions: 5 + BigInteger(10).
    friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);
    // Comparison operators.
    friend bool operator< (const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator> (const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
    // Stream operators.
    friend std::istream& operator>>(std::istream& stream, BigInteger& object);
    friend std::ostream& operator<<(std::ostream& stream,
                                    const BigInteger& object);
    // Cast operators.
    // explicit to avoid implicit converting BigInteger->bool->int.
    explicit operator bool() const;
    // String conversion.
    std::string toString() const;

  private:

    BigInteger(bool sign, const std::vector<uint8_t>& data);
    BigInteger& operator<<=(size_t offset) &;
    friend BigInteger GetMulPositive(std::vector<uint8_t> x,
                                     std::vector<uint8_t> y);
};

/// Compare two positive vectots and return -1 if lhs < rhs, 1 if lhs > rhs
/// and 0 otherwise.
int CmpData(const std::vector<uint8_t>& lhs,
                const std::vector<uint8_t>& rhs) {
  size_t lsize = lhs.size(), rsize = rhs.size();
  if (lsize != rsize) { return lsize < rsize ? -1 : 1; }
  for (int i = lsize - 1; i >= 0; --i) {
    if (lhs[i] != rhs[i]) {
      return lhs[i] < rhs[i] ? -1 : 1;
    }
  }
  return 0;
}

void RemoveZeros(std::vector<uint8_t>& data) {
  while (data.size() > 1 && data.back() == 0) {
    data.pop_back();
  }
}

// Karatsuba algorithm.
// Arguments are not references!
BigInteger GetMulPositive(std::vector<uint8_t> x, std::vector<uint8_t> y) {
  size_t size = x.size() < y.size() ? y.size() : x.size();
  // Recursion tail.
  if (size == 1) {
    uint16_t mul = (uint16_t)x[0] * (uint16_t)y[0];
    std::vector<uint8_t> mul_vec(2);
    mul_vec[0] = mul % (BigInteger::MAX_VALUE_ + 1);
    mul_vec[1] = mul / (BigInteger::MAX_VALUE_ + 1);
    return BigInteger(false, mul_vec);
  }
  size += size % 2; // Fix size.
  if (x.size() < size) {
    x.resize(size);
    for (size_t i = x.size(); i < size; ++i) { x[i] = 0; }
  }
  if (y.size() < size) {
    y.resize(size);
    for (size_t i = y.size(); i < size; ++i) { y[i] = 0; }
  }
  // x = x1*Base^(n/2) + x2, y = y1*Base^(n/2) + y2.
  // x*y = x1*y1*Base^n + (x1*y2 + x2*y1)*Base^(n/2) + x2*y2.
  // x*y = m1*Base^n + m2*Base^(n/2) + m3.
  // m2 = x_sum*y_sum - m1 - m3.
  std::vector<uint8_t> x1_vec(size/2);
  x1_vec.assign(x.begin() + size/2, x.end());
  std::vector<uint8_t> x2_vec(size/2);
  x2_vec.assign(x.begin(), x.begin() + size/2);
  std::vector<uint8_t> y1_vec(size/2);
  y1_vec.assign(y.begin() + size/2, y.end());
  std::vector<uint8_t> y2_vec(size/2);
  y2_vec.assign(y.begin(), y.begin() + size/2);
  BigInteger x1 = BigInteger(false, x1_vec);
  BigInteger x2 = BigInteger(false, x2_vec);
  BigInteger y1 = BigInteger(false, y1_vec);
  BigInteger y2 = BigInteger(false, y2_vec);
  BigInteger x_sum = x1 + x2;
  BigInteger y_sum = y1 + y2;
  auto m1 = GetMulPositive(x1_vec, y1_vec);
  auto m3 = GetMulPositive(x2_vec, y2_vec);
  auto m2 = GetMulPositive(x_sum.data_, y_sum.data_) - m1 - m3;
  m1 <<= size;   // * Base^n.
  m2 <<= size/2; // * Base^(n/2).
  RemoveZeros(m1.data_);
  RemoveZeros(m2.data_);
  RemoveZeros(m3.data_);
  return m1 + m2 + m3;
}


BigInteger& BigInteger::operator<<=(size_t offset) & {
  if (!*this) { return *this; }
  size_t prev_size = data_.size();
  data_.resize(prev_size + offset);
  for (size_t i = 0; i < prev_size; ++i) {
    data_[prev_size + offset - 1 - i] = data_[prev_size - 1 - i];
  }
  for (size_t i = 0; i < offset; ++i) { data_[i] = 0; }
  return *this;
}

BigInteger::BigInteger()
    : signed_(false),
      data_(1, 0) {}

BigInteger::BigInteger(int integer)
    : signed_(false),
      data_() {
  if (integer < 0) {
    signed_ = true;
    integer *= -1;
  }
  do {
    data_.push_back(integer % (MAX_VALUE_ + 1));
    integer /= MAX_VALUE_ + 1;
  } while (integer);
}

/*
BigInteger::BigInteger(std::string input)
    : signed_(false),
      data_() {
  size_t input_size = input.size();
  if (input_size == 0) {
    data_.push_back(0);
  } else {
    data_.reserve(input_size/2 + 1);

    int sign = 0;
    if (input[0] == '-') {
      input.erase(input.begin());
      --input_size;
      sign = 1;
    }
    int double_rank = input_size / 2;
    int i = 1;
    for (; i <= double_rank &&
         ('0' <= input[i] && input[i] <= '9'); ++i) {
      data_.push_back((input[input_size - i*2] - '0') * 10 +
                      (input[input_size - i*2 + 1] - '0'));

    }
    if (double_rank*2 != (int)input_size) { // Single symbol.
      data_.push_back(input[0] - '0');
    }
    if (sign) {
      signed_ = true;
    }
    RemoveZeros(data_);
  }
}
*/

BigInteger::BigInteger(bool sign, const std::vector<uint8_t>& data)
    : signed_(sign),
      data_(data) {
  RemoveZeros(data_);
}

BigInteger::BigInteger(const BigInteger& other)
    : signed_(other.signed_),
      data_(other.data_) {}

BigInteger::BigInteger(BigInteger&& other) noexcept
    : signed_(other.signed_),
      data_(std::move(other.data_)) {}

BigInteger& BigInteger::operator=(const BigInteger& other) & {
  if (&other == this) { return *this; }
  data_ = other.data_;
  signed_ = other.signed_;
  return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) & noexcept {
  if (&other == this) { return *this; }
  data_ = std::move(other.data_);
  signed_ = other.signed_;
  return *this;
}

size_t GetMax(size_t lhs, size_t rhs) {
  return lhs < rhs ? rhs : lhs;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) & {
  if (!other) { return *this; }
  if (!*this) { return *this = other; }
  if (*this < 0 && other > 0) {
    BigInteger tmp(other);
    tmp -= (-*this);
    return *this = tmp;
  }
  if (*this > 0 && other < 0) {
    return *this -= (-other);
  }
  // +1 for overflow bit.
  data_.reserve(GetMax(data_.size(), other.data_.size()) + 1);
  int tmp = 0;
  int carry = 0;
  for (size_t i = 0; i < GetMax(data_.size(), other.data_.size()) || carry;
       ++i) {
    if (i == data_.size()) {
      data_.push_back(0);
    }
    tmp = carry + data_[i] + (i < other.data_.size() ? other.data_[i] : 0);
    carry = tmp > BigInteger::MAX_VALUE_;
    if (carry) { // Overflow.
      tmp -= BigInteger::MAX_VALUE_ + 1;
    }
    data_[i] = tmp;
  }
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) & {
  // If different signes.
  if (!other) { return *this; }
  if (!*this) { return *this = -other; }
  if (*this < 0 && other > 0) {
    BigInteger tmp(-*this);
    tmp += other;
    return *this = -tmp;
  }
  if (*this > 0 && other < 0) {
    return *this += (-other);
  }
  if (*this < 0 && other < 0) {
    BigInteger tmp(-other);
    tmp -= (-*this);
    return *this = tmp;
  }
  if (*this < other) {
    BigInteger tmp(other);
    tmp -= *this;
    return *this = -tmp;
  }
  size_t min_size = other.data_.size();
  int tmp = 0;
  int carry = 0;
  for (size_t i = 0; i < min_size || carry; ++i) {
    tmp = data_[i] - (carry + (i < min_size ? other.data_[i] : 0));
    carry = tmp < 0;
    if (carry) {
      tmp += BigInteger::MAX_VALUE_ + 1;
    }
    data_[i] = tmp;
  }
  RemoveZeros(data_);
  return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) & {
  if (!*this) { return *this; }
  if (other.signed_) { // If other < 0 change sign.
    signed_ = !signed_;
  }
  if (!other) { // If 0.
    data_.assign(1, 0);
    signed_ = false;
    return *this;
  }
  if (other.data_[0] == 1 && other.data_.size() == 1) { // If |other| = 1.
    return *this;
  }
  data_ = GetMulPositive(data_, other.data_).data_;
  return *this;
}

// Column division.
BigInteger& BigInteger::operator/=(const BigInteger& divisor) & {
  if (!*this) { return *this; } // If *this == 0.
  if (divisor.signed_) { // If other < 0 change sign.
    signed_ = !signed_;
  }
  if (divisor.data_[0] == 1 && divisor.data_.size() == 1) { // If |other| = 1.
    return *this;
  }
  if (CmpData(data_, divisor.data_) == -1) {
    return *this = 0;
  }
  int demol = data_.size() - 1; // Position of the next demolition.
  BigInteger rest = 0, div = divisor;
  if (divisor < 0) {
    div *= -1;
  }
  std::vector<uint8_t> result;
  do {
    int counter = 0;
    while (rest >= div) {
      rest -= div;
      ++counter;
    }
    result.push_back(counter);
    rest <<= 1;
    rest += data_[demol--];
  } while (demol >= -1);
  // Reverse result vector.
  size_t res_size = result.size();
  std::vector<uint8_t> rev_result(res_size);
  for (int i = res_size - 1; i >= 0; --i) {
    rev_result[res_size - 1 - i] = result[i];
  }
  RemoveZeros(rev_result);
  data_ = rev_result;
  return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) & {
  if (!*this) { return *this; }
  BigInteger divided(*this);
  divided /= other;
  return *this -= divided * other;
}

BigInteger BigInteger::operator-() const {
  BigInteger copy(*this);
  if (copy) { copy.signed_ = !copy.signed_; }
  return copy;
}

BigInteger& BigInteger::operator++() & {
  return *this += 1;
}

BigInteger& BigInteger::operator--() & {
  return *this -= 1;
}

BigInteger BigInteger::operator++(int) & {
  BigInteger old(*this);
  *this += 1;
  return old;
}

BigInteger BigInteger::operator--(int) & {
  BigInteger old(*this);
  *this -= 1;
  return old;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger sum(lhs);
  sum += rhs;
  return sum;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger sub(lhs);
  sub -= rhs;
  return sub;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger mul(lhs);
  mul *= rhs;
  return mul;
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger div(lhs);
  div /= rhs;
  return div;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
  BigInteger mod(lhs);
  mod %= rhs;
  return mod;
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
  if (!lhs.signed_ && !rhs.signed_) { // Both positive.
    return CmpData(lhs.data_, rhs.data_) == -1;
  }
  if (lhs.signed_ && rhs.signed_) { // Both negative.
    return CmpData(lhs.data_, rhs.data_) == 1;
  }
  return lhs.signed_ > rhs.signed_;
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
  return rhs < lhs;
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
  return !(rhs < lhs);
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
  return !(lhs < rhs);
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
  return !(rhs < lhs || lhs < rhs);
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
  return rhs < lhs || lhs < rhs;
}

BigInteger::operator bool() const {
  return !(data_[0] == 0 && data_.size() == 1);
}

std::istream& operator>>(std::istream& stream, BigInteger& object) {
  object = 0;
  std::string input;
  stream >> input;
  size_t input_size = input.size();
  if (input_size == 0) {
    return stream;
  }
  object.data_.clear();
  object.data_.reserve(input_size/2 + 1);

  int sign = 0;
  if (input[0] == '-') {
    input.erase(input.begin());
    --input_size;
    sign = 1;
  }
  int double_rank = input_size / 2;
  int i = 1;
  for (; i <= double_rank; ++i) {
    object.data_.push_back((input[input_size - i*2] - '0') * 10 +
                           (input[input_size - i*2 + 1] - '0'));

  }
  if (double_rank*2 != (int)input_size) { // Single symbol.
    object.data_.push_back(input[0] - '0');
  }
  if (sign) {
    object *= -1;
  }
  RemoveZeros(object.data_);
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const BigInteger& object) {
  return stream << object.toString();
}

std::string BigInteger::toString() const {
  if (!*this) { return std::string("0"); }

  size_t data_size = data_.size();
  std::string decimal;
  decimal.reserve(data_size*2);
  if (*this < 0) {
    decimal.push_back('-');
  }
  for (int i = data_size - 1; i >= 0; --i) {
    // If one-digit non-lead bit, insert 0.
    if (data_[i] < 10 && i != (int)data_size - 1) {
      decimal.push_back('0');
    }
    decimal.append(std::to_string(data_[i]));
  }
  return decimal;
}

