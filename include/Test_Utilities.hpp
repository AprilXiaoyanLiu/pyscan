//
// Created by mmath on 10/2/17.
//

#ifndef PYSCAN_TEST_UTILITIES_HPP
#define PYSCAN_TEST_UTILITIES_HPP

#include <functional>

#include "Point.hpp"


namespace pyscantest {

	using Vec2 = std::array<double, 2>;


    auto randomVec(int test_size) -> std::vector<Vec2>;

    Vec2 maxVec2(std::vector<Vec2> const& vec, 
    	std::function<double(Vec2)> const& f);

	auto randomPoints2(size_t test_size) -> std::vector<pyscan::Point<>>;

	auto randomWPoints2(size_t test_size) -> std::vector<pyscan::WPoint<>>;

	auto randomLPoints2(size_t test_size, size_t label_count) -> std::vector<pyscan::LPoint<>>;

	auto randomLPointsUnique2(size_t test_size) -> std::vector<pyscan::LPoint<>>;

    auto randomPoints3(size_t test_size) -> std::vector<pyscan::Point<3>>;

    auto randomWPoints3(size_t test_size) -> std::vector<pyscan::WPoint<3>>;

    auto randomLPoints3(size_t test_size, size_t label_count) -> std::vector<pyscan::LPoint<3>>;

    //auto randomLPointsUnique3(size_t test_size) -> std::vector<pyscan::LPoint<>>;

    auto removeLW(pyscan::lpoint_list_t const& pts) -> pyscan::point_list_t;

	auto removeLabels(pyscan::lpoint_list_t const& pts) -> pyscan::wpoint_list_t;

}
#endif //PYSCAN_UTILITIES_HPP
