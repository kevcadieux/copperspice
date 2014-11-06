/***********************************************************************
*
* Copyright (c) 2012-2014 Barbara Geller
* Copyright (c) 2012-2014 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CopperSpice.  If not, see
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef QPLAINTEXTEDIT_P_H
#define QPLAINTEXTEDIT_P_H

#include <qabstractscrollarea_p.h>
#include <QtGui/qtextdocumentfragment.h>
#include <QtGui/qscrollbar.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qtextformat.h>
#include <QtGui/qmenu.h>
#include <QtGui/qabstracttextdocumentlayout.h>
#include <QtCore/qbasictimer.h>
#include <qtextcontrol_p.h>
#include <qplaintextedit.h>

#ifndef QT_NO_TEXTEDIT

QT_BEGIN_NAMESPACE

class QMimeData;
class QPlainTextEdit;
class ExtraArea;

class QPlainTextEditControl : public QTextControl
{
   CS_OBJECT(QPlainTextEditControl)

 public:
   QPlainTextEditControl(QPlainTextEdit *parent);


   QMimeData *createMimeDataFromSelection() const;
   bool canInsertFromMimeData(const QMimeData *source) const;
   void insertFromMimeData(const QMimeData *source);
   int hitTest(const QPointF &point, Qt::HitTestAccuracy = Qt::FuzzyHit) const;
   QRectF blockBoundingRect(const QTextBlock &block) const;

   inline QRectF cursorRect(const QTextCursor &cursor) const {
      QRectF r = QTextControl::cursorRect(cursor);
      r.setLeft(qMax(r.left(), (qreal) 0.));
      return r;
   }

   inline QRectF cursorRect() {
      return cursorRect(textCursor());
   }

   inline void ensureCursorVisible() {
      textEdit->ensureCursorVisible();
      emit microFocusChanged();
   }

   QPlainTextEdit *textEdit;
   int topBlock;
   QTextBlock firstVisibleBlock() const;

   QVariant loadResource(int type, const QUrl &name) {
      return textEdit->loadResource(type, name);
   }

};


class QPlainTextEditPrivate : public QAbstractScrollAreaPrivate
{
   Q_DECLARE_PUBLIC(QPlainTextEdit)

 public:
   QPlainTextEditPrivate();

   void init(const QString &txt = QString());
   void _q_repaintContents(const QRectF &contentsRect);

   inline QPoint mapToContents(const QPoint &point) const {
      return QPoint(point.x() + horizontalOffset(), point.y() + verticalOffset());
   }

   void _q_adjustScrollbars();
   void _q_verticalScrollbarActionTriggered(int action);
   void ensureViewportLayouted();
   void relayoutDocument();

   void pageUpDown(QTextCursor::MoveOperation op, QTextCursor::MoveMode moveMode, bool moveCursor = true);

   inline int horizontalOffset() const {
      return (q_func()->isRightToLeft() ? (hbar->maximum() - hbar->value()) : hbar->value());
   }

   qreal verticalOffset(int topBlock, int topLine) const;
   qreal verticalOffset() const;

   inline void sendControlEvent(QEvent *e) {
      control->processEvent(e, QPointF(horizontalOffset(), verticalOffset()), viewport);
   }

   void updateDefaultTextOption();

   QPlainTextEditControl *control;

   bool tabChangesFocus;

   QBasicTimer autoScrollTimer;
   QPoint autoScrollDragPos;

   QPlainTextEdit::LineWrapMode lineWrap;
   QTextOption::WrapMode wordWrap;

   uint showCursorOnInitialShow : 1;
   uint backgroundVisible : 1;
   uint centerOnScroll : 1;
   uint inDrag : 1;
   uint clickCausedFocus : 1;

   int topLine;
   qreal topLineFracture; // for non-int sized fonts

   void setTopLine(int visualTopLine, int dx = 0);
   void setTopBlock(int newTopBlock, int newTopLine, int dx = 0);

   void ensureVisible(int position, bool center, bool forceCenter = false);
   void ensureCursorVisible(bool center = false);
   void updateViewport();

   QPointer<QPlainTextDocumentLayout> documentLayoutPtr;

   void append(const QString &text, Qt::TextFormat format = Qt::AutoText);

   qreal pageUpDownLastCursorY;
   bool pageUpDownLastCursorYIsValid;

#ifdef QT_KEYPAD_NAVIGATION
   QBasicTimer deleteAllTimer;
#endif

   void _q_cursorPositionChanged();
   void _q_modificationChanged(bool);

   int originalOffsetY;
};

QT_END_NAMESPACE

#endif // QT_NO_TEXTEDIT

#endif // QPLAINTEXTEDIT_P_H
