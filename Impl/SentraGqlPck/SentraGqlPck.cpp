// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ImtCore-Commercial
#include "SentraGqlPck.h"


// ACF includes
#include <icomp/export.h>


namespace SentraGqlPck
{


I_EXPORT_PACKAGE(
			"SentraGqlPck",
			"Sentra GraphQL controller component package",
			IM_PROJECT("\"ImagingTools Sentra\"") IM_COMPANY("ImagingTools") "GraphQL");

I_EXPORT_COMPONENT(
			ScenarioServiceController,
			"Scenario GraphQL service controller",
			"Scenario Service Controller");

I_EXPORT_COMPONENT(
			ExecutionServiceController,
			"Execution GraphQL service controller",
			"Execution Service Controller");


} // namespace SentraGqlPck
