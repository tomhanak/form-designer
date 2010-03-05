/*
 * Copyright (C) 2010 Tieto Czech, s.r.o.
 * All rights reserved.
 * Contact: Tomáš Hanák <tomas.hanak@tieto.com>
 *          Radek Valášek <radek.valasek@tieto.com>
 *          Martin Kampas <martin.kampas@tieto.com>
 *          Jiří Litomyský <jiri.litomysky@tieto.com>
 *
 * This file is part of sfd [Simple Form Designer].
 *
 * SFD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QtGui/QApplication>

// Decision if to use coloredMsgHandler or no
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
# if !defined(CONFIG__ENABLE_COLORED_MSG_HANDLER)
#  define CONFIG__ENABLE_COLORED_MSG_HANDLER (true)
# endif
#else
# define CONFIG__ENABLE_COLORED_MSG_HANDLER (false)
#endif
#if CONFIG__ENABLE_COLORED_MSG_HANDLER
# include <unistd.h>
static const bool STDERR_IS_A_TTY = isatty(2);
#else
static const bool STDERR_IS_A_TTY = false;
#endif

#include "msghndl.hpp"


#if !defined(CONFIG__ENABLE_FORM_EDITOR_TEST)
# define CONFIG__ENABLE_FORM_EDITOR_TEST (false)
#endif
void testFormEditor();


using namespace sfd;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (STDERR_IS_A_TTY) {
        qInstallMsgHandler(coloredMsgHandler);
    }

    //if (CONFIG__ENABLE_FORM_EDITOR_TEST)
        testFormEditor();

    if (qApp->allWidgets().isEmpty()) {
        qWarning("%s: Application has no window, exiting...", __func__);
        return 0;
    }

    return app.exec();
}

#include <QtCore/QMimeData>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include "core/control_loader/ControlPlugin.hpp"
#include "core/control_loader/ControlPluginLoader.hpp"
#include "core/form/Form.hpp"
#include "core/project/Project.hpp"
#include "core/project/TerminalInfo.hpp"
#include "core/transl_table/UnifiedTranslTable.hpp"
#include "gui/form_editor/FormEditor.hpp"
#include "gui/main_window/MainWindow.hpp"
#include "gui/property_editor/PropertyEditor.hpp"
#include "gui/transl_table/TranslTableEditor.hpp"
class DocumentSwitch : public QListWidget
{
    Q_OBJECT

public:
    typedef core::project::Document Document;
    typedef core::project::Project Project;

public:
    DocumentSwitch(Project *project, QWidget *parent = 0)
        : QListWidget(parent)
        , p_project(project)
    {
        connect(project, SIGNAL(documentAdded(sfd::core::project::Document*)),
                this, SLOT(registerDocument(sfd::core::project::Document*)));
        connect(project, 
                SIGNAL(activeDocumentChanged(sfd::core::project::Document*)),
                this, 
                SLOT(onActiveDocumentChanged(sfd::core::project::Document*)));
        connect(this, SIGNAL(itemActivated(QListWidgetItem*)),
                this, SLOT(onItemActivated(QListWidgetItem*)));
    }

public Q_SLOTS:
    void registerDocument(sfd::core::project::Document *document)
    {
        addItem(document->name());
        p_documents.insert(document, item(count() - 1));
    }

    void onItemActivated(QListWidgetItem *item)
    {
        p_project->setActiveDocument(p_documents.key(item));
    }

    void onActiveDocumentChanged(sfd::core::project::Document *document)
    {
        setCurrentItem(p_documents.value(document));
    }

private:
    QPointer<Project> p_project;
    QMap<Document*, QListWidgetItem*> p_documents;
};

class ButtonControlPlugin : public core::control_loader::ControlPlugin
{
public:
    virtual QWidget *createWidget(QWidget *parent)
    {
        return controlize(new QPushButton(parent), "button");
    }

    virtual QString group() const { return "Buttons"; }
    virtual QIcon icon() const { return QIcon(); }
    virtual QString includeFile() const { return "abc.hpp"; }
    virtual bool isContainer() const { return false; }
    virtual QString name() const { return "QPushButton"; }
    virtual QString toolTip() const { return "button"; }
    virtual QString whatsThis() const { return "this is a button"; }

    virtual QString pluginName() const { return "TestButton"; }
    virtual bool isVisual() const { return true; }
    virtual QStringList userEditableProperties() const
    {
        return QStringList() << "translId";
    }
};

class ComboControlPlugin : public core::control_loader::ControlPlugin
{
public:
    virtual QWidget *createWidget(QWidget *parent)
    {
        return controlize(new QComboBox(parent), "combos");
    }

    virtual QString group() const { return "Selectors"; }
    virtual QIcon icon() const { return QIcon(); }
    virtual QString includeFile() const { return "abc.hpp"; }
    virtual bool isContainer() const { return false; }
    virtual QString name() const { return "QComboBox"; }
    virtual QString toolTip() const { return "combo box"; }
    virtual QString whatsThis() const { return "this is a combo box"; }

    virtual QString pluginName() const { return "TestComboBox"; }
    virtual bool isVisual() const { return true; };
    virtual QStringList userEditableProperties() const
    {
        return QStringList();
    }
};

class ControlBox : public QListWidget
{
    typedef core::control_loader::ControlPluginLoader ControlPluginLoader;
    typedef core::control_loader::ControlPlugin ControlPlugin;

public:
    ControlBox(ControlPluginLoader *controlPluginLoader, QWidget *parent = 0)
        : QListWidget(parent)
    {
        Q_ASSERT(controlPluginLoader != 0);

        Q_FOREACH(ControlPlugin *plugin, controlPluginLoader->plugins()) {
            addItem(plugin->pluginName());
        }

        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
    }

protected:
    virtual QMimeData *mimeData(const QList<QListWidgetItem *> items) const
    {
        QMimeData *md = new QMimeData;
        Q_FOREACH(QListWidgetItem *item, items) {
            md->setData(ControlPlugin::MIME_TYPE, 
                    item->data(Qt::DisplayRole).toByteArray());
        }

        return md;
    }

    virtual QStringList mimeTypes() const
    {
        return QStringList() <<ControlPlugin::MIME_TYPE;
    }
};

void testFormEditor()
{
    using core::control_loader::ControlPluginLoader;
    using core::form::Form;
    using core::transl_table::UnifiedTranslTable;
    using core::project::Project;
    using core::project::TerminalInfo;
    using gui::form_editor::FormEditor;
    using gui::main_window::MainWindow;
    using gui::transl_table::TranslTableEditor;
    using gui::property_editor::PropertyEditor;

    FormEditor::Grid *grid = new FormEditor::Grid;
    grid->setVisible(true);
    grid->setYEnabled(true);
    grid->setYStep(15);

    ControlPluginLoader *controlLoader = new ControlPluginLoader;
    controlLoader->fakeLoad(new ButtonControlPlugin);
    controlLoader->fakeLoad(new ComboControlPlugin);

    FormEditor::Creator *editorCreator = new FormEditor::Creator(grid, 
            controlLoader);
    editorCreator->setNext(new TranslTableEditor::Creator);

    MainWindow *mainWindow = new MainWindow(editorCreator);

    TerminalInfo *terminalInfo = new TerminalInfo(QString(), QSize(), 0);

    Project *project = new Project(terminalInfo);
    mainWindow->setProject(project);

    DocumentSwitch *documentSwitch = new DocumentSwitch(project);
    documentSwitch->setWindowTitle("Documents");

    ControlBox *controlBox = new ControlBox(controlLoader);
    controlBox->setWindowTitle("Plugins");

    {
        Form *form = new Form;
        form->setName("Form 1");
        form->addControl(new QLabel("QLabel1"));
        form->addControl(new QPushButton("QPushButton1"));
        QFrame *frame = new QFrame;
        new QPushButton("QPushButton2", frame);
        new QPushButton("Cancel", frame);
        frame->move(150, 150);
        frame->resize(150, 150);
        frame->setFrameShape(QFrame::Box);
        form->addControl(frame);
        QComboBox *combo = new QComboBox;
        combo->addItem("Item 1");
        combo->addItem("Item 2");
        combo->addItem("Item 3");
        form->addControl(combo);

        project->addDocument(form);
    }

    {
        Form *form = new Form;
        form->setName("Form 2");
        form->addControl(new QPushButton("QPushButton4"));
        QFrame *frame = new QFrame;
        new QPushButton("Yes", frame);
        new QPushButton("No", frame);
        frame->move(150, 150);
        frame->resize(150, 150);
        frame->setFrameShape(QFrame::Box);
        form->addControl(frame);

        FormEditor::Grid *grid = new FormEditor::Grid;
        grid->setVisible(true);
        grid->setXEnabled(true);
        grid->setYEnabled(true);

        project->addDocument(form);
    }

    {
        Form *form = new Form;
        form->setName("Form 3");
        form->addControl(new QLabel("QLabel2"));
        form->addControl(new QPushButton("QPushButton5"));
        QFrame *frame = new QFrame;
        new QPushButton("QPushButton6", frame);
        new QToolButton(frame);
        frame->move(150, 150);
        frame->resize(150, 150);
        frame->setFrameShape(QFrame::Box);
        form->addControl(frame);
        QComboBox *combo = new QComboBox;
        combo->addItem("value 1");
        combo->addItem("value 2");
        combo->addItem("value 3");
        form->addControl(combo);

        project->addDocument(form);
    }

    PropertyEditor* propEditor = new PropertyEditor(0);
    propEditor->resize(300, 750);
    propEditor->setProject(project);

    UnifiedTranslTable *table = new UnifiedTranslTable();
    {
        table->setName("Translation Table");

        const int idCount = 5;
        const int languageCount = 3;

        QString languages[languageCount] = {
            "English",
            "Deutsch",
            QString::fromUtf8("日本語")
        };
        for (int l = 0; l < languageCount; ++l) {
            table->addLanguage(languages[l]);
        }

        typedef QPair<QString, QString> ID;
        ID ids[idCount] = {
            ID("jazyk", QString::fromUtf8("řeč")),
            ID("jazyk", QString::fromUtf8("orgán")),
            ID("kniha", ""),
            ID("zima", QString::fromUtf8("chladno")),
            ID("zima", QString::fromUtf8("roční období"))
        };
        for (int i = 0; i < idCount; ++i) {
            table->addId(ids[i].first, ids[i].second);
        }

        const QString translation[idCount][languageCount] = {
            { "language", "die Sprache", QString::fromUtf8("語　【ご】") },
            { "tongue", "die Zunge", QString::fromUtf8("舌　【した】") },
            { "book", "das Buch", QString::fromUtf8("本　【ほん】") },
            { "cold", "kalt", QString::fromUtf8("寒い　【さむい】") },
            { "winter", "der Winter", QString::fromUtf8("冬　【ふゆ】") }
        };
        for (int i = 0; i < idCount; ++i) {
            const int upid = table->uniqueRowId(ids[i].first, ids[i].second);
            for (int l = 0; l < languageCount; ++l) {
                const int ulid = table->uniqueLanguageId(languages[l]);
                table->setText(translation[i][l], upid, ulid);
            }
        }

        project->addDocument(table);
    }

    mainWindow->dockPropertyEditor(propEditor);
    mainWindow->dockDocumentSwitch(documentSwitch);
    mainWindow->dockControlBox(controlBox);
    mainWindow->resize(1024, 768);
    mainWindow->show();
}


#include "main.moc"
