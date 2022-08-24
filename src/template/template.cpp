#include "template.hpp"
#include <fstream>
#include <iostream>
#include <regex>

namespace wellgen::plate {
    PlateTemplate::PlateTemplate(
            unsigned int t_version,
            std::string t_description,
            unsigned int t_cols,
            unsigned int t_rows,
            wellgen::plate::Directionality t_directionality,
            std::vector<std::vector<std::string>> t_plate_format,
            std::map<std::string, std::array<float, 2>> t_sample_map,
            std::map<std::string, float> t_control_map
    ) {
        this->m_version = t_version;
        this->m_description = t_description;
        this->m_num_cols = t_cols;
        this->m_num_rows = t_rows;
        this->m_directionality = t_directionality;
        this->m_plate_format = t_plate_format;
        this->m_sample_map = std::move(t_sample_map);
        this->m_control_map = t_control_map;
    }

    std::string PlateTemplate::to_string() const {
        using str = std::string;

        str version  = "version: ";  version += std::to_string(this->m_version); version += ", ";
        str description = "description: '" + this->m_description + "', ";

        str num_cols = "num_cols: " + std::to_string(this->m_num_cols) + ", ";
        str num_rows = "num_rows: " + std::to_string(this->m_num_rows) + ", ";

        str directionality = "directionality: '";
        directionality += this->m_directionality == Directionality::LR ? "LR" : "TB";
        directionality += "', ";

        str plate_format = "plate_format: {";
        str ln_str;
        for (const auto& line : this->m_plate_format) {
            for (const auto& well : line) {
                ln_str += well;
                ln_str += ",";
            }
            ln_str += "\n";
        }
        plate_format += "}, ";

        str sample_map = "sample_map: [";
        // sample_map.push_back(this->m_sample_map);
        for (const auto& [key, val] : this->m_sample_map) {
                sample_map += key;
                sample_map += ": [";
                    sample_map += std::to_string(val[0]); sample_map += ","; sample_map += std::to_string(val[1]);
                sample_map += "]";
            sample_map += ", ";
        }
        sample_map.erase(sample_map.end() - 2, sample_map.end());
        sample_map += "], ";

        str control_map = "control_map: [";
        // control_map.push_back(this->m_control_map);
        for (const auto& [key, val] : this->m_control_map) {
                control_map += key; control_map += ": ";
                control_map += std::to_string(val);
            control_map += ", ";
        }
        control_map.erase(control_map.end() - 2, control_map.end());
        control_map += "]";

        str str_rep = "PlateTemplate(";
        str_rep += version;
        str_rep += description;
        str_rep += num_cols;
        str_rep += num_rows;
        str_rep += directionality;
        str_rep += plate_format;
        str_rep += sample_map;
        str_rep += control_map;
        str_rep += ")";

        return str_rep;
    }

