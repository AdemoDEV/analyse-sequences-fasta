#include <cstdio>
#include <cstdlib>

#include "utils_fasta.h"
#include "utils_csv.h"

#include "region.hpp"
#include "top.hpp"

#include <iostream>
#include <cstring>
#include <cstddef>
#include <time.h>

using namespace std;

static const size_t MAX_SEQ_NAME_LEN = 101; // 100 caractères + '\0'

void closeAllFile(FILE* fp_out, FILE* fp_out_reg, FILE* fp_ref, FILE* fp_seq) {
    csv_close(fp_out);
    csv_close(fp_out_reg);
    fasta_close(fp_ref);
    fasta_close(fp_seq);
}

bool check_ALPHABET(const char* seq, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (seq[i] != 'A' && seq[i] != 'C' && seq[i] != 'G' && seq[i] != 'N' && seq[i] != 'T') {
            return false;   
        }
    }
    return true;
}

int compare_sequence(const char* seq_ref, const char* seq, char* name_ref, FILE* fp_out, size_t len_all_seq, size_t len_seq, Top& top, Region*& region) {
    for (size_t i = 0; i < len_seq; i++) {
        if (seq_ref[i] != seq[i]) {
            if (region != nullptr) {
                top.update_top(*region);
                delete region;
                region = nullptr;
            }
            CsvStatus cs = csv_write_mutation(fp_out, name_ref, len_all_seq + i, seq_ref[i], seq[i]);
            if (cs != CSV_OK) {
                cout << "ERREUR : impossible d'ecrire dans le fichier CSV des substitutions." << endl;
                return -1;
            }
        } else {
            if (region == nullptr) {
                region = new Region(len_all_seq + i, seq_ref[i]);
            } else {
                region->update_compteur(seq_ref[i]);
            }
        }
    }
    return 1;
}

int read_error(FastaStatus fs, const char* file) {
    switch (fs) {
        case FASTA_ERR_OPEN:
            cout << "ERREUR " << (file) << " : ouverture du fichier FASTA impossible." << endl; 
            break;
        case FASTA_ERR_EMPTY:
            cout << "ERREUR " << (file) << " : le fichier FASTA ne contient aucune sequence." << endl; 
            break;
        case FASTA_ERR_FORMAT:
            cout << "ERREUR " << (file) << " : le fichier n'est pas au format FASTA valide (en-tete manquant, plusieurs sequences invalide, ...) ." << endl; 
            break;
        case FASTA_ERR_NAME_TOO_LONG:
            cout << "ERREUR " << (file) << " : le nom de la sequence depasse la taille autorisee." << endl; 
            break;
        case FASTA_ERR_IO:
            cout << "ERREUR " << (file) << " : une erreur d'entree/sortie a été repéré lors de la lecture." << endl; 
            break;
        default:
        cout << "ERREUR " << (file) << " : probléme non-identifié."<< endl; 
            break;
        }
    return -1;
}

int main_read_buffer(char* name_seq, FILE* fp_ref, FILE* fp_seq, FILE* fp_out, size_t buffer_size, FILE* fp_out_regions) {
    char* seq_ref = new char[buffer_size];
    char* seq= new char[buffer_size];
    size_t n_ref = 0;
    size_t n_seq = 0;
    size_t len_all_sequence = 0;

    Top top_regions = Top();
    Region* region = nullptr;

    FastaStatus fs;

    while (true) {
        fs = fasta_read_bases(fp_ref, seq_ref, buffer_size, &n_ref);
        if (fs != FASTA_OK) {
            const char* file = "séquence de référence";
            int error = read_error(fs, file);
            if (error == -1) {
                delete region;
                delete[] seq_ref;
                delete[] seq;
                closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
                return -1;
            }
        }

        fs = fasta_read_bases(fp_seq, seq, buffer_size, &n_seq);
        if (fs != FASTA_OK) {
            const char* file = "séquence à comparer";
            int error = read_error(fs, file);
            if (error == -1) {
                delete region;
                delete[] seq_ref;
                delete[] seq;
                closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
                return -1;
            }
        }

        if (n_ref == 0 and n_seq == 0) {
            break;
        } 

        if (n_ref != n_seq) {
            cout << "ERREUR : les tailles de séquence d'ADN sont différent, ils doivent étre égaux." << endl;
            delete region;
            delete[] seq_ref;
            delete[] seq;
            closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
            return -1;
        }

        if (not check_ALPHABET(seq_ref, n_ref) or not check_ALPHABET(seq, n_seq)) {
            cout << "ERREUR : une des sequences contient un caractere qui n'appartient pas a l'alphabet ACGNT." << endl;
            delete region;
            delete[] seq_ref;
            delete[] seq;
            closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
            return -1;
        }

        size_t size = n_ref % buffer_size;

        int ok = compare_sequence(seq_ref, seq, name_seq, fp_out, len_all_sequence, n_ref, top_regions, region);

        if (ok == -1) {
            delete region;
            delete[] seq_ref;
            delete[] seq;
            closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
            return -1;
        }

        if (size > 0) {
            len_all_sequence += size;
        } else {
            len_all_sequence += buffer_size;
        }
    }

    if (region != nullptr) {
        top_regions.update_top(*region);
    }

    CsvStatus cs = csv_write_region(fp_out_regions, len_all_sequence, name_seq,top_regions.get_first(),top_regions.get_second(),top_regions.get_third());

    if (cs != CSV_OK) {
        cout << "ERREUR : impossible d'ecrire dans le fichier CSV des regions." << endl;
        delete region;
        delete[] seq_ref;
        delete[] seq;
        closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
        return -1;
    }

    delete region;
    delete[] seq_ref;
    delete[] seq;
    closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
    return 1;
}

