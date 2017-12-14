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
	EnginePath = FPaths::ConvertRelativePathToFull(FGenericPlatformMisc::EngineDir());

	CreateDotProFile();
	CreateDefinesPriFile();
	CreateIncludesPriFile();
	CreateDotProDotUserFile();
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDotProFile()
{
	FMessageLog("DevLog").Error(FText::FromString(TEXT("CreateDotProFile()")));

	// iterate over solution files
	class FSolutionVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		TArray<FString> Headers;
		TArray<FString> Sources;
		FSolutionVisitor()
		{
		}
		virtual bool Visit(const TCHAR* Filename, bool bIsDirectory)
		{
			if (!bIsDirectory)
			{
				FString StringFilename(Filename);

				// skip all paths except with /Source/ substring
				if (!StringFilename.Contains(TEXT("/Source/"), ESearchCase::CaseSensitive))
				{
					return true; // continue searching
				}
				// look for .cpp and .h files
				// collect found files
				if (StringFilename.Find(FString(".h"), ESearchCase::IgnoreCase, ESearchDir::FromEnd) == StringFilename.Len() - 2)
				{
					Headers.Add(StringFilename);
				}
				if (StringFilename.Find(FString(".cpp"), ESearchCase::IgnoreCase, ESearchDir::FromEnd) == StringFilename.Len() - 4)
				{
					Sources.Add(StringFilename);
				}
				FMessageLog("DevLog").Error(FText::FromString(StringFilename));
			}
			return true; // continue searching
		}
	};
	FSolutionVisitor SolutionVisitor;
	FFileManagerGeneric::Get().IterateDirectoryRecursively(SolutionPath.GetCharArray().GetData(), SolutionVisitor);

	// write new .pro file from scratch
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
	FString DotProTemplate = FPaths::Combine(
		IPluginManager::Get().FindPlugin(TEXT("QtCreatorSourceCodeAccess"))->GetBaseDir(),
		TEXT("Templates/project.pro.user.shared.tpl")
	);

	// can't continue without tpl file
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
			FString(ProjectName).Append(".pro.user.shared.test") // TODO: remove .test
	);
	FFileHelper::SaveStringToFile(TplFileAsString, *ProUserSharedFilePath);
}
