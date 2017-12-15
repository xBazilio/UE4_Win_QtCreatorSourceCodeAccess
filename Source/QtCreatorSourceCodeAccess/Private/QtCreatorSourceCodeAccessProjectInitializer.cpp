#include "QtCreatorSourceCodeAccessProjectInitializer.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"

#include "Logging/MessageLog.h"

#define	SOLUTION_SUBPATH "Intermediate/ProjectFiles"

void FQtCreatorSourceCodeAccessProjectInitializer::InitializeProject()
{
	EnginePath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
	FPaths::NormalizeDirectoryName(EnginePath);

	CreateDotProFile();
	CreateDefinesPriFile();
	CreateIncludesPriFile();
	CreateDotProDotUserFile();
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDotProFile()
{
	// iterate over solution files
	class FSolutionVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		TArray<FString> Headers;
		TArray<FString> Sources;
		const FString SolutionPath;
		FSolutionVisitor(const FString SolutionPath) : SolutionPath{SolutionPath}
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
					StringFilename = StringFilename.Replace(*SolutionPath, *FString("../.."), ESearchCase::CaseSensitive);
					Headers.Add(StringFilename);
				}
				if (StringFilename.Find(FString(".cpp"), ESearchCase::IgnoreCase, ESearchDir::FromEnd) == StringFilename.Len() - 4)
				{
					StringFilename = StringFilename.Replace(*SolutionPath, *FString("../.."), ESearchCase::CaseSensitive);
					Sources.Add(StringFilename);
				}
			}
			return true; // continue searching
		}
	};
	FSolutionVisitor SolutionVisitor(SolutionPath);
	FFileManagerGeneric::Get().IterateDirectoryRecursively(SolutionPath.GetCharArray().GetData(), SolutionVisitor);

	// create new .pro file content
	FString DotProFileContent;
	DotProFileContent
		.Append(TEXT("TEMPLATE = app\n"))
		.Append(TEXT("CONFIG += console\n"))
		.Append(TEXT("CONFIG -= app_bundle\n"))
		.Append(TEXT("CONFIG -= qt\n"))
		.Append(TEXT("CONFIG += c++11\n"))
		.Append(TEXT("\n"))
		.Append(TEXT("# All the defines of your project will go in this file\n"))
		.Append(TEXT("# You can put this file on your repository, but you will need to remake it once you upgrade the engine.\n"))
		.Append(TEXT("include(defines.pri)\n"))
		.Append(TEXT("\n"));

	if (SolutionVisitor.Headers.Num())
	{
		DotProFileContent.Append(TEXT("HEADERS += "));
	}

	bool bFirstLine{true};
	for (auto HeaderFile : SolutionVisitor.Headers)
	{
		if (bFirstLine)
		{
			DotProFileContent.Append(TEXT(" "));
			bFirstLine = false;
		}
		else
		{
			DotProFileContent.Append(TEXT(" \\\n    "));
		}

		DotProFileContent.Append(HeaderFile);
	}

	DotProFileContent.Append(TEXT("\n\n"));

	if (SolutionVisitor.Sources.Num())
	{
		DotProFileContent.Append(TEXT("SOURCES += "));
	}

	bFirstLine = true;
	for (auto SourceFile : SolutionVisitor.Sources)
	{
		if (bFirstLine)
		{
			DotProFileContent.Append(TEXT(" "));
			bFirstLine = false;
		}
		else
		{
			DotProFileContent.Append(TEXT(" \\\n    "));
		}

		DotProFileContent.Append(SourceFile);
	}

	DotProFileContent
		.Append(TEXT("\n\n"))
		.Append(TEXT("# All your generated includes will go in this file\n"))
		.Append(TEXT("# You can not put this on the repository as this contains hardcoded paths\n"))
		.Append(TEXT("# and is dependend on your windows install and engine version\n"))
		.Append(TEXT("include(includes.pri)\n"));

	// write new .pro file
	FString DotProFilePath = FPaths::Combine(
		SolutionPath,
		FString(SOLUTION_SUBPATH),
		FString(ProjectName).Append(".pro.test") // TODO: remove .test
	);
	FFileHelper::SaveStringToFile(DotProFileContent, *DotProFilePath);
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
	TplFileAsString = TplFileAsString.Replace(*FString("%project_name%"), *ProjectName, ESearchCase::CaseSensitive);
	TplFileAsString = TplFileAsString.Replace(*FString("%project_path%"), *SolutionPath, ESearchCase::CaseSensitive);
	TplFileAsString = TplFileAsString.Replace(*FString("%ue_engine_path%"), *EnginePath, ESearchCase::CaseSensitive);

	// write new .pro.user.shared file
	FString ProUserSharedFilePath = FPaths::Combine(
			SolutionPath,
			FString(SOLUTION_SUBPATH),
			FString(ProjectName).Append(".pro.user.shared.test") // TODO: remove .test
	);
	FFileHelper::SaveStringToFile(TplFileAsString, *ProUserSharedFilePath);
}
