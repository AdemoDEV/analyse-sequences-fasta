#include <iostream>
#include "top.hpp"

Top::Top() {
    top_regions[0] = nullptr;
    top_regions[1] = nullptr;
    top_regions[2] = nullptr;
}
void Top::update_top(Region& region) {
    if (top_regions[0] == nullptr and top_regions[1] == nullptr and top_regions[2] == nullptr) {
        top_regions[0] = new Region(region);
    } else if (top_regions[1] == nullptr) {
        if (region.get_len_() > top_regions[0]->get_len_()) {
            top_regions[1] = top_regions[0];
            top_regions[0] = new Region(region);
        } else if (region.get_len_() == top_regions[0]->get_len_()) {
            if (region.get_start() < top_regions[0]->get_start()) {
                top_regions[1] = top_regions[0];
                top_regions[0] = new Region(region);
            } else {
                top_regions[1] = new Region(region);
            }
        } else {
            top_regions[1] = new Region(region);
        }
    } else if (top_regions[2] == nullptr) {
        top_regions[2] = new Region(region);
        if (region.get_len_() > top_regions[0]->get_len_()) {
            Region * reg = top_regions[2];
            top_regions[2] = top_regions[1];
            top_regions[1] = top_regions[0];
            top_regions[0] = reg;
        } else if (region.get_len_() == top_regions[0]->get_len_()) {
            if (region.get_start() < top_regions[0]->get_start()) {
                Region * reg = top_regions[2];
                top_regions[2] = top_regions[1];
                top_regions[1] = top_regions[0];
                top_regions[0] = reg;
            } else if (top_regions[0]->get_len_() == top_regions[1]->get_len_()) {
                if (region.get_start() < top_regions[1]->get_start()) {
                    Region * reg = top_regions[2];
                    top_regions[2] = top_regions[1];
                    top_regions[1] = reg;
                }
            } else {
                Region * reg = top_regions[2];
                top_regions[2] = top_regions[1];
                top_regions[1] = reg;
            }
        } else if (region.get_len_() == top_regions[1]->get_len_()) {
            if (region.get_start() < top_regions[1]->get_start()) {
                Region * reg = top_regions[2];
                top_regions[2] = top_regions[1];
                top_regions[1] = reg;
            }
        } else if (region.get_len_() > top_regions[1]->get_len_()) {
            Region * reg = top_regions[2];
            top_regions[2] = top_regions[1];
            top_regions[1] = reg;
        }
    } else if (top_regions[0] != nullptr and top_regions[1] != nullptr and top_regions[2] != nullptr) {
        if (region.get_len_() > top_regions[0]->get_len_()) {
            delete top_regions[2];
            top_regions[2] = top_regions[1];
            top_regions[1] = top_regions[0];
            top_regions[0] = new Region(region);

        } else if (region.get_len_() == top_regions[0]->get_len_()) {
            if (region.get_start() < top_regions[0]->get_start()) {
                delete top_regions[2];
                top_regions[2] = top_regions[1];
                top_regions[1] = top_regions[0];
                top_regions[0] = new Region(region);
            } else {
                if (region.get_len_() > top_regions[1]->get_len_()) {
                    delete top_regions[2];
                    top_regions[2] = top_regions[1];
                    top_regions[1] = new Region(region);
                } else if (region.get_len_() == top_regions[1]->get_len_()) {
                    if (region.get_start() < top_regions[1]->get_start()) {
                        delete top_regions[2];
                        top_regions[2] = top_regions[1];
                        top_regions[1] = new Region(region);
                    } else if (region.get_len_() > top_regions[2]->get_len_()) {
                        delete top_regions[2];
                        top_regions[2] = new Region(region);
                    } else if (region.get_len_() == top_regions[2]->get_len_()) {
                        if (region.get_start() < top_regions[2]->get_start()) {
                            delete top_regions[2];
                            top_regions[2] = new Region(region);
                        } 
                    }
                }
            }
        } else if (region.get_len_() > top_regions[1]->get_len_()) {
            delete top_regions[2];
            top_regions[2] = top_regions[1];
            top_regions[1] = new Region(region);
        } else if (region.get_len_() == top_regions[1]->get_len_()) {
            if (region.get_start() < top_regions[1]->get_start()) {
                delete top_regions[2];
                top_regions[2] = top_regions[1];
                top_regions[1] = new Region(region);
            } else if (region.get_len_() > top_regions[2]->get_len_()) {
                delete top_regions[2];
                top_regions[2] = new Region(region);
            } else if (region.get_len_() == top_regions[2]->get_len_()) {
                if (region.get_start() < top_regions[2]->get_start()) {
                    delete top_regions[2];
                    top_regions[2] = new Region(region);
                }
            }
        } else if (region.get_len_() > top_regions[2]->get_len_()) {
            delete top_regions[2];
            top_regions[2] = new Region(region);
        } else if (region.get_len_() == top_regions[2]->get_len_()) {
            if (region.get_start() < top_regions[2]->get_start()) {
                delete top_regions[2];
                top_regions[2] = new Region(region);
            }
        }
    }
}



Region* Top::get_first() const {
    return top_regions[0];
}

Region* Top::get_second() const {
    return top_regions[1];
}

Region* Top::get_third() const {
    return top_regions[2];
}

Top::~Top() {
    delete top_regions[0];
    delete top_regions[1];
    delete top_regions[2];
}