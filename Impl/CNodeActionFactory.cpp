#include "imtsentra/CNodeActionFactory.h"

namespace imtsentra {

CNodeActionFactory::CNodeActionFactory() {
    registerDefaultActions();
}

CNodeActionFactory::~CNodeActionFactory() = default;

std::unique_ptr<INodeAction> CNodeActionFactory::createAction(NodeType type) const {
    auto it = m_creators.find(static_cast<int>(type));
    if (it != m_creators.end()) {
        return it->second();
    }
    return nullptr;
}

void CNodeActionFactory::registerAction(NodeType type, ActionCreator creator) {
    m_creators[static_cast<int>(type)] = std::move(creator);
}

bool CNodeActionFactory::hasAction(NodeType type) const {
    return m_creators.count(static_cast<int>(type)) > 0;
}

void CNodeActionFactory::registerDefaultActions() {
    // TODO: Register default action implementations for each NodeType
    // These will be implemented as separate classes:
    // - COpenUrlAction
    // - CClickElementAction
    // - CInputTextAction
    // - CWaitAction
    // - CTakeScreenshotAction
    // - CCompareScreenshotAction
    // - CAssertTextAction
    // - CAssertElementAction
    // - CAiValidationAction
    // - CConditionalBranchAction
    // - CLoopAction
}

} // namespace imtsentra
