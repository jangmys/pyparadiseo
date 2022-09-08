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

    function must have follwing signature :

    solution_encoding --> fitness

    Example
    -------
    >>> @decorators.fitness
    def some_function(array):
        return sum(array)

    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("FitnessEval"+config.TYPES[stype])

    def wrap(f):
        return class_(f)

    if fun is None:
        return wrap
    else:
        return wrap(fun)





def fitnessmethod(fun=None,stype=None):
    """
    fitness method decorator

    applied to a python callable it returns a FitnessEval class object initialized with that callable

    function must have follwing signature :

    solution_encoding --> fitness

    Example
    -------
    >>> @decorators.fitness
    def some_function(array):
        return sum(array)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("FitnessEval"+config.TYPES[stype])

    # print(class_)

    def methodwrapper(self,method):
        print('self is ',self)
        return class_(method)

    if fun is None:
        return methodwrapper(self)
    else:
        return methodwrapper(self,fun)



# def MyDecorator(argument,stype=None):
#     if stype is None:
#         stype = config._SOLUTION_TYPE
#
#     class_ = utils.get_class("FitnessEval"+config.TYPES[stype])
#
#     class _MyDecorator(class_):
#         def __init__(self, fn):
#             print("init this thing")
#             class_.__init__(self,fn)
#             self.fn = fn
#
#         def __get__(self, obj, type=None):
#             return functools.partial(self, obj)
#
#         def __call__(self, *args, **kwargs):
#             print("In my decorator before call, with arg ",argument)
#             self.fn(*args, **kwargs)
#             print("In my decorator after call, with arg ",argument)
#
#
#     return _MyDecorator
#


class MyDecoratorClass(object):
    def __init__(self, argument):
        if hasattr('argument', '__call__'):
            self.fn = argument
            self.argument = 'default foo baby'
        else:
            self.argument = argument

    def __get__(self, obj, type=None):
        return functools.partial(self, obj)

    def __call__(self, *args, **kwargs):
        if not hasattr(self, 'fn'):
            self.fn = args[0]
            return self
        print("In my decorator before call, with arg %s",self.argument)
        self.fn(*args, **kwargs)
        print("In my decorator after call, with arg %s" ,self.argument)






def neighbor_fitness(stype=None):
    """
    fitness decorator

    applied to a python callable it returns a FitnessEval class object initialized with that callable
    """
    # print("Inside fitness decorator")
    if stype is None:
        stype = config._SOLUTION_TYPE

    # print(a)
    class_ = utils.get_class("NeighborEval"+config.TYPES[stype])
    # @functools.wraps(f)
    def wrap(f):
        # print("Inside wrap()")
        return class_(f)
    return wrap


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
