#ifndef IBK_UnitH
#define IBK_UnitH

#include <string>
#include <iosfwd>

#include "IBK_configuration.h"

namespace IBK {

/*! The class Unit represents a scientific unit.
	A Unit object represents a scientific unit, such as 'g/mm3'. The units can
	be accessed either with their unique ID-number or their name.
	Every unit that can be related to another unit has a SI base unit. That base
	unit's id-number can be retrieved via the member function base_id().
	\warning Never use Unit before you initialised the unit list!
*/
class Unit {
public:

	/*! Default constructor (creates an undefined unit). */
	Unit() : m_id(0) {}

	/*! Constructor (creates a unit with the id number 'unitid').
		This function throws an out_of_range exception if the 'unitid' is invalid.
	*/
	explicit Unit(unsigned int unitid);

	/*! Constructor (creates a unit with the name 'name').
		This function throws a IBK::Exception if the unit 'name' is not in the unitlist.
		\code
		// creates a mm unit
		IBK::Unit u("mm");
		// creates a kg/m3s unit
		IBK::Unit u("kg/m3s");
		\endcode
	*/
	explicit Unit(const std::string& name);

	/*! Changes the unit to the unit with id number 'unitid'.
		This function throws an IBK::Exception if the 'unitid' is invalid.
	*/
	void set(unsigned int unitid);

	/*! Changes the unit to the unit 'name'.
		This function throws a IBK_exception exception if the unit 'name' is not in the unitlist.
	*/
	void set(const std::string& name);

	/*! Clears the unit (resets it to an undefined state). */
	void clear() { m_id = 0; }

	/*! Returns the id number of the unit. */
	unsigned int id() const { return m_id; }

	/*! Returns the name of the unit. */
	const std::string& name() const;

	/*! Returns the id number of the matching SI base unit.
		The base unit will be returned as id number so that comparing the
		base-id-numbers of two different units will yield whether the
		units are related to each other or not.
	*/
	unsigned int base_id() const;

	/*! Returns the matching SI base unit to the IO_unit. */
	Unit base_unit() const { return Unit(base_id()); }

	/*! Retrieves the calculation factor/summand and the operation necessary to relate a value
		of the unit to the unit given by 'unitid'.
	*/
	void relate_to(unsigned int unitid, double& fact, unsigned int& op);

	/*! Retrieves the calculation factor/summand and the operation necessary to relate a value
		of the unit to the unit given by 'name'.
	*/
	void relate_to(const std::string& name, double& fact, unsigned int& op);

	/*! Retrieves the calculation factor/summand and the operation necessary to relate a value
		of the unit to the unit given by 'unit'.
	*/
	void relate_to(Unit unit, double& fact, unsigned int& op);

	/*! Returns true when the unit is undefined. */
	bool operator!() const { return m_id==0; }

	/*! Comparison operator to sort units. */
	bool operator<(const Unit & other) const { return m_id < other.m_id; }

protected:
	/*! The unique unit ID. */
	unsigned int m_id;
#ifdef IBK_ENABLE_UNIT_NAME
	/*! The string representation of the unit, only needed for debugging purposes.
		\sa IBK_ENABLE_UNIT_NAME
	*/
	std::string m_name;
#endif // IBK_ENABLE_UNIT_NAME

	friend bool operator==(const Unit& lhs, const Unit& rhs);
	friend bool operator!=(const Unit& lhs, const Unit& rhs);
};

/*! Comparison operator== tests, whether two units are equal. */
inline bool operator==(const Unit& lhs, const Unit& rhs) { return lhs.m_id==rhs.m_id; }
/*! Comparison operator!= tests, whether two units are not equal. */
inline bool operator!=(const Unit& lhs, const Unit& rhs) { return lhs.m_id!=rhs.m_id; }
/*! Input stream operator>> reads a unit via name from the stream 'in'. */
std::istream& operator>>(std::istream& in, Unit& unit);
/*! Output stream operator<< writes a unit via name into the stream 'out'. */
std::ostream& operator<<(std::ostream& out, const Unit& unit);

}  // namespace IBK

/*! \file IBK_Unit.h
	\brief Contains the declaration of class Unit, a data type that represents a scientific unit.

	\example Unit.cpp
	This small example demonstrates the use of the classes IBK::Unit and IBK::UnitList.
*/

#endif // IBK_UnitH
