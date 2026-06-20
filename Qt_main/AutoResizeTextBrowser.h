#pragma once

#include <QTextBrowser>
#include <QTextDocument>
#include <QResizeEvent>
#include <QFrame>

class AutoResizeTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit AutoResizeTextBrowser(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void updateHeightFromDocument();
};
