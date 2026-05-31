// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

/**
 * \file imtsentra.h
 * \brief Main include header for ImtSentra core library
 */

/**
	\defgroup imtsentra imtsentra
	\brief Sentra — AI Visual Testing Platform core module

	The \b imtsentra module provides the core domain model and engine of the
	Sentra AI visual testing platform: scenario graph management, execution,
	image comparison, baseline management, screenshot storage, and application
	mapping. Interfaces follow the ACF component model (\c istd::IPolymorphic
	based interfaces implemented by \c icomp::CComponentBase components).

	\section imtsentra_related Related Modules
	- \b imtsentragql — GraphQL API layer
*/

#include <imtsentra/IScenarioGraph.h>
#include <imtsentra/IScenarioExecutor.h>
#include <imtsentra/INodeAction.h>
#include <imtsentra/IImageComparator.h>
#include <imtsentra/IBaselineManager.h>
#include <imtsentra/IScreenshotStorage.h>
#include <imtsentra/IAppMapBuilder.h>
#include <imtsentra/IExecutionEngine.h>

namespace imtsentra {}
