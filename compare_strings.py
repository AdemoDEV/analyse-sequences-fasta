from typing import Tuple, Optional, TextIO

from regions import Region
from top import Top

import sys
import argparse
import time

ALPHABET = {'A', 'C', 'G', 'N', 'T'}

def ERROR(title: str, msg : str):
    ORANGE = "\033[33m"
    RED = "\033[31m"
    BOLD = "\033[1m"
    RESET = "\033[0m"
    print(f"{BOLD}({ORANGE}{title}{RESET}) {BOLD}{RED}[ERREUR]{RESET} {msg}")
    
def WARNING(title: str, msg : str):
    ORANGE = "\033[33m"
    RED = "\033[31m"
    BOLD = "\033[1m"
    RESET = "\033[0m"
    print(f"{BOLD}({RED}{title}{RESET}) {BOLD}{ORANGE}[WARNING]{RESET} {msg}")

def fasta_open_and_read_header(path: str) -> Tuple[Optional[TextIO], Optional[str]]:
    try:
        fp = open(path, "r")
    except OSError as error:
        ERROR("ULB", f"Fichier ou dossier inéxistant \033[33m-->\033[0m \033[31m{error}\033[0m")
        return None, None

    header = fp.readline()
    if header == "":
        fp.close()
        ERROR("ULB", f"Fichier Vide \033[33m-->\033[0m Il doit contenir : \033[31mLigne 1\033[0m : Header et \033[31mLigne 2\033[0m : L'ADN")
        return None, None

    header = header.strip()
    if not header.startswith(">"):
        fp.close()
        ERROR("ULB", f"Nom de séquence incorrect \033[33m-->\033[0m Il doit étre de cette forme : \033[31mLigne 1\033[0m : >chr1")
        return None, None

    name = header[1:]
    
    return fp, name


def fasta_read_bases(fp: Optional[TextIO], n: int) -> Optional[str]:
    if fp is None:
        ERROR("ULB", f"ERREUR : lecture ADN impossible, fichier non ouvert dans fasta_open_and_read_header().")
        return None

    bases = []
    
    while len(bases) < n:
        c = fp.read(1)

        if c == "":
            break

        if c == "\n" or c == "\r":
            continue

        if c == ">":
            ERROR("ULB", f"ERREUR : séquence ADN incorrecte, '>' trouvé dans la ligne de séquence.")
            return None

        bases.append(c)

    return "".join(bases)


def csv_open(path: str) -> Optional[TextIO]:
    try:
        return open(path, "w")
    except OSError as error:
        ERROR("ULB", f"Fichier ou dossier inéxistant \033[33m-->\033[0m \033[31m{error}\033[0m")
        return None


def csv_write_mutation(fp: TextIO, name: str, pos: int, ref: str, alt: str) -> None:
    fp.write(f"{name},{pos},{ref},{alt}\n")

"""
    J'ai utiliser l'ia uniquement pour cette fonction car c'était à la fin de mon projet que je l'es remarqué
    aussi dans le c++ la fonction arrondi_2
"""
def arrondi_2(x):
    return f"{x + 1e-12:.2f}"

def csv_write_regions(fp: TextIO, len_sequence : int, name: str, top_regions : list) -> None:
    for region in top_regions:
        b = []
        for i in region.statistique():
            b.append(arrondi_2(i))
        fraction_a_final = ",".join(b)
        fraction_t_final = arrondi_2(region.fraction_total(len_sequence))
        fp.write(f"{name},{region.start},{region.end()},{region.len_r},{fraction_t_final},{fraction_a_final}\n")

def csv_close(fp: Optional[TextIO]) -> None:
    if fp is not None:
        fp.close()
        
def check_ALPHABET(sequence): 
    for i in sequence:
        if i not in ALPHABET:
            return False
    return True

def compare_sequence(sequence_ref, sequence, name_seq, path_out, len_seq, top_regions, region):
    for pos in range(0, len(sequence_ref)):
        if sequence_ref[pos] != sequence[pos]:
            if region != None:
                top_regions.update_top(region)
                region = None
            csv_write_mutation(path_out, name_seq, len_seq + pos, sequence_ref[pos], sequence[pos])
        else:
            if region == None:
                region = Region(len_seq + pos, sequence_ref[pos])
            else:
                region.update_compteur(sequence_ref[pos])
    return region