    PlateTemplate plate::plate_template_factory(std::string& t_file_path) {
        std::ifstream ifs(t_file_path);
        std::string line;
        unsigned int line_number = 0;

        // file data to parse
        // TODO: create some type aliases (and refactor them in)
        unsigned int version;  // using uint = unsigned int;
        std::string description;

        unsigned int cols;
        unsigned int rows;
        wellgen::plate::Directionality directionality;

        std::vector<std::vector<std::string>> plate_format;  // using PlateFormat = ...

        std::map<std::string, std::array<float, 2>> sample_map;  // using SampleMap = ...
        std::map<std::string, float> control_map;  // using ControlMap = ...

        // check file exists
        if (ifs.is_open()) {

            // values for
            std::vector<std::string> col_row_dir_vals{};
            std::string tmp_crd_val{};

            // read file
            // TODO: fix error reporting such that there is some denotation of syntax errors
            while (std::getline(ifs, line)) {
                std::cout << "DEBUG: line content (line " << line_number + 1 << ")\t\t" << line << "\n";

                // parse file
                if (line.empty()) {
                    ifs.close();
                    std::cerr << "SYNTAX ERROR: blank line detected on line" << line_number + 1 << "\n";
                    exit(EXIT_FAILURE);
                }

                switch (line_number) {
                    // parse version
                    case 0:
                        line.erase(line.begin());
                        try {
                            version = static_cast<unsigned int>(std::stoul(line));
                        } catch (std::invalid_argument& e) {
                            ifs.close();
                            std::cerr << "FAILED TO PARSE VERSION: integer expected for version, got '" << line << "'\n";
                            exit(EXIT_FAILURE);
                        } catch (std::exception& e) {
                            ifs.close();
                            std::cerr << "FAILED TO PARSE VERSION: exception raised during parsing: " << e.what() << "\n";
                            exit(EXIT_FAILURE);
                        }
                        break;

                    // parse description
                    case 1:
                        line.erase(line.begin());
                        do {
                            if (line[0] == ' ') {
                                line.erase(line.begin());
                            } else {
                                break;
                            }
                        } while (true);
                        description = line;
                        break;

                    // parse cols, rows and dir
                    case 2:
                        for (const auto& c : line) {
                            if (c == ' ') {
                                col_row_dir_vals.push_back(tmp_crd_val);
                                tmp_crd_val.clear();
                                continue;
                            }
                            tmp_crd_val.push_back(c);
                        }
                        col_row_dir_vals.push_back(tmp_crd_val);

                        if (col_row_dir_vals.size() != 3) {
                            ifs.close();
                            std::cerr << "INVALID DECLARATION BLOCK: number of declarations is not 3, got " << col_row_dir_vals.size() << "\n";
                            std::cerr << "TRACE: line content: [" << line << "] ";
                            std::cerr << "parsed content: [" << col_row_dir_vals[0] << ", " << col_row_dir_vals[1] << ", " << col_row_dir_vals[2] << "]\n";
                            exit(EXIT_FAILURE);
                        }

                        // parse cols
                        try {
                            cols = static_cast<unsigned int>(std::stoul(col_row_dir_vals[0]));
                        } catch (std::invalid_argument& e) {
                            ifs.close();
                            std::cerr << "FAILED TO PARSE COLS: unsigned integer expected for cols, got '" << col_row_dir_vals[0] << "'\n";
                            exit(EXIT_FAILURE);
                        } catch (std::exception& e) {
                            ifs.close();
                            std::cerr << "FAILED TO PARSE COLS: exception raised during parsing: " << e.what() << "\n";
                            exit(EXIT_FAILURE);
                        }

                        // parse rows
                        try {
                            rows = static_cast<unsigned int>(std::stoul(col_row_dir_vals[1]));
                        } catch (std::invalid_argument& e) {
                            ifs.close();
                            std::cerr << "FAILED TO PARSE ROWS: unsigned integer expected for rows, got '" << col_row_dir_vals[1] << "'\n";
                            exit(EXIT_FAILURE);
                        } catch (std::exception& e) {
                            ifs.close();
                            std::cerr << "FAILED TO PARSE ROWS: exception raised during parsing: " << e.what() << "\n";
                            exit(EXIT_FAILURE);
                        }

                        // parse directionality
                        if (col_row_dir_vals[2] != "LR" && col_row_dir_vals[2] != "TB") {
                            ifs.close();
                            std::cerr << "FAILED TO PARSE DIRECTIONALITY: valid values for directionality are LR & TB, got '" << col_row_dir_vals[2] << "'\n";
                            exit(EXIT_FAILURE);
                        }
                        directionality = col_row_dir_vals[2] == "LR" ? Directionality::LR : Directionality::TB;
                        break;

                    // all other parsing (format parsing, sample parsing, non-sample parsing)
                    default:
                        // for each row
                        //  find all occurrences of sx (e.g. s1, s2 ...)
                        //      whenever an occurrence of sx is found, add it to the map with initialised array of [0f, 0f]
                        //      when an occurrence of hc, lc, pc or bl is encountered, add it to the map and initialise the value to nullptr

                        // convert line into series of wells
                        // TODO: collapse token iteration into while loop
                        char* token = strtok(const_cast<char *>(line.c_str()), ",");
                        std::vector<std::string> wells{};

                        while(token != nullptr) {
                            wells.emplace_back(token);
                            token = strtok(nullptr, ",");
                        }

                        // match each well to see if there is a sx hit (e.g. s1, s2, ..., sx)
                        std::smatch motif_match;
                        std::regex motif_regex{"s([0-9]+)"};

                        for (const auto& w : wells) {
                            if (std::regex_match(w, motif_match, motif_regex)) {
                                sample_map.insert(std::pair<std::string, std::array<float, 2>>(w, {0,0}));
                            }
                        }

                        // handle non-sample type wells
                        for (const auto& w : wells) {
                            if (w == "hc" || w == "lc" || w == "pc" || w == "bl") {
                                control_map[w];
                            }
                        }

                        // for each line prefixed with '>>'
                        //  if sample
                        //      if key not present in sample map, exit with error
                        //      else, set array vals to [init conc, df] (parsed from line)
                        //          if there are not 2 parsed values, exit with error (e.g. input line is `>>s1 10` or `>>s1 10 10 10`)
                        //
                        // if not sample
                        //  if present in map,
                        //      if value is 'NA', then exit with error (cannot map well type to null value)
                        //      else, set value to parsed value
                        //
                        // if any values in control_map == nullptr, exit with error (control_map value uninitialised)

                        if (line.substr(0, 2) == ">>") {
                            line = line.substr(2, line.size() - 2);
                            std::cout << "TRACE: data block line = " << line << "\n";

                            // TODO: fix issue where samples with x value greater than 9 will not be found
                            std::string w = line.substr(0, 2);

                            if (std::regex_match(w, motif_match, motif_regex)) {
                                if (sample_map.find(w) == sample_map.end()) {
                                    ifs.close();
                                    std::cerr << "SYNTAX ERROR: sample type id '" << w <<  "' (line " << line_number + 1 << ") not declared in format block\n";
                                    exit(EXIT_FAILURE);
                                } else {
                                    char* val_token = strtok(const_cast<char *>(line.c_str()), " ");
                                    std::vector<std::string> str_vals{};
                                    std::vector<float> vals{};

                                    while(val_token != nullptr) {
                                        str_vals.emplace_back(val_token);
                                        val_token = strtok(nullptr, " ");
                                    }
                                    str_vals.erase(str_vals.begin());

                                    for (const auto& v : str_vals) {
                                        try {
                                            vals.emplace_back(static_cast<float>(std::stof(v)));
                                        } catch (std::invalid_argument &e) {
                                            ifs.close();
                                            std::cerr << "FAILED TO PARSE SAMPLE MAP: float expected for, got '" << v << "'\n";
                                            exit(EXIT_FAILURE);
                                        } catch (std::exception &e) {
                                            ifs.close();
                                            std::cerr << "FAILED TO PARSE SAMPLE MAP: exception raised during parsing: "
                                                      << e.what() << "\n";
                                            exit(EXIT_FAILURE);
                                        }
                                    }

                                    std::cout << "TRACE: parsed values for " << w << " = [" << vals[0] << "," << vals[1] << "]\n";
                                    std::cout << "TRACE: initial value for " << w << " = [" << sample_map[w][0] << "," << sample_map[w][1] << "]\n";
                                    sample_map.find(w)->second = {vals[0], vals[1]};
                                    std::cout << "TRACE: updated value for " << w << " = [" << sample_map[w][0] << "," << sample_map[w][1] << "]\n";
                                }
                            }
                            else if (w == "hc" || w == "lc" || w == "pc" || w == "bl") {
                                // split line
                                char* val_token = strtok(const_cast<char *>(line.c_str()), " ");
                                std::vector<std::string> str_vals{};

                                while(val_token != nullptr) {
                                    str_vals.emplace_back(val_token);
                                    val_token = strtok(nullptr, " ");
                                }

                                if (str_vals.size() != 2) {
                                    ifs.close();
                                    std::cerr << "SYNTAX ERROR: control block (line " << line_number + 1 << "), expected 1 value declared, got "  << str_vals.size() - 1 << "\n";
                                    exit(EXIT_FAILURE);
                                }

                                if (control_map.find(w) == control_map.end()) {
                                    if (str_vals[1] == "NA") {
                                        std::cout << "TRACE: NA specified value encountered (line " << line_number + 1 << ") for control key '" << w << "'" << "\n";
                                        continue;
                                    }
                                    ifs.close();
                                    std::cerr << "SYNTAX ERROR: control '" << w <<  "' (line " << line_number + 1 << ") not declared in format block\n";
                                    exit(EXIT_FAILURE);
                                } else {
                                    if (str_vals[1] == "NA") {
                                        ifs.close();
                                        std::cerr << "SYNTAX ERROR: control '" << w <<  "' (line " << line_number + 1 << ") declared with uninitialized value; values cannot be NA for declared controls\n";
                                        exit(EXIT_FAILURE);
                                    }

                                    try {
                                        control_map.find(w)->second = static_cast<float>(std::stof(str_vals[1]));
                                    } catch (std::invalid_argument &e) {
                                        ifs.close();
                                        std::cerr << "FAILED TO PARSE CONTROL MAP: float expected for, got '" << str_vals[1] << "'\n";
                                        exit(EXIT_FAILURE);
                                    } catch (std::exception &e) {
                                        ifs.close();
                                        std::cerr << "FAILED TO PARSE CONTROL MAP: exception raised during parsing: " << e.what() << "\n";
                                        exit(EXIT_FAILURE);
                                    }
                                }
                            }
                        }

                        break;
                }
                ++line_number;
            }

            // generate PlateTemplate
            PlateTemplate plate_template{
                version, description,
                cols, rows, directionality,
                plate_format,
                sample_map, control_map
            };

            std::cout << "DEBUG: parsed template\n\t" << plate_template.to_string() << "\n";

            ifs.close();
            return plate_template;
        } else {
            ifs.close();
            std::cerr << "template file not found in path '" << t_file_path << "'\n";
            exit(EXIT_FAILURE);
        }
    }
}
// TODO: split out parsing function into different file