int main(int argc, char* argv[]) {
    clock_t start = clock();

    if (argc != 6) {
        cout << "Argument #" << argc << " manquant dans l'éxécution, 'chemin_ref_seq' 'chemin_seq' 'chemin_output_subs' 'chemin_output_regs' 'buffer_size'" << endl; 
        return -1;
    }

    const char* path_ref = argv[1];
    const char* path_seq = argv[2];
    const char* path_out = argv[3];
    const char* path_out_regs = argv[4];
    const char* arg_bufer_size = argv[5];
    
    int buffer_size_int = atoi(arg_bufer_size);
    if (buffer_size_int <= 0) {
        cout << "ERREUR : buffer_size doit étre strictement positif" << endl;
        return -1;
    }
    size_t buffer_size = static_cast<size_t>(buffer_size_int);

    FILE* fp_ref = nullptr;
    FILE* fp_seq = nullptr;

    char name_ref[MAX_SEQ_NAME_LEN];
    char name_seq[MAX_SEQ_NAME_LEN];

    FastaStatus fs;

    fs = fasta_open_and_read_header(path_ref, &fp_ref, name_ref, MAX_SEQ_NAME_LEN);
    if (fs != FASTA_OK) {
        const char* file = "séquence de référence";
        int error = read_error(fs, file);
        if (error == -1) {
            fasta_close(fp_ref);
            fasta_close(fp_seq);
            return -1;
        }
    }


    fs = fasta_open_and_read_header(path_seq, &fp_seq, name_seq, MAX_SEQ_NAME_LEN);
    if (fs != FASTA_OK) {
        const char* file = "séquence à comparer";
        int error = read_error(fs, file);
        if (error == -1) {
            fasta_close(fp_ref);
            fasta_close(fp_seq);
            return -1;
        }
    }

    if (strcmp(name_ref, name_seq) != 0) {
        cout << "ERREUR (Nom séquence) : Nom de séquence différent que le nom de séquence a comparer" << endl; 
        fasta_close(fp_ref);
        fasta_close(fp_seq);
        return -1;
    }


    FILE* fp_out = nullptr;
    FILE* fp_out_regions = nullptr;

    CsvStatus cs;
    cs = csv_open(path_out, &fp_out);
    if (cs != CSV_OK) {
        switch (cs) {
            case CSV_ERR_OPEN:
                cout << "ERREUR (file csv) : ouverture du fichier CSV impossible." << endl; 
                break;
            case CSV_ERR_IO:
                cout << "ERREUR (file csv) : une erreur d'entree/sortie a été repéré lors de l'écriture."<< endl; 
                break;
            default:
                cout << "ERREUR (file csv) : probléme non-identifié." << endl; 
                break;
        }
        closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
        return -1;
    }


    cs = csv_open(path_out_regs, &fp_out_regions);
    if (cs != CSV_OK) {
        switch (cs) {
            case CSV_ERR_OPEN:
                cout << "ERREUR (file csv) : ouverture du fichier CSV impossible." << endl; 
                break;
            case CSV_ERR_IO:
                cout << "ERREUR (file csv) : une erreur d'entree/sortie a été repéré lors de l'écriture."<< endl; 
                break;
            default:
                cout << "ERREUR (file csv) : probléme non-identifié." << endl; 
                break;
        }
        closeAllFile(fp_out, fp_out_regions, fp_ref, fp_seq);
        return -1;
    }


    int finish = main_read_buffer(name_seq, fp_ref, fp_seq, fp_out, buffer_size, fp_out_regions);
    if (finish == -1) {
        return -1;
    }
    clock_t end = clock();
    cout << "Temps d'execution : " << double(end - start) / CLOCKS_PER_SEC << " secondes" << endl;
    return 0;
}
