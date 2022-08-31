#pragma once

#include "../template/template.hpp"

namespace wellgen::fuzz {
    /// value of A used in sigmoidal curve calculation
    constexpr float A = 1;

    /// value of B used in sigmoidal curve calculation
    constexpr float B = 1;

    ///
    enum class Equation {
        IC50, // IC50 curve - inhibition conc
        EC50, // EC50 curve - effective conc
        XC50 // mix of EC50 and IC50
    };

    /// @brief sigmoidal curve function as defined in https://en.wikipedia.org/wiki/Sigmoid_function
    /// @note f(x) = A / B + e^-x
    /// @param t_x the value of x to derive a value from
    /// @return value of y for given x on sigmoidal curve
    float sigmoidal_curve(const float& t_x);

    /// @brief generate unfuzzed plate values
    /// @param t_init_plate the well plate with mapped initial concentration/control values
    /// @param t_equation the equation to use in calculating the sigmoidal curve
    plate::WellPlate simulate_dose_response(const plate::WellPlate& t_init_plate, const Equation t_equation);

    /// @brief generate fuzzed plate values
    plate::WellPlate fuzz_dose_response(const plate::WellPlate& t_data_plate);
}
