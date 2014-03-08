#pragma once

#include <reflectionzeug/Object.h>
#include <scriptzeug/ScriptContext.h>

#include <QApplication>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QFile>
#include <QWidget>
#include <QMessageBox>
#include <QTextStream>

class ScriptWatcher : public QWidget
{
	Q_OBJECT

public:
	ScriptWatcher(QWidget* parent = 0)
		:QWidget(parent){

			QFileSystemWatcher *watcher = new QFileSystemWatcher();
			watcher->addPath("source/scripts/");
			watcher->addPath("source/scripts/ps4.js");
			//watcher->addPath("source/scripts/test.js");


			//QStringList directoryList = watcher->files();
			//Q_FOREACH(QString directory, directoryList)
			//	qDebug() << "Directory name" << directory << "\n";

			//directoryList = watcher->directories();
			//Q_FOREACH(QString directory, directoryList)
			//	qDebug() << "Directory name" << directory << "\n";


			//QObject::connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(onFolderChanged(QString)));
			QObject::connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(onFolderChanged(QString)));
			QObject::connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(onFolderChanged(QString)));
		}

	~ScriptWatcher(){}

	void watchAndLoad(const QString& filePath, scriptzeug::ScriptContext* scriptContext)
	{
		m_scriptContext = scriptContext;
		m_watchedFilePath = filePath;
		onFolderChanged("");
	};

private:

	QString m_watchedFilePath;
	scriptzeug::ScriptContext* m_scriptContext;

	public slots :
		void onFolderChanged(const QString& str)
		{
			std::cout << "script was changed" << std::endl;
			//foreach(QString file, files) {

			QFile f(m_watchedFilePath);
			if (f.open(QFile::ReadOnly | QFile::Text))
			{
				QTextStream in(&f);
				QString content = in.readAll();

				m_scriptContext->evaluate(content.toStdString());

				f.close();
			}

			// }
		}
};
