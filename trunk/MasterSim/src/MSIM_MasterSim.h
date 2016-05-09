#ifndef MSIM_MASTERSIM_H
#define MSIM_MASTERSIM_H

#include <utility> // for std::pair

#include <IBK_Path.h>

#include "MSIM_Project.h"
#include "MSIM_ArgParser.h"
#include "MSIM_FMUManager.h"
#include "MSIM_Slave.h"
#include "MSIM_OutputWriter.h"


/*! Namespace MASTER_SIM holds all classes, functions, types of the MasterSim library. */
namespace MASTER_SIM {

class AbstractAlgorithm;

/*! Main class of the library.
	This class encapsulates all functionality of the master and can be used from GUI
	applications as well as console masters.
*/
class MasterSim {
public:
	/*! Constructor. */
	MasterSim();

	/*! Destructor, cleans up slaves. */
	~MasterSim();

	/*! Initialize all FMUs (e.g. load dlls/shared libs, parse ModelDescription, do error checking.
		Throws an exception if an error occurs during instantiation.
	*/
	void instantiateFMUs(const ArgParser & args, const Project & prj);

	/*! Initialize FMUs (enter initialization mode, iterate over start conditions, everything
		up to ExitInitializationMode).
		After call to this function, everything is initialized so that doStep() can be called.
	*/
	void initialize();

	/*! Open output files. */
	void openOutputFiles(bool reopen) { m_outputWriter.openOutputFiles(reopen); }

	/*! Restores state of master and all FMUs using data stored below state directory. */
	void restoreState(double t, const IBK::Path & stateDirectory);

	/*! Serializes all FMUs and state of master to file within stateDirectory. */
	void storeState(const IBK::Path & stateDirectory);

	/*! Contains the core simulation loop.
		This is essentially a convenience function around doStep() calls.
		This function calls doStep() until simulation time has reached/passed time point.
		After each successful step, the function writeOutputs() is called.
	*/
	void simulate();

	/*! Returns current time point (of master state). */
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

	/*! Writes outputs. */
	void writeOutputs() { m_outputWriter.writeOutputs(m_tCurrent); }

private:
	/*! Defines a group of FMUs that belong to a cycle.
		A cycle may only contain a single slave.
	*/
	struct Cycle {
		/*! Holds pointers to all slaves belonging to this cycle in the order they should be
			evaluated (for Gauss-Seidel).
			\note Pointers are not owned.
		*/
		std::vector<Slave*>			m_slaves;
	};

	/*! Variable mapping structure, used to map exchange quantities between FMUs for the different data types. */
	struct VariableMapping {
		unsigned int	m_globalIndex; // matches index of VariableMapping object in m_variableMappings vector

		Slave			*m_outputSlave;
		unsigned int	m_outputLocalIndex; // index of variable slot in locally cached output variables vector

		Slave			*m_inputSlave;
		unsigned int	m_inputValueReference;
	};

	/*! This function handles the FMU unzipping and shared library loading stuff. */
	void importFMUs();

	/*! Checks all imported FMUs whether they provide the necessary functionality for the selected master algorithm. */
	void checkCapabilities();

	/*! Here all simulation slaves are instantiated. */
	void instatiateSlaves();

	/*! Convenience function that extracts slave and variables names from flatVarName and
		looks up slave and FMI variable in associated FMU.
	*/
	std::pair<const Slave*, const FMIVariable *> variableByName(const std::string & flatVarName) const;

	/*! Collects all output variables from all slaves and adds them to the variables vector, ordered according to cycles. */
	void composeVariableVector();

	/*! Initializes the master algorithm. */
	void initMasterAlgorithm();

	/*! Computes initial conditions and updates output caches of all slaves so that master algorithms can start. */
	void initialConditions();

	/*! Performs error checking.
		If test is successful, m_stepSizeProposed is updated as well.
		\return Returns true if test was passed.
	*/
	bool doErrorCheck();

	/*! Performs convergence test by comparing values in m_ytNext and m_ytNextIter.
		\return Returns true if test has passed.
	*/
	bool doConvergenceTest();

	/*! Updates all connected inputs of a given slave using the variables in provided vector. */
	void updateSlaveInputs(Slave * slave,
						   const std::vector<double> & variables,
						   const std::vector<int> &intVariables,
						   const std::vector<fmi2Boolean> &boolVariables,
						   const std::vector<std::string> &stringVariables);

