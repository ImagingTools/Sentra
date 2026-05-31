#pragma once

#include "IScreenshotStorage.h"

namespace imtsentra {

/**
 * @brief ACF Component implementing IScreenshotStorage
 *
 * Manages filesystem-based screenshot storage with structured layout.
 */
class CScreenshotStorageComp : public IScreenshotStorage {
public:
    explicit CScreenshotStorageComp(const std::string& basePath);
    ~CScreenshotStorageComp() override;

    std::string storeExecutionScreenshot(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) override;

    std::string storeBaselineScreenshot(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) override;

    std::string storeDiffImage(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) override;

    std::optional<std::string> getExecutionScreenshotPath(
        const std::string& executionId,
        const std::string& nodeId
    ) const override;

    std::optional<std::string> getBaselineScreenshotPath(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const override;

    void deleteExecutionArtifacts(const std::string& executionId) override;
    size_t getTotalStorageSize() const override;

private:
    std::string m_basePath;

    std::string ensureDirectory(const std::string& path) const;
};

} // namespace imtsentra
