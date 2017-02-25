#include "qpad.h"
#include "ui_qpad.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

qPad::qPad( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::qPad )
    , fileName( "" )
    , textChanged( false )
{
    ui->setupUi( this );

    setupUi();
    connect_actions();
}

void qPad::setupUi()
{
    textEdit = new QTextEdit;
    textEdit->setBackgroundRole( QPalette::Dark );

    setCentralWidget( textEdit );
}

void qPad::connect_actions()
{
    connect( ui->actionNew, &QAction::triggered, this, &qPad::new_doc );
    connect( ui->actionOpen, &QAction::triggered, this, &qPad::open_doc );
    connect( ui->actionSave, &QAction::triggered, this, &qPad::save_doc );
    connect( ui->actionSave_As, &QAction::triggered, this, &qPad::save_as_doc );
    connect( ui->actionExit, &QAction::triggered, this, &qPad::close );

    connect( ui->actionBold, &QAction::triggered, this, &qPad::set_bold );
    connect( ui->actionItalic, &QAction::triggered, this, &qPad::set_italic );
    connect( ui->actionUnderline, &QAction::triggered, this, &qPad::set_underline );

    connect( ui->actionUndo, &QAction::triggered, [this]() { textEdit->undo(); } );
    connect( ui->actionRedo, &QAction::triggered, [this]() { textEdit->redo(); } );
    connect( textEdit, &QTextEdit::undoAvailable, [this]() { ui->actionUndo->setEnabled( true ); } );
    connect( textEdit, &QTextEdit::redoAvailable, [this]() { ui->actionRedo->setEnabled( true ); } );

    connect( ui->actionAlign_Left, &QAction::triggered, this, &qPad::align_left );
    connect( ui->actionAlign_Right, &QAction::triggered, this, &qPad::align_right );
    connect( ui->actionAlign_Middle, &QAction::triggered, this, &qPad::align_middle );

    connect( textEdit, &QTextEdit::textChanged, [this]() { textChanged = true; } );
}

qPad::~qPad()
{
    delete ui;
}

void qPad::closeEvent( QCloseEvent* event )
{
    if ( ask_to_save() )
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

bool qPad::ask_to_save()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question( this, tr( "Save Confirmation - qPad" ),
        tr( "The document has changed\nDo you want to save?" ),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );

    switch ( reply )
    {
    case QMessageBox::Save:
        save_doc();
        return true;
        break;
    case QMessageBox::Discard:
        return true;
        break;
    case QMessageBox::Cancel:
        return false;
        break;
    default:
        break;
    }
}

void qPad::reset_align_btn()
{
    ui->actionAlign_Left->setChecked( false );
    ui->actionAlign_Right->setChecked( false );
    ui->actionAlign_Middle->setChecked( false );
}

// Slots //

void qPad::new_doc()
{
    if ( !textEdit->toPlainText().isEmpty() )
    {
        if ( textChanged && !ask_to_save() )
        {
            return;
        }

        textEdit->setText( "" );
        fileName = "";
    }
}

void qPad::open_doc()
{
    if ( textChanged && !ask_to_save() )
    {
        return;
    }

    QString name = QFileDialog::getOpenFileName( this, tr( "Open File" ), QDir::currentPath() );

    if ( !name.isEmpty() )
    {
        QFile file( name );
        if ( !file.open( QFile::ReadOnly | QFile::Text ) )
        {
            QMessageBox::critical( this, tr( "qPad" ), tr( "Could not open %1" ).arg( name ) );
            return;
        }

        QTextStream in( &file );
        textEdit->setText( in.readAll() );
        fileName = name;
        ui->actionSave->setEnabled( true );
        file.close();

        textChanged = false;
    }
}

bool qPad::save_doc()
{
    if ( fileName.isEmpty() )
    {
        save_as_doc();
    }

    QFile file( fileName );
    if ( !file.open( QFile::WriteOnly | QFile::Text ) )
    {
        QMessageBox::critical( this, tr( "qPad" ), tr( "Could not open %1" ).arg( fileName ) );
        fileName = "";
        ui->actionSave->setEnabled( false );
        return false;
    }

    QTextStream out( &file );
    out << textEdit->toPlainText();

    file.close();
    textChanged = false;
    return true;
}

bool qPad::save_as_doc()
{
    QString name = QFileDialog::getSaveFileName( this, tr( "Save file - qPad" ), QDir::currentPath() );
    if ( !name.isEmpty() )
    {
        fileName = name;
        ui->actionSave->setEnabled( true );

        return save_doc();
    }
    else
    {
        return false;
    }
}

void qPad::set_bold()
{
    textEdit->setFontWeight( ui->actionBold->isChecked() ? QFont::Weight::Bold : QFont::Weight::Normal );
}

void qPad::set_italic()
{
    textEdit->setFontItalic( ui->actionItalic->isChecked() );
}

void qPad::set_underline()
{
    textEdit->setFontUnderline( ui->actionUnderline->isChecked() );
}

void qPad::align_left()
{
    reset_align_btn();
    ui->actionAlign_Left->setChecked( true );
    textEdit->setAlignment( Qt::AlignLeft );
}

void qPad::align_right()
{
    reset_align_btn();
    ui->actionAlign_Right->setChecked( true );
    textEdit->setAlignment( Qt::AlignRight );
}

void qPad::align_middle()
{
    reset_align_btn();
    ui->actionAlign_Middle->setChecked( true );
    textEdit->setAlignment( Qt::AlignCenter );
}
