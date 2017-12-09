/*
 * Copyright (C) 2017 Vasiliy Rumyantsev
 *
 * This file is part of QtCreatorSourceCodeAccess, windows version.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "QtCreatorSourceCodeAccessModule.h"
#include "ModuleManager.h"
#include "Features/IModularFeatures.h"

IMPLEMENT_MODULE(FQtCreatorSourceCodeAccessModule, QtCreatorSourceCodeAccess)

#define LOCTEXT_NAMESPACE "FQtCreatorSourceCodeAccessModule"

void FQtCreatorSourceCodeAccessModule::StartupModule()
{
	// Bind our source control provider to the editor
	IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &QtCreatorSourceCodeAccessor);
}

void FQtCreatorSourceCodeAccessModule::ShutdownModule()
{
	// Unbind provider from editor
	IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &QtCreatorSourceCodeAccessor);
}

#undef LOCTEXT_NAMESPACE
