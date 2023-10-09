#pragma once
#include <QVariant>
#include <mpv/render_gl.h>
#include "core/mpv/mpv_error.h"

#pragma warning (push)
#pragma warning (disable: 4996)

// Helpers
namespace
{
    void freeNode(mpv_node* node);

    inline bool checkType(const QVariant& value, QMetaType::Type type)
    {
        // The Qt docs say: "Although this function is declared as returning
        // "QVariant::Type(obsolete), the return value should be interpreted
        // as QMetaType::Type."
        // So a cast really seems to be needed to avoid warnings (urgh).
        // Yeah, I don't really want to see if they've changed it in Qt6...
        return static_cast<int>(value.type()) == static_cast<int>(type);
    }

    inline char* qstringToString(const QString& value)
    {
        QByteArray byteArray = value.toUtf8();
        char* string = new char[byteArray.size() + 1];

        if (string)
            std::memcpy(string, byteArray.data(), byteArray.size() + 1);

        return string;
    }

    inline mpv_node_list* createNodeList(mpv_node* node, bool isMapFormat, int amount)
    {
        node->format = isMapFormat ? MPV_FORMAT_NODE_MAP : MPV_FORMAT_NODE_ARRAY;

        mpv_node_list* list = new mpv_node_list();
        node->u.list = list;

        if (!list)
        {
            freeNode(node);
            return nullptr;
        }

        list->values = new mpv_node[amount]();

        if (!list->values)
        {
            freeNode(node);
            return nullptr;
        }

        if (isMapFormat)
        {
            list->keys = new char* [amount]();

            if (!list->keys)
            {
                freeNode(node);
                return nullptr;
            }
        }

        return list;
    }

    inline QVariant nodeToVariant(const mpv_node* node)
    {
        switch (node->format)
        {
            case MPV_FORMAT_STRING:
                return QVariant(QString::fromUtf8(node->u.string));

            case MPV_FORMAT_FLAG:
                return QVariant(static_cast<bool>(node->u.flag));

            case MPV_FORMAT_INT64:
                return QVariant(static_cast<qlonglong>(node->u.int64));

            case MPV_FORMAT_DOUBLE:
                return QVariant(node->u.double_);

            case MPV_FORMAT_NODE_ARRAY:
            {
                mpv_node_list* list = node->u.list;
                QVariantList qlist;

                for (int i = 0; i < list->num; ++i)
                    qlist.append(nodeToVariant(&list->values[i]));

                return QVariant(qlist);
            } break;

            case MPV_FORMAT_NODE_MAP:
            {
                mpv_node_list* list = node->u.list;
                QVariantMap qmap;

                for (int i = 0; i < list->num; ++i) 
                {
                    qmap.insert(QString::fromUtf8(list->keys[i]),
                        nodeToVariant(&list->values[i]));
                }

                return QVariant(qmap);

            } break;

            default:
                return QVariant();
        }
    }

    inline void set(mpv_node* node, const QVariant& variant)
    {
        if (checkType(variant, QMetaType::Bool))
        {
            node->format = MPV_FORMAT_FLAG;
            node->u.flag = variant.toBool() ? 1 : 0;
        }

        else if (checkType(variant, QMetaType::Int) || checkType(variant, QMetaType::LongLong) || checkType(variant, QMetaType::UInt) || checkType(variant, QMetaType::ULongLong))
        {
            node->format = MPV_FORMAT_INT64;
            node->u.int64 = variant.toLongLong();
        }

        else if (checkType(variant, QMetaType::Double))
        {
            node->format = MPV_FORMAT_DOUBLE;
            node->u.double_ = variant.toDouble();
        }

        else if (checkType(variant, QMetaType::QString))
        {
            node->format = MPV_FORMAT_STRING;
            node->u.string = qstringToString(variant.toString());

            if (!node->u.string)
                node->format = MPV_FORMAT_NONE;
        }

        else if (variant.canConvert<QVariantList>())
        {
            QVariantList qlist = variant.toList();

            mpv_node_list* list = createNodeList(node, false, (int)qlist.size());
            if (!list)
            {
                node->format = MPV_FORMAT_NONE;
                return;
            }

            list->num = (int)qlist.size();

            for (int i = 0; i < qlist.size(); ++i)
                set(&list->values[i], qlist[i]);
        }

        else if (variant.canConvert<QVariantMap>())
        {
            QVariantMap qmap = variant.toMap();
            mpv_node_list* list = createNodeList(node, true, (int)qmap.size());

            if (!list)
            {
                node->format = MPV_FORMAT_NONE;
                return;
            }

            list->num = (int)qmap.size();

            for (int i = 0; i < qmap.size(); ++i)
            {
                list->keys[i] = qstringToString(qmap.keys()[i]);

                if (!list->keys[i])
                {
                    freeNode(node);
                    node->format = MPV_FORMAT_NONE;
                }

                set(&list->values[i], qmap.values()[i]);
            }
        }

        else
            node->format = MPV_FORMAT_NONE;
    }

