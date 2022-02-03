#ifndef PYMO_H_
#define PYMO_H_

#include "pyeot.h"

#include <neighborhood/moNeighbor.h>
#include <neighborhood/moIndexNeighbor.h>
#include <neighborhood/moBackableNeighbor.h>
#include <neighborhood/moDummyNeighbor.h>

namespace bp = boost::python;

struct PyNeighbor : moIndexNeighbor<PyEOT>,moBackableNeighbor<PyEOT>,bp::wrapper<moIndexNeighbor<PyEOT>>
{
public:
    typedef PyEOT EOT;

    PyNeighbor() : moIndexNeighbor<PyEOT>() { }

    PyNeighbor(bp::object move) : moIndexNeighbor<PyEOT>(),move_op(move) { }

    PyNeighbor(bp::object move,bp::object move_back) : moIndexNeighbor<PyEOT>(),move_op(move),move_back_op(move_back) { }

    PyNeighbor(const PyNeighbor& _n) : moNeighbor<PyEOT>(),moIndexNeighbor<PyEOT>(_n),moBackableNeighbor<PyEOT>(_n){
        fitness(_n.fitness());
        move_op = _n.move_op;
        move_back_op = _n.move_op;
    };

    void setMove(bp::object obj) {
        move_op = obj;
    }

    void setMoveBack(bp::object obj) {
        move_back_op = obj;
    }

    //overridng Nieghbor move (pure virtual) to with Python callback
    void move(PyEOT& _solution)
    {
        if(move_op.ptr() != Py_None)
        {
            bp::call<void>(move_op.ptr(), bp::ptr(this), boost::ref(_solution));
        }
        else
            std::cout<<"no move defined : do nothing";
    }

//     //overridng Nieghbor moveBack (pure virtual) to with Python callback
    void moveBack(PyEOT& _solution)
    {
        if(move_back_op.ptr() != Py_None)
        {
            bp::call<void>(move_back_op.ptr(), bp::ptr(this), boost::ref(_solution));
        }
        else
            std::cout<<"no moveBack defined : do nothing";
    }

    //virtual with defaults
    bool equals(PyNeighbor& _neighbor)
    {
        if (bp::override eq = this->get_override("equals"))
        {
            return eq(_neighbor);
        }
        return PyNeighbor::equals(_neighbor); //(false)
    }
    bool default_equals(PyNeighbor& _neighbor) {
        return this->moNeighbor<PyEOT>::equals(_neighbor);
    }

    //like for PyEOT, "emulating" inheritance from EO ... this is not DRY
    bp::object getFitness() const {
        return invalid()? bp::object(): bp::object(fitness());
    }
    void setFitness(bp::object f) {
        if(f.ptr() == Py_None)
        {
            invalidate();
            return;
        }

        bp::extract<double> x(f);
        if(x.check())
        {
            double d = x();
            fitness(d);
        }else{
            throw index_error("can't extract fitness\n");
        }
    }

    bool operator<(const PyNeighbor& _other) const
    {
        return getFitness() < _other.getFitness();
    }

    std::string to_string() const
    {
        std::string result;
        result += ' ';
        if(!invalid())
            result += bp::extract<const char*>(bp::str(getFitness()));
        else result += std::string("invalid");
        result += ' ';
        result += std::to_string(key);
        return result;
    }
//
private:
    bp::object move_op;
    bp::object move_back_op;
};
//
//
//
//
//
//
//
//
//
// struct PyEOTNeighbor : moNeighbor<PyEOT,PyEOT::ObjectiveVector>,wrapper<moNeighbor<PyEOT,PyEOT::ObjectiveVector>>
// {
// public:
//     PyEOTNeighbor() : moNeighbor<PyEOT,PyEOT::ObjectiveVector>() { std::cout << "PyEOTNbor DEFAULT CONSTRUCTOR" << std::endl; }
//
//     virtual void move(PyEOT& _solution)
//     {
//         std::cout<<"c++ move\n";
//         this->get_override("move")(_solution);
//     }
//
//     //virtual with defaults
//     bool equals(PyEOTNeighbor& _neighbor)
//     {
//         if (bp::override equals = this->get_override("equals"))
//         {
//             return equals(_neighbor);
//         }
//         return PyEOTNeighbor::equals(_neighbor); //(false)
//     }
//     bool default_equals(PyEOTNeighbor& _neighbor) {
//         return this->moNeighbor<PyEOT,PyEOT::ObjectiveVector>::equals(_neighbor);
//     }
//
//     //like for PyEOT, "emulating" inheritance from EO
//     //
//     //the code duplication is ugly...
//     //
//     bp::object getFitness() const {
//         // printf("get fitness\n");
//         return invalid()? bp::object(): bp::object(fitness());
//     }
//     void setFitness(bp::object f) {
//         // printf("set fitness\n");
//         bp::extract<double> x(f);
//         if(x.check())
//         {
//             double d = x();
//             fitness(d);
//         }else{
//             throw index_error("fitness not a double\n");
//             // invalidateFitness();
//         }
//             // throw index_error("fitness not a double\n");
//             // if (!x.check())
//         // if (f == Fitness()) invalidateFitness(); else fitness(f);
//     }
// };

#endif
