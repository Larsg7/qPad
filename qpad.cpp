#include "qpad.h"
#include "ui_qpad.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

qPad::qPad( QWidget* parent )
    : QMainWindow( parent )
    , fileName( "" )
    , textChanged( false )
{
    setupUi();
    connect_actions();
}

void qPad::setupUi()
{
    actionNew          = new QAction( tr( "New..." ), this );
    actionOpen         = new QAction( tr( "Open..." ), this );
    actionSave         = new QAction( tr( "Save..." ), this );
    actionSave_As      = new QAction( tr( "Save As..." ), this );
    actionExit         = new QAction( tr( "Exit..." ), this );
    actionBold         = new QAction( tr( "Bold" ), this );
    actionItalic       = new QAction( tr( "Italic" ), this );
    actionUnderline    = new QAction( tr( "Underline" ), this );
    actionUndo         = new QAction( tr( "Undo" ), this );
    actionRedo         = new QAction( tr( "Redo" ), this );
    actionAlign_Left   = new QAction( tr( "Align Left" ), this );
    actionAlign_Right  = new QAction( tr( "Align Right" ), this );
    actionAlign_Center = new QAction( tr( "Align Center" ), this );
    textEdit           = new QTextEdit;

    actionNew->setShortcut( tr( "Ctrl+N" ) );
    actionOpen->setShortcut( tr( "Ctrl+O" ) );
    actionSave->setShortcut( tr( "Ctrl+S" ) );
    actionSave_As->setShortcut( tr( "Ctrl+Shift+S" ) );
    actionExit->setShortcut( tr( "Ctrl+Q" ) );
    actionUndo->setShortcut( tr( "Ctrl+Z" ) );
    actionRedo->setShortcut( tr( "Ctrl+Shift+Z" ) );

    QIcon i1;
    i1.addFile( ":/images/images/ic_insert_drive_file_black_24px.svg" );
    actionNew->setIcon( i1 );
    QIcon i2;
    i2.addFile( ":/images/images/ic_library_books_black_24px.svg" );
    actionOpen->setIcon( i2 );
    QIcon i3;
    i3.addFile( ":/images/images/ic_save_black_24px.svg" );
    actionSave->setIcon( i3 );
    QIcon i4;
    i4.addFile( ":/images/images/ic_save_black_24px.svg" );
    actionSave_As->setIcon( i4 );
    QIcon i5;
    i5.addFile( ":/images/images/ic_close_black_24px.svg" );
    actionExit->setIcon( i5 );
    QIcon i6;
    i6.addFile( ":/images/images/ic_format_bold_black_24px.svg" );
    actionBold->setIcon( i6 );
    QIcon i7;
    i7.addFile( ":/images/images/ic_format_italic_black_24px.svg" );
    actionItalic->setIcon( i7 );
    QIcon i8;
    i8.addFile( ":/images/images/ic_format_underlined_black_24px.svg" );
    actionUnderline->setIcon( i8 );
    QIcon i9;
    i9.addFile( ":/images/images/ic_undo_black_24px.svg" );
    actionUndo->setIcon( i9 );
    QIcon i10;
    i10.addFile( ":/images/images/ic_redo_black_24px.svg" );
    actionRedo->setIcon( i10 );
    QIcon i11;
    i11.addFile( ":/images/images/ic_format_align_left_black_24px.svg" );
    actionAlign_Left->setIcon( i11 );
    QIcon i12;
    i12.addFile( ":/images/images/ic_format_align_right_black_24px.svg" );
    actionAlign_Right->setIcon( i12 );
    QIcon i13;
    i13.addFile( ":/images/images/ic_format_align_center_black_24px.svg" );
    actionAlign_Center->setIcon( i13 );

    actionSave->setEnabled( false );
    actionUndo->setEnabled( false );
    actionRedo->setEnabled( false );
    actionBold->setCheckable( true );
    actionItalic->setCheckable( true );
    actionUnderline->setCheckable( true );
    actionAlign_Left->setCheckable( true );
    actionAlign_Right->setCheckable( true );
    actionAlign_Center->setCheckable( true );
    actionAlign_Left->setChecked( true );

    fileMenu = new QMenu( tr( "File" ), this );
    fileMenu->addAction( actionNew );
    fileMenu->addAction( actionOpen );
    fileMenu->addAction( actionSave );
    fileMenu->addAction( actionSave_As );
    fileMenu->addSeparator();
    fileMenu->addAction( actionExit );

    menuBar()->addMenu( fileMenu );

    mainToolBar = new QToolBar;
    mainToolBar->addAction( actionBold );
    mainToolBar->addAction( actionItalic );
    mainToolBar->addAction( actionUnderline );
    mainToolBar->addSeparator();
    mainToolBar->addAction( actionUndo );
    mainToolBar->addAction( actionRedo );
    mainToolBar->addSeparator();
    mainToolBar->addAction( actionAlign_Left );
    mainToolBar->addAction( actionAlign_Right );
    mainToolBar->addAction( actionAlign_Center );
    this->addToolBar( Qt::TopToolBarArea, mainToolBar );

    statusBar = new QStatusBar( this );
    this->setStatusBar( statusBar );

    textEdit->setBackgroundRole( QPalette::Dark );

    setCentralWidget( textEdit );
    setWindowTitle( tr( "qPad" ) );
}

