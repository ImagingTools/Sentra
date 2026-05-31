// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CNodeActionFactory.h>

namespace imtsentra
{

CNodeActionFactory::CNodeActionFactory() {
    RegisterDefaultActions();
}

CNodeActionFactory::~CNodeActionFactory() = default;

std::unique_ptr<INodeAction> CNodeActionFactory::CreateAction(NodeType type) const {
    auto it = m_creators.find(static_cast<int>(type));
    if (it != m_creators.end()) {
        return it->second();
    }
    return nullptr;
}

void CNodeActionFactory::RegisterAction(NodeType type, ActionCreator creator) {
    m_creators[static_cast<int>(type)] = std::move(creator);
}

bool CNodeActionFactory::HasAction(NodeType type) const {
    return m_creators.count(static_cast<int>(type)) > 0;
}

void CNodeActionFactory::RegisterDefaultActions() {
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
