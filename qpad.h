#ifndef QPAD_H
#define QPAD_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QTextEdit>

namespace Ui
{
class qPad;
}

class qPad : public QMainWindow
{
    Q_OBJECT

public:
    explicit qPad( QWidget* parent = 0 );
    void closeEvent( QCloseEvent* event );
    ~qPad();

private slots:
    void new_doc();
    void open_doc();
    void save_doc();
    void save_as_doc();
    void set_bold();

private:
    void setupUi();
    void connect_actions();

    Ui::qPad* ui;
    QTextEdit* textEdit;
    QString fileName;
};

#endif // QPAD_H
