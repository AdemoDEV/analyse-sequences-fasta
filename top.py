
class Top():     
    def __init__(self):
        self.top_regions = [None,None,None]
        
    def update_top(self, Region):
        if self.top_regions[0] == None and self.top_regions[1] == None and self.top_regions[2] == None:
            self.top_regions[0] = Region
        elif self.top_regions[1] == None:
                    first = self.top_regions[0]
                    if Region.len_r > first.len_r:
                        self.top_regions[1], self.top_regions[0] = self.top_regions[0], Region
                    elif Region.len_r == first.len_r:
                        if Region.start < first.start:
                            self.top_regions[1], self.top_regions[0] = self.top_regions[0], Region
                        else:
                            self.top_regions[1] = Region
                    else:
                        self.top_regions[1] = Region
                    return
        elif self.top_regions[2] == None:
            self.top_regions[2] = Region
            first = self.top_regions[0]
            second = self.top_regions[1]
            if Region.len_r > first.len_r:
                self.top_regions[0], self.top_regions[1], self.top_regions[2] = Region, first, second
            elif Region.len_r == first.len_r:
                if Region.start < first.start:
                    self.top_regions[0], self.top_regions[1], self.top_regions[2] = Region, first, second
                elif first.len_r == second.len_r:
                    if Region.start < second.start:
                        self.top_regions[1], self.top_regions[2] = Region, second 
                else:
                    self.top_regions[1], self.top_regions[2] = Region, second
            elif Region.len_r == second.len_r:
                if Region.start < second.start:
                    self.top_regions[1], self.top_regions[2] = Region, second
            elif Region.len_r > second.len_r:
                self.top_regions[1], self.top_regions[2] = Region, second
            return
        if self.top_regions[0] != None and self.top_regions[1] != None and self.top_regions[2] != None:
            first = self.top_regions[0]
            second = self.top_regions[1]
            third = self.top_regions[2]
            if Region.len_r > first.len_r:
                self.top_regions[0], self.top_regions[1], self.top_regions[2] = Region, first, second
            elif Region.len_r == first.len_r:
                if Region.start < first.start:
                    self.top_regions[0], self.top_regions[1], self.top_regions[2] = Region, first, second
                else:
                    if Region.len_r > second.len_r:
                        self.top_regions[0], self.top_regions[1], self.top_regions[2] = first, Region, second
                    elif Region.len_r == second.len_r:
                        if Region.start < second.start:
                            self.top_regions[1], self.top_regions[2] = Region, second
                        elif Region.len_r > third.len_r:
                            self.top_regions[2] = Region
                        elif Region.len_r == third.len_r:
                            if Region.start < third.start:
                                self.top_regions[2] = Region
            elif Region.len_r > second.len_r: 
                self.top_regions[0], self.top_regions[1], self.top_regions[2] = first, Region, second 
            elif Region.len_r == second.len_r:
                if Region.start < second.start:
                    self.top_regions[1], self.top_regions[2] = Region, second 
                elif Region.len_r > third.len_r:
                    self.top_regions[2] = Region
                elif Region.len_r == third.len_r:
                    if Region.start < third.start:
                        self.top_regions[2] = Region
            elif Region.len_r > third.len_r:
                self.top_regions[0], self.top_regions[1], self.top_regions[2] = first, second, Region  
            elif Region.len_r == third.len_r:
                if Region.start < third.start:
                    self.top_regions[2] = Region 
                    
    def get_regions(self):
        regions = []
        for v in self.top_regions:
            if v != None:
                regions.append(v)
        return regions