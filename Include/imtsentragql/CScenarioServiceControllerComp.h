// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>
#include <istd/IPolymorphic.h>

// Qt includes
#include <QtCore/QString>

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
    QString ListScenarios(const QString& projectId) const;
    QString GetScenario(const QString& scenarioId) const;

    // Mutation resolvers
    QString CreateScenario(const QString& input);
    QString UpdateScenario(const QString& scenarioId, const QString& input);
    bool DeleteScenario(const QString& scenarioId);
    QString DuplicateScenario(const QString& scenarioId);

    // Import/Export
    QString ExportScenario(const QString& scenarioId, const QString& format) const;
    QString ImportScenario(const QString& data, const QString& format);
};

} // namespace imtsentragql
