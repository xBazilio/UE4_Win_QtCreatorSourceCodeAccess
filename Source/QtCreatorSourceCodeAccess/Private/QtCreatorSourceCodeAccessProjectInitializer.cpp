#include "QtCreatorSourceCodeAccessProjectInitializer.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Logging/MessageLog.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/Paths.h"

void FQtCreatorSourceCodeAccessProjectInitializer::InitializeProject()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("InitializeProject()")));

	FString EngineRootDir;
	EngineRootDir = FPaths::ConvertRelativePathToFull(FGenericPlatformMisc::EngineDir());
	FMessageLog("DevLog").Error(FText::FromString(EngineRootDir));

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
}
