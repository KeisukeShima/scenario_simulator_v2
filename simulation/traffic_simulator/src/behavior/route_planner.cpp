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

#include <traffic_simulator/behavior/route_planner.hpp>

namespace traffic_simulator
{
RoutePlanner::RoutePlanner(const std::shared_ptr<hdmap_utils::HdMapUtils> & hdmap_utils_ptr)
: hdmap_utils_ptr_(hdmap_utils_ptr)
{
}

std::vector<std::int64_t> RoutePlanner::getRouteLanelets(
  const traffic_simulator_msgs::msg::LaneletPose & entity_lanelet_pose,
  const std::vector<traffic_simulator_msgs::msg::LaneletPose> & waypoints, double horizon)
{
  waypoint_queue_.clear();
  if (waypoints.empty()) {
    return getRouteLanelets(entity_lanelet_pose, horizon);
  }
  for (const auto & waypoint : waypoints) {
    waypoint_queue_.push_back(waypoint);
  }
  return getRouteLanelets(entity_lanelet_pose, waypoint_queue_.front(), horizon);
}

std::vector<std::int64_t> RoutePlanner::getRouteLanelets(
  const traffic_simulator_msgs::msg::LaneletPose & entity_lanelet_pose, double horizon)
{
  if (!whole_route_) {
    return hdmap_utils_ptr_->getFollowingLanelets(entity_lanelet_pose.lanelet_id, horizon, true);
  }
  if (!waypoint_queue_.empty()) {
    if (waypoint_queue_.front().lanelet_id == entity_lanelet_pose.lanelet_id) {
      cancelGoal(entity_lanelet_pose);
    }
  } else {
    if (hdmap_utils_ptr_->isInRoute(entity_lanelet_pose.lanelet_id, whole_route_.value())) {
      return hdmap_utils_ptr_->getFollowingLanelets(
        entity_lanelet_pose.lanelet_id, whole_route_.value(), horizon, true);
    }
  }
  cancelGoal(entity_lanelet_pose);
  if (waypoint_queue_.empty()) {
    return hdmap_utils_ptr_->getFollowingLanelets(entity_lanelet_pose.lanelet_id, horizon, true);
  }
  return getRouteLanelets(entity_lanelet_pose, waypoint_queue_.front(), horizon);
}

auto RoutePlanner::getRouteLanelets(
  const traffic_simulator_msgs::msg::LaneletPose & entity_lanelet_pose,
  const traffic_simulator_msgs::msg::LaneletPose & target_lanelet_pose, double horizon)
  -> std::vector<std::int64_t>
{
  plan(entity_lanelet_pose, target_lanelet_pose);

  if (not whole_route_ or whole_route_->empty()) {
    whole_route_ = std::nullopt;
    return hdmap_utils_ptr_->getFollowingLanelets(entity_lanelet_pose.lanelet_id, horizon, true);
  } else {
    return hdmap_utils_ptr_->getFollowingLanelets(
      entity_lanelet_pose.lanelet_id, whole_route_.value(), horizon, true);
  }
}

void RoutePlanner::cancelGoal() { whole_route_ = std::nullopt; }

void RoutePlanner::cancelGoal(const traffic_simulator_msgs::msg::LaneletPose & entity_lanelet_pose)
{
  while (true) {
    if (waypoint_queue_.empty()) {
      cancelGoal();
      break;
    }
    if (waypoint_queue_.front().lanelet_id == entity_lanelet_pose.lanelet_id) {
      waypoint_queue_.pop_front();
      continue;
    } else {
      break;
    }
  }
}

std::vector<geometry_msgs::msg::Pose> RoutePlanner::getGoalPosesInWorldFrame()
{
  std::vector<geometry_msgs::msg::Pose> ret;
  for (const auto & lanelet_pose : waypoint_queue_) {
    ret.emplace_back(hdmap_utils_ptr_->toMapPose(lanelet_pose).pose);
  }
  return ret;
}

std::vector<traffic_simulator_msgs::msg::LaneletPose> RoutePlanner::getGoalPoses()
{
  std::vector<traffic_simulator_msgs::msg::LaneletPose> goal_poses;
  std::copy(
    std::cbegin(waypoint_queue_), std::cend(waypoint_queue_), std::back_inserter(goal_poses));
  return goal_poses;
}

void RoutePlanner::plan(
  const traffic_simulator_msgs::msg::LaneletPose & entity_lanelet_pose,
  const traffic_simulator_msgs::msg::LaneletPose & target_lanelet_pose)
{
  if (
    target_lanelet_pose.lanelet_id == entity_lanelet_pose.lanelet_id &&
    target_lanelet_pose.s <= entity_lanelet_pose.s) {
    cancelGoal(entity_lanelet_pose);
    if (waypoint_queue_.empty()) {
      return;
    }
  }
  if (!whole_route_) {
    whole_route_ =
      hdmap_utils_ptr_->getRoute(entity_lanelet_pose.lanelet_id, target_lanelet_pose.lanelet_id);
    return;
  }
  if (hdmap_utils_ptr_->isInRoute(entity_lanelet_pose.lanelet_id, whole_route_.value())) {
    return;
  } else {
    whole_route_ =
      hdmap_utils_ptr_->getRoute(entity_lanelet_pose.lanelet_id, target_lanelet_pose.lanelet_id);
    return;
  }
}
}  // namespace traffic_simulator
