#include "vect.h"
#include <cmath>

vect vect::operator-() const { return vect(-v[0], -v[1], -v[2]); };

vect vect::operator+=(const vect &vec) {
  v[0] += vec[0];
  v[1] += vec[1];
  v[2] += vec[2];
  return *this;
};

vect vect::operator*=(const vect &vec) {
  v[0] *= vec[0];
  v[1] *= vec[1];
  v[2] *= vec[2];
  return *this;
};

vect vect::operator-=(const vect &vec) {
  v[0] -= vec[0];
  v[1] -= vec[1];
  v[2] -= vec[2];
  return *this;
};

vect vect::operator/=(double s) {
  v[0] /= s;
  v[1] /= s;
  v[2] /= s;
  return *this;
};

double vect::lenSquared() const {
  return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
};

double vect::len() const { return std::sqrt(lenSquared()); };

bool vect::nearZero() const {
  double epsilon = 1e-8;
  return (abs(v[0]) < epsilon && abs(v[1]) < epsilon && abs(v[2]) < epsilon);
}

vect vect::random() {
  return vect(randomDouble(), randomDouble(), randomDouble());
}
vect vect::random(double lower, double upper) {
  return vect(randomDouble(lower, upper), randomDouble(lower, upper),
              randomDouble(lower, upper));
}
