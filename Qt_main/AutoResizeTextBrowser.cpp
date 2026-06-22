#include "AutoResizeTextBrowser.h"

AutoResizeTextBrowser::AutoResizeTextBrowser(QWidget* parent)
    : QTextBrowser(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setLineWrapMode(QTextEdit::WidgetWidth);

    setMaximumWidth(375);

    connect(document(), &QTextDocument::contentsChanged,
        this, &AutoResizeTextBrowser::updateHeightFromDocument);
}

void AutoResizeTextBrowser::resizeEvent(QResizeEvent* event)
{
    QTextBrowser::resizeEvent(event);
    updateHeightFromDocument();
}

void AutoResizeTextBrowser::updateHeightFromDocument(){

    QTextDocument* doc = document();
    int width = viewport()->width();
    if (width <= 0)
        return;

    doc->setTextWidth(width);
    qreal h = doc->size().height();
    setMinimumHeight(h + 30);   // laisse le layout gérer, évite setFixedHeight
    setMaximumHeight(h + 30);


}

void AutoResizeTextBrowser::wheelEvent(QWheelEvent* event)
{
    event->ignore();   // empêche le scroll
}

