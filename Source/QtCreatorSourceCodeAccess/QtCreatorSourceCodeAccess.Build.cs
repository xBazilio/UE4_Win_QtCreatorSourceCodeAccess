// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QtCreatorSourceCodeAccess : ModuleRules
{
	public QtCreatorSourceCodeAccess(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"QtCreatorSourceCodeAccess/Public"
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"QtCreatorSourceCodeAccess/Private"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"SourceCodeAccess",
					"DesktopPlatform",
				}
		);

		if (UEBuildConfiguration.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("HotReload");
		}
	}
}
