// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/IAiProvider.h>
#include <unordered_map>
#include <mutex>

namespace imtsentra
{

/**
 * \brief AI Agent that orchestrates intent resolution and validation
 *
 * Manages AI provider selection, caching, retry logic,
 * and fallback chains for reliable intent resolution.
 */
class CAiAgent {
public:
    CAiAgent();
    ~CAiAgent();

    /**
     * \brief Register an AI provider
     */
    void RegisterProvider(const std::string& name, std::unique_ptr<IAiProvider> provider);

    /**
     * \brief Set the active provider
     */
    void SetActiveProvider(const std::string& name);

    /**
     * \brief Resolve intent with retry and fallback logic
     *
     * Resolution chain:
     * 1. Accessibility tree matching
     * 2. Text content matching
     * 3. Visual AI resolution
     * 4. CSS selector fallback
     */
    IntentResolution ResolveIntent(
        const std::string& description,
        const std::string& domSnapshot,
        const std::string& accessibilityTree,
        const std::string& screenshotBase64
    );

    /**
     * \brief Validate assertion with AI
     */
    AiValidationResult Validate(
        const std::string& assertion,
        const std::string& screenshotBase64,
        const std::string& semanticSnapshot
    );

    /**
     * \brief Suggest actions for current state
     */
    std::vector<ActionSuggestion> SuggestActions(
        const std::string& currentState,
        const std::optional<std::string>& goal = std::nullopt
    );

    /**
     * \brief Clear the response cache
     */
    void ClearCache();

    /**
     * \brief Set maximum retries for AI calls
     */
    void SetMaxRetries(int retries);

private:
    std::unordered_map<std::string, std::unique_ptr<IAiProvider>> m_providers;
    std::string m_activeProvider;
    int m_maxRetries = 3;
    mutable std::mutex m_mutex;

    // Cache: key → response
    std::unordered_map<std::string, IntentResolution> m_intentCache;

    std::string MakeCacheKey(const std::string& description, const std::string& domHash) const;
    IAiProvider* GetProvider() const;
};

} // namespace imtsentra
