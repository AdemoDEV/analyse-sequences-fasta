
class Region(): 
    def __init__(self, start, caractere):
        self.start = start
        self.compteur = {
            "A" : 0,
            "C" : 0,
            "G" : 0,
            "T" : 0,
            "N" : 0
        }
        self.len_r = 0
        self.update_compteur(caractere)
        
    def len_regions(self):
        len_ = 0
        for k,v in self.compteur.items():
            len_ += v
        return len_
        

    #self.len_r += 1 -> Maniére plus optimisé pour la taille 
    #que utiliser la methode len_regions et qui parcours tout a chaque séquence
    def update_compteur(self, caractere):
        self.compteur[caractere] += 1
        self.len_r += 1 
        # self.len_r = self.len_regions()
    
    def end(self):
        return (self.start + self.len_r) - 1
    
    def statistique(self):
        stat = []
        for k,v in self.compteur.items():
            stat.append(v / self.len_r)
        return stat
    
    def fraction_total(self, len_sequence):
        return self.len_r / len_sequence
            