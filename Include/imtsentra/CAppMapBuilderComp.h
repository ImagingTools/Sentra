// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

// ACF includes
#include <icomp/CComponentBase.h>

// ImtSentra includes
#include <imtsentra/IAppMapBuilder.h>

// Standard includes
#include <mutex>
#include <unordered_map>

namespace imtsentra
{

/**
 * \brief ACF component implementing IAppMapBuilder
 *
 * Builds application map by tracking navigation during test executions.
 * Detects new screens based on URL changes and major DOM differences.
 *
 * \ingroup imtsentra
 */
class CAppMapBuilderComp:
        public icomp::CComponentBase,
        virtual public IAppMapBuilder
{
public:
    typedef icomp::CComponentBase BaseClass;

    I_BEGIN_COMPONENT(CAppMapBuilderComp)
        I_REGISTER_INTERFACE(IAppMapBuilder);
    I_END_COMPONENT

    // reimplemented (imtsentra::IAppMapBuilder)
    void RecordScreen(
        const std::string& url,
        const std::string& title,
        const std::optional<std::string>& screenshotPath = std::nullopt
    ) override;

    void RecordTransition(
        const std::string& fromUrl,
        const std::string& toUrl,
        const std::string& action
    ) override;

    std::vector<AppScreen> GetScreens() const override;
    std::vector<AppTransition> GetTransitions() const override;
    std::vector<AppScreen> GetUntestedScreens() const override;
    float GetCoveragePercentage() const override;
    std::string ToJson() const override;

private:
    mutable std::mutex m_mutex;
    std::unordered_map<std::string, AppScreen> m_screens;  // keyed by URL
    std::vector<AppTransition> m_transitions;

    std::string GenerateId() const;
    std::string NormalizeUrl(const std::string& url) const;
};

} // namespace imtsentra
