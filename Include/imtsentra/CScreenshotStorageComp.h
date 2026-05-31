// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IScreenshotStorage.h>

namespace imtsentra
{

/**
 * \brief ACF component implementing IScreenshotStorage
 *
 * Manages filesystem-based screenshot storage with structured layout.
 * Uses iimg::IBitmap for image I/O.
 *
 * \ingroup imtsentra
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

    // reimplemented (imtsentra::IScreenshotStorage)
    std::string StoreExecutionScreenshot(
        const std::string& executionId,
        const std::string& nodeId,
        const iimg::IBitmap& bitmap
    ) override;

    std::string StoreBaselineScreenshot(
        const std::string& scenarioId,
        const std::string& nodeId,
        const iimg::IBitmap& bitmap
    ) override;

    std::string StoreDiffImage(
        const std::string& executionId,
        const std::string& nodeId,
        const iimg::IBitmap& bitmap
    ) override;

    std::shared_ptr<iimg::IBitmap> LoadScreenshot(
        const std::string& path
    ) const override;

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

protected:
    // reimplemented (icomp::CComponentBase)
    void OnComponentCreated() override;

private:
    std::string m_basePath;

    std::string EnsureDirectory(const std::string& path) const;
    bool SaveBitmap(const iimg::IBitmap& bitmap, const std::string& path) const;

    I_TEXTATTR(m_basePathAttrPtr);
};

} // namespace imtsentra
