///----------------------------------------------------------------------------------------------------
/// Copyright (c) Raidcore.GG - All rights reserved.
///
/// Name         :  CtlWindow.h
/// Description  :  Contains the functionality for a window.
/// Authors      :  K. Bieniek
///----------------------------------------------------------------------------------------------------

#ifndef CONTROLS_WINDOW_H
#define CONTROLS_WINDOW_H

#include <string>

///----------------------------------------------------------------------------------------------------
	/// IWindow Interface Class
	///----------------------------------------------------------------------------------------------------
class IWindow
{
	public:
	///----------------------------------------------------------------------------------------------------
	/// ctor
	///----------------------------------------------------------------------------------------------------
	IWindow() = default;
	///----------------------------------------------------------------------------------------------------
	/// dtor
	///----------------------------------------------------------------------------------------------------
	~IWindow() = default;

	///----------------------------------------------------------------------------------------------------
	/// Render:
	/// 	Renders the given window.
	///----------------------------------------------------------------------------------------------------
	void Render();

	///----------------------------------------------------------------------------------------------------
	/// GetName:
	/// 	Returns the name of the window.
	///----------------------------------------------------------------------------------------------------
	const std::string& GetName();

	///----------------------------------------------------------------------------------------------------
	/// IsVisible:
	/// 	Returns whether the window is currently being rendered.
	///----------------------------------------------------------------------------------------------------
	bool IsVisible();

	///----------------------------------------------------------------------------------------------------
	/// IsPopOut:
	/// 	Returns whether the window is currently a standalone window (popout) or sub-rendered.
	///----------------------------------------------------------------------------------------------------
	bool IsPopOut();

	protected:
	bool Visible = false;
	bool PoppedOut = false;

	std::string Name;

	///----------------------------------------------------------------------------------------------------
	/// RenderBegin:
	/// 	Should only be called if this->IsPopOut is true. Calls ImGui::Begin.
	///----------------------------------------------------------------------------------------------------
	void RenderBegin();

	///----------------------------------------------------------------------------------------------------
	/// RenderContent:
	/// 	Contains the actual content of the window.
	///----------------------------------------------------------------------------------------------------
	virtual void RenderContent() = 0;

	///----------------------------------------------------------------------------------------------------
	/// RenderEnd:
	/// 	Should only be called if this->IsPopOut is true. Calls ImGui::End.
	///----------------------------------------------------------------------------------------------------
	void RenderEnd();
};

#endif
