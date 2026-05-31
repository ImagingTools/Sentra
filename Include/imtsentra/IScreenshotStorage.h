#pragma once

#include <string>
#include <vector>
#include <optional>

namespace imtsentra {

/**
 * @brief Interface for screenshot storage management
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
     * @brief Store a screenshot from execution
     * @param executionId Execution identifier
     * @param nodeId Node identifier
     * @param imageData Raw image data (PNG)
     * @return Path where the screenshot was stored
     */
    virtual std::string storeExecutionScreenshot(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) = 0;

    /**
     * @brief Store a baseline screenshot
     */
    virtual std::string storeBaselineScreenshot(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) = 0;

    /**
     * @brief Store a diff image
     */
    virtual std::string storeDiffImage(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) = 0;

    /**
     * @brief Get path to execution screenshot
     */
    virtual std::optional<std::string> getExecutionScreenshotPath(
        const std::string& executionId,
        const std::string& nodeId
    ) const = 0;

    /**
     * @brief Get path to baseline screenshot
     */
    virtual std::optional<std::string> getBaselineScreenshotPath(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const = 0;

    /**
     * @brief Delete all artifacts for an execution
     */
    virtual void deleteExecutionArtifacts(const std::string& executionId) = 0;

    /**
     * @brief Get total storage size in bytes
     */
    virtual size_t getTotalStorageSize() const = 0;
};

} // namespace imtsentra
