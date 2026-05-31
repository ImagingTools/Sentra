// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <sentra/CAppMapBuilderComp.h>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>

// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QMutexLocker>

namespace sentra
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

bool CAppMapBuilderComp::Serialize(iser::IArchive& archive)
{
    QMutexLocker lock(&m_mutex);

    istd::CChangeNotifier changeNotifier(archive.IsStoring() ? nullptr : this);

    bool retVal = true;

    // Screens collection
    {
        QList<AppScreen> screens = m_screens.values();
        int count = screens.size();
        if (!archive.IsStoring()){
            m_screens.clear();
            count = 0;
        }

        iser::CArchiveTag screensTag("Screens", "Application screens", iser::CArchiveTag::TT_MULTIPLE);
        iser::CArchiveTag screenTag("Screen", "Application screen", iser::CArchiveTag::TT_GROUP, &screensTag);

        retVal = retVal && archive.BeginMultiTag(screensTag, screenTag, count);

        for (int index = 0; index < count; ++index){
            retVal = retVal && archive.BeginTag(screenTag);

            AppScreen screen;
            if (archive.IsStoring()){
                screen = screens[index];
            }

            retVal = retVal && SerializeScreen(archive, screen);

            retVal = retVal && archive.EndTag(screenTag);

            if (retVal && !archive.IsStoring()){
                m_screens.insert(screen.url, screen);
            }
        }

        retVal = retVal && archive.EndTag(screensTag);
    }

    // Transitions collection
    {
        int count = m_transitions.size();
        if (!archive.IsStoring()){
            m_transitions.clear();
            count = 0;
        }

        iser::CArchiveTag transitionsTag("Transitions", "Screen transitions", iser::CArchiveTag::TT_MULTIPLE);
        iser::CArchiveTag transitionTag("Transition", "Screen transition", iser::CArchiveTag::TT_GROUP, &transitionsTag);

        retVal = retVal && archive.BeginMultiTag(transitionsTag, transitionTag, count);

        for (int index = 0; index < count; ++index){
            retVal = retVal && archive.BeginTag(transitionTag);

            AppTransition transition;
            if (archive.IsStoring()){
                transition = m_transitions[index];
            }

            retVal = retVal && SerializeTransition(archive, transition);

            retVal = retVal && archive.EndTag(transitionTag);

            if (retVal && !archive.IsStoring()){
                m_transitions << transition;
            }
        }

        retVal = retVal && archive.EndTag(transitionsTag);
    }

    return retVal;
}

bool CAppMapBuilderComp::SerializeScreen(iser::IArchive& archive, AppScreen& screen)
{
    bool retVal = true;

    static iser::CArchiveTag idTag("Id", "Screen identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(idTag);
    retVal = retVal && archive.Process(screen.id);
    retVal = retVal && archive.EndTag(idTag);

    static iser::CArchiveTag urlTag("Url", "Screen URL", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(urlTag);
    retVal = retVal && archive.Process(screen.url);
    retVal = retVal && archive.EndTag(urlTag);

    static iser::CArchiveTag titleTag("Title", "Screen title", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(titleTag);
    retVal = retVal && archive.Process(screen.title);
    retVal = retVal && archive.EndTag(titleTag);

    QString screenshotPath = screen.screenshotPath.value_or(QString());
    static iser::CArchiveTag screenshotTag("ScreenshotPath", "Screenshot path", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(screenshotTag);
    retVal = retVal && archive.Process(screenshotPath);
    retVal = retVal && archive.EndTag(screenshotTag);
    if (!archive.IsStoring()){
        screen.screenshotPath = screenshotPath.isEmpty() ? std::optional<QString>() : screenshotPath;
    }

    retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeContainer(
        archive, screen.elements, "Elements", "Element");

    static iser::CArchiveTag discoveredTag("DiscoveredAt", "Discovery timestamp", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(discoveredTag);
    retVal = retVal && archive.Process(screen.discoveredAt);
    retVal = retVal && archive.EndTag(discoveredTag);

    static iser::CArchiveTag lastSeenTag("LastSeenAt", "Last seen timestamp", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(lastSeenTag);
    retVal = retVal && archive.Process(screen.lastSeenAt);
    retVal = retVal && archive.EndTag(lastSeenTag);

    static iser::CArchiveTag visitCountTag("VisitCount", "Visit count", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(visitCountTag);
    retVal = retVal && archive.Process(screen.visitCount);
    retVal = retVal && archive.EndTag(visitCountTag);

    return retVal;
}

bool CAppMapBuilderComp::SerializeTransition(iser::IArchive& archive, AppTransition& transition)
{
    bool retVal = true;

    static iser::CArchiveTag idTag("Id", "Transition identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(idTag);
    retVal = retVal && archive.Process(transition.id);
    retVal = retVal && archive.EndTag(idTag);

    static iser::CArchiveTag sourceTag("SourceScreenId", "Source screen identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(sourceTag);
    retVal = retVal && archive.Process(transition.sourceScreenId);
    retVal = retVal && archive.EndTag(sourceTag);

    static iser::CArchiveTag targetTag("TargetScreenId", "Target screen identifier", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(targetTag);
    retVal = retVal && archive.Process(transition.targetScreenId);
    retVal = retVal && archive.EndTag(targetTag);

    static iser::CArchiveTag actionTag("Action", "Transition action", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(actionTag);
    retVal = retVal && archive.Process(transition.action);
    retVal = retVal && archive.EndTag(actionTag);

    QString label = transition.label.value_or(QString());
    static iser::CArchiveTag labelTag("Label", "Transition label", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(labelTag);
    retVal = retVal && archive.Process(label);
    retVal = retVal && archive.EndTag(labelTag);
    if (!archive.IsStoring()){
        transition.label = label.isEmpty() ? std::optional<QString>() : label;
    }

    static iser::CArchiveTag discoveredTag("DiscoveredAt", "Discovery timestamp", iser::CArchiveTag::TT_LEAF);
    retVal = retVal && archive.BeginTag(discoveredTag);
    retVal = retVal && archive.Process(transition.discoveredAt);
    retVal = retVal && archive.EndTag(discoveredTag);

    return retVal;
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

} // namespace sentra
