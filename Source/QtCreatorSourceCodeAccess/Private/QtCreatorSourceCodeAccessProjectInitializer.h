#pragma once

#include "CoreMinimal.h"

class FQtCreatorSourceCodeAccessProjectInitializer
{

public:
	FQtCreatorSourceCodeAccessProjectInitializer(const FString SolutionPath, const FString ProjectName)
		: SolutionPath{SolutionPath},
		  ProjectName{ProjectName}
	{}

	void InitializeProject();

private:
	const FString SolutionPath;
	const FString ProjectName;
	FString EnginePath;

	void CreateDotProFile();
	void CreateDefinesPriFile();
	void CreateIncludesPriFile();
	void CreateDotProDotUserFile();
};
