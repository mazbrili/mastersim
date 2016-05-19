#include "MSIM_AlgorithmNewton.h"

#include <IBK_assert.h>

#include "MSIM_MasterSim.h"
#include "MSIM_Slave.h"

namespace MASTER_SIM {

void AlgorithmNewton::init() {
	// resize matrix and variable index mapping vectors
	unsigned int nCycles = m_master->m_cycles.size();
	m_jacobianMatrix.resize(nCycles);
	m_variableIdxMapping.resize(nCycles);

	// now process each cycle
	for (unsigned int c=0; c<nCycles; ++c) {
		const MasterSim::Cycle & cycle = m_master->m_cycles[c];
		// loop over all variables and collect indexes of all variables
		// that are both input and output of the slaves in this cycle
		for (unsigned int varIdx=0; varIdx<m_master->m_realVariableMapping.size(); ++varIdx) {
			// check if slave is in current cycle
			bool foundInput = false;
			bool foundOutput = false;
			for (unsigned int s=0; s<cycle.m_slaves.size(); ++s) {
				const Slave * slave = cycle.m_slaves[s];
				// mind that an output of a slave may be directly connected to its own input
				// so we can have the same slave being used
				if (m_master->m_realVariableMapping[varIdx].m_inputSlave == slave)
					foundInput = true;
				if (m_master->m_realVariableMapping[varIdx].m_outputSlave == slave)
					foundOutput = true;
			}
			if (foundInput && foundOutput) {
				// remember global variable index
				m_variableIdxMapping[c].push_back(varIdx);
			}
		}

		// finally resize DenseMatrix
		unsigned int dim = m_variableIdxMapping[c].size();
		if (dim != 0)
			m_jacobianMatrix[c].resize(dim);
	}
}


AlgorithmNewton::Result AlgorithmNewton::doStep() {
	const char * const FUNC_ID = "[AlgorithmNewton::doStep]";

	// to make things simpler, let's just use fmi2status variables
	IBK_STATIC_ASSERT((int)fmiOK == (int)fmi2OK);

	// master and FMUs are expected to be at current time point t
	double t = m_master->m_t;

	// all slave output variables are expected to be in sync with internal states of slaves
	// i.e. cacheOutputs() has been called successfully on all slaves

	// global variable array is expected to be in sync with all slaves


	// ** algorithm start **

	// create a copy of variable array to updated variable array, since we will use this for input in Gauss-Seidel
	MasterSim::copyVector(m_master->m_realyt, m_master->m_realytNext);
	MasterSim::copyVector(m_master->m_intyt, m_master->m_intytNext);
	MasterSim::copyVector(m_master->m_boolyt, m_master->m_boolytNext);
	std::copy(m_master->m_stringyt.begin(), m_master->m_stringyt.end(), m_master->m_stringytNext.begin());

	// loop over all cycles
	for (unsigned int c=0; c<m_master->m_cycles.size(); ++c) {
		const MasterSim::Cycle & cycle = m_master->m_cycles[c];

		unsigned int iteration = 0; // iteration counter in current cycle
		while (++iteration <= m_master->m_project.m_maxIterations) {

			// create copy of current variables
			/// \todo only copy variables that are port of this cycle, may speed up code a 'little'
			MasterSim::copyVector(m_master->m_realytNext, m_master->m_realytNextIter);
			MasterSim::copyVector(m_master->m_intytNext, m_master->m_intytNextIter);
			MasterSim::copyVector(m_master->m_boolytNext, m_master->m_boolytNextIter);
			MasterSim::copyVector(m_master->m_stringytNext, m_master->m_stringytNextIter);

			// in first iteration generate Jacobian matrix
			if (iteration == 1) {
				generateJacobian(c);
			}
			// roll-back all slaves in this cycle, except for first iteration
			else {
				for (unsigned int s=0; s<cycle.m_slaves.size(); ++s) {
					Slave * slave = cycle.m_slaves[s];
					slave->setState(t, m_master->m_iterationStates[slave->m_slaveIndex]);
				}
			}

			// loop over all slaves
			for (unsigned int s=0; s<cycle.m_slaves.size(); ++s) {
				Slave * slave = cycle.m_slaves[s];

				// update input variables in all slaves, using updated variables from time t
				m_master->updateSlaveInputs(slave, m_master->m_realytNext, m_master->m_intytNext, m_master->m_boolytNext, m_master->m_stringytNext);

				// advance slave, we have no roll-back
				int res = slave->doStep(m_master->m_h, true);
				switch (res) {
					case fmi2Discard	:
					case fmi2Error		:
						return R_RETRY;
					case fmi2Pending	: throw IBK::Exception("Asynchronous slaves are not supported, yet.", FUNC_ID);
					case fmi2Fatal		:
						throw IBK::Exception(IBK::FormatString("Error in doStep() call of FMU slave '%1'").arg(slave->m_name), FUNC_ID);
					case fmi2OK			:
					default				:
						break;
				}

				// slave is now at time level tNext and its outputs are updated accordingly
				// sync results into vector with newly computed quantities, so that next slave will use newly
				// computed values already
				m_master->syncSlaveOutputs(slave, m_master->m_realytNext, m_master->m_intytNext, m_master->m_boolytNext, m_master->m_stringytNext);
			}

			// do convergence test
			if (m_master->doConvergenceTest())
				break; // break iteration loop
		}
		if (iteration > m_master->m_project.m_maxIterations)
			return R_ITERATION_LIMIT_EXCEEDED;
	}

	// ** algorithm end **

	// m_XXXyt     -> still values at time point t
	// m_XXXytNext -> values at time point t + h
	return R_CONVERGED;
}


void AlgorithmNewton::generateJacobian(unsigned int cycleIdx) {

	// loop all variables in this cycle
	// modify variable

	// loop all slaves in cycle
	//   update all slave inputs
	//   doStep in all slaves

	// compute difference quotient for each variable in cycle
	// reset all slaves


	// factorize matrix
}


} // namespace MASTER_SIM