// =================================== + =======================================
    Point2D& operator+=(const Point2D& other) {
      x_ += other.x_;
      y_ += other.y_;
      return *this;
    }

    Point2D operator+(const Point2D& other) const {
      Point2D tmp(*this);
      return tmp += other;
    }

// ================================= + NUM =====================================
    Point2D& operator+=(Scale_t num) {
      x_ += num;
      y_ += num;
      return *this;
    }

    Point2D operator+(Scale_t num) {
      Point2D tmp(*this);
      return tmp += num;
    }
//------------------------------------------------------------------------------

// =================================== - =======================================
    Point2D& operator-=(const Point2D& other) {
      x_ -= other.x_;
      y_ -= other.y_;
      return *this;
    }

    Point2D operator-(const Point2D& other) const {
      Point2D tmp(*this);
      return tmp -= other;
    }

// ================================= - NUM =====================================
    Point2D& operator-=(Scale_t num) {
      x_ -= num;
      y_ -= num;
      return *this;
    }

    Point2D operator-(Scale_t num) {
      Point2D tmp(*this);
      return tmp -= num;
    }
//------------------------------------------------------------------------------

// ================================= * NUM =====================================
    Point2D& operator*=(Scale_t num) {
      x_ *= num;
      y_ *= num;
      return *this;
    }

    Point2D operator*(Scale_t num) {
      Point2D tmp(*this);
      return tmp *= num;
    }
//------------------------------------------------------------------------------

// ================================= / NUM =====================================
    Point2D& operator/=(Scale_t num) {
      x_ /= num;
      y_ /= num;
      return *this;
    }

    Point2D operator/(Scale_t num) {
      Point2D tmp(*this);
      return tmp /= num;
    }
//------------------------------------------------------------------------------

