#include "pch.h"
#include "create_project_dialog.h"

#include "utils/dialog.h"
#include "utils/filesystem_utils.h"
#include "utils/extension_utils.h"

namespace sore
{
    CreateProjectDialog::CreateProjectDialog(QWidget* parent)
        : QDialog(parent)
    {
        setWindowModality(Qt::ApplicationModal);
        setWindowFlags(Qt::Dialog);

        ui.setupUi(this);

        statusBar = new QStatusBar(this);
        layout()->addWidget(statusBar);

        setupLabels();
        setupIcons();

        handleLocationTextChange();
        handleLocationBtn();
        handleEpisodesChanged();
        handleSubtitlesChanged();
        handleValidation();
    }

    void CreateProjectDialog::validate()
    {
        if (ui.projectNameInput->text().isEmpty())
        {
            statusBar->showMessage("You must provide a valid project name");
            return;
        }

        if (ui.locationInput->text().isEmpty())
        {
            statusBar->showMessage("You must provide a valid project root folder");
            return;
        }

        accept();
    }

    CreateProjectDialogData CreateProjectDialog::getData()
    {
        std::string projectName = ui.projectNameInput->text().toStdString();
        std::string rootFolderLocation = ui.locationInput->text().toStdString();
        
        return {
            projectName,
            rootFolderLocation,
            m_Episodes,
            m_Subtitles
        };
    }
 
    void CreateProjectDialog::setupLabels()
    {
        ui.emptyProjectLabel->setStyleSheet("font-family: Krub; font-size: 32px;");
        ui.sourcesLabel->setStyleSheet("font-family: Krub; font-size: 24px;");
    }
    
    void CreateProjectDialog::setupIcons()
    {
        ui.locationBtn->setText("");
        ui.locationBtn->setIcon(QIcon("C:/Users/paulo/Projects/sore-crow/sore-crow/resources/icons/ryan/search.png"));
    }

    void CreateProjectDialog::handleLocationTextChange()
    {
        QObject::connect(ui.locationInput, &QLineEdit::textChanged, [&](const QString& location) {
            if (m_CurrentFilepath == location.toStdString())
                return;

            QFileInfo fileInfo(location);
            if (!fileInfo.isDir())
                return;
            
            m_CurrentFilepath = location.toStdString();
            
            clearLists();
            
            for (const auto filepath : getFilesInDir(location.toStdString()))
            {
                if (isFileSupportedVideo(filepath))
                    m_Episodes.push_back(filepath);

                if (isFileSupportedSubtitle(filepath))
                    m_Subtitles.push_back(filepath);
            }

            emit episodesChanged();
            emit subtitlesChanged();
        });
    }

    void CreateProjectDialog::handleLocationBtn()
    {
        QObject::connect(ui.locationBtn, &QPushButton::released, [&]() {
            std::string location = openProjectFolderDialog();

            QFileInfo fileInfo(location.c_str());
            if (!fileInfo.isDir())
                return;

            ui.locationInput->setText(fileInfo.filePath());
        });
    }

    void CreateProjectDialog::handleEpisodesChanged()
    {
        QObject::connect(this, &CreateProjectDialog::episodesChanged, [&]() {
            for (const auto& episode : m_Episodes)
                ui.videosList->addItem(QString::fromStdString(episode));
        });
    }

    void CreateProjectDialog::handleSubtitlesChanged()
    {
        QObject::connect(this, &CreateProjectDialog::subtitlesChanged, [&]() {
            for (const auto& subtitle : m_Subtitles)
                ui.subtitlesList->addItem(QString::fromStdString(subtitle));
        });
    }

    void CreateProjectDialog::handleValidation()
    {
        QObject::connect(ui.createBtn, &QPushButton::released, [&]() {
            validate();
        });
    }

    void CreateProjectDialog::clearLists()
    {
        m_Episodes.clear();
        m_Subtitles.clear();
        ui.videosList->clear();
        ui.subtitlesList->clear();
    }
}