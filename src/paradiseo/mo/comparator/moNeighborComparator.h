/*
  <moNeighborComparator.h>
  Copyright (C) DOLPHIN Project-Team, INRIA Lille - Nord Europe, 2006-2010

  Sébastien Verel, Arnaud Liefooghe, Jérémie Humeau

  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software.  You can  use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".

  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited liability.

  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and,  more generally, to use and operate it in the
  same conditions as regards security.
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.

  ParadisEO WebSite : http://paradiseo.gforge.inria.fr
  Contact: paradiseo-help@lists.gforge.inria.fr
*/

#ifndef _moNeighborComparator_h
#define _moNeighborComparator_h

#include <neighborhood/moNeighbor.h>
#include <comparator/moComparator.h>

/**
 * Comparator of two neighbors
 */
template< class Neighbor >
class moNeighborComparator : public moComparator<Neighbor, Neighbor>
{
public:
    /**
     * Compare two neighbors
     * @param _neighbor1 the first neighbor
     * @param _neighbor2 the second neighbor
     * @return true if the neighbor2 is better than neighbor1
     */
    virtual bool operator()(const Neighbor& _neighbor1, const Neighbor& _neighbor2) {
        // std::cout<<"compare "<<_neighbor1.to_string()<<std::endl;
        // std::cout<<_neighbor1.fitness() << " " << _neighbor2.fitness() << std::endl;
        return (_neighbor1.fitness() < _neighbor2.fitness());
    }

    /**
     * Compare two neighbors
     * @param _neighbor1 the first neighbor
     * @param _neighbor2 the second neighbor
     * @return true if the neighbor2 is equal to neighbor1
     */
    virtual bool equals(const Neighbor& _neighbor1, const Neighbor& _neighbor2) {
        return (_neighbor1.fitness() == _neighbor2.fitness());
    }

    /**
     * Return the class Name
     * @return the class name as a std::string
     */
    virtual std::string className() const {
        return "moNeighborComparator";
    }
};


#endif
