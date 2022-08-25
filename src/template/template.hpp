#pragma once

#include <string>
#include <vector>
#include <map>
#include <array>

namespace wellgen::plate {

    /// All valid values for the directionality of a plate, these being LR (L->R) & RB (T->B)
    /// @note LR (L->R) means that the dilution scheme is carried out left to right (top to bottom)
    /// e.g. a plate with dimensions 6x2 and 6 distinct samples would read as row1=`s1,s,s2,s,s3,s`, row2=`s4,s,s5,s,s6,s`
    ///
    /// @note TB (T->B) means that the dilution scheme is carried out top to bottom (left to right)
    /// e.g. a plate with dimensions 6x2 and 6 distinct samples would read as row1=`s1,s2,s3,s4,s5,s6`, row2=`s,s,s,s,s,s`
    enum class Directionality {
        LR, TB
    };

    // type aliases
    using uint = unsigned int;
    using PlateFormat = std::vector<std::vector<std::string>>;
    using SampleMap = std::map<std::string, std::array<float, 2>>;
    using ControlMap = std::map<std::string, float>;

    /// @brief deserialized template file
    class PlateTemplate {
    private:
        /// version of template file
        uint m_version;

        /// description of file
        std::string m_description;

        /// number of cols and rows
        uint m_num_cols, m_num_rows;

        /// indicator for directionality of dilution schemes in plate
        Directionality m_directionality;

        /// CSV formatted block for the plate layout
        PlateFormat m_plate_format;

        /// map for all sample type information
        SampleMap m_sample_map;

        /// map for all non-sample types; includes [hc, lc, bl, pc]
        ControlMap m_control_map;

    public:
        PlateTemplate() = default;

        PlateTemplate(
                uint t_version,
                std::string t_description,
                uint t_cols,
                uint t_rows,
                Directionality t_directionality,
                PlateFormat t_plate_format,
                SampleMap t_sample_map,
                ControlMap t_control_map
        );

        std::string to_string() const;
    };
}
