#include <iostream>
#include <cstdlib>

#include <IBK_WaitOnExit.h>
#include <IBK_SolverArgsParser.h>
#include <IBK_Exception.h>
#include <IBK_messages.h>

#include <MSIM_MasterSim.h>
#include <MSIM_Project.h>
#include <MSIM_ArgParser.h>
#include <MSIM_Constants.h>

int main(int argc, char * argv[]) {
	const char * const FUNC_ID = "[main]";

	try {
		// parse command line
		MASTER_SIM::ArgParser parser;
		parser.parse(argc, argv);

		// help and man-page
		if (parser.handleDefaultFlags(std::cout))
			return EXIT_SUCCESS;

		IBK::WaitOnExit wait(parser.flagEnabled('x'));

		if (parser.flagEnabled('v')) {
			std::cout << "MasterSimulator, version " << MASTER_SIM::LONG_VERSION << std::endl;
			return EXIT_SUCCESS;
		}

		if (!parser.m_projectFile.isValid()) {
			std::cerr << "Missing project file argument. Use --help for syntax." << std::endl;
			return EXIT_FAILURE;
		}

		// setup directory structure
		if (!parser.m_workingDir.exists() && !IBK::Path::makePath(parser.m_workingDir))
			throw IBK::Exception(IBK::FormatString("Error creating working directory: '%1'").arg(parser.m_workingDir), FUNC_ID);

		IBK::Path logPath = parser.m_workingDir / "log";
		if (!logPath.exists() && !IBK::Path::makePath(parser.m_workingDir))
			throw IBK::Exception(IBK::FormatString("Error creating log directory: '%1'").arg(logPath), FUNC_ID);

		// and initialize log file
		IBK::MessageHandler::setupUtf8Console();
		IBK::MessageHandler * msgHandler = IBK::MessageHandlerRegistry::instance().messageHandler();
		msgHandler->setConsoleVerbosityLevel(parser.m_verbosityLevel);
		IBK::Path logFile = logPath / "screenlog.txt";
		std::string errmsg;
		if (!msgHandler->openLogFile(logFile.str(), false, errmsg))
			std::cerr << "Cannot write log file '" << logFile.str() << "'." << std::endl;

		// instantiate project
		MASTER_SIM::Project project;

		// read project file
		IBK::IBK_Message(IBK::FormatString("Reading project '%1'\n").arg(parser.m_projectFile), IBK::MSG_PROGRESS, FUNC_ID);
		project.read(IBK::Path(parser.m_projectFile ));

		// create simulator
		MASTER_SIM::MasterSimulator masterSim;
		// initialize all FMUs (e.g. load dlls/shared libs, parse ModelDescription, do error checking
		masterSim.instantiateFMUs(parser, project);

#if HAVE_SERIALIZATION_CODE
		// set master and all FMUs to start time point
		double tStart = masterSim.tStart(); // override with command line argument
		masterSim.restoreState(tStart, stateDir);
#else
		// run master for entire simulation
		masterSim.initialize();
		masterSim.writeOutputs();
#endif

		if (parser.flagEnabled("test-init")) {
			IBK::IBK_Message("Stopping after successful initialization.\n", IBK::MSG_PROGRESS, FUNC_ID);
			return EXIT_SUCCESS;
		}

		// let master run the simulation until end
		IBK::IBK_Message( IBK::FormatString("Starting simulation from t=%1.\n").arg(masterSim.tCurrent()), IBK::MSG_PROGRESS, FUNC_ID);
		masterSim.simulate();

	}
	catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

