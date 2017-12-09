#pragma once

class FQtCreatorSourceCodeAccessProjectInitializer
{
public:
	void InitializeProject();

private:
	void CreateDotProFile();
	void CreateDefinesPriFile();
	void CreateIncludesPriFile();
	void CreateDotProDotUserFile();
};
