#pragma once
#include <QMenu>
#include <QAction>

namespace sore
{
	inline void uncheckAll(QMenu* menu)
	{
		for (QAction* action : menu->actions())
			action->setChecked(false);
	}

	inline void uncheckAllButOne(QMenu* menu, QAction* ignoredAction)
	{
		for (QAction* action : menu->actions())
		{
			if (action == ignoredAction)
				continue;

			action->setChecked(false);
		}
	}
}