#include "imtsentra/CScreenshotStorageComp.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace imtsentra {

CScreenshotStorageComp::CScreenshotStorageComp(const std::string& basePath)
    : m_basePath(basePath) {
    fs::create_directories(basePath);
}

CScreenshotStorageComp::~CScreenshotStorageComp() = default;

std::string CScreenshotStorageComp::storeExecutionScreenshot(
    const std::string& executionId,
    const std::string& nodeId,
    const std::vector<uint8_t>& imageData
) {
    auto dir = ensureDirectory(m_basePath + "/executions/" + executionId + "/" + nodeId);
    auto path = dir + "/actual.png";

    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());

    return path;
}

std::string CScreenshotStorageComp::storeBaselineScreenshot(
    const std::string& scenarioId,
    const std::string& nodeId,
    const std::vector<uint8_t>& imageData
) {
    auto dir = ensureDirectory(m_basePath + "/baselines/" + scenarioId + "/" + nodeId);
    auto path = dir + "/baseline.png";

    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());

    return path;
}

std::string CScreenshotStorageComp::storeDiffImage(
    const std::string& executionId,
    const std::string& nodeId,
    const std::vector<uint8_t>& imageData
) {
    auto dir = ensureDirectory(m_basePath + "/diffs/" + executionId + "/" + nodeId);
    auto path = dir + "/diff.png";

    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());

    return path;
}

std::optional<std::string> CScreenshotStorageComp::getExecutionScreenshotPath(
    const std::string& executionId,
    const std::string& nodeId
) const {
    auto path = m_basePath + "/executions/" + executionId + "/" + nodeId + "/actual.png";
    if (fs::exists(path)) return path;
    return std::nullopt;
}

std::optional<std::string> CScreenshotStorageComp::getBaselineScreenshotPath(
    const std::string& scenarioId,
    const std::string& nodeId
) const {
    auto path = m_basePath + "/baselines/" + scenarioId + "/" + nodeId + "/baseline.png";
    if (fs::exists(path)) return path;
    return std::nullopt;
}

void CScreenshotStorageComp::deleteExecutionArtifacts(const std::string& executionId) {
    auto execPath = m_basePath + "/executions/" + executionId;
    auto diffPath = m_basePath + "/diffs/" + executionId;

    if (fs::exists(execPath)) fs::remove_all(execPath);
    if (fs::exists(diffPath)) fs::remove_all(diffPath);
}

size_t CScreenshotStorageComp::getTotalStorageSize() const {
    size_t total = 0;
    for (const auto& entry : fs::recursive_directory_iterator(m_basePath)) {
        if (entry.is_regular_file()) {
            total += entry.file_size();
        }
    }
    return total;
}

std::string CScreenshotStorageComp::ensureDirectory(const std::string& path) const {
    fs::create_directories(path);
    return path;
}

} // namespace imtsentra
