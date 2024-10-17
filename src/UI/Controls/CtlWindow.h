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
	/// Render:
	/// 	Renders the given window.
	///----------------------------------------------------------------------------------------------------
	virtual void Render() = 0;

	///----------------------------------------------------------------------------------------------------
	/// GetName:
	/// 	Returns the name of the window.
	///----------------------------------------------------------------------------------------------------
	const std::string& GetName();

	///----------------------------------------------------------------------------------------------------
	/// GetDisplayName:
	/// 	Returns the display name of the window.
	///----------------------------------------------------------------------------------------------------
	const std::string& GetDisplayName();

	///----------------------------------------------------------------------------------------------------
	/// Invalidate:
	/// 	Invalidates all dynamic resources and refreshes them.
	///----------------------------------------------------------------------------------------------------
	virtual void Invalidate();

	///----------------------------------------------------------------------------------------------------
	/// GetVisibleStatePtr:
	/// 	Returns a pointer to the visible state bool.
	///----------------------------------------------------------------------------------------------------
	virtual bool* GetVisibleStatePtr();

	protected:
	std::string              Name;
	std::string              DisplayName;
	bool                     IsVisible = false;
	bool                     IsInvalid = false;
};

#endif