void qPad::connect_actions()
{
    connect( actionNew, &QAction::triggered, this, &qPad::new_doc );
    connect( actionOpen, &QAction::triggered, this, &qPad::open_doc );
    connect( actionSave, &QAction::triggered, this, &qPad::save_doc );
    connect( actionSave_As, &QAction::triggered, this, &qPad::save_as_doc );
    connect( actionExit, &QAction::triggered, this, &qPad::close );

    connect( actionBold, &QAction::triggered, this, &qPad::set_bold );
    connect( actionItalic, &QAction::triggered, this, &qPad::set_italic );
    connect( actionUnderline, &QAction::triggered, this, &qPad::set_underline );

    connect( actionUndo, &QAction::triggered, [this]() { textEdit->undo(); } );
    connect( actionRedo, &QAction::triggered, [this]() { textEdit->redo(); } );
    connect( textEdit, &QTextEdit::undoAvailable, [this]() { actionUndo->setEnabled( true ); } );
    connect( textEdit, &QTextEdit::redoAvailable, [this]() { actionRedo->setEnabled( true ); } );

    connect( actionAlign_Left, &QAction::triggered, this, &qPad::align_left );
    connect( actionAlign_Right, &QAction::triggered, this, &qPad::align_right );
    connect( actionAlign_Center, &QAction::triggered, this, &qPad::align_middle );

    connect( textEdit, &QTextEdit::textChanged, [this]() { textChanged = true; } );
}

qPad::~qPad()
{
}

void qPad::closeEvent( QCloseEvent* event )
{
    if ( !textChanged || ask_to_save() )
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
        return false;
        break;
    }
}

void qPad::reset_align_btn()
{
    actionAlign_Left->setChecked( false );
    actionAlign_Right->setChecked( false );
    actionAlign_Center->setChecked( false );
}

// Slots //

void qPad::new_doc()
{
    if ( textChanged && !ask_to_save() )
    {
        return;
    }

    textEdit->setText( "" );
    fileName = "";
    statusBar->showMessage( tr( "Cleared Document..." ) );
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
        actionSave->setEnabled( true );
        file.close();

        textChanged = false;
        statusBar->showMessage( tr( "Opened Document..." ) );
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
        actionSave->setEnabled( false );
        return false;
    }

    QTextStream out( &file );
    out << textEdit->toPlainText();

    file.close();
    textChanged = false;
    statusBar->showMessage( tr( "Saved Document..." ) );
    return true;
}

bool qPad::save_as_doc()
{
    QString name = QFileDialog::getSaveFileName( this, tr( "Save file - qPad" ), QDir::currentPath() );
    if ( !name.isEmpty() )
    {
        fileName = name;
        actionSave->setEnabled( true );

        return save_doc();
    }
    else
    {
        return false;
    }
}

void qPad::set_bold()
{
    textEdit->setFontWeight( actionBold->isChecked() ? QFont::Weight::Bold : QFont::Weight::Normal );
}

void qPad::set_italic()
{
    textEdit->setFontItalic( actionItalic->isChecked() );
}

void qPad::set_underline()
{
    textEdit->setFontUnderline( actionUnderline->isChecked() );
}

void qPad::align_left()
{
    reset_align_btn();
    actionAlign_Left->setChecked( true );
    textEdit->setAlignment( Qt::AlignLeft );
}

void qPad::align_right()
{
    reset_align_btn();
    actionAlign_Right->setChecked( true );
    textEdit->setAlignment( Qt::AlignRight );
}

void qPad::align_middle()
{
    reset_align_btn();
    actionAlign_Center->setChecked( true );
    textEdit->setAlignment( Qt::AlignCenter );
}
