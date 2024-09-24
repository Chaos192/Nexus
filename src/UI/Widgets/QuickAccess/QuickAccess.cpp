///----------------------------------------------------------------------------------------------------
/// Copyright (c) Raidcore.GG - All rights reserved.
///
/// Name         :  QuickAccess.cpp
/// Description  :  Contains the logic for the Quick Access HUD element.
/// Authors      :  K. Bieniek
///----------------------------------------------------------------------------------------------------

#include "QuickAccess.h"

#include <vector>

#include "imgui/imgui_extensions.h"
#include "imgui/imgui_internal.h"
#include "ImAnimate/ImAnimate.h"

#include "Consts.h"
#include "Renderer.h"
#include "resource.h"
#include "Services/Settings/Settings.h"
#include "Shared.h"
#include "UI/UIContext.h"

#define GW2_QUICKACCESS_ITEMS 10;

float size = 32.0f;

std::string CQuickAccess::EQAVisibilityToString(EQAVisibility aQAVisibility)
{
	switch (aQAVisibility)
	{
		case EQAVisibility::AlwaysShow:
			return "((000047))";
		case EQAVisibility::Gameplay:
			return "((000093))";
		case EQAVisibility::OutOfCombat:
			return "((000094))";
		case EQAVisibility::InCombat:
			return "((000095))";
		case EQAVisibility::Hide:
			return "((000096))";
		default:
			return "(null)";
	}
}

std::string CQuickAccess::EQAPositionToString(EQAPosition aQAPosition)
{
	switch (aQAPosition)
	{
		case EQAPosition::Extend:
			return "((000067))";
		case EQAPosition::Under:
			return "((000068))";
		case EQAPosition::Bottom:
			return "((000069))";
		case EQAPosition::Custom:
			return "((000070))";
		default:
			return "(null)";
	}
}

CQuickAccess::CQuickAccess(CDataLink* aDataLink, CLogHandler* aLogger, CInputBindApi* aInputBindApi, CTextureLoader* aTextureService, CLocalization* aLocalization)
{
	this->NexusLink = (NexusLinkData*)aDataLink->GetResource(DL_NEXUS_LINK);
	this->MumbleLink = (Mumble::Data*)aDataLink->GetResource(DL_MUMBLE_LINK);
	this->Logger = aLogger;
	this->InputBindApi = aInputBindApi;
	this->TextureService = aTextureService;
	this->Language = aLocalization;

	/* load settings */
	if (!Settings::Settings.is_null())
	{
		bool anyDefaultSet = false;

		if (!Settings::Settings[OPT_QAVERTICAL].is_null())
		{
			Settings::Settings[OPT_QAVERTICAL].get_to(this->VerticalLayout);
		}
		else
		{
			Settings::Settings[OPT_QAVERTICAL] = this->VerticalLayout;
			anyDefaultSet = true;
		}

		if (!Settings::Settings[OPT_QALOCATION].is_null())
		{
			Settings::Settings[OPT_QALOCATION].get_to(this->Location);
		}
		else
		{
			Settings::Settings[OPT_QALOCATION] = this->Location;
			anyDefaultSet = true;
		}

		if (!Settings::Settings[OPT_QAOFFSETX].is_null() && !Settings::Settings[OPT_QAOFFSETY].is_null())
		{
			Settings::Settings[OPT_QAOFFSETX].get_to(this->Offset.x);
			Settings::Settings[OPT_QAOFFSETY].get_to(this->Offset.y);
		}
		else
		{
			Settings::Settings[OPT_QAOFFSETX] = this->Offset.x;
			Settings::Settings[OPT_QAOFFSETY] = this->Offset.y;
			anyDefaultSet = true;
		}

		if (!Settings::Settings[OPT_QAVISIBILITY].is_null())
		{
			Settings::Settings[OPT_QAVISIBILITY].get_to(this->Visibility);
		}
		else
		{
			Settings::Settings[OPT_QAVISIBILITY] = this->Visibility;
			anyDefaultSet = true;
		}

		if (anyDefaultSet)
		{
			Settings::Save();
		}
	}
}

CQuickAccess::~CQuickAccess()
{
	this->NexusLink = nullptr;
	this->MumbleLink = nullptr;
	this->Logger = nullptr;
	this->InputBindApi = nullptr;
	this->TextureService = nullptr;
	this->Language = nullptr;
}

