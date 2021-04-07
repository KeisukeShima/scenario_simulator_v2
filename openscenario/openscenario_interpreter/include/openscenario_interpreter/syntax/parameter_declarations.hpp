// Copyright 2015-2020 Tier IV, Inc. All rights reserved.
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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__PARAMETER_DECLARATIONS_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__PARAMETER_DECLARATIONS_HPP_

#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/syntax/parameter_declaration.hpp>
#include <vector>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- ParameterDeclarations --------------------------------------------------
 *
 *  <xsd:complexType name="ParameterDeclarations">
 *    <xsd:sequence>
 *      <xsd:element name="ParameterDeclaration" minOccurs="0" maxOccurs="unbounded" type="ParameterDeclaration"/>
 *    </xsd:sequence>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct ParameterDeclarations : public std::vector<ParameterDeclaration>
{
  ParameterDeclarations() = default;

  template <typename Node, typename Scope>
  explicit ParameterDeclarations(const Node & node, Scope & outer_scope)
  {
    callWithElements(node, "ParameterDeclaration", 0, unbounded, [&](auto && each) {
      return emplace_back(each, outer_scope);
    });
  }
};

std::ostream & operator<<(std::ostream & os, const ParameterDeclarations &)
{
  return os << (indent++) << blue << "<ParameterDeclarations>" << reset << "\n"
            << (--indent) << blue << "</ParameterDeclarations>";
}
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__PARAMETER_DECLARATIONS_HPP_
