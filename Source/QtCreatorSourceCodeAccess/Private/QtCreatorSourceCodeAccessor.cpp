// Implementation of ISourceCodeAccessor
#include "QtCreatorSourceCodeAccessor.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Windows/WindowsHWrapper.h"
#include <TlHelp32.h>
#include "Logging/MessageLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogQtCreatorAccessor, Log, All)

#define LOCTEXT_NAMESPACE "QtCreatorSourceCodeAccessor"
#define QT_PATH "c:/Qt"
#define	SOLUTION_SUBPATH "Intermediate/ProjectFiles"

bool FQtCreatorSourceCodeAccessor::CanAccessSourceCode() const
{
	FString IDEPath;
	return CanRunQtCreator(IDEPath);
}

FName FQtCreatorSourceCodeAccessor::GetFName() const
{
	return FName("QtCreatorSourceCodeAccessor");
}

FText FQtCreatorSourceCodeAccessor::GetNameText() const
{
	return LOCTEXT("QtCreatorDisplayName", "Qt Creator 4.x");
}

FText FQtCreatorSourceCodeAccessor::GetDescriptionText() const
{
	return LOCTEXT("QtCreatorDisplayDesc", "Create and write source code with Qt Creator");
}

bool FQtCreatorSourceCodeAccessor::OpenSolution()
{
	return OpenSolutionAtPath(GetSolutionPath());
}

bool FQtCreatorSourceCodeAccessor::OpenSolutionAtPath(const FString& InSolutionPath)
{
	int32 PID;
	if (IsIDERunning(PID))
	{
		return true;
	}

	FString Solution = FPaths::Combine(FPaths::GetPath(InSolutionPath), TEXT(SOLUTION_SUBPATH));
	FString IDEPath;

	if (!CanRunQtCreator(IDEPath))
	{
		UE_LOG(LogQtCreatorAccessor, Warning, TEXT("FQtCreatorSourceCodeAccessor::OpenSourceFiles: cannot find Qt Creator"));
		return false;
	}

	FProcHandle Proc = FWindowsPlatformProcess::CreateProc(*IDEPath, *Solution, true, false, false, nullptr, 0, nullptr, nullptr);
	if (Proc.IsValid())
	{
		FPlatformProcess::CloseProc(Proc);
		// Double IDE workaround
		FPlatformProcess::Sleep(1);
		return true;
	}
	return false;
}

bool FQtCreatorSourceCodeAccessor::DoesSolutionExist() const
{
	FString SolutionPath = GetSolutionPath();
	const FString FullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*SolutionPath);
	return FPaths::FileExists(FullPath);
}

bool FQtCreatorSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
	// Qt Creator should be opened to open files in context of the solution
	if (!OpenSolution()) return false;

	TArray<FString> Stub;
	return OpenFilesInQtCreator(Stub, FullPath, LineNumber, ColumnNumber);
}

bool FQtCreatorSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
	// Qt Creator should be opened to open files in context of the solution
	if (!OpenSolution()) return false;

	return OpenFilesInQtCreator(AbsoluteSourcePaths, FString(""));
}

