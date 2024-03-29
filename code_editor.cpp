/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "code_editor.h"
#include "main.h"

//![constructor]

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent), row(0)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    // todo:can be problem this big text
//    connect(this, &QPlainTextEdit::textChanged, this, [&]() {
//        this->blockSignals(true);
//        QTextCursor cursor = this->textCursor();
//        this->setPlainText(this->toPlainText().toUpper());
//        this->setTextCursor(cursor);
//        this->blockSignals(false);
//    });

//    installEventFilter(this);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

//    QFont font = this->document()->defaultFont();
    QFont font("unexistent");
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(14);
    this->document()->setDefaultFont(font);
    this->setFont(font);

#ifdef DARK_GUI_THEME
    QPalette palette = this->palette();
    palette.setColor(QPalette::Text, QColor(Qt::yellow).lighter(80));
    this->setPalette(palette);
#endif
}

//![constructor]

//![extraAreaWidth]

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

//    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    int space = 3 + fontMetrics().averageCharWidth() * digits;

    return space;
}

int CodeEditor::lineHeight() const {
    return fontMetrics().lineSpacing();
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent *e) {
    if (!e->text().isNull() && !e->text().isEmpty() && e->modifiers() == Qt::NoModifier && e->key() >= Qt::Key_A && e->key() <= Qt::Key_Z)
        insertPlainText(e->text().toUpper());
    else {
        QPlainTextEdit::keyPressEvent(e);
    }
}

//bool CodeEditor::eventFilter(QObject *obj, QEvent *event) {
//    if (event->type() == QEvent::KeyPress) {
//        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

//        QString text = keyEvent->text();
//        if (!text.isEmpty()) {
//            insertPlainText(text.toUpper());
//            return true;
//        }

////        Qt::KeyboardModifiers modifieres = keyEvent->modifiers();
////        modifieres.setFlag(Qt::ShiftModifier, true);
////        keyEvent->setModifiers(modifieres);
//    }

//    // standard event processing
//    return QPlainTextEdit::eventFilter(obj, event);
//}

//![resizeEvent]

//![cursorPositionChanged]

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

#ifdef DARK_GUI_THEME
        QColor lineColor = QColor(Qt::darkGray).lighter(50);
#else
        QColor lineColor = QColor(Qt::yellow).lighter(160);
#endif

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);

    if (row != this->textCursor().blockNumber()) {
        row = this->textCursor().blockNumber();
        emit lineChanged(row);
    }
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
#ifdef DARK_GUI_THEME
    painter.fillRect(event->rect(), QColor(Qt::darkGray).lighter(50));
#else
    painter.fillRect(event->rect(), Qt::lightGray);
#endif

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
#ifdef DARK_GUI_THEME
            painter.setPen(Qt::lightGray);
#else
            painter.setPen(Qt::black);
#endif
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]
