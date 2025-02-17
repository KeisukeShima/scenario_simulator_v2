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

#include <traffic_simulator/entity/misc_object_entity.hpp>

namespace traffic_simulator
{
namespace entity
{
MiscObjectEntity::MiscObjectEntity(
  const std::string & name, const traffic_simulator_msgs::msg::EntityStatus & entity_status,
  const traffic_simulator_msgs::msg::MiscObjectParameters &)
: EntityBase(name, entity_status)
{
}

void MiscObjectEntity::onUpdate(double, double)
{
  status_.action_status.accel = geometry_msgs::msg::Accel();
  status_.action_status.twist = geometry_msgs::msg::Twist();
  status_.action_status.current_action = "static";
  status_before_update_ = status_;
}

auto MiscObjectEntity::getCurrentAction() const -> std::string
{
  if (not npc_logic_started_) {
    return "waiting";
  } else {
    return status_.action_status.current_action;
  }
}

auto MiscObjectEntity::getBehaviorParameter() const
  -> traffic_simulator_msgs::msg::BehaviorParameter
{
  THROW_SEMANTIC_ERROR("getBehaviorParameter function does not support in MiscObjectEntity.");
}

void MiscObjectEntity::setBehaviorParameter(const traffic_simulator_msgs::msg::BehaviorParameter &)
{
  THROW_SEMANTIC_ERROR("setBehaviorParameter function does not support in MiscObjectEntity.");
}

auto MiscObjectEntity::getDefaultDynamicConstraints() const
  -> const traffic_simulator_msgs::msg::DynamicConstraints &
{
  static const auto default_dynamic_constraints = []() {
    auto dynamic_constraints = traffic_simulator_msgs::msg::DynamicConstraints();
    dynamic_constraints.max_speed = 0.0;
    dynamic_constraints.max_acceleration = 0.0;
    dynamic_constraints.max_acceleration_rate = 0.0;
    dynamic_constraints.max_deceleration = 0.0;
    dynamic_constraints.max_deceleration_rate = 0.0;
    return dynamic_constraints;
  }();

  return default_dynamic_constraints;
}

void MiscObjectEntity::requestSpeedChange(double, bool)
{
  THROW_SEMANTIC_ERROR("requestSpeedChange function cannot not use in MiscObjectEntity");
}

void MiscObjectEntity::requestSpeedChange(const speed_change::RelativeTargetSpeed &, bool)
{
  THROW_SEMANTIC_ERROR("requestSpeedChange function cannot not use in MiscObjectEntity");
}

void MiscObjectEntity::requestSpeedChange(
  const double, const speed_change::Transition, const speed_change::Constraint, const bool)
{
  THROW_SEMANTIC_ERROR("requestSpeedChange function cannot not use in MiscObjectEntity");
}
}  // namespace entity
}  // namespace traffic_simulator
