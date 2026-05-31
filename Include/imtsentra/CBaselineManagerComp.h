// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IBaselineManager.h>

// Qt includes
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QString>

namespace imtsentra
{

/**
 * \brief ACF component implementing IBaselineManager
 *
 * Manages baseline images with versioning, approval workflows,
 * and configurable update strategies.
 *
 * \ingroup imtsentra
 */
class CBaselineManagerComp:
        public icomp::CComponentBase,
        virtual public IBaselineManager
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CBaselineManagerComp)
        I_REGISTER_INTERFACE(IBaselineManager);
    I_END_COMPONENT

    // reimplemented (imtsentra::IBaselineManager)
    std::optional<BaselineEntry> GetBaseline(
        const QString& scenarioId,
        const QString& nodeId
    ) const override;

    BaselineEntry SetBaseline(
        const QString& scenarioId,
        const QString& nodeId,
        const QString& screenshotPath
    ) override;

    void ApplyDecision(
        const QString& baselineId,
        BaselineDecision decision,
        const QString& decidedBy
    ) override;

    QList<BaselineEntry> GetBaselines(const QString& scenarioId) const override;

    QList<BaselineEntry> GetHistory(
        const QString& scenarioId,
        const QString& nodeId
    ) const override;

    BaselineStrategy GetStrategy() const override;
    void SetStrategy(BaselineStrategy strategy) override;

private:
    mutable QMutex m_mutex;
    BaselineStrategy m_strategy = BS_MANUAL;
    // Key: "scenarioId/nodeId"
    QHash<QString, QList<BaselineEntry>> m_baselines;

    QString MakeKey(const QString& scenarioId, const QString& nodeId) const;
};

} // namespace imtsentra
