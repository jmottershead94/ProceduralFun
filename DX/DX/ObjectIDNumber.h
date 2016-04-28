// Jason Mottershead, 1300455.

// Header guard.
#ifndef _OBJECTIDNUMBER_H
#define _OBJECTIDNUMBER_H

const static class ObjectIDNumber
{

	public:
		// Attributes.
		const static int ID_TREE = 0;				// If we are selecting a tree object.
		const static int ID_BUSH = ID_TREE + 1;		// If we are selecting a bush object.
		const static int ID_GRASS = ID_BUSH + 1;	// If we are selecting grass objects.

};

#endif