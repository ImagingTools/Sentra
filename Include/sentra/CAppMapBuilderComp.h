// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// Sentra includes
#include <sentra/IAppMapBuilder.h>

// Qt includes
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QString>

namespace sentra
{

/**
 * \brief ACF component implementing IAppMapBuilder
 *
 * Builds application map by tracking navigation during test executions.
 * Detects new screens based on URL changes and major DOM differences.
 *
 * \ingroup sentra
 */
class CAppMapBuilderComp:
        public icomp::CComponentBase,
        virtual public IAppMapBuilder
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CAppMapBuilderComp)
        I_REGISTER_INTERFACE(IAppMapBuilder);
    I_END_COMPONENT

    // reimplemented (sentra::IAppMapBuilder)
    void RecordScreen(
        const QString& url,
        const QString& title,
        const std::optional<QString>& screenshotPath = std::nullopt
    ) override;

    void RecordTransition(
        const QString& fromUrl,
        const QString& toUrl,
        const QString& action
    ) override;

    QList<AppScreen> GetScreens() const override;
    QList<AppTransition> GetTransitions() const override;
    QList<AppScreen> GetUntestedScreens() const override;
    float GetCoveragePercentage() const override;
    QString ToJson() const override;

private:
    mutable QMutex m_mutex;
    QHash<QString, AppScreen> m_screens;  // keyed by URL
    QList<AppTransition> m_transitions;

    QString GenerateId() const;
    QString NormalizeUrl(const QString& url) const;
};

} // namespace sentra
