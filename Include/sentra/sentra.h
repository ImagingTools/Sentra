// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#pragma once

/**
 * \file sentra.h
 * \brief Main include header for Sentra core library
 */

/**
	\defgroup sentra sentra
	\brief Sentra — AI Visual Testing Platform core module

	The \b sentra module provides the core domain model and engine of the
	Sentra AI visual testing platform: scenario graph management, execution,
	image comparison, baseline management, screenshot storage, and application
	mapping. Interfaces follow the ACF component model (\c istd::IPolymorphic
	based interfaces implemented by \c icomp::CComponentBase components).

	\section sentra_related Related Modules
	- \b sentragql — GraphQL API layer
*/

#include <sentra/IScenarioGraph.h>
#include <sentra/IScenarioExecutor.h>
#include <sentra/INodeAction.h>
#include <sentra/IImageComparator.h>
#include <sentra/IBaselineManager.h>
#include <sentra/IScreenshotStorage.h>
#include <sentra/IAppMapBuilder.h>
#include <sentra/IExecutionEngine.h>

namespace sentra {}
