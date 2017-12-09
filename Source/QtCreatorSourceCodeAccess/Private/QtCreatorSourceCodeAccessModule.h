#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "QtCreatorSourceCodeAccessor.h"

class FQtCreatorSourceCodeAccessModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FQtCreatorSourceCodeAccessor QtCreatorSourceCodeAccessor;
};
