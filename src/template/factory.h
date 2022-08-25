#pragma once

#include "template.hpp"
#include <iostream>

namespace wellgen::plate {
    /// @brief parse a template file, deserialize the data and create a PlateTemplate object from it
    /// @param t_file_path path to tempalte file
    /// @return deserialized template
    PlateTemplate plate_template_factory(std::string& t_file_path);
}
