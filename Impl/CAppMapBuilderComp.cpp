// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CAppMapBuilderComp.h>
#include <chrono>
#include <sstream>
#include <algorithm>

namespace imtsentra
{

CAppMapBuilderComp::CAppMapBuilderComp() = default;
CAppMapBuilderComp::~CAppMapBuilderComp() = default;

void CAppMapBuilderComp::RecordScreen(
    const std::string& url,
    const std::string& title,
    const std::optional<std::string>& screenshotPath
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto normalizedUrl = NormalizeUrl(url);
    auto now = std::to_string(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );

    auto it = m_screens.find(normalizedUrl);
    if (it != m_screens.end()) {
        it->second.lastSeenAt = now;
        it->second.visitCount++;
        if (screenshotPath) {
            it->second.screenshotPath = *screenshotPath;
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
    const std::string& fromUrl,
    const std::string& toUrl,
    const std::string& action
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto normFrom = NormalizeUrl(fromUrl);
    auto normTo = NormalizeUrl(toUrl);

    // Check if transition already exists
    for (const auto& t : m_transitions) {
        if (t.sourceScreenId == normFrom && t.targetScreenId == normTo && t.action == action) {
            return;  // Already recorded
        }
    }

    auto now = std::to_string(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );

    AppTransition transition;
    transition.id = GenerateId();
    transition.sourceScreenId = normFrom;
    transition.targetScreenId = normTo;
    transition.action = action;
    transition.discoveredAt = now;
    m_transitions.push_back(std::move(transition));
}

std::vector<AppScreen> CAppMapBuilderComp::GetScreens() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<AppScreen> screens;
    screens.reserve(m_screens.size());
    for (const auto& [url, screen] : m_screens) {
        screens.push_back(screen);
    }
    return screens;
}

std::vector<AppTransition> CAppMapBuilderComp::GetTransitions() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_transitions;
}

std::vector<AppScreen> CAppMapBuilderComp::GetUntestedScreens() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<AppScreen> untested;
    for (const auto& [url, screen] : m_screens) {
        if (screen.visitCount <= 1) {  // Only discovered, not tested
            untested.push_back(screen);
        }
    }
    return untested;
}

float CAppMapBuilderComp::GetCoveragePercentage() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_screens.empty()) return 0.0f;

    int tested = 0;
    for (const auto& [url, screen] : m_screens) {
        if (screen.visitCount > 1) tested++;
    }
    return static_cast<float>(tested) / static_cast<float>(m_screens.size()) * 100.0f;
}

std::string CAppMapBuilderComp::ToJson() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ostringstream ss;
    ss << "{\"screens\":[";
    bool first = true;
    for (const auto& [url, screen] : m_screens) {
        if (!first) ss << ",";
        ss << "{\"id\":\"" << screen.id << "\","
           << "\"url\":\"" << screen.url << "\","
           << "\"title\":\"" << screen.title << "\","
           << "\"visitCount\":" << screen.visitCount << "}";
        first = false;
    }
    ss << "],\"transitions\":[";
    first = true;
    for (const auto& t : m_transitions) {
        if (!first) ss << ",";
        ss << "{\"id\":\"" << t.id << "\","
           << "\"from\":\"" << t.sourceScreenId << "\","
           << "\"to\":\"" << t.targetScreenId << "\","
           << "\"action\":\"" << t.action << "\"}";
        first = false;
    }
    ss << "]}";
    return ss.str();
}

std::string CAppMapBuilderComp::GenerateId() const {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    return "map-" + std::to_string(ms);
}

std::string CAppMapBuilderComp::NormalizeUrl(const std::string& url) const {
    // Remove trailing slash and query params for grouping
    auto result = url;
    if (!result.empty() && result.back() == '/') {
        result.pop_back();
    }
    return result;
}

} // namespace imtsentra
