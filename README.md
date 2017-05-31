# QtCreatorSourceCodeAccess
Source code accessor plugin for Unreal Editor 4, windows version. It alows to manage C++ code in beautiful Qt Creator instead of default source code editor overly tied to the UE4.

# Installation

1. Read Unreal Engine Wiki for [how to setup your project for use with Qt Creator](https://wiki.unrealengine.com/Using_QtCreator_With_UnrealEngine4)

## As an editor plugin
2. (As an editor plugin) Download [build]https://github.com/xBazilio/UE4_Win_QtCreatorSourceCodeAccess/releases/download/2.0/QtCreatorSourceCodeAccess.zip) and put it in the `UNREAL_ENGINE_4/Plugins/Developer/QtCreatorSourceCodeAccess` folder, so that you have `QtCreatorSourceCodeAccess.uplugin` file in path `UNREAL_ENGINE_4/Plugins/Developer/QtCreatorSourceCodeAccess/QtCreatorSourceCodeAccess.uplugin`

## As a project plugin

2. Download [build]https://github.com/xBazilio/UE4_Win_QtCreatorSourceCodeAccess/releases/download/2.0/QtCreatorSourceCodeAccess.zip) or source code and put it in the `YOUR_PROJECT/Plugins/QtCreatorSourceCodeAccess` folder, so that you have `QtCreatorSourceCodeAccess.uplugin` file in path `YOUR_PROJECT/Plugins/QtCreatorSourceCodeAccess/QtCreatorSourceCodeAccess.uplugin`
3. Open your project. If it suggests to recompile the plugin - accept

## Settings

4. Go to Settings / Plugins, find Qt Creator Source Code Access plugin under "Project / Programming" or "Installed / Programming" category and enable it if not yet, reopen project if necessary
5. Go to Edit / Editor preferences, under General / Source Code category select Qt Creator as Source Code Editor
6. (Optional) Disable Visual Studio Plugin, you do not need it any more, but you do need Visual Studio installed itself

From now on you can create C++ classes and compile your project directly from Unreal Editor. Happy coding!
