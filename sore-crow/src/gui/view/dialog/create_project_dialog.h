#pragma once
#include <vector>
#include <string>
#include <QtWidgets/QDialog>
#include "ui_create_project_dialog.h"

#include "gui/data/create_project_dialog_data.h"

namespace sore
{
    class CreateProjectDialog : public QDialog
    {
        Q_OBJECT

    public:
        CreateProjectDialog(QWidget* parent = nullptr);

    public:
        CreateProjectDialogData getData();

        void validate();

    private:
        void setupLabels();

        void setupIcons();

    private:
        void handleLocationTextChange();

        void handleLocationBtn();

        void handleEpisodesChanged();

        void handleValidation();

    private:
        void clearLists();

    signals:
        void episodesChanged();

    private:
        Ui::CreateProjectDialog ui;
        QStatusBar* statusBar;

    private:
        std::vector<std::string> m_Episodes;
        std::string m_CurrentFilepath;
    };
}