	/*! Copies all connected outputs of a given slave into the vector 'variables'. */
	void syncSlaveOutputs(const Slave * slave,
						  std::vector<double> & realVariables,
						  std::vector<int> & intVariables,
						  std::vector<fmi2Boolean> &boolVariables,
						  std::vector<std::string> & stringVariables);

	/*! Loops over all slaves and retrieves current states. */
	void storeCurrentSlaveStates(std::vector<void *> & slaveStates);


	/*! Copy of arg parser. */
	ArgParser				m_args;
	/*! Copy of project data. */
	Project					m_project;

	/*! Holds and owns all FMU objects (contant of the FMU archives). */
	FMUManager				m_fmuManager;

	/*! Vector of instantiated simulation slaves (owned by MasterSim). */
	std::vector<Slave*>		m_slaves;

	/*! All cycles in order of their evaluation priority. */
	std::vector<Cycle>		m_cycles;

	/*! Pointer to the actual master algorithm implementation (owned). */
	AbstractAlgorithm		*m_masterAlgorithm;

	/*! Current simulation time point. */
	double					m_tCurrent;

	/*! Simulation time step (that was used in last call to doStep(). */
	double					m_tStepSize;

	/*! Simulation time step that shall be used in next call to doStep(). */
	double					m_tStepSizeProposed;

	/*! Manager of output files, handles all output file writing. */
	OutputWriter			m_outputWriter;


	/*! Mapping of all connected variables of type real. */
	std::vector<VariableMapping>	m_realVariableMapping;
	/*! Mapping of all connected variables of type int. */
	std::vector<VariableMapping>	m_intVariableMapping;
	/*! Mapping of all connected variables of type bool. */
	std::vector<VariableMapping>	m_boolVariableMapping;
	/*! Mapping of all connected variables of type string. */
	std::vector<VariableMapping>	m_stringVariableMapping;

	// exchange variables of type real

	/*! Slave variables (input and output) at current master time. */
	std::vector<double>				m_realyt;
	/*! Slave variables (input and output) at next master time (may be iterative quantities). */
	std::vector<double>				m_realytNext;
	/*! Slave variables (input and output) at next master time and last iteration level, needed for convergence test. */
	std::vector<double>				m_realytNextIter;

	// exchange variables of type int

	/*! Slave variables (input and output) at current master time. */
	std::vector<int>				m_intyt;
	/*! Slave variables (input and output) at next master time (may be iterative quantities). */
	std::vector<int>				m_intytNext;
	/*! Slave variables (input and output) at next master time and last iteration level, needed for convergence test. */
	std::vector<int>				m_intytNextIter;

	// exchange variables of type boolean

	/*! Slave variables (input and output) at current master time. */
	std::vector<fmi2Boolean>		m_boolyt;
	/*! Slave variables (input and output) at next master time (may be iterative quantities). */
	std::vector<fmi2Boolean>		m_boolytNext;
	/*! Slave variables (input and output) at next master time and last iteration level, needed for convergence test. */
	std::vector<fmi2Boolean>		m_boolytNextIter;

	// exchange variables of type string

	/*! Slave variables (input and output) at current master time. */
	std::vector<std::string>		m_stringyt;
	/*! Slave variables (input and output) at next master time (may be iterative quantities). */
	std::vector<std::string>		m_stringytNext;
	/*! Slave variables (input and output) at next master time and last iteration level, needed for convergence test. */
	std::vector<std::string>		m_stringytNextIter;

	/*! Vector for holding states of FMU slaves at begin of master algorithm to roll back during iterations. */
	std::vector<void*>				m_iterationStates;

	template<typename T>
	static void copyVector(const std::vector<T> & src, std::vector<T> & target) {
		IBK_ASSERT(src.size() == target.size());
		IBK_ASSERT(!src.empty());

		std::memcpy(&target[0], &src[0], src.size()*sizeof(T));
	}

	friend class AlgorithmGaussJacobi;
	friend class AlgorithmGaussSeidel;
	friend class AlgorithmNewton;
};

} // namespace MASTER_SIM

#endif // MSIM_MASTERSIM_H
