/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef CPPCODECOMPLETION_H
#define CPPCODECOMPLETION_H

#include <ASTfwd.h>
#include <FullySpecifiedType.h>
#include <cplusplus/Icons.h>
#include <cplusplus/Overview.h>
#include <cplusplus/TypeOfExpression.h>

#include <texteditor/icompletioncollector.h>

#include <QtCore/QObject>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE
class QTextCursor;
QT_END_NAMESPACE

namespace TextEditor {
class ITextEditor;
class BaseTextEditor;
}

namespace CPlusPlus {
class LookupItem;
class ClassOrNamespace;
}

namespace CppTools {
namespace Internal {

class CppModelManager;
class FunctionArgumentWidget;

class CppCodeCompletion : public TextEditor::ICompletionCollector
{
    Q_OBJECT
public:
    explicit CppCodeCompletion(CppModelManager *manager);

    void setObjcEnabled(bool objcEnabled)
    { m_objcEnabled = objcEnabled; }

    TextEditor::ITextEditable *editor() const;
    int startPosition() const;
    QList<TextEditor::CompletionItem> getCompletions();
    bool supportsEditor(TextEditor::ITextEditable *editor);
    bool triggersCompletion(TextEditor::ITextEditable *editor);
    int startCompletion(TextEditor::ITextEditable *editor);
    void completions(QList<TextEditor::CompletionItem> *completions);

    bool typedCharCompletes(const TextEditor::CompletionItem &item, QChar typedChar);
    void complete(const TextEditor::CompletionItem &item, QChar typedChar);
    bool partiallyComplete(const QList<TextEditor::CompletionItem> &completionItems);
    void cleanup();

    QIcon iconForSymbol(CPlusPlus::Symbol *symbol) const;

private:
    void addKeywords();
    void addMacros(const QString &fileName, const CPlusPlus::Snapshot &snapshot);
    void addMacros_helper(const CPlusPlus::Snapshot &snapshot,
                          const QString &fileName,
                          QSet<QString> *processed,
                          QSet<QString> *definedMacros);
    void addCompletionItem(CPlusPlus::Symbol *symbol);

    bool completeInclude(const QTextCursor &cursor);
    void completePreprocessor();

    void globalCompletion(CPlusPlus::Scope *scope);

    bool completeConstructorOrFunction(const QList<CPlusPlus::LookupItem> &results,
                                       int endOfExpression, bool toolTipOnly);

    bool completeMember(const QList<CPlusPlus::LookupItem> &results);
    bool completeScope(const QList<CPlusPlus::LookupItem> &results);

    void completeNamespace(CPlusPlus::ClassOrNamespace *binding);

    void completeClass(CPlusPlus::ClassOrNamespace *b,
                       bool staticLookup = true);

    bool completeConstructors(CPlusPlus::Class *klass);

    bool completeQtMethod(const QList<CPlusPlus::LookupItem> &results,
                          bool wantSignals);

    bool completeSignal(const QList<CPlusPlus::LookupItem> &results)
    { return completeQtMethod(results, true); }

    bool completeSlot(const QList<CPlusPlus::LookupItem> &results)
    { return completeQtMethod(results, false); }

    int findStartOfName(int pos = -1) const;

    int startCompletionHelper(TextEditor::ITextEditable *editor);

    int startCompletionInternal(TextEditor::BaseTextEditor *edit,
                                const QString fileName,
                                unsigned line, unsigned column,
                                const QString &expression,
                                int endOfExpression);

    QList<TextEditor::CompletionItem> removeDuplicates(const QList<TextEditor::CompletionItem> &items);

private:
    void completeObjCMsgSend(CPlusPlus::ClassOrNamespace *binding,
                             bool staticClassAccess);
    bool tryObjCCompletion(TextEditor::BaseTextEditor *edit);
    bool objcKeywordsWanted() const;

    static QStringList preprocessorCompletions;

    CppModelManager *m_manager;
    TextEditor::ITextEditable *m_editor;
    int m_startPosition;     // Position of the cursor from which completion started

    bool m_forcedCompletion;
    unsigned m_completionOperator;
    bool m_objcEnabled;

    CPlusPlus::Icons m_icons;
    CPlusPlus::Overview overview;
    CPlusPlus::TypeOfExpression typeOfExpression;
    QPointer<FunctionArgumentWidget> m_functionArgumentWidget;

    QList<TextEditor::CompletionItem> m_completions;
};

} // namespace Internal
} // namespace CppTools

Q_DECLARE_METATYPE(CPlusPlus::Symbol *)

#endif // CPPCODECOMPLETION_H
