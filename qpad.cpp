#include "qpad.h"
#include "ui_qpad.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

qPad::qPad( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::qPad )
    , fileName( "" )
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
    connect( ui->actionNew, SIGNAL( triggered( bool ) ), this, SLOT( new_doc() ) );
    connect( ui->actionOpen, SIGNAL( triggered( bool ) ), this, SLOT( open_doc() ) );
    connect( ui->actionSave, SIGNAL( triggered( bool ) ), this, SLOT( save_doc() ) );
    connect( ui->actionSave_As, SIGNAL( triggered( bool ) ), this, SLOT( save_as_doc() ) );
    connect( ui->actionExit, SIGNAL( triggered( bool ) ), this, SLOT( close() ) );
    connect( ui->actionBold, SIGNAL( triggered( bool ) ), this, SLOT( set_bold() ) );
}

qPad::~qPad()
{
    delete ui;
}

void qPad::closeEvent( QCloseEvent* event )
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, tr( "Close - qPad" ), tr( "Are you sure to close qPad?" ), QMessageBox::Yes | QMessageBox::Cancel );

    if ( reply == QMessageBox::Yes )
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void qPad::new_doc()
{
    if ( !textEdit->toPlainText().isEmpty() )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( this, tr( "New Document - qPad" ),
            tr( "Are you sure to delete your current document?" ), QMessageBox::Yes | QMessageBox::No );
        if ( reply == QMessageBox::Yes )
        {
            textEdit->setText( "" );
            fileName = "";
        }
    }
}

void qPad::open_doc()
{
    if ( !textEdit->toPlainText().isEmpty() )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( this, tr( "Open Document - qPad" ),
            tr( "Are you sure to delete your current document?" ), QMessageBox::Yes | QMessageBox::No );
        if ( reply == QMessageBox::No )
        {
            return;
        }
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
        file.close();
    }
}

void qPad::save_doc()
{
    QFile file( fileName );
    if ( !file.open( QFile::WriteOnly | QFile::Text ) )
    {
        QMessageBox::critical( this, tr( "qPad" ), tr( "Could not open %1" ).arg( fileName ) );
        fileName = "";
        ui->actionSave->setEnabled( false );
        return;
    }

    QTextStream out( &file );
    out << textEdit->toPlainText();

    file.close();
}

void qPad::save_as_doc()
{
    QString name = QFileDialog::getSaveFileName( this, tr( "Save file - qPad" ), QDir::currentPath() );
    if ( !name.isEmpty() )
    {
        fileName = name;
        ui->actionSave->setEnabled( true );

        save_doc();
    }
}

void qPad::set_bold()
{
}
