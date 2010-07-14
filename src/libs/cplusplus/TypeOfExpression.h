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
#ifndef CPLUSPLUS_TYPEOFEXPRESSION_H
#define CPLUSPLUS_TYPEOFEXPRESSION_H

#include "CppDocument.h"
#include "LookupContext.h"
#include "PreprocessorEnvironment.h"

#include <ASTfwd.h>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace CPlusPlus {

class Environment;
class Macro;

class CPLUSPLUS_EXPORT TypeOfExpression
{
    Q_DISABLE_COPY(TypeOfExpression)

public:
    TypeOfExpression();

    /**
     * Sets the documents used to evaluate expressions. Should be set before
     * calling this functor.
     *
     * Also clears the lookup context, so can be used to make sure references
     * to the documents previously used are removed.
     */
    void init(Document::Ptr thisDocument, const Snapshot &snapshot,
              QSharedPointer<CreateBindings> bindings = QSharedPointer<CreateBindings>());

    void reset();

    enum PreprocessMode {
        NoPreprocess,
        Preprocess
    };

    /**
     * Returns a list of possible fully specified types associated with the
     * given expression.
     *
     * NOTE: The fully specified types only stay valid for as long as this
     * expression evaluator instance still exists, and no new call to evaluate
     * has been made!
     *
     * @param expression        The expression to evaluate.
     * @param scope             The scope enclosing the expression.
     */
    QList<LookupItem> operator()(const QString &expression,
                                 Scope *scope,
                                 PreprocessMode mode = NoPreprocess);

    /**
     * Returns a list of possible fully specified types associated with the
     * given expression AST from the given document.
     *
     * NOTE: The fully specified types only stay valid for as long as this
     * expression evaluator instance still exists, and no new call to evaluate
     * has been made!
     *
     * @param expression        The expression to evaluate.
     * @param document          The document in which the expression lives.
     * @param scope             The scope enclosing the expression.
     */
    QList<LookupItem> operator()(ExpressionAST *expression,
                                 Document::Ptr document,
                                 Scope *scope);

    QString preprocess(const QString &expression) const;

    /**
     * Returns the AST of the last evaluated expression.
     */
    ExpressionAST *ast() const;

    /**
     * Returns the lookup context of the last evaluated expression.
     */
    const LookupContext &context() const;
    Scope *scope() const;

    ExpressionAST *expressionAST() const;

private:
    ExpressionAST *extractExpressionAST(Document::Ptr doc) const;
    Document::Ptr documentForExpression(const QString &expression) const;

    void processEnvironment(Document::Ptr doc, Environment *env,
                            QSet<QString> *processed) const;

    QString preprocessedExpression(const QString &expression) const;

private:
    Document::Ptr m_thisDocument;
    Snapshot m_snapshot;
    QSharedPointer<CreateBindings> m_bindings;
    ExpressionAST *m_ast;
    Scope *m_scope;
    LookupContext m_lookupContext;
    mutable QSharedPointer<Environment> m_environment;
};

} // namespace CPlusPlus

#endif // CPLUSPLUS_TYPEOFEXPRESSION_H
