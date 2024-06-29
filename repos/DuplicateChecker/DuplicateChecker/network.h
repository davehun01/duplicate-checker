#include <iostream>
#include <vector>

struct Network{
	enum POS { ID = 0, FROM_NODE, TO_NODE, FRC_A, ONEWAY, SPEED_LIMIT, KPH, STREET_NAME, STREET_TYPE, SCHEMELEFT,
		FROMLEFT, TOLEFT, SCHEMERIGHT, FROMRIGHT, TORIGHT, ORDER8_NAME };

	int		id;					// road element identification code
	int		from_node;			// index to the node_id field of the junctions table
	int		to_node;			// index to the node_id field of the junctions table
	std::string	frc_a;			// functional road class
	std::string	oneway;			// one-way information
	int		speed_limit;		// official speed limit on road element in km/h
	int		kph;				// average driving speed on the road element
	std::string	street_name;	// official name of the street
	std::string	street_type;	// official type/suffix of the street name
	std::string	schemeleft;		// left house numbering scheme
	int		fromleft;			// left starting house number
	int		toleft;				// left ending house number
	std::string	schemeright;		// right house numbering scheme
	int		fromright;			// right starting house number
	int		toright;			// right ending house number
	std::string	order8_name;	// the standard name of the order8 administrative area
};
