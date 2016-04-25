#ifndef IBK_LineH
#define IBK_LineH

#include "IBK_point.h"

namespace IBK {

/*! Represents a line in 2D.
	This class offers some computation functionality for lines.
*/
class Line {
	/*! Tests whether the lines specified by their point coordinates intersect. */
public:
	/*! Returns true if both lines intersect each other.
		This function computes the intersection point of the lines give by the first 8 parameters,
		and stores this pointer in argument p if successful.
	*/
	static bool intersection(
		double line1_x1, double line1_y1, double line1_x2, double line1_y2,
		double line2_x1, double line2_y1, double line2_x2, double line2_y2,
		IBK::point2D<double> & p);

	/*! Default constructor. */
	Line() {}
	/*! Initializing constructor, takes two points. */
	Line(const IBK::point2D<double> & p1, const IBK::point2D<double> & p2);
	/*! Initializing constructor, line is defined by four coordinates. */
	Line(double x1, double y1, double x2, double y2);

	/*! A line is valid when the points m_p1 and m_p2 are not the same. */
	bool isValid() const { return m_p1 != m_p2; }

	/*! Convenience function around intersection(). */
	bool intersects(const Line & other, IBK::point2D<double> & p) {
		return intersection(m_p1.m_x, m_p1.m_y, m_p2.m_x, m_p2.m_y,
							other.m_p1.m_x, other.m_p1.m_y, other.m_p2.m_x, other.m_p2.m_y,
							p);
	}

	/*! First point of line. */
	IBK::point2D<double>	m_p1;
	/*! Second point of line, if the same as m_p1, the line is invalid. */
	IBK::point2D<double>	m_p2;
};

} // namespace IBK

/*! \file IBK_Line.h
	\brief Contains the declaration of the class Line with intersection calculation functionality.
*/

#endif // IBK_LineH