void CQuickAccess::Render()
{
	this->NexusLink->QuickAccessIconsCount = static_cast<int>(this->Registry.size());
	this->NexusLink->QuickAccessMode = (int)this->Location;
	this->NexusLink->QuickAccessIsVertical = this->VerticalLayout;

	switch (this->Visibility)
	{
		/* continue rendering */
		default:
		case EQAVisibility::AlwaysShow: { break; }

		case EQAVisibility::Gameplay:
		{
			/* don't render if not gameplay */
			if (!this->NexusLink->IsGameplay)
			{
				return;
			}

			break;
		}
		case EQAVisibility::OutOfCombat:
		{
			/* don't render if not gameplay */
			if (!this->NexusLink->IsGameplay)
			{
				return;
			}

			/* don't render if in combat */
			if (this->MumbleLink->Context.IsInCombat)
			{
				return;
			}

			break;
		}
		case EQAVisibility::InCombat:
		{
			/* don't render if not gameplay */
			if (!this->NexusLink->IsGameplay)
			{
				return;
			}

			/* don't render if out of combat */
			if (!this->MumbleLink->Context.IsInCombat)
			{
				return;
			}

			break;
		}

		/* don't render*/
		case EQAVisibility::Hide: { return; }
	}

	bool isActive = false;

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, this->Opacity);

	ImVec2 pos = ImVec2(0.0f, 0.0f);

	switch (this->Location)
	{
		case EQAPosition::Extend:
			pos.x += (size * Renderer::Scaling) * GW2_QUICKACCESS_ITEMS;
			break;
		case EQAPosition::Under:
			pos.y += size * Renderer::Scaling;
			break;
		case EQAPosition::Bottom:
			pos.y += Renderer::Height - (size * 2 * Renderer::Scaling);
			break;
	}

	pos.x += this->Offset.x;
	pos.y += this->Offset.y;

	ImGui::SetNextWindowPos(pos);
	if (ImGui::Begin("QuickAccess", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar))
	{
		bool menuFound = false;

		unsigned c = 0;
		const std::lock_guard<std::mutex> lock(this->Mutex);
		for (auto& [identifier, shortcut] : this->Registry)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));

			//Logger->Debug(CH_QUICKACCESS, "size: %f | c: %d | scale: %f", size, c, Renderer::Scaling);

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.f, 0.f }); // smol checkbox
			if (this->VerticalLayout)
			{
				ImGui::SetCursorPos(ImVec2(0, ((size * c) + (c ? 1 : 0)) * Renderer::Scaling));
			}
			else
			{
				ImGui::SetCursorPos(ImVec2(((size * c) + (c ? 1 : 0)) * Renderer::Scaling, 0));
			}

			ImVec2 pos = ImGui::GetCursorPos();

			bool iconHovered = false;

			if (shortcut.TextureNormal && shortcut.TextureNormal->Resource &&
				shortcut.TextureHover && shortcut.TextureHover->Resource)
			{
				if (ImGui::ImageButton(!shortcut.IsHovering ? shortcut.TextureNormal->Resource : shortcut.TextureHover->Resource, ImVec2(size * Renderer::Scaling, size * Renderer::Scaling)))
				{
					isActive = true;
					if (shortcut.InputBind.length() > 0)
					{
						shortcut.HasNotification = false;
						this->InputBindApi->Invoke(shortcut.InputBind);
					}
				}
				iconHovered = ImGui::IsItemHovered();
			}
			else if (shortcut.TextureGetAttempts < 10)
			{
				shortcut.TextureNormal = this->TextureService->Get(shortcut.TextureNormalIdentifier.c_str());
				shortcut.TextureHover = this->TextureService->Get(shortcut.TextureHoverIdentifier.c_str());
				shortcut.TextureGetAttempts++;
			}
			else
			{
				this->Logger->Warning(CH_QUICKACCESS, "Cancelled getting textures for shortcut \"%s\" after 10 failed attempts.", identifier.c_str());

				/* fallback icons */
				shortcut.TextureNormal = this->TextureService->Get(ICON_GENERIC);
				shortcut.TextureHover = this->TextureService->Get(ICON_GENERIC_HOVER);

				/* absolute sanity check */
				if (shortcut.TextureNormal == nullptr || shortcut.TextureHover == nullptr)
				{
					this->Logger->Warning(CH_QUICKACCESS, "Neither promised textures nor fallback textures could be loaded, removing shortcut \"%s\".", identifier.c_str());

					// call this async because we're currently iterating the list
					std::thread([this, identifier]()
					{
						this->RemoveShortcut(identifier.c_str());
					}).detach();
				}
			}

			bool notifHovered = false;

			if (this->IconNotification && this->IconNotification->Resource)
			{
				if (shortcut.HasNotification)
				{
					float offIcon = (size * Renderer::Scaling) / 2.0f;

					pos.x += offIcon;
					pos.y += offIcon;

					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::SetCursorPos(pos);
					ImGui::Image(this->IconNotification->Resource, ImVec2(offIcon, offIcon));
					ImGui::PopItemFlag();
				}
				notifHovered = ImGui::IsItemHovered();
			}
			else
			{
				this->IconNotification = this->TextureService->GetOrCreate(ICON_NOTIFICATION, RES_ICON_NOTIFICATION, NexusHandle);
			}

			ImGui::PopStyleVar();
			shortcut.IsHovering = iconHovered || notifHovered;
			if (shortcut.TooltipText.length() > 0)
			{
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text(this->Language->Translate(shortcut.TooltipText.c_str()));
					if (shortcut.ContextItems.size() > 0)
					{
						ImGui::TextDisabled(this->Language->Translate("((Right-Click to for more options.))"));
					}
					ImGui::EndTooltip();
				}
			}

			ImGui::PopStyleColor(3);

			c++;

			this->RenderContextMenu(identifier, shortcut, &isActive);
		}

		bool isHoveringNative = false;
		if (this->Location == EQAPosition::Extend)
		{
			ImVec2 mPos = ImGui::GetMousePos();
			if (mPos.x != -FLT_MAX && mPos.y != -FLT_MAX && mPos.x < pos.x - this->Offset.x && mPos.y < Renderer::Scaling * size)
			{
				isHoveringNative = true;
			}
		}

		bool isHovering = ImGui::IsWindowHovered() || isActive || isHoveringNative;

		if (isHovering)
		{
			ImGui::Animate(0.5f, 1, 350, &this->Opacity, ImAnimate::ECurve::InCubic);
		}
		else
		{
			ImGui::Animate(1, 0.5f, 350, &this->Opacity, ImAnimate::ECurve::InCubic);
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

void CQuickAccess::RenderContextMenu(const std::string& aIdentifier, const Shortcut& aShortcut, bool* aIsActive)
{
	if (aShortcut.ContextItems.size() > 0)
	{
		std::string ctxId = "ShortcutsCtxMenu##" + aIdentifier;
		if (ImGui::BeginPopupContextItem(ctxId.c_str()))
		{
			*aIsActive = true;

			int amtItems = aShortcut.ContextItems.size();
			int idx = 0;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.f, 0.f }); // smol checkbox
			for (auto& [cbidentifier, cbshortcut] : aShortcut.ContextItems)
			{
				idx++;

				if (cbshortcut.Callback)
				{
					ImGui::TextDisabled(Loader::GetOwner(cbshortcut.Callback).c_str());
					cbshortcut.Callback();

					if (idx != amtItems)
					{
						ImGui::Separator();
					}
				}
			}
			ImGui::PopStyleVar();

			ImGui::EndPopup();
		}
		ImGui::OpenPopupOnItemClick(ctxId.c_str(), 1);
	}
}