    void freeNode(mpv_node* node)
    {
        switch (node->format)
        {
            case MPV_FORMAT_STRING:
                delete[] node->u.string;
                break;

            case MPV_FORMAT_NODE_ARRAY:
            case MPV_FORMAT_NODE_MAP:
            {
                mpv_node_list* list = node->u.list;

                if (!list)
                    break;

                for (int i = 0; i < list->num; ++i)
                {
                    if (list->keys)
                        delete[] list->keys[i];

                    if (list->values)
                        freeNode(&list->values[i]);
                }

                delete[] list->keys;
                delete[] list->values;

                delete list;
            } break;
        }

        node->format = MPV_FORMAT_NONE;
    }
}

// NodeBuilder & Node
namespace sore
{
    struct NodeBuilder
    {
        NodeBuilder(const QVariant& variant) 
        {
            set(&m_Node, variant);
        }

        ~NodeBuilder() 
        {
            freeNode(&m_Node);
        }

    public:
        mpv_node* node() { return &m_Node; }

    private:
        mpv_node m_Node;
    };

    struct MPVNode
    {
    public:
        MPVNode(mpv_node* node = nullptr) 
            : nodeData(node)
        {

        }
        
        ~MPVNode() 
        { 
            mpv_free_node_contents(nodeData);
        }

    public:
        mpv_node* nodeData = nullptr;
    };
}

// Node Utils:
namespace sore::mpv
{
    inline QVariant getProperty(mpv_handle* handle, const QString& name)
    {
        mpv_node node;
        int error = mpv_get_property(handle, name.toUtf8().data(), MPV_FORMAT_NODE, &node);

        if (error < 0)
            return QVariant::fromValue(NodeError(error));

        MPVNode f(&node);
        return nodeToVariant(&node);
    }

    inline int setProperty(mpv_handle* handle, const QString& propertyName, const QVariant& value)
    {
        NodeBuilder node(value);
        return mpv_set_property(handle, propertyName.toUtf8().data(), MPV_FORMAT_NODE, node.node());
    }

    inline int setPropertyAsync(mpv_handle* handle, const QString& propertyName, const QVariant& value, int id = 0)
    {
        NodeBuilder node(value);
        return mpv_set_property_async(handle, id, propertyName.toUtf8().data(), MPV_FORMAT_NODE, node.node());
    }

    inline int setOption(mpv_handle* handle, const QString& propertyName, const QVariant& value)
    {
        NodeBuilder node(value);
        return mpv_set_option(handle, propertyName.toUtf8().data(), MPV_FORMAT_NODE, node.node());
    }
}

// Property Utils:
namespace sore::mpv
{
    inline QVariant propertyToVariant(mpv_event_property* propertyData)
    {
        QVariant data;

        switch (propertyData->format)
        {
            case MPV_FORMAT_DOUBLE:
                data = *reinterpret_cast<double*>(propertyData->data);
                break;
                
            case MPV_FORMAT_STRING:
                data = QString::fromStdString(*reinterpret_cast<char**>(propertyData->data));
                break;

            case MPV_FORMAT_INT64:
                data = qlonglong(*reinterpret_cast<int64_t*>(propertyData->data));
                break;

            case MPV_FORMAT_FLAG:
                data = *reinterpret_cast<bool*>(propertyData->data);
                break;
            case MPV_FORMAT_NODE:
                data = nodeToVariant(reinterpret_cast<mpv_node*>(propertyData->data));
                break;
        }

        return data;
    }
}

#pragma warning (pop)
