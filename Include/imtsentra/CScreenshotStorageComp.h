// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

#include <imtsentra/IScreenshotStorage.h>

namespace imtsentra
{

/**
 * \brief ACF Component implementing IScreenshotStorage
 *
 * Manages filesystem-based screenshot storage with structured layout.
 */
class CScreenshotStorageComp : public IScreenshotStorage {
public:
    explicit CScreenshotStorageComp(const std::string& basePath);
    ~CScreenshotStorageComp() override;

    std::string StoreExecutionScreenshot(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) override;

    std::string StoreBaselineScreenshot(
        const std::string& scenarioId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) override;

    std::string StoreDiffImage(
        const std::string& executionId,
        const std::string& nodeId,
        const std::vector<uint8_t>& imageData
    ) override;

    std::optional<std::string> GetExecutionScreenshotPath(
        const std::string& executionId,
        const std::string& nodeId
    ) const override;

    std::optional<std::string> GetBaselineScreenshotPath(
        const std::string& scenarioId,
        const std::string& nodeId
    ) const override;

    void DeleteExecutionArtifacts(const std::string& executionId) override;
    size_t GetTotalStorageSize() const override;

private:
    std::string m_basePath;

    std::string EnsureDirectory(const std::string& path) const;
};

} // namespace imtsentra