void CQuickAccess::AddShortcut(const char* aIdentifier, const char* aTextureIdentifier, const char* aTextureHoverIdentifier, const char* aInputBindIdentifier, const char* aTooltipText)
{
	std::string str = aIdentifier;
	std::string strTexId = aTextureIdentifier;
	std::string strTexHoverId = aTextureHoverIdentifier;
	std::string strKbId = aInputBindIdentifier;
	std::string strTT = aTooltipText;

	/* explicit scoping due to this->WhereAreMyParents() below */
	{
		const std::lock_guard<std::mutex> lock(this->Mutex);
		if (this->Registry.find(str) == this->Registry.end())
		{
			Texture* normal = this->TextureService->Get(strTexId.c_str());
			Texture* hover = this->TextureService->Get(strTexHoverId.c_str());
			Shortcut sh{};
			sh.TextureNormalIdentifier = aTextureIdentifier;
			sh.TextureHoverIdentifier = aTextureHoverIdentifier;
			sh.TextureNormal = normal;
			sh.TextureHover = hover;
			sh.InputBind = aInputBindIdentifier;
			sh.TooltipText = aTooltipText;
			sh.TextureGetAttempts = 0;
			this->Registry[str] = sh;

			int amt = 0;
			if (sh.TextureNormal != nullptr) { amt++; }
			if (sh.TextureHover != nullptr) { amt++; }
		}
	}

	this->WhereAreMyParents();
}

