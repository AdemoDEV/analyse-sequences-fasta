#include <iostream>
#include "region.hpp"
#include <cstddef>


Region::Region(int start, char caractere) : _start(start), _caractere(caractere), _len_r(0) {
    update_compteur(caractere);
}

void Region::update_compteur(char caractere) {
    switch (caractere) {
    case 'A':
        _compteur.A++;
        break;
    case 'C':
        _compteur.C++;
        break;
    case 'G':
        _compteur.G++;
        break;
    case 'T':
        _compteur.T++;
        break;
    case 'N':
        _compteur.N++;
        break;
    default:
        break;
    }
    _len_r++;
}

int Region::get_start() const {
    return _start;
}

int Region::get_len_() const {
    return _len_r;
}

int Region::end() const {
    return (_start + _len_r) - 1;
}

void Region::statistique(double stat[5]) const {
    stat[0] = (double) _compteur.A / _len_r;
    stat[1] = (double) _compteur.C / _len_r;
    stat[2] = (double) _compteur.G / _len_r;
    stat[3] = (double) _compteur.T / _len_r;
    stat[4] = (double) _compteur.N / _len_r;
}

double Region::fraction_total(size_t len_sequence) const {
    return (double) _len_r / len_sequence;
}