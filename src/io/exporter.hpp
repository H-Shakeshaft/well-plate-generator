#pragma once

#include <string>
#include "../template/template.hpp"

namespace wellgen::io {

    enum class ExportFormat {
        CSV,
        TSV
    };

    /// @brief write data to an output file
    /// @param t_export_dir path to directory to export data to
    /// @param t_format the format to export data as
    /// @param t_plate the plate data to export
    void write_file(const std::string& t_export_dir, const ExportFormat t_format, const plate::WellPlate& t_plate);
}
