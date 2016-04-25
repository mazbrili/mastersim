#ifndef IBK_MessageHandlerH
#define IBK_MessageHandlerH

#include <string>
#include "IBK_FormatString.h"

namespace IBK {

// Prototypes
class MessageIndentor;

/*! Different categories of messages.
	For MSG_PROGRESS, MSG_CONTINUED and MSG_DEBUG the verbosity level determines whether
	a message is shown or not. MSG_WARNING, MSG_ERROR and are always printed.
*/
enum msg_type_t {
	/*! General progress/informative messages, printed indented. */
	MSG_PROGRESS,
	/*! General progress/informative messages, printed without indentation. */
	MSG_CONTINUED,
	/*! Warnings, by default written to std::cout with different color. */
	MSG_WARNING,
	/*! Errors, should be printed regardless of verbosity level, by default written to std::cerr. */
	MSG_ERROR,
	/*! Debug messages, by default written to std::clog. */
	MSG_DEBUG
};

/*! Default verbosity levels, usable with MSG_PROGRESS and MSG_CONTINUED. */
enum verbosity_levels_t {
	/*! Show always, independent of user settings. */
	VL_ALL = -1,
	/*! Special case, show only if requested verbosity level is == 0.
		Messages with VL_SPECIAL will never be written to the logfile (use for progress bars etc.)
	*/
	VL_SPECIAL = 0,
	/*! Standard, show if requested verbosity is 1 or above. */
	VL_STANDARD = 1,
	/*! Informative (for advanced users), show if requested verbosity is 2 or above. */
	VL_INFO = 2,
	/*! Detailed (for expert users), show if requested verbosity is 3 or above. */
	VL_DETAILED = 3,
	/*! Debug (for developers), show if requested verbosity is 4 or above. */
	VL_DEVELOPER = 4
};

/*! Implementation of a default message handler for console and logfile output.
	This implementation just redirects messages of type error to std::cerr,
	and other messages to std::cout, using coloring in the messages.
*/
class MessageHandler {
public:

	/*! Default constructor. */
	MessageHandler();

	/*! Default destructor (closes logfile of opened and releases memory). */
	virtual ~MessageHandler();

	/*! Re-implemented message function. */
	virtual void msg(	const std::string& msg,
						msg_type_t t = MSG_PROGRESS,
						const char * func_id = NULL,
						int verbose_level = VL_ALL);

	/*! Sets the desired console verbosity level.
		\sa verbosity_levels_t
	*/
	void setConsoleVerbosityLevel(int verbosity);

	/*! Sets the desired logfile verbosity level.
		\sa verbosity_levels_t
	*/
	void setLogfileVerbosityLevel(int verbosity);

	/*! Sets up console on Windows for UTF8 string output. */
	static void setupUtf8Console();

	/*! Create/open log file.
		\param logfile Full path to logfile (directory must exist). Pass an empty string to close the current
					   logfile and disable logfile usage.
		\param append If true, the logfile will be opened in append-mode, otherwise
					  it will be opened in truncation mode.
		\param errmsg Holds an error message in case of error.
		\return Returns false, if logfile cannot be opened. errmsg will hold a description
				of the error that was encountered.
	*/
	virtual bool openLogFile(const std::string& logfile, bool append, std::string & errmsg);

	/*! Sets format of the time stamp to be prepended to the message (only in logfile).
		\param timeStampFormat The format of the time stamp as defined for std::strftime(). Set
								empty string to disable time stamp (the default).
		\note Time stamp is not printed for MSG_CONTINUED type.
		\code
			setTimeStampFormat("%Y-%m-%d %H:%M:%S");
			// prints: "2000-12-25 07:20:15 <msg>"
		\endcode
	*/
	void setTimeStampFormat(const std::string & timeStampFormat);

	/*! Additional prefix to be printed before the function-ID string in the logfile. */
	std::string		m_contextPrefix;

	/*! Number of indentation characters for context string. */
	unsigned int	m_contextIndentation;

protected:
	/*! Holds level of detail that defines which messages should be printed on the console.
		For example, if m_requestedConsoleVerbosityLevel == 2 and a message with
		priority/detail level VL_DETAILED is printed, it will not be shown
		on the console (it is too detailed).
	*/
	int				m_requestedConsoleVerbosityLevel;

	/*! Holds level of detail that defines which messages should be written to logfile.
		\sa m_requestedConsoleVerbosityLevel
	*/
	int				m_requestedLogfileVerbosityLevel;

	/*! Pointer to log file. */
	std::ofstream * m_logfile;

	/*! Time stamp format to be prepended to the message.
		\note Time stamp is not printed for MSG_CONTINUED type.
		\code
			m_timeStampFormat = "%Y-%m-%d %H:%M:%S";
			// gives: "2000-12-25 07:20:15"
		\endcode
	*/
	std::string		m_timeStampFormat;

	/*! Holds current indentation level.
		While you can adjust indentation level manually in your derived class,
		it is recommended to use the MessageHandler::Indentor class to do so.
	*/
	int				m_indentation;

private:

	/*! Closes currently open logfile and delete allocated memory. */
	void closeLogFile();

	friend class MessageIndentor;
};

} // namespace IBK

/*! \file IBK_MessageHandler.h
	\brief Contains declaration of class MessageHandler and available message types.
*/

#endif // IBK_MessageHandlerH

