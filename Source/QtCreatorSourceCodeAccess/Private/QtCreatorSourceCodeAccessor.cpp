// Implementation of ISourceCodeAccessor
/*
 * Copyright (C) 2017 Vasiliy Rumyantsev
 *
 * This file is part of QtCreatorSourceCodeAccess, windows version.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "QtCreatorSourceCodeAccessHeader.h"
#include "QtCreatorSourceCodeAccessor.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Logging/MessageLog.h"
#include "Windows/WindowsHWrapper.h"
#include <TlHelp32.h>

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
	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::OpenSolution()")));

	int32 PID;
	if (IsIDERunning(PID))
	{
		return true;
	}

	FString Solution = FPaths::Combine(FPaths::GetPath(GetSolutionPath()), TEXT(SOLUTION_SUBPATH));
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
		while (!IsIDERunning(PID)) { Sleep(100); }
		return true;
	}
	return false;
}

bool FQtCreatorSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
	// Qt Creator should be opened to open files in context of the solution
	if (!OpenSolution()) return false;

	FMessageLog("dfdfd").Error(FText::FromString(FullPath));
	TArray<FString> Stub;
	return OpenFilesInQtCreator(Stub, FullPath, LineNumber, ColumnNumber);
}

bool FQtCreatorSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
	// Qt Creator should be opened to open files in context of the solution
	if (!OpenSolution()) return false;

	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::OpenSourceFiles")));
	return OpenFilesInQtCreator(AbsoluteSourcePaths, FString(""));
}

bool FQtCreatorSourceCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules)
{
	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::AddSourceFiles")));

	// Path to .pro file
	FString ProFile = FPaths::Combine(
			FPaths::GetPath(GetSolutionPath()),
			FString("Intermediate/ProjectFiles/").Append(FPaths::GetBaseFilename(GetSolutionPath()).Append(".pro"))
	);
	if (!FPaths::FileExists(ProFile)) return false;
	FMessageLog("dfdfd").Error(FText::FromString(ProFile));

	FString WholeFile;
	FFileHelper::LoadFileToString(WholeFile, ProFile.GetCharArray().GetData());
	FMessageLog("dfdfd").Error(FText::FromString(WholeFile));

	TArray<FString> FileAsArray;
	TCHAR Delim = '\n';
	WholeFile.ParseIntoArray(FileAsArray, &Delim, false);

	for (const auto Strings : FileAsArray)
	{
		FMessageLog("dfdfd").Error(FText::FromString(Strings));
	}


	// separate AbsoluteSourcePaths for headers and sources
	// read .pro file in buffer, separate HEADERS and SOURCES bufers
	// add headers to HEADERS, sources to SOURCES
	// rewrite .pro file

	for (const auto AbsoluteSourcePath : AbsoluteSourcePaths)
	{
		FMessageLog("dfdfd").Error(FText::FromString(AbsoluteSourcePath));
	}

	STUBBED("FQtCreatorSourceCodeAccessor::AddSourceFiles");
	return false;
}

bool FQtCreatorSourceCodeAccessor::SaveAllOpenDocuments() const
{
	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::SaveAllOpenDocuments")));
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

	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::IsIDERunning()")));
	FMessageLog("dfdfd").Error(FText::FromString(FString::FromInt(PID)));

	return PID > 0;
}

bool FQtCreatorSourceCodeAccessor::CanRunQtCreator(FString& IDEPath) const
{
	// assuming Qt Creator installed in the default path
	if (!FPaths::DirectoryExists(TEXT(QT_PATH))) return false;

	class FQtVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		FString QtPath;
		FQtVisitor()
		{
		}
		virtual bool Visit(const TCHAR* Filename, bool bIsDirectory)
		{
			if (bIsDirectory)
			{
				QtPath = FString(Filename);
				return false; // stop searching
			}
			return true; // continue searching
		}
	};
	FQtVisitor QtVistor;
	FFileManagerGeneric::Get().IterateDirectory(TEXT(QT_PATH), QtVistor);
	if (!QtVistor.QtPath.IsEmpty())
	{
		IDEPath = FPaths::Combine(QtVistor.QtPath, FString("Tools/QtCreator/bin/qtcreator.exe"));
		return FPaths::FileExists(IDEPath);
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
			CachedSolutionPath = FPaths::ConvertRelativePathToFull(SolutionPath);
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


	FString IDEPath;
	if (!CanRunQtCreator(IDEPath)) return false;
	int32 PID;
	if (!IsIDERunning(PID)) return false;

	FString IDEArguments;

	// PID
	IDEArguments.Append(" -pid ").Append(FString::FromInt(PID)).Append(" ");
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
	FMessageLog("dfdfd").Error(FText::FromString(TEXT("Private FQtCreatorSourceCodeAccessor::OpenFilesInQtCreator()")));
	FMessageLog("dfdfd").Error(FText::FromString(IDEPath));
	FMessageLog("dfdfd").Error(FText::FromString(IDEArguments));

	FProcHandle Proc = FWindowsPlatformProcess::CreateProc(*IDEPath, *IDEArguments, true, false, false, nullptr, 0, nullptr, nullptr);
	if (Proc.IsValid())
	{
		FPlatformProcess::CloseProc(Proc);
		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