void CQuickAccess::RemoveShortcut(const char* aIdentifier)
{
	std::string str = aIdentifier;

	const std::lock_guard<std::mutex> lock(this->Mutex);

	auto it = this->Registry.find(str);

	if (it != this->Registry.end())
	{
		for (auto& orphan : it->second.ContextItems)
		{
			this->OrphanedCallbacks[orphan.first] = orphan.second;
		}
	}

	this->Registry.erase(str);
}

void CQuickAccess::NotifyShortcut(const char* aIdentifier)
{
	std::string str = aIdentifier;

	const std::lock_guard<std::mutex> lock(this->Mutex);

	auto it = this->Registry.find(str);

	if (it != this->Registry.end())
	{
		it->second.HasNotification = true;
	}
}

void CQuickAccess::SetNotificationShortcut(const char* aIdentifier, bool aState)
{
	std::string str = aIdentifier;

	const std::lock_guard<std::mutex> lock(this->Mutex);

	auto it = this->Registry.find(str);

	if (it != this->Registry.end())
	{
		it->second.HasNotification = aState;
	}
}

void CQuickAccess::AddContextItem(const char* aIdentifier, GUI_RENDER aShortcutRenderCallback)
{
	this->AddContextItem(aIdentifier, QA_MENU, aShortcutRenderCallback);
}

void CQuickAccess::AddContextItem(const char* aIdentifier, const char* aTargetShortcutIdentifier, GUI_RENDER aShortcutRenderCallback)
{
	std::string str = aIdentifier;
	std::string tarShStr = aTargetShortcutIdentifier ? aTargetShortcutIdentifier : QA_MENU;

	ContextItem shortcut{
		aIdentifier,
		aShortcutRenderCallback
	};

	const std::lock_guard<std::mutex> lock(this->Mutex);

	auto it = this->Registry.find(tarShStr);
	if (it != this->Registry.end())
	{
		it->second.ContextItems[str] = shortcut;
	}
	else
	{
		this->OrphanedCallbacks[str] = shortcut;
	}
}

void CQuickAccess::RemoveContextItem(const char* aIdentifier)
{
	std::string str = aIdentifier;

	const std::lock_guard<std::mutex> lock(this->Mutex);

	for (auto& [identifier, shortcut] : this->Registry)
	{
		shortcut.ContextItems.erase(aIdentifier);
	}

	this->OrphanedCallbacks.erase(aIdentifier);
}

int CQuickAccess::Verify(void* aStartAddress, void* aEndAddress)
{
	int refCounter = 0;

	const std::lock_guard<std::mutex> lock(this->Mutex);

	/* remove active callbacks */
	for (auto& [identifier, shortcut] : this->Registry)
	{
		std::vector<std::string> remove;

		for (auto& [cbidentifier, cbshortcut] : shortcut.ContextItems)
		{
			if (cbshortcut.Callback >= aStartAddress && cbshortcut.Callback <= aEndAddress)
			{
				remove.push_back(cbidentifier);
				refCounter++;
			}
		}

		for (std::string remidentifier : remove)
		{
			shortcut.ContextItems.erase(remidentifier);
		}
	}

	/* remove bastard children */
	std::vector<std::string> remove;

	for (auto& [cbidentifier, cbshortcut] : this->OrphanedCallbacks)
	{
		if (cbshortcut.Callback >= aStartAddress && cbshortcut.Callback <= aEndAddress)
		{
			remove.push_back(cbidentifier);
			refCounter++;
		}
	}

	for (std::string remidentifier : remove)
	{
		this->OrphanedCallbacks.erase(remidentifier);
	}

	return refCounter;
}

void CQuickAccess::WhereAreMyParents()
{
	const std::lock_guard<std::mutex> lock(this->Mutex);

	std::vector<std::string> remove;

	for (auto& [identifier, shortcut] : this->OrphanedCallbacks)
	{
		auto it = this->Registry.find(shortcut.TargetShortcut);
		if (it != this->Registry.end())
		{
			it->second.ContextItems[identifier] = shortcut;
		}
	}

	/* no longer orphans */
	for (std::string remidentifier : remove)
	{
		this->OrphanedCallbacks.erase(remidentifier);
	}
}