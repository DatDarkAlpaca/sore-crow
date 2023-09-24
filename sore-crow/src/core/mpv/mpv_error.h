#pragma once
#include <QMetaType>

namespace sore
{
    struct NodeError
    {
    public:
        explicit NodeError(int errorCode) noexcept : error(errorCode) { }

    public:
        int error = 0;
    };

    Q_DECLARE_METATYPE(NodeError);
}