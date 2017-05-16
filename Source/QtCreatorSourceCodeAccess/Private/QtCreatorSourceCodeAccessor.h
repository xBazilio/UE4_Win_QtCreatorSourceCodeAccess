#pragma once

#include "CoreMinimal.h"
#include "ISourceCodeAccessor.h"

class FQtCreatorSourceCodeAccessor : public ISourceCodeAccessor
{
public:
//	/** Initialise internal systems, register delegates etc. */
//	void Startup();

//	/** Shut down internal systems, unregister delegates etc. */
//	void Shutdown();

	/** ISourceCodeAccessor implementation */
	virtual void RefreshAvailability() override { }
	virtual bool CanAccessSourceCode() const override;
	virtual FName GetFName() const override;
	virtual FText GetNameText() const override;
	virtual FText GetDescriptionText() const override;
	virtual bool OpenSolution() override;
	virtual bool OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber = 0) override;
	virtual bool OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths) override;
	virtual bool AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules) override;
	virtual bool SaveAllOpenDocuments() const override;
	virtual void Tick(const float DeltaTime) override;

private:

	/** String storing the solution path obtained from the module manager to avoid having to use it on a thread */
	mutable FString CachedSolutionPath;

	/** Check if QtCreator is already running */
	bool IsIDERunning();

	/** Gets solution path */
	FString GetSolutionPath() const;
};
