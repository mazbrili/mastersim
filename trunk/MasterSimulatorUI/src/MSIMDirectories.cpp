#include "MSIMDirectories.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>

#include <IBK_configuration.h>
#include "MSIMUIConstants.h"

QString MSIMDirectories::resourcesRootDir() {
	QString installPath = qApp->applicationDirPath();

#ifdef IBK_DEPLOYMENT
	// deployment mode

#if defined(Q_OS_WIN)
	// in Deployment mode, resources are below install directory
	return installPath + "/resources";
#elif defined(Q_OS_MAC)
	// in deployment mode, we have them in MasterSim.app/Contents/Resources
	return installPath + "/../Resources";
#elif defined(Q_OS_UNIX)

	// in deployment mode, we have them in "/usr/share/MasterSim" or "/usr/local/share/MasterSim"
	// unless otherwise specified in the settings

	QString dbpath;
	if (installPath.indexOf("/usr/bin") == 0)
		dbpath = "/usr/share/MasterSim";
	else if (installPath.indexOf("/usr/local/bin") == 0)
		dbpath = "/usr/local/share/MasterSim";
	else
		dbpath = installPath + "/../Resources/MasterSim";

#endif


#else // IBK_DEPLOYMENT

	// development (IDE) mode

#if defined(Q_OS_WIN)
	// in development mode, we have the resources in the data directory
	// executables are build in bin/debug or bin/release
	return installPath + "/../../data";
#elif defined(Q_OS_MAC)
	// in development mode, we have the resources outside the bundle
	return installPath + "/../../../../data";
#elif defined(Q_OS_UNIX)
	return installPath + "/../../data";
#endif

#endif // IBK_DEPLOYMENT
}


QString MSIMDirectories::translationsDir() {
#ifdef IBK_DEPLOYMENT
	// deployment mode
	return resourcesRootDir() + "/translations";

#else // IBK_DEPLOYMENT
	// development (IDE) mode
	return resourcesRootDir() + "/translations";

#endif // IBK_DEPLOYMENT
}


QString MSIMDirectories::userDataDir() {
	// we have different user data directories, based on OS
#if defined(Q_OS_WIN)
	// on Windows, we store user data unter %HOME%/AppData/Local
	return QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/MasterSim";
#else
	// on Unix/Mac OS we store user data under home directory
	return QDir::toNativeSeparators(QDir::home().absolutePath() + "/.MasterSim");
#endif // Q_OS_WIN
}


QString MSIMDirectories::globalLogFile() {
	return userDataDir() + "/MasterSim.log";
}

