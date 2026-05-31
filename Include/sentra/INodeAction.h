// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Qt includes
#include <QtCore/QString>

// Standard includes
#include <memory>
#include <optional>

namespace sentra
{

/**
 * \brief Context provided to node actions for execution
 */
struct ActionContext {
    QString currentUrl;
    QString domSnapshot;         // JSON DOM tree
    QString accessibilityTree;   // JSON accessibility tree
    QString screenshotPath;      // Path to current screenshot
    QString executionId;
    QString nodeId;
};

/**
 * \brief Result of a node action execution
 */
struct ActionResult {
    bool success;
    std::optional<QString> screenshotPath;
    std::optional<QString> errorMessage;
    std::optional<QString> resolvedSelector;
    int durationMs = 0;
};

/**
 * \brief Interface for individual node actions
 *
 * Each node type has a corresponding INodeAction implementation
 * that knows how to execute the intent described in the node config.
 *
 * \ingroup sentra
 */
class INodeAction: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Execute the action with given context
     * \param config JSON configuration from ScenarioNode::config
     * \param context Current execution context
     * \return Action result
     */
    virtual ActionResult Execute(
        const QString& config,
        const ActionContext& context
    ) = 0;

    /**
     * \brief Validate that the config is well-formed for this action type
     */
    virtual bool ValidateConfig(const QString& config) const = 0;

    /**
     * \brief Human-readable description of what this action does
     */
    virtual QString Describe(const QString& config) const = 0;
};

typedef istd::TUniqueInterfacePtr<INodeAction> INodeActionUniquePtr;

} // namespace sentra
