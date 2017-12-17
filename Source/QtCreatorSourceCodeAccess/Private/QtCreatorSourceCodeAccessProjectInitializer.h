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
	void CreateDotPriFile(
		const FString& StartTag,
		const FString& EndTag,
		const FString& VarName,
		const FString& FileName
	);
	void CreateIncludesPriFile();
	void CreateDotProDotUserFile();
};
