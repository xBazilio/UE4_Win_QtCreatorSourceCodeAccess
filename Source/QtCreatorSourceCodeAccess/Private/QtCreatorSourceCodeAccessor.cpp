#pragma once

#include "QtCreatorSourceCodeAccessHeader.h"
#include "QtCreatorSourceCodeAccessor.h"
#include "DesktopPlatformModule.h"

DEFINE_LOG_CATEGORY_STATIC(LogQtCreatorAccessor, Log, All)

#define LOCTEXT_NAMESPACE "QtCreatorSourceCodeAccessor"

bool FQtCreatorSourceCodeAccessor::CanAccessSourceCode() const
{
	// TODO implement checking that Qt Creator is present in system
	return true;
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
	if (IsIDERunning())
	{
		// use qdbus to open the project within session?
		STUBBED("OpenSolution: if IsIDERunning bring it to the foreground");
		return false;
	}

	FString Solution = GetSolutionPath();
	FString IDEPath;
	// TODO implement opening Qt Creator
	STUBBED("OpenSolution: Lounch Qt Creator");

//	if (!CanRunKDevelop(IDEPath))
//	{
//		UE_LOG(LogKDevelopAccessor, Warning, TEXT("FKDevelopSourceCodeAccessor::OpenSourceFiles: cannot find kdevelop binary"));
//		return false;
//	}

//	FProcHandle Proc = FPlatformProcess::CreateProc(*IDEPath, *Solution, true, false, false, nullptr, 0, nullptr, nullptr);
//	if (Proc.IsValid())
//	{
//		FPlatformProcess::CloseProc(Proc);
//		return true;
//	}
	return false;
}

bool FQtCreatorSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
	STUBBED("FQtCreatorSourceCodeAccessor::OpenFileAtLine");
	return false;
}

bool FQtCreatorSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
	if (IsIDERunning())
	{
		// use qdbus
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
	// TODO implement IsIDERunning
	STUBBED("IsIDERunning: Check if QtCreator is running?");
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