bool FQtCreatorSourceCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules)
{
	// Path to the .pro file
	FString ProFilePath = FPaths::Combine(
			FPaths::GetPath(GetSolutionPath()),
			FString(SOLUTION_SUBPATH),
			FPaths::GetBaseFilename(GetSolutionPath()).Append(".pro")
	);
	if (!FPaths::FileExists(ProFilePath)) return false;

	// split AbsoluteSourcePaths to headers and sources
	TArray<FString> Headers;
	TArray<FString> Sources;
	for (FString AbsoluteSourcePath : AbsoluteSourcePaths)
	{
		AbsoluteSourcePath.RemoveFromStart(FPaths::GetPath(GetSolutionPath()), ESearchCase::IgnoreCase);
		AbsoluteSourcePath = FString("../..").Append(AbsoluteSourcePath);
		if (AbsoluteSourcePath.Find(FString(".h"), ESearchCase::IgnoreCase, ESearchDir::FromEnd) == AbsoluteSourcePath.Len() - 2)
		{
			Headers.Add(AbsoluteSourcePath);
		}
		if (AbsoluteSourcePath.Find(FString(".cpp"), ESearchCase::IgnoreCase, ESearchDir::FromEnd) == AbsoluteSourcePath.Len() - 4)
		{
			Sources.Add(AbsoluteSourcePath);
		}
	}

	// Read the .pro file into FString
	FString ProFileAsString;
	FFileHelper::LoadFileToString(ProFileAsString, ProFilePath.GetCharArray().GetData());
	// Fix newline characters
	ProFileAsString.Replace(*FString("\r\n"), *FString("\n"), ESearchCase::CaseSensitive);

	// Split .pro string into array
	TArray<FString> ProFileAsArray;
	ProFileAsString.ParseIntoArray(ProFileAsArray, *FString("\n"), false);

	// Read .pro file line by line and generate new content with new files added
	FString NewProFileAsString;
	bool HEADERS_Found{false};
	bool SOURCES_Found{false};
	for (FString ProString : ProFileAsArray)
	{
		// flag that we found HEADERS list
		if (ProString.Contains(TEXT("HEADERS += "), ESearchCase::CaseSensitive))
		{
			HEADERS_Found = true;
			// we're not at SOURCES list any more
			SOURCES_Found = false;
		}
		// flag that we found SOURCES list
		if (ProString.Contains(TEXT("SOURCES += "), ESearchCase::CaseSensitive))
		{
			SOURCES_Found = true;
			// we're not at HEADERS list any more
			HEADERS_Found = false;
		}

		if (ProString.TrimEnd().IsEmpty())
		{
			if (HEADERS_Found)
			{
				// add header to the HEADERS list
				for (FString Header : Headers)
				{
					NewProFileAsString.Append(FString(" \\\n    ")).Append(Header);
				}
				HEADERS_Found = false;
			}
			if (SOURCES_Found)
			{
				// add source to the SOURCES list
				for (FString Source : Sources)
				{
					NewProFileAsString.Append(FString(" \\\n    ")).Append(Source);
				}
				SOURCES_Found = false;
			}
		}

		if (!NewProFileAsString.IsEmpty())
		{
			NewProFileAsString.Append(FString("\n"));
		}
		NewProFileAsString.Append(ProString);
	}

	// rewrite .pro file
	FFileHelper::SaveStringToFile(NewProFileAsString, *ProFilePath);

	// Workaround for "file is outside the project directory" warning
	// TODO: not always works. Switching between 2 opened files may help.
	FPlatformProcess::Sleep(1);

	return true;
}

bool FQtCreatorSourceCodeAccessor::SaveAllOpenDocuments() const
{
	// Can't find when this method is called
	STUBBED("FQtCreatorSourceCodeAccessor::SaveAllOpenDocuments");
	return false;
}

void FQtCreatorSourceCodeAccessor::Tick(const float DeltaTime)
{
}

bool FQtCreatorSourceCodeAccessor::IsIDERunning(int32& PID)
{
	PID = 0;

	HANDLE ProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (ProcessSnap == INVALID_HANDLE_VALUE) return false;

	PROCESSENTRY32 PE32;
	PE32.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(ProcessSnap, &PE32))
	{
		CloseHandle(ProcessSnap);
		return false;
	}

	do
	{
		FString ExeFile = FString(PE32.szExeFile);
		if (ExeFile.Equals(TEXT("qtcreator.exe")))
		{
			PID = static_cast<int32>(PE32.th32ProcessID);
			break;
		}
	}
	while (Process32Next(ProcessSnap, &PE32));

	CloseHandle(ProcessSnap);

	return PID > 0;
}

