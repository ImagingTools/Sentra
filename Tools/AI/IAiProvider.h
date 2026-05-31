// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// Qt includes
#include <QtCore/QString>
#include <QtCore/QList>

// Standard includes
#include <optional>

namespace imtsentra
{

/**
 * \brief AI intent resolution result
 */
struct IntentResolution {
    bool resolved;
    QString selector;
    std::optional<QString> coordinates;  // "x,y"
    QString actionType;
    float confidence;
    QString explanation;
    std::optional<QString> fallbackSelector;
};

/**
 * \brief AI validation result
 */
struct AiValidationResult {
    bool passed;
    float confidence;
    QString explanation;
    QList<QString> findings;
};

/**
 * \brief AI action suggestion
 */
struct ActionSuggestion {
    QString description;
    QString nodeType;
    QString config;
    float relevanceScore;
};

/**
 * \brief Interface for AI provider abstraction
 *
 * Supports multiple AI backends (OpenAI GPT-4V, Anthropic Claude, etc.)
 * with rate limiting, caching, and retry logic.
 */
class IAiProvider {
public:
    virtual ~IAiProvider() = default;

    /**
     * \brief Resolve a user intent to a concrete UI action
     * \param description User's intent description (e.g., "Click the Login button")
     * \param domSnapshot Current DOM state (JSON)
     * \param accessibilityTree Current accessibility tree (JSON)
     * \param screenshotBase64 Current screenshot (base64 PNG)
     * \return Resolution result with selector and confidence
     */
    virtual IntentResolution ResolveIntent(
        const QString& description,
        const QString& domSnapshot,
        const QString& accessibilityTree,
        const QString& screenshotBase64
    ) = 0;

    /**
     * \brief Validate a visual assertion using AI
     * \param assertion Natural language assertion
     * \param screenshotBase64 Current screenshot
     * \param semanticSnapshot Semantic model of current page
     * \return Validation result with confidence and explanation
     */
    virtual AiValidationResult Validate(
        const QString& assertion,
        const QString& screenshotBase64,
        const QString& semanticSnapshot
    ) = 0;

    /**
     * \brief Suggest next actions based on current state
     * \param currentState JSON describing current page state
     * \param goal Optional test goal description
     * \return List of suggested actions
     */
    virtual QList<ActionSuggestion> SuggestActions(
        const QString& currentState,
        const std::optional<QString>& goal = std::nullopt
    ) = 0;

    /**
     * \brief Get provider name (e.g., "openai", "anthropic")
     */
    virtual QString GetProviderName() const = 0;

    /**
     * \brief Check if the provider is available and configured
     */
    virtual bool IsAvailable() const = 0;
};

} // namespace imtsentra
