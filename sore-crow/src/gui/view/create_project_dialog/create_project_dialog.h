#pragma once
#include "gui/model/project_data.h"
#include "ui_create_project_dialog.h"

namespace sore
{
    class CreateProjectDialog : public QDialog
    {
        Q_OBJECT

    public:
        CreateProjectDialog(QWidget* parent = nullptr);

    public:
        ProjectData getProjectData() const;
        
        QString getFilepath() const;

    private slots:
        void onValidate();

        void onSearchButtonPressed();

        void onLocationChanged(const QString& folderPath);

    private:
        void setupStatusBar();

        void setupUI();

    private:
        QString createProjectFilepath() const;

        void populateEpisodeList();

        void clear();

    private:
        struct Data
        {
            std::vector<QString> episodeFilepaths;
            QDir currentDirectory;
        } data;
        
        QStatusBar* m_StatusBar = nullptr;
        Ui::CreateProjectDialog ui;
    };
}