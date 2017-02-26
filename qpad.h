#ifndef QPAD_H
#define QPAD_H

#include <QAction>
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
    bool save_doc();
    bool save_as_doc();

    void set_bold();
    void set_italic();
    void set_underline();

    void align_left();
    void align_right();
    void align_middle();

private:
    bool ask_to_save();
    void setupUi();
    void connect_actions();
    void reset_align_btn();

    QAction* actionNew;
    QAction* actionOpen;
    QAction* actionSave;
    QAction* actionSave_As;
    QAction* actionExit;
    QAction* actionBold;
    QAction* actionItalic;
    QAction* actionUnderline;
    QAction* actionUndo;
    QAction* actionRedo;
    QAction* actionAlign_Left;
    QAction* actionAlign_Right;
    QAction* actionAlign_Center;
    QAction* actionTo_PDF;

    QMenu* fileMenu;
    QMenu* editMenu;

    QToolBar* mainToolBar;
    QStatusBar* statusBar;

    Ui::qPad* ui;
    QTextEdit* textEdit;
    QString fileName;
    bool textChanged;
};

#endif // QPAD_H
