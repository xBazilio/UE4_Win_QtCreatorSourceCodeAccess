#include "QtCreatorSourceCodeAccessModule.h"
#include "Modules/ModuleManager.h"
#include "Features/IModularFeatures.h"

IMPLEMENT_MODULE(FQtCreatorSourceCodeAccessModule, QtCreatorSourceCodeAccess)

#define LOCTEXT_NAMESPACE "FQtCreatorSourceCodeAccessModule"

void FQtCreatorSourceCodeAccessModule::StartupModule()
{
	// Bind our source control provider to the editor
	IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &QtCreatorSourceCodeAccessor);
}

void FQtCreatorSourceCodeAccessModule::ShutdownModule()
{
	// Unbind provider from editor
	IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &QtCreatorSourceCodeAccessor);
}

#undef LOCTEXT_NAMESPACE
