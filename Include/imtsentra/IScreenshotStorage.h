// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <iimg/IBitmap.h>

#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include <memory>

namespace imtsentra
{

/**
 * \brief Interface for screenshot storage management
 *
 * Uses iimg::IBitmap from the Acf framework for image representation.
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
     * \param bitmap Bitmap to store
     * \return Path where the screenshot was stored
     */
    virtual std::string StoreExecutionScreenshot(
        const std::string& executionId,
        const std::string& nodeId,
        const iimg::IBitmap& bitmap
    ) = 0;

    /**
     * \brief Store a baseline screenshot
     */
    virtual std::string StoreBaselineScreenshot(
        const std::string& scenarioId,
        const std::string& nodeId,
        const iimg::IBitmap& bitmap
    ) = 0;

    /**
     * \brief Store a diff image
     */
    virtual std::string StoreDiffImage(
        const std::string& executionId,
        const std::string& nodeId,
        const iimg::IBitmap& bitmap
    ) = 0;

    /**
     * \brief Load a screenshot as a bitmap
     * \param path Path to the screenshot file
     * \return Loaded bitmap, or nullptr on failure
     */
    virtual std::shared_ptr<iimg::IBitmap> LoadScreenshot(
        const std::string& path
    ) const = 0;

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
