#pragma once
#include <QVariant>
#include "mpv_node.h"

namespace sore
{
	enum class SeekFlag
	{
		SEEK_RELATIVE = 0,
		SEEK_ABSOLUTE, 
		SEEK_ABSOLUTE_PERCENT,
		SEEK_RELATIVE_PERCENT,
		SEEK_KEYFRAMES,
		SEEK_EXACT
	};

	inline QString seekFlagString(const SeekFlag& flag)
	{
		switch (flag)
		{
			case SeekFlag::SEEK_RELATIVE: return "relative";
			case SeekFlag::SEEK_ABSOLUTE: return "absolute";
			case SeekFlag::SEEK_ABSOLUTE_PERCENT: return "absolute-percent";
			case SeekFlag::SEEK_RELATIVE_PERCENT: return "relative-percent";
			case SeekFlag::SEEK_KEYFRAMES: return "keyframes";
			case SeekFlag::SEEK_EXACT: return "exact";
			default:
				return "";
		}
	}

	inline QVariant executeCommand(mpv_handle* handle, const QVariant& args)
	{
		NodeBuilder nodeBuilder(args);
		mpv_node node;

		int error = mpv_command_node(handle, nodeBuilder.node(), &node);

		if (error < 0)
			return QVariant::fromValue(NodeError(error));

		MPVNode f(&node);
		return nodeToVariant(&node);
	}

	inline QVariant executeCommandAsync(mpv_handle* handle, const QVariant& args, int id = 0)
	{
		mpv_node node;
		set(&node, args);
		return mpv_command_node_async(handle, id, &node);
	}
}