#include "template.hpp"
#include <regex>
#include <map>
#include <iostream>

namespace wellgen::plate {
    PlateTemplate::PlateTemplate(
            uint t_version,
            std::string t_description,
            uint t_cols,
            uint t_rows,
            Directionality t_directionality,
            PlateFormat t_plate_format,
            SampleMap t_sample_map,
            ControlMap t_control_map
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

        str plate_format = "plate_format: [";
        str ln_str;
        for (const auto& line : this->m_plate_format) {
            ln_str = "[";
            for (const auto& well : line) {
                ln_str += well;
                ln_str += ",";
            }
            ln_str.erase(ln_str.end() - 1);
            ln_str += "], ";
            plate_format += ln_str;
        }
        plate_format.erase(plate_format.end() - 2, plate_format.end());
        plate_format += "], ";

        str sample_map = "sample_map: [";
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
        for (const auto& [key, val] : this->m_control_map) {
                control_map += key; control_map += ": ";
                control_map += std::to_string(val);
            control_map += ", ";
        }
        control_map.erase(control_map.end() - 2, control_map.end());
        control_map += "]";

        return "PlateTemplate(" + version + description + num_cols + num_rows + directionality + plate_format + sample_map + control_map + ")";
    }

    void PlateTemplate::create_initial_plate() {
        WellPlate plate{};
        std::vector<float> row;
        plate.reserve(this->m_num_rows);
        row.reserve(this->m_num_cols);

        std::smatch motif_match;
        std::regex motif_regex{"s([0-9]+)"};

        if (this->m_directionality == Directionality::LR) {
            for (int j = 0; j < this->m_num_rows; ++j) {
                row = {};
                std::string init_sample_conc_loc;

                for (int i = 0; i < this->m_num_cols; ++i) {
                    const auto& well = this->m_plate_format[j][i];

                    // initial concentration for samples
                    if (std::regex_match(well, motif_match, motif_regex)) {
                        row.push_back(this->m_sample_map[well][0]);
                        init_sample_conc_loc = well;
                    }
                    // dilution schemes
                    else if (well == "s") {
                        row.push_back(row[i - 1] / this->m_sample_map[init_sample_conc_loc][1]);
                    }
                    // controls
                    else {
                        row.push_back(this->m_control_map[well]);
                    }
                }
                plate.push_back(row);
            }
        } else if (this->m_directionality == Directionality::TB) {
            std::map<int, std::string> columnar_init_sample_conc_loc{};
            for (int j = 0; j < this->m_num_rows; ++j) {
                row = {};

                for (int i = 0; i < this->m_num_cols; ++i) {
                    const auto& well = this->m_plate_format[j][i];

                    // initial concentration for samples
                    if (std::regex_match(well, motif_match, motif_regex)) {
                        row.push_back(this->m_sample_map[well][0]);
                        columnar_init_sample_conc_loc.insert(std::pair<int, std::string>(i, well));
                    }
                    // dilution schemes
                    else if (well == "s") {
                        row.push_back(plate[j - 1][i] / this->m_sample_map[columnar_init_sample_conc_loc[i]][1]);
                    }
                    // controls
                    else {
                        row.push_back(this->m_control_map[well]);
                    }
                }
                plate.push_back(row);
            }
        }

        this->m_initial_plate = plate;
    }

    WellPlate PlateTemplate::initial_plate() {
        return this->m_initial_plate;
    }
}
