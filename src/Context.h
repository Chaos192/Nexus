///----------------------------------------------------------------------------------------------------
/// Copyright (c) Raidcore.GG - All rights reserved.
///
/// Name         :  Context.h
/// Description  :  Contains the main context.
/// Authors      :  K. Bieniek
///----------------------------------------------------------------------------------------------------

#include "Events/EventHandler.h"
#include "Inputs/GameBinds/GameBindsHandler.h"
#include "Inputs/InputBinds/InputBindHandler.h"
#include "Inputs/RawInput/RawInputApi.h"
#include "Loader/AddonVersion.h"
#include "Services/DataLink/DataLink.h"
#include "Services/Localization/Localization.h"
#include "Services/Logging/LogHandler.h"
#include "Services/Mumble/Reader.h"
#include "Services/Settings/Settings.h"
#include "Services/Textures/TextureLoader.h"
#include "Services/Updater/Updater.h"
#include "UI/UiContext.h"

#ifndef CONTEXT_H
#define CONTEXT_H

class CContext
{
	public:
	static CContext* GetContext();

	CContext(CContext const&)       = delete;
	void operator=(CContext const&) = delete;

	AddonVersion const& GetVersion();

	const char* GetBuild();

	void SetModule(HMODULE aModule);

	HMODULE GetModule();

	DWORD GetModuleSize();

	CLogHandler* GetLogger();

	CLocalization* GetLocalization();

	CUpdater* GetUpdater();

	CTextureLoader* GetTextureService();

	CDataLink* GetDataLink();

	CEventApi* GetEventApi();

	CRawInputApi* GetRawInputApi();

	CInputBindApi* GetInputBindApi();

	CGameBindsApi* GetGameBindsApi();

	CUiContext* GetUIContext();

	CSettings* GetSettingsCtx();

	CMumbleReader* GetMumbleReader();

	private:
	CContext() = default;

	HMODULE Module;
	DWORD   ModuleSize;
};

#endif
