// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>
#include <istd/IPolymorphic.h>

// Standard includes
#include <string>
#include <vector>
#include <optional>

namespace imtsentragql
{

/**
 * \brief GraphQL service controller for Scenario CRUD operations
 *
 * Handles:
 * - listScenarios / GetScenario(id)
 * - createScenario / updateScenario / deleteScenario
 * - duplicateScenario
 * - exportScenario / importScenario
 *
 * \ingroup imtsentragql
 */
class CScenarioServiceControllerComp:
        public icomp::CComponentBase,
        virtual public istd::IPolymorphic
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CScenarioServiceControllerComp)
        I_REGISTER_INTERFACE(istd::IPolymorphic);
    I_END_COMPONENT

    // Query resolvers
    std::string ListScenarios(const std::string& projectId) const;
    std::string GetScenario(const std::string& scenarioId) const;

    // Mutation resolvers
    std::string CreateScenario(const std::string& input);
    std::string UpdateScenario(const std::string& scenarioId, const std::string& input);
    bool DeleteScenario(const std::string& scenarioId);
    std::string DuplicateScenario(const std::string& scenarioId);

    // Import/Export
    std::string ExportScenario(const std::string& scenarioId, const std::string& format) const;
    std::string ImportScenario(const std::string& data, const std::string& format);
};

} // namespace imtsentragql
