#include "factory.h"
#include "parser.hpp"

namespace wellgen::plate {
    PlateTemplate plate_template_factory(std::string &t_file_path) {
        std::ifstream ifs(t_file_path);
        // check file exists
        if (ifs.is_open()) {
            PlateTemplate plate_template = parse_template_file(ifs);
            plate_template.create_initial_plate();
            ifs.close();
            return plate_template;
        } else {
            ifs.close();
            std::cerr << "template file not found in path '" << t_file_path << "'\n";
            exit(EXIT_FAILURE);
        }
    }
}
