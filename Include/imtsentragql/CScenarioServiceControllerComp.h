#pragma once

#include <string>
#include <vector>
#include <optional>

namespace imtsentra {
namespace gql {

/**
 * @brief GraphQL service controller for Scenario CRUD operations
 *
 * Handles:
 * - listScenarios / getScenario(id)
 * - createScenario / updateScenario / deleteScenario
 * - duplicateScenario
 * - exportScenario / importScenario
 */
class CScenarioServiceControllerComp {
public:
    CScenarioServiceControllerComp();
    ~CScenarioServiceControllerComp();

    // Query resolvers
    std::string listScenarios(const std::string& projectId) const;
    std::string getScenario(const std::string& scenarioId) const;

    // Mutation resolvers
    std::string createScenario(const std::string& input);
    std::string updateScenario(const std::string& scenarioId, const std::string& input);
    bool deleteScenario(const std::string& scenarioId);
    std::string duplicateScenario(const std::string& scenarioId);

    // Import/Export
    std::string exportScenario(const std::string& scenarioId, const std::string& format) const;
    std::string importScenario(const std::string& data, const std::string& format);
};

} // namespace gql
} // namespace imtsentra
