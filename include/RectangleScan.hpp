//
// Created by mmath on 5/28/17.
//

#ifndef PYSCAN_RECTANGLESCAN_HPP
#define PYSCAN_RECTANGLESCAN_HPP
#include <memory>
#include <vector>
#include <forward_list>
#include <algorithm>
#include <cmath>
#include <random>
#include <deque>
#include <functional>
#include <tuple>
#include <limits>
#include <iostream>
#include "BloomFilter.hpp"

#include "Range.hpp"
#include "Statistics.hpp"
#include "Point.hpp"
#include "Utilities.hpp"

//#define DEBUG

namespace pyscan {
    using point_it= std::vector<Point<> >::iterator;
    using subgrid = std::tuple<int, int, int, int, double>;


    class Subgrid {
        size_t u_x, u_y, l_x, l_y;
        double value;
    public:
        Subgrid(size_t ux, size_t uy, size_t lx, size_t ly, double val) :
                u_x(ux),
                u_y(uy),
                l_x(lx),
                l_y(ly),
                value(val) {}


        std::string toString() const {
           std::stringstream ss;
           ss << "(" << u_x << " " << u_y << " " << l_x << " " << l_y << " " << value << ")";
           return ss.str();
        }

        void print(std::ostream& os) const {
           os <<  "(" << u_x << " " << u_y << " " << l_x << " " << l_y << " " << value << ")";
        }

        bool contains(Point<> const& pt) const {
            return u_x >= pt(0) && pt(0) >= l_x && u_y >= pt(1) && pt(1) >= l_y;
        }

        void setValue(double v) {
          value = v;
        }
        size_t lowX() const { return l_x; }
        size_t upX() const { return u_x; }
        size_t lowY() const { return l_y; }
        size_t upY() const { return u_y; }
        double fValue() const { return value; }
    };


    class Rectangle : public Range<2> {
        double u_x, u_y, l_x, l_y;
    public:
        Rectangle(const pt2_t& p1, const pt2_t& p2, const pt2_t& p3, const pt2_t& p4) {
            u_x = std::max({p1(0), p2(0), p3(0), p4(0)});
            l_x = std::min({p1(0), p2(0), p3(0), p4(0)});
            u_y = std::max({p1(1), p2(1), p3(1), p4(1)});
            l_y = std::min({p1(1), p2(1), p3(1), p4(1)});
        }

        Rectangle() : u_x(0.0), u_y(0.0), l_x(0.0), l_y(0.0) {}

        Rectangle(double ux, double uy, double lx, double ly) : u_x(ux), u_y(uy), l_x(lx), l_y(ly) {}

        inline bool contains(const pt2_t& p1) const final {
            return (u_x > p1(0) && p1(0) >= l_x && u_y > p1(1) && p1(1) >= l_y);
        }

        inline bool intersects_segment(const pt2_t &p1, const pt2_t &p2) const final {
            //Check if any of points are inside of the rectangle.
            if (contains(p1) || contains(p2)) {
                return true;
            } else {
                //Check if any of the segments cross the line segment defined by p1<->p2.
                auto ur_pt = pt2_t(u_x, u_y, 1.0);
                auto ul_pt = pt2_t(l_x, u_y, 1.0);
                auto lr_pt = pt2_t(u_x, l_y, 1.0);
                auto ll_pt = pt2_t(l_x, l_y, 1.0);
                return crosses_segment(ur_pt, ul_pt, p1, p2) || crosses_segment(ul_pt, ll_pt, p1, p2) ||
                        crosses_segment(ur_pt, lr_pt, p1, p2) || crosses_segment(lr_pt, ll_pt, p1, p2);
            }
        }

        std::string toString() const {
            std::stringstream ss;
            ss << "Rectangle(" << u_x << ", " << u_y << ", " << l_x << ", " << l_y << ")";
            return ss.str();
        }
        double lowX() const { return l_x; }
        double upX() const { return u_x; }
        double lowY() const { return l_y; }
        double upY() const { return u_y; }
    };

    enum class I_Type {
        VALUE,
        MAX
    };

    class Grid {
        /*
         * Grid as defined in the SODA paper. Construction takes O(mlog r + r^2) time where
         * m = end - begin.
         */
        long r;
        std::vector<double> red_counts;
        std::vector<double> blue_counts;
        std::vector<double> x_coords;
        std::vector<double> y_coords;
        double total_red_weight = 0;
        double total_blue_weight = 0;
    public:
        Grid(size_t r_arg, wpoint_list_t const& red, wpoint_list_t const& blue);
        Grid(wpoint_list_t const& red_points, wpoint_list_t const& blue_points);
        Grid(point_list_t const& net, wpoint_list_t const& red, wpoint_list_t const& blue);
        double totalRedWeight() const;
        double totalBlueWeight() const;
        double redCount(size_t row, size_t col) const;
        double blueCount(size_t row, size_t col) const;
        double redWeight(size_t row, size_t col) const;
        double blueWeight(size_t row, size_t col) const;
        double redSubWeight(Subgrid const& sg) const;
        double blueSubWeight(Subgrid const& sg) const;

        double yCoord(size_t row) const;
        double xCoord(size_t col) const;

        size_t size() const;
        Rectangle toRectangle(Subgrid const &sg) const;
    };

    inline Grid make_exact_grid(const wpoint_list_t& m_pts, const wpoint_list_t& b_pts) {
        return Grid(m_pts, b_pts);
    }

    inline Grid make_net_grid(const point_list_t& pts, const wpoint_list_t& m_pts, const wpoint_list_t& b_pts) {
        return Grid(pts, m_pts, b_pts);
    }


    std::tuple<Rectangle, double> max_rect_labeled(size_t r, double max_w, lpoint_list_t const& m_points, lpoint_list_t const& b_points, const discrepancy_func_t& func);

    std::tuple<Rectangle, double> max_rect_labeled_scale(
            size_t r,
            double max_r,
            double alpha,
            const point_list_t &net,
            const lpoint_list_t &red,
            const lpoint_list_t &blue,
            const discrepancy_func_t &f);

    Subgrid max_subgrid_convex(Grid const &grid, double eps, discrepancy_func_t const &f);
    Subgrid max_subgrid_linear(Grid const &grid, double a, double b);
    Subgrid max_subgrid(Grid const &grid, discrepancy_func_t const &func);

    std::tuple<Rectangle, double> max_rectangle(const wpoint_list_t& m_points, const wpoint_list_t& b_points, double eps, double a, double b);

}
#endif //PYSCAN_RECTANGLESCAN_HPP
