#include "parser.hpp"
#include <iostream>
#include <regex>

namespace wellgen::plate {
    PlateTemplate parse_template_file(std::ifstream& t_ifs) {
        std::string line;
        uint line_number = 0;

        // parse file data into these values
        uint version, cols, rows;
        std::string description;
        Directionality directionality;
        PlateFormat plate_format;
        SampleMap sample_map;
        ControlMap control_map;

        // values for
        std::vector<std::string> col_row_dir_vals{};
        std::string tmp_crd_val{};

        // TODO: fix error reporting such that there is some denotation of syntax errors
        // parse file
        while (std::getline(t_ifs, line)) {
            // std::cout << "DEBUG: line content (line " << line_number + 1 << ")\t\t" << line << "\n";
            if (line.empty()) {
                t_ifs.close();
                std::cerr << "SYNTAX ERROR (BLANK LINE): blank line detected on line " << line_number + 1 << "\n";
                exit(EXIT_FAILURE);
            }

            // TODO: move logic for switch statement into if-then-else blocks
            switch (line_number) {
                case 0: // parse version
                    line.erase(line.begin());
                    try {
                        version = static_cast<uint>(std::stoul(line));
                    } catch (std::invalid_argument& e) {
                        t_ifs.close();
                        std::cerr << "SYNTAX ERROR (FAILED TO PARSE VERSION): integer expected for version, got '"
                                  << line << "'\n";
                        exit(EXIT_FAILURE);
                    } catch (std::exception& e) {
                        t_ifs.close();
                        std::cerr << "SYNTAX ERROR (FAILED TO PARSE VERSION): exception raised during parsing: "
                                  << e.what() << "\n";
                        exit(EXIT_FAILURE);
                    }
                    break;

                case 1: // parse description
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

                case 2: // parse cols, rows and directionality
                    for (const auto &c: line) {
                        if (c == ' ') {
                            col_row_dir_vals.push_back(tmp_crd_val);
                            tmp_crd_val.clear();
                            continue;
                        } else {
                            tmp_crd_val.push_back(c);
                        }
                    }
                    col_row_dir_vals.push_back(tmp_crd_val);

                    if (col_row_dir_vals.size() != 3) {
                        t_ifs.close();
                        std::cerr << "SYNTAX ERROR (INVALID DECLARATION BLOCK): number of declarations is not 3, got "
                                  << col_row_dir_vals.size() << "\n"
                                  << "TRACE: line content: [" << line << "] "
                                  << "parsed content: [" << col_row_dir_vals[0] << ", " << col_row_dir_vals[1] << ", "
                                  << col_row_dir_vals[2] << "]\n";
                        exit(EXIT_FAILURE);
                    }

                    // parse cols
                    try {
                        cols = static_cast<uint>(std::stoul(col_row_dir_vals[0]));
                    } catch (std::invalid_argument& e) {
                        t_ifs.close();
                        std::cerr << "SYNTAX ERROR (FAILED TO PARSE COLS): unsigned integer expected for cols, got '"
                                  << col_row_dir_vals[0] << "'\n";
                        exit(EXIT_FAILURE);
                    } catch (std::exception& e) {
                        t_ifs.close();
                        std::cerr << "FAILED TO PARSE COLS: exception raised during parsing: " << e.what() << "\n";
                        exit(EXIT_FAILURE);
                    }

                    // parse rows
                    try {
                        rows = static_cast<uint>(std::stoul(col_row_dir_vals[1]));
                    } catch (std::invalid_argument& e) {
                        t_ifs.close();
                        std::cerr << "SYNTAX ERROR (FAILED TO PARSE ROWS): unsigned integer expected for rows, got '"
                                  << col_row_dir_vals[1] << "'\n";
                        exit(EXIT_FAILURE);
                    } catch (std::exception& e) {
                        t_ifs.close();
                        std::cerr << "FAILED TO PARSE ROWS: exception raised during parsing: " << e.what() << "\n";
                        exit(EXIT_FAILURE);
                    }

                    // parse directionality
                    if (col_row_dir_vals[2] != "LR" && col_row_dir_vals[2] != "TB") {
                        t_ifs.close();
                        std::cerr << "SYNTAX ERROR (INVALID DIRECTIONALITY): "
                                  << "valid values for directionality are LR & TB, got '"
                                  << col_row_dir_vals[2] << "'\n";
                        exit(EXIT_FAILURE);
                    }
                    directionality = col_row_dir_vals[2] == "LR" ? Directionality::LR : Directionality::TB;
                    break;

                default: // parse samples and controls
                    std::smatch motif_match;
                    std::regex motif_regex{"s([0-9]+)"};

                    if (line.substr(0, 2) != ">>") {
                        // convert line into series of wells
                        char* token = strtok(const_cast<char *>(line.c_str()), ",");
                        std::vector<std::string> wells{};
                        while (token != nullptr) {
                            wells.emplace_back(token);
                            token = strtok(nullptr, ",");
                        }

                        // match each well to see if there is a sx hit (e.g. s1, s2, ..., sx)
                        for (const auto &w: wells) {
                            if (std::regex_match(w, motif_match, motif_regex)) {
                                sample_map.insert(std::pair<std::string, std::array<float, 2>>(w, {0, 0}));
                            }
                        }

                        // match non-sample type wells and default initialise them in the control map
                        for (const auto &w: wells) {
                            if (w == "hc" || w == "lc" || w == "pc" || w == "bl") {
                                control_map[w];
                            }
                        }

                        // insert vectorised line into the internal representation of the plate format
                        plate_format.push_back(wells);
                    } else {
                        // remove the `>>` prefix for each line
                        line = line.substr(2, line.size() - 2);

                        // split sample line into array of all delimited values (e.g. s1 10 10)
                        char *val_token = strtok(const_cast<char *>(line.c_str()), " ");
                        std::vector<std::string> str_vals{};
                        std::vector<float> vals{};

                        while (val_token != nullptr) {
                            str_vals.emplace_back(val_token);
                            val_token = strtok(nullptr, " ");
                        }
                        std::string well_type_id = str_vals[0];

                        // if sample found (sx)
                        if (std::regex_match(well_type_id, motif_match, motif_regex)) {
                            if (str_vals.size() != 3) {
                                t_ifs.close();
                                std::cerr << "SYNTAX ERROR (MALFORMED SAMPLE BLOCK): sample block (line "
                                          << line_number + 1 << "), expected 2 values declared, got "
                                          << str_vals.size() - 1 << "\n";
                                exit(EXIT_FAILURE);
                            }

                            if (sample_map.find(well_type_id) == sample_map.end()) {
                                t_ifs.close();
                                std::cerr << "SYNTAX ERROR (UNDECLARED CONTROL): sample type id '" << well_type_id
                                          << "' (line " << line_number + 1 << ") not declared in format block\n";
                                exit(EXIT_FAILURE);
                            } else {
                                // try to parse the values for init concentration and dilution factor
                                str_vals.erase(str_vals.begin());
                                for (const auto& v: str_vals) {
                                    try {
                                        vals.emplace_back(static_cast<float>(std::stof(v)));
                                    } catch (std::invalid_argument& e) {
                                        t_ifs.close();
                                        std::cerr << "FAILED TO PARSE SAMPLE MAP: float expected for, got '" << v
                                                  << "'\n";
                                        exit(EXIT_FAILURE);
                                    } catch (std::exception& e) {
                                        t_ifs.close();
                                        std::cerr << "FAILED TO PARSE SAMPLE MAP: exception raised during parsing: "
                                                  << e.what() << "\n";
                                        exit(EXIT_FAILURE);
                                    }
                                }

                                sample_map.find(well_type_id)->second = {vals[0], vals[1]};
                            }
                        } else if (well_type_id == "hc" || well_type_id == "lc" || well_type_id == "pc" || well_type_id == "bl") {
                            if (str_vals.size() != 2) {
                                t_ifs.close();
                                std::cerr << "SYNTAX ERROR (MALFORMED CONTROL BLOCK): control block (line "
                                          << line_number + 1 << "), expected 1 value declared, got "
                                          << str_vals.size() - 1 << "\n";
                                exit(EXIT_FAILURE);
                            }

                            if (control_map.find(well_type_id) == control_map.end()) {
                                if (str_vals[1] == "NA") {
                                    // std::cout << "TRACE: NA specified value encountered (line " << line_number + 1
                                    //           << ") for control key '" << well_type_id << "'" << "\n";
                                    continue;
                                } else {
                                    t_ifs.close();
                                    std::cerr << "SYNTAX ERROR (CONTROL NOT DECLARED): control '" << well_type_id
                                              << "' (line " << line_number + 1 << ") not declared in format block\n";
                                    exit(EXIT_FAILURE);
                                }
                            } else {
                                // attempt to parse the value associated with the control
                                if (str_vals[1] == "NA") {
                                    t_ifs.close();
                                    std::cerr << "SYNTAX ERROR: control '" << well_type_id << "' (line " << line_number + 1
                                              << ") declared with uninitialized value; values cannot be NA for declared controls\n";
                                    exit(EXIT_FAILURE);
                                }

                                try {
                                    control_map.find(well_type_id)->second = static_cast<float>(std::stof(str_vals[1]));
                                } catch (std::invalid_argument& e) {
                                    t_ifs.close();
                                    std::cerr << "FAILED TO PARSE CONTROL MAP: float expected for, got '" << str_vals[1]
                                              << "'\n";
                                    exit(EXIT_FAILURE);
                                } catch (std::exception& e) {
                                    t_ifs.close();
                                    std::cerr << "FAILED TO PARSE CONTROL MAP: exception raised during parsing: "
                                              << e.what() << "\n";
                                    exit(EXIT_FAILURE);
                                }
                            }
                        }
                    }
                    break;
            }
            ++line_number;
        }

        return PlateTemplate{
                version, description,
                cols, rows, directionality,
                plate_format,
                sample_map, control_map
        };
    }
}
