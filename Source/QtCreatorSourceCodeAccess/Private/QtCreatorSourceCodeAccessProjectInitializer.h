#pragma once

#include "CoreMinimal.h"

class FQtCreatorSourceCodeAccessProjectInitializer
{

public:
	FQtCreatorSourceCodeAccessProjectInitializer(const FString SolutionPath) : SolutionPath{SolutionPath}
	{}

	void InitializeProject();

private:
	const FString SolutionPath;
	FString EnginePath;
	FString ProjectName;

	void CreateDotProFile();
	void CreateDefinesPriFile();
	void CreateIncludesPriFile();
	void CreateDotProDotUserFile();
};
