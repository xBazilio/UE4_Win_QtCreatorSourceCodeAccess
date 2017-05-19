#pragma once

#include "QtCreatorSourceCodeAccessHeader.h"
#include "QtCreatorSourceCodeAccessor.h"
#include "DesktopPlatformModule.h"
#include "FileManagerGeneric.h"
#include "Logging/MessageLog.h"

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
	return LOCTEXT("QtCreatorDisplayDesc", "Open source code files in Qt Creator");
}

bool FQtCreatorSourceCodeAccessor::OpenSolution()
{
	UE_LOG(LogQtCreatorAccessor, Warning, TEXT("FQtCreatorSourceCodeAccessor::OpenSolution()"));
	FMessageLog("dfdfd").Error(FText::FromString(TEXT("FQtCreatorSourceCodeAccessor::OpenSolution()")));
	// c:\Qt\Qt5.8.0\Tools\QtCreator\bin\qtcreator.exe d:\Bazilio\unreal\Projects\QtCreatorPlugin\Intermediate\ProjectFiles\QtCreatorPlugin.pro

	//http://doc.qt.io/qtcreator/creator-cli.html

	if (IsIDERunning())
	{
		// use qdbus to open the project within session?
		STUBBED("OpenSolution: if IsIDERunning bring it to the foreground");
		return false;
	}

	FString Solution = FPaths::GetPath(GetSolutionPath());
	FString IDEPath;
	// TODO implement opening Qt Creator
	STUBBED("OpenSolution: Lounch Qt Creator");

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
	UE_LOG(LogQtCreatorAccessor, Warning, TEXT("FQtCreatorSourceCodeAccessor::OpenFileAtLine"));
	FMessageLog("dfdfd").Error(FText::FromString(FullPath));
	FMessageLog("dfdfd").Error(FText::FromString(FString::FromInt(LineNumber)));
	FMessageLog("dfdfd").Error(FText::FromString(FString::FromInt(ColumnNumber)));
	STUBBED("FQtCreatorSourceCodeAccessor::OpenFileAtLine");
	return false;
}

bool FQtCreatorSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
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
	// http://doc.qt.io/qtcreator/creator-cli.html
	// tasklist qtcreator.exe

	// TODO implement IsIDERunning
	STUBBED("IsIDERunning: Check if QtCreator is running?");
	return false;
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
