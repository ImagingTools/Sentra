// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <iimg/IBitmap.h>
#include <istd/IPolymorphic.h>
#include <istd/TUniqueInterfacePtr.h>

// Qt includes
#include <QtCore/QString>
#include <QtCore/QtGlobal>

// Standard includes
#include <optional>
#include <memory>

namespace sentra
{

/**
 * \brief Interface for screenshot storage management
 *
 * Uses iimg::IBitmap from the Acf framework for image representation.
 * Organizes screenshots in a structured filesystem layout:
 *   screenshots/baselines/{scenarioId}/{nodeId}/baseline.png
 *   screenshots/executions/{executionId}/{nodeId}/actual.png
 *   diffs/{executionId}/{nodeId}/diff.png
 *
 * \ingroup sentra
 */
class IScreenshotStorage: virtual public istd::IPolymorphic
{
public:
    /**
     * \brief Store a screenshot from execution
     * \param executionId Execution identifier
     * \param nodeId Node identifier
     * \param bitmap Bitmap to store
     * \return Path where the screenshot was stored
     */
    virtual QString StoreExecutionScreenshot(
        const QString& executionId,
        const QString& nodeId,
        const iimg::IBitmap& bitmap
    ) = 0;

    /**
     * \brief Store a baseline screenshot
     */
    virtual QString StoreBaselineScreenshot(
        const QString& scenarioId,
        const QString& nodeId,
        const iimg::IBitmap& bitmap
    ) = 0;

    /**
     * \brief Store a diff image
     */
    virtual QString StoreDiffImage(
        const QString& executionId,
        const QString& nodeId,
        const iimg::IBitmap& bitmap
    ) = 0;

    /**
     * \brief Load a screenshot as a bitmap
     * \param path Path to the screenshot file
     * \return Loaded bitmap, or nullptr on failure
     */
    virtual std::shared_ptr<iimg::IBitmap> LoadScreenshot(
        const QString& path
    ) const = 0;

    /**
     * \brief Get path to execution screenshot
     */
    virtual std::optional<QString> GetExecutionScreenshotPath(
        const QString& executionId,
        const QString& nodeId
    ) const = 0;

    /**
     * \brief Get path to baseline screenshot
     */
    virtual std::optional<QString> GetBaselineScreenshotPath(
        const QString& scenarioId,
        const QString& nodeId
    ) const = 0;

    /**
     * \brief Delete all artifacts for an execution
     */
    virtual void DeleteExecutionArtifacts(const QString& executionId) = 0;

    /**
     * \brief Get total storage size in bytes
     */
    virtual qint64 GetTotalStorageSize() const = 0;
};

typedef istd::TUniqueInterfacePtr<IScreenshotStorage> IScreenshotStorageUniquePtr;

} // namespace sentra
