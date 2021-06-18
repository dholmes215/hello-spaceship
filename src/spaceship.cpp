// Copyright (C) 2021 David Holmes <dholmes@dholmes.us>. All rights reserved.
#include <algorithm>
#include <array>
#include <cmath>
#include <compare>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>

#ifdef __cpp_impl_three_way_comparison
#ifdef __cpp_lib_three_way_comparison
#define COMPARE_3WAY
#endif  // __cpp_lib_three_way_comparison
#endif  // __cpp_impl_three_way_comparison

namespace {

enum class ship_class {
    constitution,
    danube,
    defiant,
    excelsior,
    galaxy,
    intrepid,
    miranda,
    nebula,
};

struct ship {
    std::string name;
    std::string registry;
    ship_class model{};
    int first_seen{};

#ifdef COMPARE_3WAY
    friend auto operator<=>(const ship& l, const ship& r) noexcept = default;
    // friend std::strong_ordering operator<=>(const ship& l,
    //                                         const ship& r) noexcept =
    //                                         default;
#else
    friend bool operator<(const ship& l, const ship& r) noexcept
    {
        return std::tie(l.name, l.registry, l.model, l.first_seen) <
               std::tie(r.name, r.registry, r.model, r.first_seen);
    }
    friend bool operator>(const ship& l, const ship& r) noexcept
    {
        return l > r;
    }
    friend bool operator<=(const ship& l, const ship& r) noexcept
    {
        return !(l > r);
    }
    friend bool operator>=(const ship& l, const ship& r) noexcept
    {
        return !(l < r);
    }

    // defaulted == and != is also a C++20 feature, but they're implied by
    // defaulted operator<=>.
    friend bool operator==(const ship& l, const ship& r) noexcept = default;

    // In C++20 != is now automatically defaulted if == is defined.
    // friend bool operator!=(const ship& l, const ship& r) noexcept = default;
#endif  // COMPARE_3WAY
};

std::ostream& operator<<(std::ostream& out, const ship& obj)
{
    out << "USS " << obj.name << " (" << obj.registry << ")";
    return out;
}

// TODO: Try concepts
void print_ships(std::ostream& out, const std::set<ship>& ships)
{
    for (const auto& ship : ships) {
        out << ship << '\n';
    }
    out << '\n';
}

[[maybe_unused]] const char* to_string(std::strong_ordering o) noexcept
{
    if (o == std::strong_ordering::less) {
        return "std::strong_ordering::less";
    }
    else if (o == std::strong_ordering::equivalent) {
        return "std::strong_ordering::equivalent";
    }
    else if (o == std::strong_ordering::equal) {
        return "std::strong_ordering::equal";
    }
    else {
        return "std::strong_ordering::greater";
    }
}

[[maybe_unused]] const char* to_string(std::weak_ordering o) noexcept
{
    if (o == std::weak_ordering::less) {
        return "std::weak_ordering::less";
    }
    else if (o == std::weak_ordering::equivalent) {
        return "std::weak_ordering::equivalent";
    }
    else {
        return "std::weak_ordering::greater";
    }
}

[[maybe_unused]] const char* to_string(std::partial_ordering o) noexcept
{
    if (o == std::partial_ordering::less) {
        return "std::partial_ordering::less";
    }
    else if (o == std::partial_ordering::equivalent) {
        return "std::partial_ordering::equivalent";
    }
    else if (o == std::partial_ordering::greater) {
        return "std::partial_ordering::greater";
    }
    else {
        return "std::partial_ordering::unordered";
    }
}

}  // namespace

int main()
{
    std::set<ship> ships;

    // clang-format off
    ships.emplace(ship{"Defiant",    "NX-74205",   ship_class::defiant,      2370 });
    ships.emplace(ship{"Enterprise", "NCC-1701",   ship_class::constitution, 2245 });
    ships.emplace(ship{"Excelsior",  "NCC-2000",   ship_class::excelsior,    2285 });
    ships.emplace(ship{"Phoenix",    "NCC-65420",  ship_class::nebula,       2363 });
    ships.emplace(ship{"Reliant",    "NCC-1864",   ship_class::miranda,      2267 });
    ships.emplace(ship{"Sutherland", "NCC-72015",  ship_class::nebula,       2367 });
    ships.emplace(ship{"Voyager",    "NCC-74656",  ship_class::intrepid,     2371 });
    // clang-format on
    std::cout << "Here are some ships:\n";
    print_ships(std::cout, ships);

    // clang-format off
    std::set<ship> runabouts{
        {"Ganges",         "NCC-72454", ship_class::danube, 2369},
        {"Orinoco",        "NCC-72905", ship_class::danube, 2370},
        {"Rio Grande",     "NCC-72452", ship_class::danube, 2369},
        {"Rubicon",        "NCC-72936", ship_class::danube, 2371},
        {"Shenandoah",     "NCC-73024", ship_class::danube, 2373},
        {"Yangtzee Kiang", "NCC-72453", ship_class::danube, 2369},
    };
    // clang-format on
    std::cout << "Deep Space Nine runabouts:\n";
    print_ships(std::cout, runabouts);

    std::cout << "Combine them all together:\n";
    ships.merge(runabouts);
    print_ships(std::cout, ships);

    // Sometimes ships get blown up and have to be replaced.
    // clang-format off
    std::set<ship> replacements{
        {"Defiant",    "NX-74205-A", ship_class::defiant,      2375},
        {"Enterprise", "NCC-1701-D", ship_class::galaxy,       2363},
    };
    // clang-format on

    for (auto iter{replacements.begin()}; iter != replacements.end(); iter++) {
        auto original_ship_iter =
            std::find_if(ships.begin(), ships.end(),
                         [iter](auto& s) { return s.name == iter->name; });
        if (original_ship_iter != ships.end()) {
            ships.emplace_hint(original_ship_iter, *iter);
            ships.erase(original_ship_iter);
        }
    }

    std::cout << "Updated list with replacements:\n";
    print_ships(std::cout, ships);

    ship defiant{"Defiant", "NX-74205-A", ship_class::defiant, 2375};
    ship enterprise{"Enterprise", "NCC-1701-D", ship_class::galaxy, 2363};
    std::cout << defiant << " <=> " << enterprise << ": "
              << to_string(defiant <=> enterprise) << '\n';

    std::cout << "1.0f <=> 2.0f: " << to_string(1.0f <=> 2.0f) << '\n';
    std::cout << "1.0f <=> 1.0f: " << to_string(1.0f <=> 1.0f) << '\n';
    std::cout << "1.0f/0.0f <=> 1.0f: " << to_string(1.0f / 0.0f <=> 1.0f)
              << '\n';
    std::cout << "std::nanf(\"1\") <=> std::nanf(\"2\"): "
              << to_string(std::nanf("1") <=> std::nanf("2")) << '\n';
    std::cout << "std::weak_order(std::nanf(\"1\"), std::nanf(\"2\")): "
              << to_string(std::weak_order(std::nanf("1"), std::nanf("2")))
              << '\n';

    std::array<float, 6> floats = {
        2.0, std::nanf("1"), 1.0f / 0.0f, std::nanf("2"), std::nanf("1"), 1.0};
    std::sort(floats.begin(), floats.end());
    for (float f : floats) {
        std::cout << f << ',';
    }
    std::cout << '\n';
}
