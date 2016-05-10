#ifndef MSIM_ABSTRACTALGORITHM_H
#define MSIM_ABSTRACTALGORITHM_H

namespace MASTER_SIM {

class MasterSim;

/*! Abstract base class for implementation of master algorithms.
*/
class AbstractAlgorithm {
public:
	enum Result {
		R_CONVERGED,
		R_DIVERGED,
		R_ITERATION_LIMIT_EXCEEDED,
		R_RETRY
	};

	/*! Default constructor. */
	AbstractAlgorithm(MasterSim * master) : m_master(master) {}

	/*! Virtual d'tor. */
	virtual ~AbstractAlgorithm() {}

	/*! Main stepper function for master algorithm. */
	virtual Result doStep() = 0;

protected:
	/*! Cached pointer to master data structure (not owned). */
	MasterSim	*m_master;
};

} // namespace MASTER_SIM

#endif // MSIM_ABSTRACTALGORITHM_H