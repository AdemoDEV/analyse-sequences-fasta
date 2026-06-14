#ifndef TOP_HPP
#define TOP_HPP

#include "region.hpp"

class Top { 
    Region* top_regions[3];
public: 
    Top();
    void update_top(Region& other);
    Region* get_first() const;
    Region* get_second() const;
    Region* get_third() const;
    ~Top();
};

#endif