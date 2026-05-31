// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <sentra/CBaselineManagerComp.h>

// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QMutexLocker>

namespace sentra
{

std::optional<BaselineEntry> CBaselineManagerComp::GetBaseline(
    const QString& scenarioId,
    const QString& nodeId
) const {
    QMutexLocker lock(&m_mutex);
    auto key = MakeKey(scenarioId, nodeId);
    auto it = m_baselines.find(key);
    if (it != m_baselines.end() && !it.value().isEmpty()) {
        return it.value().last();  // Latest version
    }
    return std::nullopt;
}

BaselineEntry CBaselineManagerComp::SetBaseline(
    const QString& scenarioId,
    const QString& nodeId,
    const QString& screenshotPath
) {
    QMutexLocker lock(&m_mutex);
    auto key = MakeKey(scenarioId, nodeId);

    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());

    BaselineEntry entry;
    entry.id = QStringLiteral("bl-") + timestamp;
    entry.scenarioId = scenarioId;
    entry.nodeId = nodeId;
    entry.screenshotPath = screenshotPath;
    entry.createdAt = timestamp;
    entry.updatedAt = timestamp;

    auto& versions = m_baselines[key];
    entry.version = static_cast<int>(versions.size()) + 1;
    versions.push_back(entry);

    return entry;
}

void CBaselineManagerComp::ApplyDecision(
    const QString& baselineId,
    BaselineDecision decision,
    const QString& decidedBy
) {
    QMutexLocker lock(&m_mutex);
    // TODO: Record the decision and update baseline if accepted
}

QList<BaselineEntry> CBaselineManagerComp::GetBaselines(const QString& scenarioId) const {
    QMutexLocker lock(&m_mutex);
    QList<BaselineEntry> result;
    for (const auto& versions : m_baselines) {
        if (!versions.isEmpty() && versions.last().scenarioId == scenarioId) {
            result.push_back(versions.last());
        }
    }
    return result;
}

QList<BaselineEntry> CBaselineManagerComp::GetHistory(
    const QString& scenarioId,
    const QString& nodeId
) const {
    QMutexLocker lock(&m_mutex);
    auto key = MakeKey(scenarioId, nodeId);
    auto it = m_baselines.find(key);
    if (it != m_baselines.end()) {
        return it.value();
    }
    return {};
}

BaselineStrategy CBaselineManagerComp::GetStrategy() const {
    return m_strategy;
}

void CBaselineManagerComp::SetStrategy(BaselineStrategy strategy) {
    m_strategy = strategy;
}

QString CBaselineManagerComp::MakeKey(const QString& scenarioId, const QString& nodeId) const {
    return scenarioId + QLatin1Char('/') + nodeId;
}

} // namespace sentra
