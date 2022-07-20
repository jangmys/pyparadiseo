import importlib


def get_class(class_,module_="pyparadiseo._core"):
    """
    get ``class_`` (str) from ``module_`` (str)
    """
    mod = importlib.import_module(module_)
    return getattr(mod,class_)
