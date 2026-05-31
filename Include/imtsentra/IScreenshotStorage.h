// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstdint>

namespace imtsentra
{

/**
 * \brief Interface for screenshot storage management
 *
 * Organizes screenshots in a structured filesystem layout:
 *   screenshots/baselines/{scenarioId}/{nodeId}/baseline.png
 *   screenshots/executions/{executionId}/{nodeId}/actual.png
 *   diffs/{executionId}/{nodeId}/diff.png
 */
class IScreenshotStorage {
public:
    virtual ~IScreenshotStorage() = default;

    /**
     * \brief Store a screenshot from execution
     * \param executionId Execution identifier
     * \param nodeId Node identifier
     * \param imageData Raw image data (PNG)
     * \return Path where the screenshot was stored
     */
    virtual std::string StoreExecutionScreenshot(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) = 0;

    /**
     * \brief Store a baseline screenshot
     */
    virtual std::string StoreBaselineScreenshot(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) = 0;

    /**
     * \brief Store a diff image
     */
    virtual std::string StoreDiffImage(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) = 0;

    /**
     * \brief Get path to execution screenshot
     */
    virtual std::optional<std::string> GetExecutionScreenshotPath(
        const std::string& executionId,
        const std::string& nodeId
    ) const = 0;

    /**
     * \brief Get path to baseline screenshot
     */
    virtual std::optional<std::string> GetBaselineScreenshotPath(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const = 0;

    /**
     * \brief Delete all artifacts for an execution
     */
    virtual void DeleteExecutionArtifacts(const std::string& executionId) = 0;

    /**
     * \brief Get total storage size in bytes
     */
    virtual size_t GetTotalStorageSize() const = 0;
};

} // namespace imtsentra
