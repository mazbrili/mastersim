#include "MSIM_MasterSim.h"

namespace MASTER_SIM {

MasterSimulator::MasterSimulator() {
}




void MasterSimulator::instantiateFMUs(const Project & prj, const IBK::Path & workingDir) {

	m_tStepSize = prj.m_tStepStart;
}


void MasterSimulator::initialize() {
	m_tLastOutput = -1;
}


void MasterSimulator::restoreState(double t, const IBK::Path & stateDirectory) {

}


void MasterSimulator::doStep() {

	// step size reduction loop



	// ..

	m_tCurrent += m_tStepSize;
}


void MasterSimulator::writeOutputs() {
	if (m_tLastOutput < 0 || m_tLastOutput + m_tOutputStepMin < m_tCurrent) {
		// outputHandler.write()....
	}
}


} // namespace MASTER_SIM

