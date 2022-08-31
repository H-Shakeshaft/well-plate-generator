#include "fuzzer.hpp"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <random>
#include <iostream>
#include <chrono>

namespace wellgen::fuzz {
    float sigmoidal_curve(const float& t_x) {
        return A / (B + exp(t_x * -1));
    }

    plate::WellPlate simulate_dose_response(const plate::WellPlate& t_init_plate, const Equation t_equation) {
        plate::WellPlate dr_plate;
        dr_plate.reserve(t_init_plate.size());
        std::vector<float> tmp_row;
        srand(std::chrono::steady_clock::now().time_since_epoch().count());

        const auto apply_ec50 = [&](float c) {
            tmp_row.push_back(sigmoidal_curve(c));
        };

        const auto apply_ic50 = [&](float c) {
            tmp_row.push_back(1 / sigmoidal_curve(c));
        };

        for (const auto& row : t_init_plate) {
            tmp_row = {};
            for (const auto& col : row) {
                if (t_equation == Equation::EC50) {
                    apply_ec50(col);
                } else if (t_equation == Equation::IC50) {
                    apply_ic50(col);
                } else if (t_equation == Equation::XC50) {
                    auto r_num = rand() % 10 + 1;
                    if (r_num % 2 == 0) {
                        apply_ec50(col);
                    } else {
                        apply_ic50(col);
                    }
                }
            }
            dr_plate.push_back(tmp_row);
        }
        return dr_plate;
    }

    plate::WellPlate fuzz_dose_response(const plate::WellPlate& t_data_plate) {
        plate::WellPlate f_plate;
        f_plate.reserve(t_data_plate.size());
        std::vector<float> tmp_row;

        unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::normal_distribution<float> distribution(0.5, 0.3);

        for (const auto& row : t_data_plate) {
            tmp_row = {};
            for (const auto& col : row) {
                tmp_row.push_back(col + distribution(gen));
            }
            f_plate.push_back(tmp_row);
        }
        return f_plate;
    }
}
