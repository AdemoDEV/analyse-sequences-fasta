#ifndef REGION_HPP
#define REGION_HPP

#include <cstddef>

struct compteur {
    int A = 0;
    int C = 0;
    int G = 0;
    int T = 0;
    int N = 0;
};

class Region {
    int _start;
    char _caractere;
    int _len_r;
    compteur _compteur;

public:
    Region(int, char);
    void update_compteur(char caractere);
    int get_start() const;
    int get_len_() const;
    int end() const;
    void statistique(double stat[5]) const;
    double fraction_total(size_t len_sequence) const;
};

#endif