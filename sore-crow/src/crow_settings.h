#pragma once
#include <memory>
#include <QSettings>

namespace sore
{
	inline std::unique_ptr<QSettings> settings = nullptr;
}