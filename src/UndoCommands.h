#ifndef MUDLET_UNDOCOMMANDS_H
#define MUDLET_UNDOCOMMANDS_H

#include "pre_guard.h"
#include <QUndoCommand>
#include <QVariantMap>
#include "post_guard.h"

class TTrigger;
class TAlias;
class TKey;
class TScript;
class TTimer;
class TAction;
class TVar;
class dlgTriggerEditor;
class QTreeWidgetItem;

class PropertyEditCommand : public QUndoCommand
{
public:
    PropertyEditCommand(dlgTriggerEditor* editor,
                        QVariant item,
                        const QVariantMap& oldState,
                        const QVariantMap& newState);

    void undo() override;
    void redo() override;

private:
    dlgTriggerEditor* mEditor;
    QVariant mItem;
    QVariantMap mOldState;
    QVariantMap mNewState;
};

class DeleteItemCommand : public QUndoCommand
{
public:
    DeleteItemCommand(dlgTriggerEditor* editor,
                        QVariant item,
                        QTreeWidgetItem* treeItem);

    void undo() override;
    void redo() override;

private:
    dlgTriggerEditor* mEditor;
    QVariant mItem;
    QString mItemXml;
    int mParentId;
    QTreeWidgetItem* mTreeItem;
};

class AddItemCommand : public QUndoCommand
{
public:
    AddItemCommand(dlgTriggerEditor* editor,
                   QVariant item,
                   QTreeWidgetItem* treeItem);
    void undo() override;
    void redo() override;

private:
    dlgTriggerEditor* mEditor;
    QVariant mItem;
    QTreeWidgetItem* mTreeItem;
};

#endif // MUDLET_UNDOCOMMANDS_H