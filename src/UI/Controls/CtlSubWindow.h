///----------------------------------------------------------------------------------------------------
/// Copyright (c) Raidcore.GG - All rights reserved.
///
/// Name         :  CtlSubWindow.h
/// Description  :  Contains the functionality for a sub window.
/// Authors      :  K. Bieniek
///----------------------------------------------------------------------------------------------------

#ifndef CONTROLS_SUBWINDOW_H
#define CONTROLS_SUBWINDOW_H

#include <string>

#include "CtlWindow.h"
#include "Services/Textures/Texture.h"

///----------------------------------------------------------------------------------------------------
/// ISubWindow Interface Class
///----------------------------------------------------------------------------------------------------
class ISubWindow : public virtual IWindow
{
	public:
	///----------------------------------------------------------------------------------------------------
	/// Render:
	/// 	Renders the given window.
	///----------------------------------------------------------------------------------------------------
	void Render() override;

	///----------------------------------------------------------------------------------------------------
	/// RenderSubWindows:
	/// 	Renders the independent sub windows of the window.
	///----------------------------------------------------------------------------------------------------
	virtual void RenderSubWindows();

	///----------------------------------------------------------------------------------------------------
	/// GetIcon:
	/// 	Returns the icon of the window.
	///----------------------------------------------------------------------------------------------------
	const Texture* GetIcon();

	///----------------------------------------------------------------------------------------------------
	/// IsPopOut:
	/// 	Returns whether the window is currently a standalone window (popout) or sub-rendered.
	///----------------------------------------------------------------------------------------------------
	bool IsPopOut();

	///----------------------------------------------------------------------------------------------------
	/// PopIn:
	/// 	Pops the window back in, if it is currently a pop-out.
	///----------------------------------------------------------------------------------------------------
	void PopIn();

	///----------------------------------------------------------------------------------------------------
	/// HasSubWindows:
	/// 	Returns whether the window has independently rendered sub windows.
	///----------------------------------------------------------------------------------------------------
	bool HasSubWindows();

	///----------------------------------------------------------------------------------------------------
	/// GetVisibleStatePtr:
	/// 	Returns a pointer to the popout state bool.
	///----------------------------------------------------------------------------------------------------
	bool* GetVisibleStatePtr() override;

	protected:
	bool                     IsPoppedOut = false;
	bool                     IsHost = false;
	bool                     IsAnchored = false;

	std::string              IconIdentifier;
	int                      IconID;
	Texture*                 Tex_Icon;
	Texture*                 Tex_PopoutIcon;

	///----------------------------------------------------------------------------------------------------
	/// RenderContent:
	/// 	Renders the actual content of the window.
	///----------------------------------------------------------------------------------------------------
	virtual void RenderContent() = 0;
};

#endif
