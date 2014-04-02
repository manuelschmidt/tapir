#ifndef GRIDPOSITION_HPP_
#define GRIDPOSITION_HPP_

#include <cmath>                        // for abs, pow, sqrt

#include <iostream>
#include <sstream>

struct GridPosition {
    long i;
    long j;
    GridPosition() : i(0), j(0) {
    }
    GridPosition(long _i, long _j) : i(_i), j(_j) {
    }

    double euclideanDistanceTo(GridPosition const &other) const {
        return std::sqrt(std::pow(i - other.i, 2) + std::pow(j - other.j, 2));
    }

    long manhattanDistanceTo(GridPosition const &other) const {
        return std::abs(i - other.i) + std::abs(j - other.j);
    }
};

inline std::ostream &operator<<(std::ostream &os, GridPosition const &obj) {
    os << "(" << obj.i << ", " << obj.j << ")";
    return os;
}

inline std::istream &operator>>(std::istream &is, GridPosition &obj) {
    std::string tmpStr;
    std::getline(is, tmpStr, '(');
    std::getline(is, tmpStr, ',');
    std::istringstream(tmpStr) >> obj.i;
    std::getline(is, tmpStr, ')');
    std::istringstream(tmpStr) >> obj.j;
    return is;
}

inline bool operator==(GridPosition const &lhs, GridPosition const &rhs) {
    return lhs.i == rhs.i && lhs.j == rhs.j;
}

inline bool operator!=(GridPosition const &lhs, GridPosition const &rhs) {
    return lhs.i != rhs.i || lhs.j != rhs.j;
}

#endif /* GRIDPOSITION_HPP_ */
