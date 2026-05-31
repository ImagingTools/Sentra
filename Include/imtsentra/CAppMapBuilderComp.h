#pragma once

#include "IAppMapBuilder.h"
#include <mutex>
#include <unordered_map>

namespace imtsentra {

/**
 * @brief ACF Component implementing IAppMapBuilder
 *
 * Builds application map by tracking navigation during test executions.
 * Detects new screens based on URL changes and major DOM differences.
 */
class CAppMapBuilderComp : public IAppMapBuilder {
public:
    CAppMapBuilderComp();
    ~CAppMapBuilderComp() override;

    void recordScreen(
        const std::string& url,
        const std::string& title,
        const std::optional<std::string>& screenshotPath = std::nullopt
    ) override;

    void recordTransition(
        const std::string& fromUrl,
        const std::string& toUrl,
        const std::string& action
    ) override;

    std::vector<AppScreen> getScreens() const override;
    std::vector<AppTransition> getTransitions() const override;
    std::vector<AppScreen> getUntestedScreens() const override;
    float getCoveragePercentage() const override;
    std::string toJson() const override;

private:
    mutable std::mutex m_mutex;
    std::unordered_map<std::string, AppScreen> m_screens;  // keyed by URL
    std::vector<AppTransition> m_transitions;

    std::string generateId() const;
    std::string normalizeUrl(const std::string& url) const;
};

} // namespace imtsentra
