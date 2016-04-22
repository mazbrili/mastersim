#ifndef MASTER_SIM_H
#define MASTER_SIM_H

#include <IBK_Path.h>

namespace eas {

class MasterSimulator {
public:
	MasterSimulator();

	/*! Reads project file, utf8 encoded path name. */
	void readProjectFile(const IBK::Path & prjFile);

	/*! initialize all FMUs (e.g. load dlls/shared libs, parse ModelDescription, do error checking. */
	void instantiateFMUs(const IBK::Path & workingDir);

	/*! Initialize FMUs (enter initialization mode, iterate over start conditions, everything
		up to ExitInitializationMode).
		After call to this function, everything is initialized so that doStep() can be called.
	*/
	void initialize();

	/*! Restores state of master and all FMUs using data stored below state directory. */
	void restoreState(double t, const IBK::Path & stateDirectory);

	double tStart() const { return m_tStart; }
	double tEnd() const { return m_tEnd; }
	double tCurrent() const { return m_tCurrent; }

	/*! Attempts to integrate forward a single step.
		For fixed step size mode, the step size m_h is used.
		For variably step size mode, the step size m_h is attempted, and in case of
		convergence failure or error check failure, step size is reduced and
		integration step attempted again.

		Function throws an Exception in case of failure (solver cannot continue).
		For fixed step size mode failure
		means convergence failure or FMU reported error. For variable step size
		failure means time step reduction below acceptable limit or any other error.

		For variable time step scheme you can query the time step finally used by
		using tStepSize() member function. The time point reached by the solver
		can be queried with tCurrent().
	*/
	void doStep();

	/*! Time step size used in last call to doStep(). */
	double tStepSize() const { return m_tStepSize; }

	/*! Updates outputs when scheduled. */
	void writeOutputs();

	const std::string & projectFile() const { return m_projectFile; }

private:
	std::string		m_projectFile;
	double			m_tStart;
	double			m_tEnd;
	double			m_tCurrent;
	double			m_tStepSize;

	double			m_tLastOutput;
	double			m_tOutputStepMin;
};

} // namespace eas

#endif // MASTER_SIM_H
