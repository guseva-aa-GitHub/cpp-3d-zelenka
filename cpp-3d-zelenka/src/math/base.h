#pragma once

namespace ag {
namespace math {

template <typename Treal>
inline Treal pi() {
    return Treal(3.14159265358979323846264338327950288);
}

template <typename Treal>
inline Treal half_pi() {
    return Treal(1.57079632679489661923132169163975144);
}

template <typename Treal>
inline Treal radians(Treal const& degrees) {
    const Treal pi = Treal(3.1415926535897932384626433832795);
    return degrees * (pi / Treal(180.0));
}

template <typename Treal>
inline Treal degrees(Treal const& radians) {
    const Treal pi = Treal(3.1415926535897932384626433832795);
    return radians * (Treal(180.0) / pi);
}

template<typename Treal>
inline Treal minimum(const Treal& a, const Treal& b) {
    return b < a ? b : a;
}

template<typename Treal>
inline Treal maximum(const Treal& a, const Treal& b) {
    return a < b ? b : a;
}

}
}

