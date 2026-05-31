// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CBaselineManagerComp.h>
#include <chrono>
#include <sstream>

namespace imtsentra
{

CBaselineManagerComp::CBaselineManagerComp() = default;
CBaselineManagerComp::~CBaselineManagerComp() = default;

std::optional<BaselineEntry> CBaselineManagerComp::GetBaseline(
    const std::string& scenarioId,
    const std::string& nodeId
) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto key = MakeKey(scenarioId, nodeId);
    auto it = m_baselines.find(key);
    if (it != m_baselines.end() && !it->second.empty()) {
        return it->second.back();  // Latest version
    }
    return std::nullopt;
}

BaselineEntry CBaselineManagerComp::SetBaseline(
    const std::string& scenarioId,
    const std::string& nodeId,
    const std::string& screenshotPath
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto key = MakeKey(scenarioId, nodeId);

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string timestamp = std::to_string(ms);

    BaselineEntry entry;
    entry.id = "bl-" + timestamp;
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
    const std::string& baselineId,
    BaselineDecision decision,
    const std::string& decidedBy
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    // TODO: Record the decision and update baseline if accepted
}

std::vector<BaselineEntry> CBaselineManagerComp::GetBaselines(const std::string& scenarioId) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<BaselineEntry> result;
    for (const auto& [key, versions] : m_baselines) {
        if (!versions.empty() && versions.back().scenarioId == scenarioId) {
            result.push_back(versions.back());
        }
    }
    return result;
}

std::vector<BaselineEntry> CBaselineManagerComp::GetHistory(
    const std::string& scenarioId,
    const std::string& nodeId
) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto key = MakeKey(scenarioId, nodeId);
    auto it = m_baselines.find(key);
    if (it != m_baselines.end()) {
        return it->second;
    }
    return {};
}

BaselineStrategy CBaselineManagerComp::GetStrategy() const {
    return m_strategy;
}

void CBaselineManagerComp::SetStrategy(BaselineStrategy strategy) {
    m_strategy = strategy;
}

std::string CBaselineManagerComp::MakeKey(const std::string& scenarioId, const std::string& nodeId) const {
    return scenarioId + "/" + nodeId;
}

} // namespace imtsentra
