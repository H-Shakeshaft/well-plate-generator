#include "exporter.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../util/uuid.hpp"

namespace wellgen::io {

    void write_file(const std::string& t_export_dir, const ExportFormat t_format, const plate::WellPlate& t_plate) {
        if (!std::filesystem::exists(std::filesystem::path({t_export_dir}))) {
            std::cerr << "ERROR: directory '" << t_export_dir << "' does not exist\n";
            exit(EXIT_FAILURE);
        }

        std::string f_name = util::get_uuid();
        std::ofstream file{t_export_dir + "/" + f_name + (t_format == ExportFormat::CSV ? ".csv" : ".tsv")};

        if (file.is_open()) {
            if (t_format == ExportFormat::CSV) {
                for (const auto& row : t_plate) {
                    std::stringstream tmp_s{};
                    for (const auto& col : row) {
                        // file << col << ",";
                        tmp_s << col << ",";
                    }
                    std::string s = tmp_s.str();
                    s.erase(s.end() - 1, s.end());
                    file << s << "\n";
                    std::cout << "(len=" << s.size() << ")" << s  << "\n";
                }
            } else if (t_format == ExportFormat::TSV) {
                for (const auto& row : t_plate) {
                    std::stringstream tmp_s{};
                    for (const auto& col : row) {
                        // file << col << ",";
                        tmp_s << col << "\t";
                    }
                    std::string s = tmp_s.str();
                    s.erase(s.end() - 1, s.end());
                    file << s << "\n";
                }
            }
            file.close();
        } else {
            file.close();
            std::cerr << "ERROR: file '" << f_name << "' not could not be written to\n";
            exit(EXIT_FAILURE);
        }
    }
}
