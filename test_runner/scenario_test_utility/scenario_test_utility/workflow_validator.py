#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright 2020 Tier IV, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import os
import yamale

from ament_index_python.packages import get_package_share_directory


class WorkflowValidator():
    """Workflow validation class."""

    def __init__(self):
        share_directory_path = os.path.join(get_package_share_directory('scenario_test_utility'))
        relative_path = '/../ament_index/resource_index/packages/workflow_schema.yaml'
        schema_path = share_directory_path + relative_path
        self.workflow_schema = yamale.make_schema(schema_path)

    def validate_workflow_file(self, workflow):
        data = yamale.make_data(workflow)
        yamale.validate(self.workflow_schema, data)


def main():
    """Run Validation."""
    parser = argparse.ArgumentParser(description='Validator for workflow .yaml file')
    parser.add_argument('workflow', help='path to workflow .yaml file')
    args = parser.parse_args()
    validator = WorkflowValidator()
    validator.validate_workflow_file(args.workflow)


if __name__ == '__main__':
    """Entrypoint."""
    main()
