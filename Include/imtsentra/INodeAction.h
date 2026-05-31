// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <string>
#include <memory>
#include <optional>

namespace imtsentra
{

/**
 * \brief Context provided to node actions for execution
 */
struct ActionContext {
    std::string currentUrl;
    std::string domSnapshot;         // JSON DOM tree
    std::string accessibilityTree;   // JSON accessibility tree
    std::string screenshotPath;      // Path to current screenshot
    std::string executionId;
    std::string nodeId;
};

/**
 * \brief Result of a node action execution
 */
struct ActionResult {
    bool success;
    std::optional<std::string> screenshotPath;
    std::optional<std::string> errorMessage;
    std::optional<std::string> resolvedSelector;
    int durationMs = 0;
};

/**
 * \brief Interface for individual node actions
 *
 * Each node type has a corresponding INodeAction implementation
 * that knows how to execute the intent described in the node config.
 */
class INodeAction {
public:
    virtual ~INodeAction() = default;

    /**
     * \brief Execute the action with given context
     * \param config JSON configuration from ScenarioNode::config
     * \param context Current execution context
     * \return Action result
     */
    virtual ActionResult Execute(
        const std::string& config,
        const ActionContext& context
    ) = 0;

    /**
     * \brief Validate that the config is well-formed for this action type
     */
    virtual bool ValidateConfig(const std::string& config) const = 0;

    /**
     * \brief Human-readable description of what this action does
     */
    virtual std::string Describe(const std::string& config) const = 0;
};

} // namespace imtsentra
