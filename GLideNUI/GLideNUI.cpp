#include <thread>
#include <QApplication>

#include "GLideNUI.h"
#include "AboutDialog.h"
#include "ConfigDialog.h"
#include "Settings.h"

#ifndef _DEBUG
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

inline void initMyResource() { Q_INIT_RESOURCE(icon); }
inline void cleanMyResource() { Q_CLEANUP_RESOURCE(icon); }

static
int openConfigDialog(const wchar_t * _strFileName, bool & _accepted)
{
	cleanMyResource();
	initMyResource();
	QString strIniFileName = QString::fromWCharArray(_strFileName);
	loadSettings(strIniFileName);

	int argc = 0;
	char * argv = 0;
	QApplication a(argc, &argv);

	ConfigDialog w;

	w.setIniPath(strIniFileName);
	w.show();
	const int res = a.exec();
	_accepted = w.isAccepted();
	return res;
}

static
int openAboutDialog()
{
	cleanMyResource();
	initMyResource();

	int argc = 0;
	char * argv = 0;
	QApplication a(argc, &argv);

	AboutDialog w;
	w.show();
	return a.exec();
}

bool runConfigThread(const wchar_t * _strFileName) {
	bool accepted = false;
	std::thread configThread(openConfigDialog, _strFileName, std::ref(accepted));
	configThread.join();
	return accepted;
}

int runAboutThread() {
	std::thread aboutThread(openAboutDialog);
	aboutThread.join();
	return 0;
}

EXPORT bool CALL RunConfig(const wchar_t * _strFileName)
{
	return runConfigThread(_strFileName);
}

EXPORT int CALL RunAbout()
{
	return runAboutThread();
}

EXPORT void CALL LoadConfig(const wchar_t * _strFileName)
{
	loadSettings(QString::fromWCharArray(_strFileName));
}

EXPORT void CALL LoadCustomRomSettings(const wchar_t * _strFileName, const char * _romName)
{
	loadCustomRomSettings(QString::fromWCharArray(_strFileName), _romName);
}
