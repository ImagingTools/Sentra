#pragma once

#include "INodeAction.h"
#include "IScenarioGraph.h"
#include <memory>
#include <unordered_map>
#include <functional>

namespace imtsentra {

/**
 * @brief Factory for creating INodeAction instances by NodeType
 */
class CNodeActionFactory {
public:
    using ActionCreator = std::function<std::unique_ptr<INodeAction>()>;

    CNodeActionFactory();
    ~CNodeActionFactory();

    /**
     * @brief Create an action for the given node type
     */
    std::unique_ptr<INodeAction> createAction(NodeType type) const;

    /**
     * @brief Register a custom action creator for a node type
     */
    void registerAction(NodeType type, ActionCreator creator);

    /**
     * @brief Check if an action is registered for the given type
     */
    bool hasAction(NodeType type) const;

private:
    std::unordered_map<int, ActionCreator> m_creators;

    void registerDefaultActions();
};

} // namespace imtsentra