def main_read_buffer(name_seq, fp_ref, fp_seq, fp_out, buffer_size, fp_out_regs):
    seq_ref, seq = [], []
    len_seq = 0
    
    top_regions = Top()
    region = None
        
    while not seq_ref == "" and not seq == "":
        seq_ref = fasta_read_bases(fp_ref, buffer_size)
        seq = fasta_read_bases(fp_seq, buffer_size)
        
        if (seq_ref is None) or (seq is None):
            ERROR("ULB", "ERREUR : une erreur c'est produite lors de la lecture des séquences.")
            return 1
        
        if seq_ref == "" and seq == "":
            break
        
        if seq_ref == "" and not seq == "":
            ERROR("ULB", f"ERREUR : les tailles de séquence d'ADN sont différent, ils doivent étre égaux")
            return 1
        
        if not seq_ref == "" and seq == "":
            ERROR("ULB", f"ERREUR : les tailles de séquence d'ADN sont différent, ils doivent étre égaux")
            return 1
    
        if len(seq_ref) != len(seq):
            ERROR("ULB", f"ERREUR : les tailles de séquence d'ADN sont différent, ils doivent étre égaux")
            return 1
            
        if not check_ALPHABET(seq_ref) or not check_ALPHABET(seq):
            ERROR("ULB", "ERREUR : une des sequences contient un caractere qui n'appartient pas a l'alphabet ACGNT.")
            return 1
            
        size = len(seq_ref) % buffer_size
        region = compare_sequence(seq_ref, seq, name_seq, fp_out, len_seq, top_regions, region)
        
        if size > 0:
            len_seq += size
        else:
            len_seq += buffer_size
    
    if region != None:
        top_regions.update_top(region) 
    
    regions_top = top_regions.get_regions()
    csv_write_regions(fp_out_regs, len_seq, name_seq, regions_top)
        
    csv_close(fp_out)
    csv_close(fp_out_regs)
    fp_ref.close()
    fp_seq.close()


def main():
    start = time.time()
    parser = argparse.ArgumentParser()
    parser.add_argument("path_seq_ref", type=str, help="Entrée de la premiére séquence de ref à comparer (FASTA).")
    parser.add_argument("path_seq", type=str, help="Entrée de la deuxiéme séquence à comparer (FASTA).")
    parser.add_argument("path_output_sub", type=str, help="Sorti du résultat des substitues (CSV).")
    parser.add_argument("path_output_regs", type=str, help="Sorti du résultat des régions (CSV).")
    parser.add_argument("buffer_size", type=int, help="Taille de lecture de caractére.")
    args = parser.parse_args()
    
    path_ref = args.path_seq_ref
    path_seq = args.path_seq
    path_out = args.path_output_sub
    path_out_regs = args.path_output_regs
    buffer_size = args.buffer_size
    
    if buffer_size <= 0:
        ERROR("ULB", "buffer_size doit être strictement positif")
        return 1
        

    fp_ref, name_ref = fasta_open_and_read_header(path_ref)
    if fp_ref is None:
        return 1

    fp_seq, name_seq = fasta_open_and_read_header(path_seq)
    if fp_seq is None:
        fp_ref.close()
        return 1

    if name_ref != name_seq:
        ERROR("ULB", f"Nom de séquence de référence : \033[33m{name_ref}\033[0m différent que le nom de séquence à comparer \033[33m{name_seq}\033[0m")
        fp_ref.close()
        fp_seq.close()
        return 1

    fp_out = csv_open(path_out)
    if fp_out is None:
        fp_ref.close()
        fp_seq.close()
        return 1

    fp_out_regs = csv_open(path_out_regs)
    if fp_out_regs is None:
        csv_close(fp_out)
        fp_ref.close()
        fp_seq.close()
        return 1

    read = main_read_buffer(name_ref, fp_ref, fp_seq, fp_out, buffer_size, fp_out_regs)
    if read == 1:
        csv_close(fp_out)
        csv_close(fp_out_regs)
        fp_ref.close()
        fp_seq.close()
        return 1
                
    end = time.time()
    print(f"Temps d'exécution : {(end - start)} secondes")
    return 0


if __name__ == "__main__":
    sys.exit(main())
