from pyparadiseo import utils,config

import functools


class helloDecorator(object):
    def __init__(self,f):
        print("inside myDecorator.__init__()")
        self.a = 42
        self.f = f
        # f() # Prove that function definition has completed

    def __call__(self):
        print("inside myDecorator.__call__()")
        self.f()

class aDecorator(object):
    def __init__(self, f):
        print("inside myDecorator.__init__()")
        self.a = 42
        self.f = f
        # f() # Prove that function definition has completed

    def __call__(self):
        print("inside myDecorator.__call__()")
        self.f()



class myDecorator(object):
    def __init__(self, f):
        print("inside myDecorator.__init__()")
        self.f = f
        # f() # Prove that function definition has completed

    def __call__(self):
        print("inside myDecorator.__call__()")
        self.f()



def decoratorFunctionWithArguments(arg1, arg2, arg3):
    def wrap(f):
        print("Inside wrap()")
        def wrapped_f(*args):
            print("Inside wrapped_f()")
            print("Decorator arguments:", arg1, arg2, arg3)
            f(*args)
            print("After f(*args)")
        return wrapped_f
    return wrap



def fitness(fun=None,stype=None):
    """
    fitness decorator

    applied to a python callable it returns a FitnessEval class object initialized with that callable
    """
    # print("Inside fitness decorator")
    if stype is None:
        stype = config._SOLUTION_TYPE

    # print(a)
    class_ = utils.get_class("FitnessEval"+config.TYPES[stype])

    print(class_)

    # @functools.wraps(f)
    def wrap(f):
        # print("Inside wrap()")
        return class_(f)

    if fun is None:
        return wrap
    else:
        return wrap(fun)


def objectives(fun=None,stype=None):
    """
    fitness decorator

    applied to a python callable it returns a FitnessEval class object initialized with that callable
    """
    if fun is not None and stype is None and isinstance(fun,str):
        stype = fun
        fun = None
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype is None:
        stype = config._SOLUTION_TYPE

    # print(a)
    class_ = utils.get_class("ObjectiveEval"+config.TYPES[stype])
    # @functools.wraps(f)
    def wrap(f):
        # print("Inside wrap()")
        return class_(f)

    if fun is None:
        return wrap
    else:
        return wrap(fun)


def initializer(fun=None,stype=None):
    """
    initializer decorator

    applied to a python callable, return eoInit
    """
    if fun is not None and stype is None and isinstance(fun,str):
        stype = fun
        fun = None
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = None
    if stype == 'gen':
        class_ = utils.get_class("pyeoInit")
    else:
        NotImplementedError("Not yet implemented")

    def wrap(f):
        if class_ is not None:
            return class_(f)

    if fun is None:
        return wrap
    else:
        return wrap(fun)


def mutation(fun=None,stype=None):
    """
    applied to callable return eoMonOp
    """
    if fun is not None and stype is None and isinstance(fun,str):
        stype = fun
        fun = None
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = None
    if stype == 'gen':
        class_ = utils.get_class("pyMonOp")
    else:
        NotImplementedError("Not yet implemented")

    def wrap(f):
        if class_ is not None:
            return class_(f)

    if fun is None:
        return wrap
    else:
        return wrap(fun)



def crossover(fun=None,stype=None):
    """
    applied to callable return eoQuadOp
    """
    if fun is not None and stype is None and isinstance(fun,str):
        stype = fun
        fun = None
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = None
    if stype == 'gen':
        class_ = utils.get_class("pyQuadOp")
    else:
        NotImplementedError("Not yet implemented")

    def wrap(f):
        if class_ is not None:
            return class_(f)

    if fun is None:
        return wrap
    else:
        return wrap(fun)


def counting_eval(fun=None,stype=None):
    """
    make a counting evaluator
    """
    if fun is not None and stype is None and isinstance(fun,str):
        stype = fun
        fun = None
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEvalFuncCounter"+config.TYPES[stype])

    def wrapper(f):
        # print("Inside wrap()")
        return class_(f)

    if fun is None:
        return wrapper
    else:
        return wrapper(fun)
