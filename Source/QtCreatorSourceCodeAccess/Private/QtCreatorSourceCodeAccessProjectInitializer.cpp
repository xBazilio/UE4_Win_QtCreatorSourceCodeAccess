#include "QtCreatorSourceCodeAccessProjectInitializer.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"

#include "Logging/MessageLog.h"

#define	SOLUTION_SUBPATH "Intermediate/ProjectFiles"

void FQtCreatorSourceCodeAccessProjectInitializer::InitializeProject()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("InitializeProject()")));

	FMessageLog("DevLog").Error(FText::FromString(TEXT("Paths:")));
	EnginePath = FPaths::ConvertRelativePathToFull(FGenericPlatformMisc::EngineDir());
	FMessageLog("DevLog").Error(FText::FromString(EnginePath));

	FMessageLog("DevLog").Error(FText::FromString(SolutionPath));

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
		TEXT("Templates/project.pro.user.shared.tpl")
	);
	FMessageLog("DevLog").Error(FText::FromString(DotProTemplate));

	if (!FPaths::FileExists(DotProTemplate)) return;

	// read tpl file
	FString TplFileAsString;
	FFileHelper::LoadFileToString(TplFileAsString, DotProTemplate.GetCharArray().GetData());

	// replace marcos to actual values
	TplFileAsString.Replace(*FString("%project_name%"), *ProjectName, ESearchCase::CaseSensitive);
	TplFileAsString.Replace(*FString("%project_path%"), *SolutionPath, ESearchCase::CaseSensitive);
	TplFileAsString.Replace(*FString("%ue_engine_path%"), *EnginePath, ESearchCase::CaseSensitive);

	// write new .pro.user.shared file
	FString ProUserSharedFilePath = FPaths::Combine(
			SolutionPath,
			FString(SOLUTION_SUBPATH),
			FString(ProjectName).Append(".pro.user.shared.test")
	);
	FMessageLog("DevLog").Error(FText::FromString(ProUserSharedFilePath));
	FFileHelper::SaveStringToFile(TplFileAsString, *ProUserSharedFilePath);
}
