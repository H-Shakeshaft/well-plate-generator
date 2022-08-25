#include "template.hpp"
#include <iostream>
#include "parser.hpp"

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

    PlateTemplate plate_template_factory(std::string& t_file_path) {
        std::ifstream ifs(t_file_path);
        // check file exists
        if (ifs.is_open()) {
            PlateTemplate plate_template = parse_template_file(ifs);
            ifs.close();
            return plate_template;
        } else {
            ifs.close();
            std::cerr << "template file not found in path '" << t_file_path << "'\n";
            exit(EXIT_FAILURE);
        }
    }
}
