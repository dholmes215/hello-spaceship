// Copyright (C) 2021 David Holmes <dholmes@dholmes.us>. All rights reserved.
#include <algorithm>
#include <compare>
#include <iostream>
#include <set>
#include <string>
#include <tuple>

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
    using year = int;
    using ship_name = std::string;
    using ship_registry = std::string;

    ship_name name;
    ship_registry registry;
    ship_class model{};
    year first_seen{};

    auto operator<=>(const ship& other) const noexcept = default;

    // bool operator<(const ship& other) const noexcept
    // {
    //     return std::tie(name, registry, model, first_seen) <
    //            std::tie(other.name, other.registry, other.model,
    //                     other.first_seen);
    // }
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
}
