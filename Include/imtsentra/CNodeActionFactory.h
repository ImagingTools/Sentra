// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/INodeAction.h>
#include <imtsentra/IScenarioGraph.h>

#include <QtCore/QHash>

#include <memory>
#include <functional>

namespace imtsentra
{

/**
 * \brief Factory for creating INodeAction instances by NodeType
 */
class CNodeActionFactory {
public:
    using ActionCreator = std::function<std::unique_ptr<INodeAction>()>;

    CNodeActionFactory();
    ~CNodeActionFactory();

    /**
     * \brief Create an action for the given node type
     */
    std::unique_ptr<INodeAction> CreateAction(NodeType type) const;

    /**
     * \brief Register a custom action creator for a node type
     */
    void RegisterAction(NodeType type, ActionCreator creator);

    /**
     * \brief Check if an action is registered for the given type
     */
    bool HasAction(NodeType type) const;

private:
    QHash<int, ActionCreator> m_creators;

    void RegisterDefaultActions();
};

} // namespace imtsentra
