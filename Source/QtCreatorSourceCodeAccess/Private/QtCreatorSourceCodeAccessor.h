// Implementation of ISourceCodeAccessor
#pragma once

#include "CoreMinimal.h"
#include "ISourceCodeAccessor.h"

class FQtCreatorSourceCodeAccessor : public ISourceCodeAccessor
{
public:
	/** ISourceCodeAccessor implementation */
	virtual void RefreshAvailability() override { }
	virtual bool CanAccessSourceCode() const override;
	virtual FName GetFName() const override;
	virtual FText GetNameText() const override;
	virtual FText GetDescriptionText() const override;
	virtual bool OpenSolution() override;
	virtual bool OpenSolutionAtPath(const FString& InSolutionPath) override;
	virtual bool DoesSolutionExist() const override;
	virtual bool OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber = 0) override;
	virtual bool OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths) override;
	virtual bool AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules) override;
	virtual bool SaveAllOpenDocuments() const override;
	virtual void Tick(const float DeltaTime) override;

private:

	/** Solution path */
	mutable FString CachedSolutionPath;

	mutable bool bQtCretorProjectInitialized = false;

	/** Checks if QtCreator is already running */
	bool IsIDERunning(int32& PID);

	/** Checks if Qt Creator is present in the system, stores found path in given parameter **/
	bool CanRunQtCreator(FString& IDEPath) const;

	/** Gets solution path */
	FString GetSolutionPath() const;

	bool OpenFilesInQtCreator(
		const TArray<FString>& FilePaths,
		const FString& FilePath,
		int32 LineNumber = 0,
		int32 ColumnNumber = 0
	);

	/** Auto Qt Creator Project initialization **/
	void InitQtCreatorProject(const FString& SolutionPath) const;
};
