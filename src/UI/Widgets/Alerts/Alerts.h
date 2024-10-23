///----------------------------------------------------------------------------------------------------
/// Copyright (c) Raidcore.GG - All rights reserved.
///
/// Name         :  Alerts.h
/// Description  :  Contains the logic for the Alerts HUD element.
/// Authors      :  K. Bieniek
///----------------------------------------------------------------------------------------------------

#ifndef ALERTS_H
#define ALERTS_H

#include <mutex>
#include <string>
#include <vector>

#include "Loader/NexusLinkData.h"
#include "Services/DataLink/DataLink.h"
#include "UI/Controls/CtlWindow.h"

///----------------------------------------------------------------------------------------------------
/// CAlerts Class
///----------------------------------------------------------------------------------------------------
class CAlerts : public virtual IWindow
{
	public:
	///----------------------------------------------------------------------------------------------------
	/// ctor
	///----------------------------------------------------------------------------------------------------
	CAlerts(CDataLink* aDataLink);

	///----------------------------------------------------------------------------------------------------
	/// dtor
	///----------------------------------------------------------------------------------------------------
	~CAlerts();

	///----------------------------------------------------------------------------------------------------
	/// Render:
	/// 	Renders the Alerts.
	///----------------------------------------------------------------------------------------------------
	void Render() override;

	///----------------------------------------------------------------------------------------------------
	/// Notify:
	/// 	Queues a new alert.
	///----------------------------------------------------------------------------------------------------
	void Notify(const char* aMessage);

	private:
	struct Alert
	{
		std::string    Message;
		double         StartTime  = 0;
	};

	NexusLinkData*     NexusLink;

	std::mutex         Mutex;
	std::vector<Alert> Queue;

	float              Opacity    = 1.0f;
};

#endif
