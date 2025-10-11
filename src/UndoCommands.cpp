#include "UndoCommands.h"

#include "TAction.h"
#include "TAlias.h"
#include "TKey.h"
#include "TScript.h"
#include "TTimer.h"
#include "TTrigger.h"
#include "dlgTriggerEditor.h"

// AddItemCommand
AddItemCommand::AddItemCommand(dlgTriggerEditor* editor, bool isFolder,
                               TTrigger* parent, QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd),
      mEditor(editor),
      mIsFolder(isFolder),
      mParent(parent),
      mNewTrigger(nullptr)
{
}

void AddItemCommand::undo()
{
    if (mNewTrigger) {
        mRow = mEditor->getTriggerRow(mNewTrigger);
        mEditor->delete_trigger_for_undo(mNewTrigger);
    }
}

void AddItemCommand::redo()
{
    if (!mNewTrigger) {
        mNewTrigger = mEditor->addTrigger(mIsFolder, mParent, false);
    } else {
        mEditor->addTrigger(mNewTrigger, mParent, mRow);
    }
    setText(QObject::tr("Add Trigger %1").arg(mNewTrigger->getName()));
}

// DeleteTriggerCommand
DeleteTriggerCommand::DeleteTriggerCommand(dlgTriggerEditor* editor,
                                           TTrigger* trigger,
                                           QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd), mEditor(editor), mTrigger(trigger)
{
    mRow = mEditor->getTriggerRow(trigger);
    mParent = mTrigger->getParent();
    setText(QObject::tr("Delete Trigger %1").arg(mTrigger->getName()));
}

void DeleteTriggerCommand::undo()
{
    mEditor->addTrigger(mTrigger, mParent, mRow);
}

void DeleteTriggerCommand::redo()
{
    mEditor->delete_trigger_for_undo(mTrigger);
}

// MoveTriggerCommand
MoveTriggerCommand::MoveTriggerCommand(dlgTriggerEditor* editor, TTrigger* trigger,
                                       TTrigger* oldParent, TTrigger* newParent,
                                       int oldRow, QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd),
      mEditor(editor),
      mTrigger(trigger),
      mOldParent(oldParent),
      mNewParent(newParent),
      mOldRow(oldRow)
{
    setText(QObject::tr("Move Trigger %1").arg(mTrigger->getName()));
}

void MoveTriggerCommand::undo()
{
    mEditor->addTrigger(mTrigger, mOldParent, mOldRow);
    mEditor->updateTriggerView(mTrigger);
}

void MoveTriggerCommand::redo()
{
    mEditor->addTrigger(mTrigger, mNewParent, -1); // Appends to the end
    mEditor->updateTriggerView(mTrigger);
}

// EditTriggerCommand
EditTriggerCommand::EditTriggerCommand(
    dlgTriggerEditor* editor, TTrigger* trigger,
    const QVariantMap& oldProperties, const QVariantMap& newProperties,
    QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd),
      mEditor(editor),
      mTrigger(trigger),
      mOldProperties(oldProperties),
      mNewProperties(newProperties)
{
    setText(QObject::tr("Edit Trigger %1").arg(mTrigger->getName()));
}

void EditTriggerCommand::undo()
{
    mTrigger->fromVariantMap(mOldProperties);
    mEditor->updateTriggerView(mTrigger);
}

void EditTriggerCommand::redo()
{
    mTrigger->fromVariantMap(mNewProperties);
    mEditor->updateTriggerView(mTrigger);
}


// AddAliasCommand
AddAliasCommand::AddAliasCommand(dlgTriggerEditor* editor, bool isFolder,
                               TAlias* parent, QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd),
      mEditor(editor),
      mIsFolder(isFolder),
      mParent(parent),
      mNewAlias(nullptr)
{
}

void AddAliasCommand::undo()
{
    if (mNewAlias) {
        mRow = mEditor->getAliasRow(mNewAlias);
        mEditor->delete_alias_for_undo(mNewAlias);
    }
}

void AddAliasCommand::redo()
{
    if (!mNewAlias) {
        mNewAlias = mEditor->addAlias(mIsFolder, mParent, false);
    } else {
        mEditor->addAlias(mNewAlias, mParent, mRow);
    }
    setText(QObject::tr("Add Alias %1").arg(mNewAlias->getName()));
}

// DeleteAliasCommand
DeleteAliasCommand::DeleteAliasCommand(dlgTriggerEditor* editor, TAlias* alias,
                                       QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd), mEditor(editor), mAlias(alias)
{
    mRow = mEditor->getAliasRow(alias);
    mParent = mAlias->getParent();
    setText(QObject::tr("Delete Alias %1").arg(mAlias->getName()));
}

void DeleteAliasCommand::undo()
{
    mEditor->addAlias(mAlias, mParent, mRow);
}

void DeleteAliasCommand::redo()
{
    mEditor->delete_alias_for_undo(mAlias);
}

// MoveAliasCommand
MoveAliasCommand::MoveAliasCommand(dlgTriggerEditor* editor, TAlias* alias,
                                   TAlias* oldParent, TAlias* newParent,
                                   int oldRow, QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd),
      mEditor(editor),
      mAlias(alias),
      mOldParent(oldParent),
      mNewParent(newParent),
      mOldRow(oldRow)
{
    setText(QObject::tr("Move Alias %1").arg(mAlias->getName()));
}

void MoveAliasCommand::undo()
{
    mEditor->addAlias(mAlias, mOldParent, mOldRow);
    mEditor->updateAliasView(mAlias);
}

void MoveAliasCommand::redo()
{
    mEditor->addAlias(mAlias, mNewParent, -1);
    mEditor->updateAliasView(mAlias);
}

// EditAliasCommand
EditAliasCommand::EditAliasCommand(dlgTriggerEditor* editor, TAlias* alias,
                                     const QVariantMap& oldProperties,
                                     const QVariantMap& newProperties,
                                     QUndoCommand* parentCmd)
    : QUndoCommand(parentCmd),
      mEditor(editor),
      mAlias(alias),
      mOldProperties(oldProperties),
      mNewProperties(newProperties)
{
    setText(QObject::tr("Edit Alias %1").arg(mAlias->getName()));
}

void EditAliasCommand::undo()
{
    mAlias->fromVariantMap(mOldProperties);
    mEditor->updateAliasView(mAlias);
}

void EditAliasCommand::redo()
{
    mAlias->fromVariantMap(mNewProperties);
    mEditor->updateAliasView(mAlias);
}