bool FQtCreatorSourceCodeAccessor::CanRunQtCreator(FString& IDEPath) const
{
	// assuming Qt Creator installed in the default path
	if (!FPaths::DirectoryExists(TEXT(QT_PATH))) return false;

	class FQtVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		TArray<FString> QtCreatorPossiblePaths;
		FQtVisitor()
		{
		}
		virtual bool Visit(const TCHAR* Filename, bool bIsDirectory)
		{
			if (bIsDirectory)
			{
				QtCreatorPossiblePaths.Add(FString(Filename));
			}
			return true; // continue searching
		}
	};
	TArray<FString> QtCreatorExePaths
	{
		"bin/qtcreator.exe",
		"Tools/QtCreator/bin/qtcreator.exe"
	};
	FQtVisitor QtVistor;
	FFileManagerGeneric::Get().IterateDirectory(TEXT(QT_PATH), QtVistor);
	if (QtVistor.QtCreatorPossiblePaths.Num() == 0)
	{
		return false;
	}
	QtVistor.QtCreatorPossiblePaths.Sort();
	FString QtCreatorPossiblePath;
	while (QtVistor.QtCreatorPossiblePaths.Num() > 0)
	{
		QtCreatorPossiblePath = QtVistor.QtCreatorPossiblePaths.Pop();
		for (const FString QtCreatorExePath : QtCreatorExePaths)
		{
			IDEPath = FPaths::Combine(QtCreatorPossiblePath, QtCreatorExePath);
			if (FPaths::FileExists(IDEPath))
			{
				return true;
			}
		}
	}

	return false;
}

FString FQtCreatorSourceCodeAccessor::GetSolutionPath() const
{
	if(IsInGameThread())
	{
		FString SolutionPath;
		if(FDesktopPlatformModule::Get()->GetSolutionPath(SolutionPath))
		{
			FMessageLog("DevLog").Error(FText::FromString(TEXT("Init solution path")));
			CachedSolutionPath = FPaths::ConvertRelativePathToFull(SolutionPath);
			FMessageLog("DevLog").Error(FText::FromString(CachedSolutionPath));

			// here we check if Qt Creator project is set up and, if not, initialize it
			InitQtCreatorProject();
		}
	}
	return CachedSolutionPath;
}

bool FQtCreatorSourceCodeAccessor::OpenFilesInQtCreator(
		const TArray<FString>& FilePaths,
		const FString& FilePath,
		int32 LineNumber,
		int32 ColumnNumber
)
{
	if (FilePaths.Num() == 0 && FilePath.IsEmpty()) return false;

	FString IDEArguments;

	FString IDEPath;
	if (!CanRunQtCreator(IDEPath)) return false;
	int32 PID;
	if (!IsIDERunning(PID)) return false;

	// PID
	IDEArguments.Append("-pid ").Append(FString::FromInt(PID)).Append(" ");
	// Solution path
	IDEArguments.Append(FPaths::Combine(FPaths::GetPath(GetSolutionPath()), TEXT(SOLUTION_SUBPATH)));

	// File path
	if (FilePaths.Num() > 0)
	{
		for (auto OneFile : FilePaths)
		{
			IDEArguments.Append(" ").Append(OneFile);
		}
	}
	else
	{
		IDEArguments.Append(" ")
				// space in path workaround
				.Append(FString("\"")).Append(FilePath).Append(FString("\""))
				.Append(":").Append(FString::FromInt(LineNumber))
				.Append(":").Append(FString::FromInt(ColumnNumber));
	}

	FProcHandle Proc = FWindowsPlatformProcess::CreateProc(*IDEPath, *IDEArguments, true, false, false, nullptr, 0, nullptr, nullptr);
	if (Proc.IsValid())
	{
		FPlatformProcess::CloseProc(Proc);
		return true;
	}
	return false;
}

void FQtCreatorSourceCodeAccessor::InitQtCreatorProject()
{
	if (bQtCretorProjectInitialized) return;

	// search for %project_name%.pro.user in ProjectFiles folder
	FString QtCreatorProjectFilePath = FPaths::Combine(
		FPaths::GetPath(GetSolutionPath()),
		TEXT(SOLUTION_SUBPATH),
		FPaths::GetBaseFilename(GetSolutionPath()).Append(".pro.user")
	);
	FMessageLog("DevLog").Error(FText::FromString(QtCreatorProjectFilePath));

	if (FPaths::FileExists(QtCreatorProjectFilePath))
	{
		FMessageLog("DevLog").Error(FText::FromString(TEXT("Found project file")));

		bQtCretorProjectInitialized = true;
		return;
	}

	// if no such file, do initializaion
}

#undef LOCTEXT_NAMESPACE
