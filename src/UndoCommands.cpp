/***************************************************************************
 *   Copyright (C) 2024 by Lecker Kebap - Leris@mudlet.org                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "UndoCommands.h"
#include "dlgTriggerEditor.h"
#include "TTrigger.h"
#include "TAlias.h"
#include "TKey.h"
#include "TScript.h"
#include "TTimer.h"
#include "TAction.h"
#include "TVar.h"
#include "Host.h"
#include "KeyUnit.h"
#include "ActionUnit.h"
#include "XMLexport.h"
#include "XMLimport.h"

PropertyEditCommand::PropertyEditCommand(dlgTriggerEditor* editor,
                                           QVariant item,
                                           const QVariantMap& oldState,
                                           const QVariantMap& newState)
    : mEditor(editor)
    , mItem(item)
    , mOldState(oldState)
    , mNewState(newState)
{
    if (item.canConvert<TTrigger*>()) {
        setText(QObject::tr("Trigger property change"));
    } else if (item.canConvert<TAlias*>()) {
        setText(QObject::tr("Alias property change"));
    } else if (item.canConvert<TKey*>()) {
        setText(QObject::tr("Key property change"));
    } else if (item.canConvert<TScript*>()) {
        setText(QObject::tr("Script property change"));
    } else if (item.canConvert<TTimer*>()) {
        setText(QObject::tr("Timer property change"));
    } else if (item.canConvert<TAction*>()) {
        setText(QObject::tr("Action property change"));
    }
}

void PropertyEditCommand::undo()
{
    applyState(mOldState);
}

void PropertyEditCommand::redo()
{
    applyState(mNewState);
}

void PropertyEditCommand::applyState(const QVariantMap& state)
{
    if (mItem.canConvert<TTrigger*>()) {
        TTrigger* trigger = mItem.value<TTrigger*>();
        if (!trigger) return;
        trigger->setName(state["name"].toString());
        trigger->setCommand(state["command"].toString());
        trigger->setRegexCodeList(state["patterns"].toStringList(), state["patternKinds"].value<QList<int>>());
        trigger->setScript(state["script"].toString());
        trigger->setIsMultiline(state["isMultiline"].toBool());
        trigger->mPerlSlashGOption = state["perlSlashGOption"].toBool();
        trigger->mFilterTrigger = state["filterTrigger"].toBool();
        trigger->mStayOpen = state["stayOpen"].toInt();
        trigger->mSoundTrigger = state["soundTrigger"].toBool();
        trigger->setSound(state["soundFile"].toString());
        trigger->setIsColorizerTrigger(state["isColorizerTrigger"].toBool());
        trigger->setColorizerFgColor(state["fgColor"].value<QColor>());
        trigger->setColorizerBgColor(state["bgColor"].value<QColor>());
        trigger->setConditionLineDelta(state["conditionLineDelta"].toInt());
        trigger->state(); // re-evaluate state
    } else if (mItem.canConvert<TAlias*>()) {
        TAlias* alias = mItem.value<TAlias*>();
        if (!alias) return;
        alias->setName(state["name"].toString());
        alias->setCommand(state["command"].toString());
        alias->setRegexCode(state["regexCode"].toString());
        alias->setScript(state["script"].toString());
        alias->state();
    } else if (mItem.canConvert<TKey*>()) {
        TKey* key = mItem.value<TKey*>();
        if (!key) return;
        key->setName(state["name"].toString());
        key->setCommand(state["command"].toString());
        key->setScript(state["script"].toString());
        key->setKeyCode(static_cast<Qt::Key>(state["keyCode"].toInt()));
        key->setKeyModifiers(static_cast<Qt::KeyboardModifiers>(state["keyModifiers"].toInt()));
        key->state();
    } else if (mItem.canConvert<TScript*>()) {
        TScript* script = mItem.value<TScript*>();
        if (!script) return;
        script->setName(state["name"].toString());
        script->setScript(state["script"].toString());
        script->setEventHandlerList(state["eventHandlers"].toStringList());
        script->compileAll();
    } else if (mItem.canConvert<TTimer*>()) {
        TTimer* timer = mItem.value<TTimer*>();
        if (!timer) return;
        timer->setName(state["name"].toString());
        timer->setCommand(state["command"].toString());
        timer->setScript(state["script"].toString());
        timer->setTime(state["time"].toTime());
        timer->state();
    } else if (mItem.canConvert<TAction*>()) {
        TAction* action = mItem.value<TAction*>();
        if (!action) return;
        action->setName(state["name"].toString());
        action->setIcon(state["icon"].toString());
        action->setCommandButtonDown(state["commandDown"].toString());
        action->setCommandButtonUp(state["commandUp"].toString());
        action->setScript(state["script"].toString());
        action->setIsPushDownButton(state["isPushDown"].toBool());
        action->mLocation = state["location"].toInt();
        action->mOrientation = state["orientation"].toInt();
        action->setButtonRotation(state["rotation"].toInt());
        action->setButtonColumns(state["columns"].toInt());
        action->css = state["css"].toString();
        action->setDataChanged();
        if (action->getHost()) {
            action->getHost()->getActionUnit()->updateToolbar();
        }
        action->state();
    }

    mEditor->refreshCurrentItemView();
}

DeleteItemCommand::DeleteItemCommand(dlgTriggerEditor* editor,
                                     QVariant item,
                                     QTreeWidgetItem* treeItem)
    : mEditor(editor)
    , mItem(item)
    , mTreeItem(treeItem)
{
    if (item.canConvert<TTrigger*>()) {
        setText(QObject::tr("Delete trigger"));
        TTrigger* trigger = item.value<TTrigger*>();
        mParentId = trigger->getParent() ? trigger->getParent()->getID() : 0;
        XMLexport exporter(trigger);
        mItemXml = exporter.exportTriggerToString();
    } else if (item.canConvert<TAlias*>()) {
        setText(QObject::tr("Delete alias"));
        TAlias* alias = item.value<TAlias*>();
        mParentId = alias->getParent() ? alias->getParent()->getID() : 0;
        XMLexport exporter(alias);
        mItemXml = exporter.exportAliasToString();
    } else if (item.canConvert<TKey*>()) {
        setText(QObject::tr("Delete key"));
        TKey* key = item.value<TKey*>();
        mParentId = key->getParent() ? key->getParent()->getID() : 0;
        XMLexport exporter(key);
        mItemXml = exporter.exportKeyToString();
    } else if (item.canConvert<TScript*>()) {
        setText(QObject::tr("Delete script"));
        TScript* script = item.value<TScript*>();
        mParentId = script->getParent() ? script->getParent()->getID() : 0;
        XMLexport exporter(script);
        mItemXml = exporter.exportScriptToString();
    } else if (item.canConvert<TTimer*>()) {
        setText(QObject::tr("Delete timer"));
        TTimer* timer = item.value<TTimer*>();
        mParentId = timer->getParent() ? timer->getParent()->getID() : 0;
        XMLexport exporter(timer);
        mItemXml = exporter.exportTimerToString();
    } else if (item.canConvert<TAction*>()) {
        setText(QObject::tr("Delete action"));
        TAction* action = item.value<TAction*>();
        mParentId = action->getParent() ? action->getParent()->getID() : 0;
        XMLexport exporter(action);
        mItemXml = exporter.exportActionToString();
    }
}

void DeleteItemCommand::undo()
{
    XMLimport importer(mEditor->getHost());
    QList<int> imported_ids = importer.importFromXml(mItemXml, mParentId);
    if (!imported_ids.isEmpty()) {
        mEditor->refreshTree(mItem);
        mEditor->selectItem(mItem, imported_ids.first());
    }
}

void DeleteItemCommand::redo()
{
    mEditor->deleteItem(mItem, mTreeItem, false);
}

AddItemCommand::AddItemCommand(dlgTriggerEditor* editor,
                               QVariant item,
                               QTreeWidgetItem* treeItem)
    : mEditor(editor)
    , mItem(item)
    , mTreeItem(treeItem)
{
    if (item.canConvert<TTrigger*>()) {
        setText(QObject::tr("Add trigger"));
    } else if (item.canConvert<TAlias*>()) {
        setText(QObject::tr("Add alias"));
    } else if (item.canConvert<TKey*>()) {
        setText(QObject::tr("Add key"));
    } else if (item.canConvert<TScript*>()) {
        setText(QObject::tr("Add script"));
    } else if (item.canConvert<TTimer*>()) {
        setText(QObject::tr("Add timer"));
    } else if (item.canConvert<TAction*>()) {
        setText(QObject::tr("Add action"));
    }
}

void AddItemCommand::undo()
{
    mEditor->deleteItem(mItem, mTreeItem, false);
}

void AddItemCommand::redo()
{
    mEditor->addItem(mItem, mTreeItem);
}