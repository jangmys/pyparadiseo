/*
* <moeoDistance.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2007
* (C) OPAC Team, LIFL, 2002-2007
*
* Arnaud Liefooghe
*
* This software is governed by the CeCILL license under French law and
* abiding by the rules of distribution of free software.  You can  use,
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info".
*
* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited liability.
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or
* data to be ensured and,  more generally, to use and operate it in the
* same conditions as regards security.
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*
* ParadisEO WebSite : http://paradiseo.gforge.inria.fr
* Contact: paradiseo-help@lists.gforge.inria.fr
*
*/
//-----------------------------------------------------------------------------

#ifndef MOEODISTANCE_H_
#define MOEODISTANCE_H_

#include <eoFunctor.h>
#include <utils/eoRealBounds.h> //was missing (JG)

/**
 * The base class for distance computation.
 */
template < class MOEOT , class Type >
class moeoDistance : public eoBF < const MOEOT &, const MOEOT &, const Type >
  {
  public:

    /**
     * Nothing to do
     * @param _pop the population
     */
    virtual void setup(const eoPop < MOEOT > & _pop)
    {}


    /**
     * Nothing to do
     * @param _min lower bound
     * @param _max upper bound
     * @param _obj the objective index
     */
    virtual void setup(double _min, double _max, unsigned int _obj)
    {}


    /**
     * Nothing to do
     * @param _realInterval the eoRealInterval object
     * @param _obj the objective index
     */
    virtual void setup(eoRealInterval _realInterval, unsigned int _obj)
    {}

  };

#endif /*MOEODISTANCE_H_*/
