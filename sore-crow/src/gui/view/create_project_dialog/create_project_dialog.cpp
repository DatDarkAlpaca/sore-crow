#include "pch.h"
#include "core/handlers.h"
#include "utils/dialog_utils.h"
#include "create_project_dialog.h"
#include "core/filesystem/filesystem_handler.h"

namespace sore
{
    CreateProjectDialog::CreateProjectDialog(QWidget* parent)
        : QDialog(parent)
    {
        setWindowModality(Qt::ApplicationModal);
        setWindowFlags(Qt::Dialog);

        ui.setupUi(this);
        setupUI();

        connect(ui.createBtn, &QPushButton::released, this, &CreateProjectDialog::onValidate);
        connect(ui.locationBtn, &QPushButton::released, this, &CreateProjectDialog::onSearchButtonPressed);
        connect(ui.locationInput, &QLineEdit::textChanged, this, &CreateProjectDialog::onLocationChanged);
    }

    ProjectData CreateProjectDialog::getProjectData() const
    {
        const auto& projectName = ui.projectNameInput->text();
        const auto& episodeFilepaths = data.episodeFilepaths;

        ProjectHeader header(projectName);

        ProjectMediaData mediaData;
        for (const auto& episodeFilepath : episodeFilepaths)
        {
            EpisodeData episodeData(episodeFilepath);
            mediaData.episodeData.push_back(episodeData);
        }

        ProjectData projectData(header, mediaData);
        return projectData;
    }

    QString CreateProjectDialog::getFilepath() const
    {
        return createProjectFilepath();
    }

    void CreateProjectDialog::onValidate()
    {
        if (ui.projectNameInput->text().isEmpty())
        {
            m_StatusBar->showMessage("You must provide a valid project name");
            return;
        }

        if (!FilesystemHandler::isValidFolderPath(data.currentDirectory.path()))
        {
            m_StatusBar->showMessage("You must provide a valid project root folder");
            return;
        }

        accept();
    }

    void CreateProjectDialog::onSearchButtonPressed()
    {
        QString projectFolder = openProjectFolderDialog();
        if (projectFolder.isEmpty())
            return;

        ui.locationInput->setText(projectFolder);

        data.currentDirectory = projectFolder;
        clear();

        populateEpisodeList();
    }

    void CreateProjectDialog::onLocationChanged(const QString& folderPath)
    {
        if (data.currentDirectory == folderPath)
            return;

        if (!FilesystemHandler::isValidFolderPath(folderPath))
            return;

        data.currentDirectory = folderPath;
        clear();

        populateEpisodeList();
    }

    void CreateProjectDialog::setupStatusBar()
    {
        m_StatusBar = new QStatusBar(this);
        layout()->addWidget(m_StatusBar);
    }

    void CreateProjectDialog::setupUI()
    {
        m_StatusBar = new QStatusBar(this);
        layout()->addWidget(m_StatusBar);

        ui.emptyProjectLabel->setStyleSheet("font-size: 32px;");
        ui.sourcesLabel->setStyleSheet("font-size: 24px;");
    }

    QString CreateProjectDialog::createProjectFilepath() const
    {
        auto& settings = SettingsHandler::settings;
        const auto& projectName = ui.projectNameInput->text();
        const auto& projectExtension = settings->getString("project/extension");

        return QDir::cleanPath(data.currentDirectory.path() + QDir::separator() + projectName + "." + projectExtension);
    }

    void CreateProjectDialog::populateEpisodeList()
    {
        for (const auto& fileDirectory : FilesystemHandler::getFilesInDirectory(data.currentDirectory.path()))
        {
            if (!FilesystemHandler::isFileSupportedVideoFormat(fileDirectory))
                continue;

            auto relativePath = data.currentDirectory.relativeFilePath(fileDirectory);
            data.episodeFilepaths.push_back(relativePath);
            ui.videosList->addItem(fileDirectory);
        }
    }

    void CreateProjectDialog::clear()
    {
        data.episodeFilepaths.clear();
        ui.videosList->clear();
    }
}