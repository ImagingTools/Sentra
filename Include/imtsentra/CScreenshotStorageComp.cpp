// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include <imtsentra/CScreenshotStorageComp.h>

// Acf includes
#include <iimg/IBitmap.h>

// Qt includes
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

namespace imtsentra
{

void CScreenshotStorageComp::OnComponentCreated() {
    BaseClass::OnComponentCreated();

    m_basePath = *m_basePathAttrPtr;
    QDir().mkpath(m_basePath);
}

QString CScreenshotStorageComp::StoreExecutionScreenshot(
    const QString& executionId,
    const QString& nodeId,
    const iimg::IBitmap& bitmap
) {
    auto dir = EnsureDirectory(m_basePath + QStringLiteral("/executions/") + executionId + QLatin1Char('/') + nodeId);
    auto path = dir + QStringLiteral("/actual.png");
    SaveBitmap(bitmap, path);
    return path;
}

QString CScreenshotStorageComp::StoreBaselineScreenshot(
    const QString& scenarioId,
    const QString& nodeId,
    const iimg::IBitmap& bitmap
) {
    auto dir = EnsureDirectory(m_basePath + QStringLiteral("/baselines/") + scenarioId + QLatin1Char('/') + nodeId);
    auto path = dir + QStringLiteral("/baseline.png");
    SaveBitmap(bitmap, path);
    return path;
}

QString CScreenshotStorageComp::StoreDiffImage(
    const QString& executionId,
    const QString& nodeId,
    const iimg::IBitmap& bitmap
) {
    auto dir = EnsureDirectory(m_basePath + QStringLiteral("/diffs/") + executionId + QLatin1Char('/') + nodeId);
    auto path = dir + QStringLiteral("/diff.png");
    SaveBitmap(bitmap, path);
    return path;
}

std::shared_ptr<iimg::IBitmap> CScreenshotStorageComp::LoadScreenshot(
    const QString& path
) const {
    if (!QFile::exists(path)) {
        return nullptr;
    }

    // TODO: Use iimg codec to load bitmap from file
    return nullptr;
}

std::optional<QString> CScreenshotStorageComp::GetExecutionScreenshotPath(
    const QString& executionId,
    const QString& nodeId
) const {
    auto path = m_basePath + QStringLiteral("/executions/") + executionId + QLatin1Char('/') + nodeId + QStringLiteral("/actual.png");
    if (QFile::exists(path)) return path;
    return std::nullopt;
}

std::optional<QString> CScreenshotStorageComp::GetBaselineScreenshotPath(
    const QString& scenarioId,
    const QString& nodeId
) const {
    auto path = m_basePath + QStringLiteral("/baselines/") + scenarioId + QLatin1Char('/') + nodeId + QStringLiteral("/baseline.png");
    if (QFile::exists(path)) return path;
    return std::nullopt;
}

void CScreenshotStorageComp::DeleteExecutionArtifacts(const QString& executionId) {
    auto execPath = m_basePath + QStringLiteral("/executions/") + executionId;
    auto diffPath = m_basePath + QStringLiteral("/diffs/") + executionId;

    if (QFileInfo::exists(execPath)) QDir(execPath).removeRecursively();
    if (QFileInfo::exists(diffPath)) QDir(diffPath).removeRecursively();
}

qint64 CScreenshotStorageComp::GetTotalStorageSize() const {
    qint64 total = 0;
    QDirIterator it(m_basePath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        total += it.fileInfo().size();
    }
    return total;
}

QString CScreenshotStorageComp::EnsureDirectory(const QString& path) const {
    QDir().mkpath(path);
    return path;
}

bool CScreenshotStorageComp::SaveBitmap(
    const iimg::IBitmap& bitmap,
    const QString& path
) const {
    // TODO: Use iimg codec to save bitmap to file
    (void)bitmap;
    (void)path;
    return false;
}

} // namespace imtsentra
