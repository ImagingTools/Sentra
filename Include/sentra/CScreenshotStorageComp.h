// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// Sentra includes
#include <sentra/IScreenshotStorage.h>

// Qt includes
#include <QtCore/QString>

namespace sentra
{

/**
 * \brief ACF component implementing IScreenshotStorage
 *
 * Manages filesystem-based screenshot storage with structured layout.
 * Uses iimg::IBitmap for image I/O.
 *
 * \ingroup sentra
 */
class CScreenshotStorageComp:
        public icomp::CComponentBase,
        virtual public IScreenshotStorage
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CScreenshotStorageComp)
        I_REGISTER_INTERFACE(IScreenshotStorage);
        I_ASSIGN(m_basePathAttrPtr, "BasePath", "Base directory used to store screenshots, baselines and diffs", true, "screenshots");
    I_END_COMPONENT

    // reimplemented (sentra::IScreenshotStorage)
    QString StoreExecutionScreenshot(
        const QString& executionId,
        const QString& nodeId,
        const iimg::IBitmap& bitmap
    ) override;

    QString StoreBaselineScreenshot(
        const QString& scenarioId,
        const QString& nodeId,
        const iimg::IBitmap& bitmap
    ) override;

    QString StoreDiffImage(
        const QString& executionId,
        const QString& nodeId,
        const iimg::IBitmap& bitmap
    ) override;

    std::shared_ptr<iimg::IBitmap> LoadScreenshot(
        const QString& path
    ) const override;

    std::optional<QString> GetExecutionScreenshotPath(
        const QString& executionId,
        const QString& nodeId
    ) const override;

    std::optional<QString> GetBaselineScreenshotPath(
        const QString& scenarioId,
        const QString& nodeId
    ) const override;

    void DeleteExecutionArtifacts(const QString& executionId) override;
    qint64 GetTotalStorageSize() const override;

protected:
    // reimplemented (icomp::CComponentBase)
    void OnComponentCreated() override;

private:
    QString m_basePath;

    QString EnsureDirectory(const QString& path) const;
    bool SaveBitmap(const iimg::IBitmap& bitmap, const QString& path) const;

    I_TEXTATTR(m_basePathAttrPtr);
};

} // namespace sentra
