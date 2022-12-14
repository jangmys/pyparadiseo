#ifndef PYMO_H_
#define PYMO_H_

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

#include <neighborhood/moNeighbor.h>
#include <neighborhood/moIndexNeighbor.h>
#include <neighborhood/moBackableNeighbor.h>
#include <neighborhood/moDummyNeighbor.h>

namespace bp = boost::python;


// template<typename Nbor,typename Sol>
// struct Move
// {
//     void operator()(const Nbor& n, Sol& s){
//         s[n.index()] = !s[n.index()];
//         s.invalidate();
//     }
// };





//Neighbor and derivatives IndexNeighbor and BackableNeighbor are ABC with move, moveBack as pure virtuals.
//In paradiseo users will specialize those before instantiating algorithms with derived neighbor types (specializing )

// template<typename SolutionType>
//BASE Neighbor : PyEO + MOVE
//MOVE needs solution type

//NOT deriving from moNeighbor,moIndexNeighbor etc because this is used as a template parameter : mo-classes are instantiated with PyNeighbor...specializing MO for usage in Python.
template<typename SolutionType>
struct PyNeighbor : SolutionType
{
    typedef SolutionType EOT;
    typedef void (*ExtMove)(PyNeighbor& n, SolutionType& s);//Function definition   // magic word!

    PyNeighbor() : SolutionType(),key(0){}

    // PyNeighbor is not supposed to be instantiated directly, move_op is static
    // PyNeighbor(bp::object move,bp::object move_back) : move_op(move),move_back_op(move_back) { }

    // static
    void setMove(bp::object obj) {
        move_op = obj;
    }

    // static
    void setMoveBack(bp::object obj) {
        move_back_op = obj;
    }

    //overridng Nieghbor move (pure virtual) to with Python callback
    virtual void move(SolutionType& _solution)
    {
        if(external_move){
            // std::cout<<"apply external move\n";
            external_move(*this,_solution);
        }else if(move_op.ptr() != Py_None)
        {
            // std::cout<<"apply python move\n";
            // boost::python::call<void>(move_op.ptr(), bp::ptr(this), boost::ref(_solution));
            move_op(bp::ptr(this), boost::ref(_solution));
            //should be a bit faster but less generic...
            // move_op(bp::ptr(this), _solution.encoding);
        }
        else
            std::cout<<"no move defined : do nothing"<<std::endl;
    }

//     //overridng Nieghbor moveBack (pure virtual) to with Python callback
    virtual void moveBack(SolutionType& _solution)
    {
        if(move_back_op.ptr() != Py_None)
        {
            move_back_op(bp::ptr(this), boost::ref(_solution));
            // bp::call<void>(move_back_op.ptr(), bp::ptr(this), boost::ref(_solution));
        }
        else
            std::cout<<"no moveBack defined : do nothing";
    }

    //from IndexNeighbor
	bool equals(PyNeighbor& _neighbor) {
		return (key == _neighbor.index());
	}

    inline unsigned int index() const {
		return key;
	}

    void index(unsigned int _key) {
      key = _key;
    }

    virtual void index(EOT & _solution, unsigned int _key) {
      key = _key;
    }

    PyNeighbor& operator=(const PyNeighbor& _source) {
		SolutionType::operator=(_source);
		this->key = _source.key;
        this->move_op = _source.move_op;
        this->move_back_op = _source.move_back_op;

		return *this;
	}


    void set_external_move(ExtMove func){
        external_move = func;
    }

    static void set_index_table(const std::vector<std::vector<int>>& _index_table)
    {
        index_table = _index_table;
    }

    std::vector<int>& get_indices()
    {
        return index_table[key];
    }

    // ExtMove get_external_move(){return external_move;}
    static std::vector<std::vector<int>> index_table;
protected:
    unsigned int key;
    //base
    bp::object move_op;
    //backable
    bp::object move_back_op;

    ExtMove external_move=nullptr;
};

//static members
// template<typename SolutionType>
// boost::python::object PyNeighbor<SolutionType>::move_op;
//
// template<typename SolutionType>
// boost::python::object PyNeighbor<SolutionType>::move_back_op;

template<typename SolutionType>
std::vector<std::vector<int>> PyNeighbor<SolutionType>::index_table(0,std::vector<int>(0));


#endif
