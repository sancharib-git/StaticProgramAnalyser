//
// Created by Yen Peng Yip on 5/9/21.
//

#ifndef INC_21S1_CP_SPA_TEAM_11_HASHER_H
#define INC_21S1_CP_SPA_TEAM_11_HASHER_H

#pragma once

/*
 * Specialized hash function to store pairs in unordered_set
 * Using Szudzik's elegant pairing function: http://szudzik.com/ElegantPairing.pdf
 */
struct intIntHash {
public:
    template <typename T, typename U>
    size_t operator()(std::pair<T, U> const& a) const {
        T x = a.first;
        U y = a.second;
        return x >= y ? (x * x + x + y) : (y * y + x);
    }
};

struct intStrHash {
public:
    template <typename T, typename U>
    size_t operator()(std::pair<T, U> const& a) const {
        std::hash<std::string> hash_fn;
        T x = a.first;
        size_t y = hash_fn(a.second);
        return x >= y ? (x * x + x + y) : (y * y + x);
    }
};

#endif //INC_21S1_CP_SPA_TEAM_11_HASHER_H
