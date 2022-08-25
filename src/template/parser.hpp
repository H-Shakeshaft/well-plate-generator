#pragma once

#include "template.hpp"
#include <fstream>

namespace wellgen::plate {
    /// parses a template file (deserializing it) and returns an internal representation of the file
    /// @param t_ifs file stream for template to be parsed
    /// @return serialized plate template
    PlateTemplate parse_template_file(std::ifstream& t_ifs);
}
