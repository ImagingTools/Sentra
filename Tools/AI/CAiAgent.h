// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/IAiProvider.h>

// Qt includes
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QString>

// Standard includes
#include <memory>

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
    void RegisterProvider(const QString& name, std::unique_ptr<IAiProvider> provider);

    /**
     * \brief Set the active provider
     */
    void SetActiveProvider(const QString& name);

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
        const QString& description,
        const QString& domSnapshot,
        const QString& accessibilityTree,
        const QString& screenshotBase64
    );

    /**
     * \brief Validate assertion with AI
     */
    AiValidationResult Validate(
        const QString& assertion,
        const QString& screenshotBase64,
        const QString& semanticSnapshot
    );

    /**
     * \brief Suggest actions for current state
     */
    QList<ActionSuggestion> SuggestActions(
        const QString& currentState,
        const std::optional<QString>& goal = std::nullopt
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
    QHash<QString, std::unique_ptr<IAiProvider>> m_providers;
    QString m_activeProvider;
    int m_maxRetries = 3;
    mutable QMutex m_mutex;

    // Cache: key → response
    QHash<QString, IntentResolution> m_intentCache;

    QString MakeCacheKey(const QString& description, const QString& domHash) const;
    IAiProvider* GetProvider() const;
};

} // namespace imtsentra
