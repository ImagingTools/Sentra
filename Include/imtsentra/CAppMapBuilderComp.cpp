// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CAppMapBuilderComp.h>

// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QMutexLocker>

namespace imtsentra
{

void CAppMapBuilderComp::RecordScreen(
    const QString& url,
    const QString& title,
    const std::optional<QString>& screenshotPath
) {
    QMutexLocker lock(&m_mutex);
    auto normalizedUrl = NormalizeUrl(url);
    auto now = QString::number(QDateTime::currentMSecsSinceEpoch());

    auto it = m_screens.find(normalizedUrl);
    if (it != m_screens.end()) {
        it.value().lastSeenAt = now;
        it.value().visitCount++;
        if (screenshotPath) {
            it.value().screenshotPath = *screenshotPath;
        }
    } else {
        AppScreen screen;
        screen.id = GenerateId();
        screen.url = normalizedUrl;
        screen.title = title;
        screen.screenshotPath = screenshotPath;
        screen.discoveredAt = now;
        screen.lastSeenAt = now;
        screen.visitCount = 1;
        m_screens[normalizedUrl] = std::move(screen);
    }
}

void CAppMapBuilderComp::RecordTransition(
    const QString& fromUrl,
    const QString& toUrl,
    const QString& action
) {
    QMutexLocker lock(&m_mutex);
    auto normFrom = NormalizeUrl(fromUrl);
    auto normTo = NormalizeUrl(toUrl);

    // Check if transition already exists
    for (const auto& t : m_transitions) {
        if (t.sourceScreenId == normFrom && t.targetScreenId == normTo && t.action == action) {
            return;  // Already recorded
        }
    }

    auto now = QString::number(QDateTime::currentMSecsSinceEpoch());

    AppTransition transition;
    transition.id = GenerateId();
    transition.sourceScreenId = normFrom;
    transition.targetScreenId = normTo;
    transition.action = action;
    transition.discoveredAt = now;
    m_transitions.push_back(std::move(transition));
}

QList<AppScreen> CAppMapBuilderComp::GetScreens() const {
    QMutexLocker lock(&m_mutex);
    QList<AppScreen> screens;
    screens.reserve(m_screens.size());
    for (const auto& screen : m_screens) {
        screens.push_back(screen);
    }
    return screens;
}

QList<AppTransition> CAppMapBuilderComp::GetTransitions() const {
    QMutexLocker lock(&m_mutex);
    return m_transitions;
}

QList<AppScreen> CAppMapBuilderComp::GetUntestedScreens() const {
    QMutexLocker lock(&m_mutex);
    QList<AppScreen> untested;
    for (const auto& screen : m_screens) {
        if (screen.visitCount <= 1) {  // Only discovered, not tested
            untested.push_back(screen);
        }
    }
    return untested;
}

float CAppMapBuilderComp::GetCoveragePercentage() const {
    QMutexLocker lock(&m_mutex);
    if (m_screens.isEmpty()) return 0.0f;

    int tested = 0;
    for (const auto& screen : m_screens) {
        if (screen.visitCount > 1) tested++;
    }
    return static_cast<float>(tested) / static_cast<float>(m_screens.size()) * 100.0f;
}

QString CAppMapBuilderComp::ToJson() const {
    QMutexLocker lock(&m_mutex);
    QString json = QStringLiteral("{\"screens\":[");
    bool first = true;
    for (const auto& screen : m_screens) {
        if (!first) json += QLatin1Char(',');
        json += QStringLiteral("{\"id\":\"") + screen.id + QStringLiteral("\",")
              + QStringLiteral("\"url\":\"") + screen.url + QStringLiteral("\",")
              + QStringLiteral("\"title\":\"") + screen.title + QStringLiteral("\",")
              + QStringLiteral("\"visitCount\":") + QString::number(screen.visitCount)
              + QStringLiteral("}");
        first = false;
    }
    json += QStringLiteral("],\"transitions\":[");
    first = true;
    for (const auto& t : m_transitions) {
        if (!first) json += QLatin1Char(',');
        json += QStringLiteral("{\"id\":\"") + t.id + QStringLiteral("\",")
              + QStringLiteral("\"from\":\"") + t.sourceScreenId + QStringLiteral("\",")
              + QStringLiteral("\"to\":\"") + t.targetScreenId + QStringLiteral("\",")
              + QStringLiteral("\"action\":\"") + t.action + QStringLiteral("\"}");
        first = false;
    }
    json += QStringLiteral("]}");
    return json;
}

QString CAppMapBuilderComp::GenerateId() const {
    auto ms = QDateTime::currentDateTime().toMSecsSinceEpoch();
    return QStringLiteral("map-") + QString::number(ms);
}

QString CAppMapBuilderComp::NormalizeUrl(const QString& url) const {
    // Remove trailing slash and query params for grouping
    auto result = url;
    if (!result.isEmpty() && result.endsWith(QLatin1Char('/'))) {
        result.chop(1);
    }
    return result;
}

} // namespace imtsentra
