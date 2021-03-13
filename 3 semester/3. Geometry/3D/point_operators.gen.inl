// =================================== + =======================================
    Point3D& operator+=(const Point3D& other) {
      x_ += other.x_;
      y_ += other.y_;
      z_ += other.z_;
      return *this;
    }

    Point3D operator+(const Point3D& other) const {
      Point3D tmp(*this);
      return tmp += other;
    }

// ================================= + NUM =====================================
    Point3D& operator+=(Scale_t num) {
      x_ += num;
      y_ += num;
      z_ += num;
      return *this;
    }

    Point3D operator+(Scale_t num) {
      Point3D tmp(*this);
      return tmp += num;
    }
//------------------------------------------------------------------------------

// =================================== - =======================================
    Point3D& operator-=(const Point3D& other) {
      x_ -= other.x_;
      y_ -= other.y_;
      z_ -= other.z_;
      return *this;
    }

    Point3D operator-(const Point3D& other) const {
      Point3D tmp(*this);
      return tmp -= other;
    }

// ================================= - NUM =====================================
    Point3D& operator-=(Scale_t num) {
      x_ -= num;
      y_ -= num;
      z_ -= num;
      return *this;
    }

    Point3D operator-(Scale_t num) {
      Point3D tmp(*this);
      return tmp -= num;
    }
//------------------------------------------------------------------------------

// =================================== * =======================================
    Point3D& operator*=(const Point3D& other) {
      x_ *= other.x_;
      y_ *= other.y_;
      z_ *= other.z_;
      return *this;
    }

    Point3D operator*(const Point3D& other) const {
      Point3D tmp(*this);
      return tmp *= other;
    }

// ================================= * NUM =====================================
    Point3D& operator*=(Scale_t num) {
      x_ *= num;
      y_ *= num;
      z_ *= num;
      return *this;
    }

    Point3D operator*(Scale_t num) {
      Point3D tmp(*this);
      return tmp *= num;
    }
//------------------------------------------------------------------------------

// =================================== / =======================================
    Point3D& operator/=(const Point3D& other) {
      x_ /= other.x_;
      y_ /= other.y_;
      z_ /= other.z_;
      return *this;
    }

    Point3D operator/(const Point3D& other) const {
      Point3D tmp(*this);
      return tmp /= other;
    }

// ================================= / NUM =====================================
    Point3D& operator/=(Scale_t num) {
      x_ /= num;
      y_ /= num;
      z_ /= num;
      return *this;
    }

    Point3D operator/(Scale_t num) {
      Point3D tmp(*this);
      return tmp /= num;
    }
//------------------------------------------------------------------------------

