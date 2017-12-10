#include "QtCreatorSourceCodeAccessProjectInitializer.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Logging/MessageLog.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"

void FQtCreatorSourceCodeAccessProjectInitializer::InitializeProject()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("InitializeProject()")));

	FMessageLog("DevLog").Error(FText::FromString(TEXT("Paths:")));
	EnginePath = FPaths::ConvertRelativePathToFull(FGenericPlatformMisc::EngineDir());
	FMessageLog("DevLog").Error(FText::FromString(EnginePath));

	FMessageLog("DevLog").Error(FText::FromString(SolutionPath));

	ProjectName = FPaths::GetBaseFilename(SolutionPath);
	FMessageLog("DevLog").Error(FText::FromString(ProjectName));

	CreateDotProFile();
	CreateDefinesPriFile();
	CreateIncludesPriFile();
	CreateDotProDotUserFile();
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDotProFile()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("CreateDotProFile()")));
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDefinesPriFile()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("CreateDefinesPriFile()")));
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateIncludesPriFile()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("CreateIncludesPriFile()")));
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDotProDotUserFile()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("CreateDotProDotUserFile()")));

	FString DotProTemplate = FPaths::Combine(
		IPluginManager::Get().FindPlugin(TEXT("QtCreatorSourceCodeAccess"))->GetBaseDir(),
		TEXT("Templates/QtCreatorPlugin.pro.user.shared.tpl")
	);
	FMessageLog("DevLog").Error(FText::FromString(DotProTemplate));
}
