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
#include "Logging/MessageLog.h"
#include <stdio.h>
#include <stdlib.h>

DEFINE_LOG_CATEGORY_STATIC(LogQtCreatorAccessor, Log, All)

#define LOCTEXT_NAMESPACE "QtCreatorSourceCodeAccessor"
#define QT_PATH "c:/Qt"

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
	//http://doc.qt.io/qtcreator/creator-cli.html

	if (IsIDERunning())
	{
		// attach to the pid
		STUBBED("OpenSolution: if IsIDERunning bring it to the foreground");
		return false;
	}

	FString Solution = FPaths::Combine(FPaths::GetPath(GetSolutionPath()), TEXT("Intermediate/ProjectFiles"));
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
		return true;
	}
	return false;
}

bool FQtCreatorSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
	if (!OpenSolution()) return false;

	UE_LOG(LogQtCreatorAccessor, Warning, TEXT("FQtCreatorSourceCodeAccessor::OpenFileAtLine"));
	FMessageLog("dfdfd").Error(FText::FromString(FullPath));
	STUBBED("FQtCreatorSourceCodeAccessor::OpenFileAtLine");
	return false;
}

bool FQtCreatorSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
	if (!OpenSolution()) return false;
	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::OpenSourceFiles")));
	for (auto SourceFile : AbsoluteSourcePaths)
	{
		FMessageLog("dfdfd").Error(FText::FromString(SourceFile));
	}
	if (IsIDERunning())
	{
		STUBBED("OpenSourceFiles: QtCreator is running");
		return false;
	}

	STUBBED("FQtCreatorSourceCodeAccessor::OpenSourceFiles");
	return false;
}

bool FQtCreatorSourceCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules)
{
	STUBBED("FQtCreatorSourceCodeAccessor::AddSourceFiles");
	return false;
}

bool FQtCreatorSourceCodeAccessor::SaveAllOpenDocuments() const
{
	STUBBED("FQtCreatorSourceCodeAccessor::SaveAllOpenDocuments");
	return false;
}

void FQtCreatorSourceCodeAccessor::Tick(const float DeltaTime)
{
}

bool FQtCreatorSourceCodeAccessor::IsIDERunning()
{
	uint8 OutLinesCount{0};
	FILE* pipe = _popen("tasklist /FI \"IMAGENAME eq qtcreator.exe\" /FO LIST", "r");
	if (!pipe) return false; // we can't continue in this case
	char buffer[1000];
	while (fgets(buffer, sizeof(buffer)-1, pipe) != NULL)
	{
		OutLinesCount++;
	}
	_pclose(pipe);

	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::IsIDERunning()")));
	FMessageLog("dfdfd").Error(FText::FromString(FString::FromInt(OutLinesCount)));

	// TODO hide popping cmd windows

	return OutLinesCount > 1;
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

#undef LOCTEXT_NAMESPACE
