// Copyright 2015 TIER IV, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SIMPLE_SENSOR_SIMULATOR__SENSOR_SIMULATION__OCCUPANCY_GRID__GRID_HPP_
#define SIMPLE_SENSOR_SIMULATOR__SENSOR_SIMULATION__OCCUPANCY_GRID__GRID_HPP_

#include <boost/optional.hpp>
#include <geometry/polygon/line_segment.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <simple_sensor_simulator/sensor_simulation/primitives/box.hpp>
#include <vector>

namespace simple_sensor_simulator
{
/**
 * @brief grid
 */
class Grid
{
public:
  Grid(
    double resolution, size_t height, size_t width, int8_t occupied_cost = 100,
    int8_t invisible_cost = 50);

  const double resolution;
  const size_t height;
  const size_t width;
  const int8_t occupied_cost;
  const int8_t invisible_cost;

  /**
   * @brief mark invisible area and occpuied area of primitive
   */
  auto add(const primitives::Primitive & primitive) -> void;

  auto reset(const geometry_msgs::msg::Pose & origin) -> void;

  auto construct() -> void;

  auto get() const -> const std::vector<int8_t> &;

private:
  /**
   * @brief origin
   * @note Grid treats Ego's origin as its origin
   */
  geometry_msgs::msg::Pose origin_;

  /**
   * @brief a vector that express cell is invisible
   * @note Grid access this 1d vector by calculating an index from a 2d grid coordinate
   */
  std::vector<int8_t> invisible_grid_;

  /**
   * @brief a vector that express cell is occupied
   * @note Grid access this 1d vector by calculating an index from a 2d grid coordinate
   */
  std::vector<int8_t> occupied_grid_;

  std::vector<ssize_t> mincols_, maxcols_;

  /**
   * @brief traverse cells along from start to end
   * @param start
   * @param end
   * @param f a funciton object which takes cell coordinate
   */
  template <class F>
  inline auto traverse(
    const geometry_msgs::msg::Point & start, const geometry_msgs::msg::Point & end, const F & f)
    -> void
  {
    // A Fast Voxel Traversal Algorithm for Ray Tracing
    // https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.42.3443&rep=rep1&type=pdf

    double vx = end.x - start.x;
    double vy = end.y - start.y;

    ssize_t step_x = ssize_t(std::copysign(1.0, vx));
    ssize_t step_y = ssize_t(std::copysign(1.0, vy));

    double tdx = step_x / vx;
    double tdy = step_y / vy;

    double tx = vx > 0 ? std::ceil(start.x) : std::floor(start.x);
    double ty = vy > 0 ? std::ceil(start.y) : std::floor(start.y);

    tx = vx != 0 ? (tx - start.x) / vx : tdx;
    ty = vy != 0 ? (ty - start.y) / vy : tdy;

    ssize_t x = ssize_t(start.x);
    ssize_t y = ssize_t(start.y);

    while (tx <= 1.0 || ty <= 1.0) {
      f(x, y);
      if (tx < ty) {
        tx += tdx;
        x += step_x;
      } else {
        ty += tdy;
        y += step_y;
      }
    }
  }

  /**
   * @brief mark grid area surrounded by convex_hull
   */
  inline auto markConvexHull(
    std::vector<int8_t> & grid, const std::vector<geometry_msgs::msg::Point> & convex_hull) -> void;

  /**
   * @brief Convert point in world coordinate to point in grid cooridnate
   * @return Point in grid coordinate
   */
  inline auto transformToGrid(const geometry_msgs::msg::Point & world_point) const
    -> geometry_msgs::msg::Point;

  /**
   * @brief Digitize point in grid coordinate
   * @return Digitized point
   */
  inline auto transformToPixel(const geometry_msgs::msg::Point & grid_point) const
    -> geometry_msgs::msg::Point;

  inline auto constructPoint(double x, double y, double z) const -> geometry_msgs::msg::Point;

  inline auto minmaxAnglePoint(const std::vector<geometry_msgs::msg::Point> & polygon) const;

  /**
   *
   */
  inline auto constructOccupiedConvexHull(const primitives::Primitive & primitive) const
    -> std::vector<geometry_msgs::msg::Point>;

  inline auto constructInvisibleConvexHull(
    const std::vector<geometry_msgs::msg::Point> & occupied_convex_hull) const
    -> std::vector<geometry_msgs::msg::Point>;
};
}  // namespace simple_sensor_simulator

#endif  // SIMPLE_SENSOR_SIMULATOR__SENSOR_SIMULATION__OCCUPANCY_GRID__GRID_HPP_
