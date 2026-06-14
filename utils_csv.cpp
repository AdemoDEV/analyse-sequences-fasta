#include "utils_csv.h"
#include "region.hpp"


static double arrondi_2(double x)
{
    long long y = (long long)(x * 100.0 + 0.5 + 1e-12);
    return y / 100.0;
}

CsvStatus csv_open(const char* path, FILE** out_fp) {
    if (!path || !out_fp) return CSV_ERR_OPEN;
    *out_fp = std::fopen(path, "w");
    return (*out_fp) ? CSV_OK : CSV_ERR_OPEN;
}

CsvStatus csv_write_mutation(FILE* out_fp, const char* name, size_t pos, char ref, char alt) {
    if (!out_fp || !name) return CSV_ERR_IO;
    if (std::fprintf(out_fp, "%s,%zu,%c,%c\n", name, pos, ref, alt) < 0) return CSV_ERR_IO;
    return std::ferror(out_fp) ? CSV_ERR_IO : CSV_OK;
}

CsvStatus csv_write_region(FILE* out_fp, size_t len_sequence, const char* name, Region* first, Region* second, Region* third) {
    if (!out_fp || !name) return CSV_ERR_IO;
    if (first != nullptr) {
        double stat[5];
        first->statistique(stat);
        double fraction_total = first->fraction_total(len_sequence);
        if (std::fprintf(out_fp, "%s,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", name, first->get_start(), first->end(), first->get_len_(), arrondi_2(fraction_total), arrondi_2(stat[0]), arrondi_2(stat[1]), arrondi_2(stat[2]), arrondi_2(stat[3]), arrondi_2(stat[4])) < 0) {
            return CSV_ERR_IO;
        }
    } 
    
    if (second != nullptr) {
        double stat[5];
        second->statistique(stat);
        double fraction_total = second->fraction_total(len_sequence);
        if (std::fprintf(out_fp, "%s,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", name, second->get_start(), second->end(), second->get_len_(), arrondi_2(fraction_total), arrondi_2(stat[0]), arrondi_2(stat[1]), arrondi_2(stat[2]), arrondi_2(stat[3]), arrondi_2(stat[4])) < 0) {
            return CSV_ERR_IO;
        }
    }
    
    if (third != nullptr) {
        double stat[5];
        third->statistique(stat);
        double fraction_total = third->fraction_total(len_sequence);
        if (std::fprintf(out_fp, "%s,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", name, third->get_start(), third->end(), third->get_len_(), arrondi_2(fraction_total), arrondi_2(stat[0]), arrondi_2(stat[1]), arrondi_2(stat[2]), arrondi_2(stat[3]), arrondi_2(stat[4])) < 0) {
            return CSV_ERR_IO;
        }
    }
    return std::ferror(out_fp) ? CSV_ERR_IO : CSV_OK;
}

void csv_close(FILE* out_fp) {
    if (out_fp) std::fclose(out_fp);
}
