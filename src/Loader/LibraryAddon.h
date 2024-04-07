#ifndef LIBRARYADDON_H
#define LIBRARYADDON_H

#include <string>

#include "EUpdateProvider.h"

struct LibraryAddon
{
	bool			IsInstalling = false;
	signed int      Signature;
	std::string		Name;
	std::string		Description;
	EUpdateProvider Provider;
	std::string		DownloadURL;
	std::string		ToSComplianceNotice;
};

#endif