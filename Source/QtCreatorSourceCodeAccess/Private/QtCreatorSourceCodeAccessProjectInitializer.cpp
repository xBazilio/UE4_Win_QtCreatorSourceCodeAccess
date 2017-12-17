#include "QtCreatorSourceCodeAccessProjectInitializer.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"

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
		FString(ProjectName).Append(TEXT(".pro"))
	);
	FFileHelper::SaveStringToFile(DotProFileContent, *DotProFilePath);
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDefinesPriFile()
{
	CreateDotPriFile(
		FString("<NMakePreprocessorDefinitions>"),
		FString("</NMakePreprocessorDefinitions>"),
		FString("DEFINES"),
		FPaths::Combine(
			SolutionPath,
			FString(SOLUTION_SUBPATH),
			FString("defines.pri")
		)
	);
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateIncludesPriFile()
{
	CreateDotPriFile(
		FString("<NMakeIncludeSearchPath>"),
		FString("</NMakeIncludeSearchPath>"),
		FString("INCLUDEPATH"),
		FPaths::Combine(
			SolutionPath,
			FString(SOLUTION_SUBPATH),
			FString("includes.pri")
		)
	);
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDotPriFile(
	const FString& StartTag,
	const FString& EndTag,
	const FString& VarName,
	const FString& FileName
)
{
	FString VcxProjFilePath = FPaths::Combine(
		SolutionPath,
		TEXT(SOLUTION_SUBPATH),
		FString(ProjectName).Append(TEXT(".vcxproj"))
	);

	// nothing to do here without .vcxproj file
	if (!FPaths::FileExists(VcxProjFilePath)) return;

	FString VcxProjFileContent;
	FFileHelper::LoadFileToString(VcxProjFileContent, VcxProjFilePath.GetCharArray().GetData());

	int32 StartPosition = VcxProjFileContent.Find(StartTag, ESearchCase::IgnoreCase);
	if (!StartPosition)
	{
		// Can't continue without tag content
		return;
	}
	else
	{
		StartPosition += StartTag.Len(); // shift position to the start of content
	}
	int32 EndPosition = VcxProjFileContent.Find(EndTag, ESearchCase::IgnoreCase);
	if (!EndPosition)
	{
		// Can't continue without tag content
		return;
	}

	FString ReplacementMid("\"\n");
	ReplacementMid = ReplacementMid.Append(VarName).Append(TEXT(" += \""));
	FString DotPriFileContent = VcxProjFileContent.Mid(StartPosition, EndPosition - StartPosition);
	DotPriFileContent = DotPriFileContent.Replace(*FString(";"), *ReplacementMid);
	DotPriFileContent = FString(VarName).Append(TEXT(" += \"")).Append(DotPriFileContent).Append(TEXT("\"\n"));

	// write new .pri file
	FFileHelper::SaveStringToFile(DotPriFileContent, *FileName);
}

void FQtCreatorSourceCodeAccessProjectInitializer::CreateDotProDotUserFile()
{
	FString DotProTplPath = FPaths::Combine(
		IPluginManager::Get().FindPlugin(TEXT("QtCreatorSourceCodeAccess"))->GetBaseDir(),
		TEXT("Templates/project.pro.user.shared.tpl")
	);

	// can't continue without tpl file
	if (!FPaths::FileExists(DotProTplPath)) return;

	// read tpl file
	FString DotProTplContent;
	FFileHelper::LoadFileToString(DotProTplContent, DotProTplPath.GetCharArray().GetData());

	// replace marcos to actual values
	DotProTplContent = DotProTplContent.Replace(*FString("%project_name%"), *ProjectName, ESearchCase::CaseSensitive);
	DotProTplContent = DotProTplContent.Replace(*FString("%project_path%"), *SolutionPath, ESearchCase::CaseSensitive);
	DotProTplContent = DotProTplContent.Replace(*FString("%ue_engine_path%"), *EnginePath, ESearchCase::CaseSensitive);

	// write new .pro.user.shared file
	FString DotProUserSharedFilePath = FPaths::Combine(
			SolutionPath,
			FString(SOLUTION_SUBPATH),
			FString(ProjectName).Append(TEXT(".pro.user.shared"))
	);
	FFileHelper::SaveStringToFile(DotProTplContent, *DotProUserSharedFilePath);
}
