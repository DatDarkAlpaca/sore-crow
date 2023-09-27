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
		uncheckAll(menu);
		ignoredAction->setChecked(true);
	}
}