# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import re
import sys
import pydata_sphinx_theme

sys.path.insert(0, os.path.abspath('.'))
# sys.path.insert(0, os.path.abspath('../'))
# sys.path.insert(0, os.path.abspath('../../src/pyparadiseo'))
sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-build'))
# sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-install/src/pyparadiseo/core/_core.so'))
# sys.path.insert(0, os.path.abspath('../src'))
# sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-install/src'))
sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-install/src/pyparadiseo'))
sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-install/src/pyparadiseo/mo'))
sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-install/src/pyparadiseo/moeo'))
sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-install/src/pyparadiseo/eo'))
sys.path.insert(0, os.path.abspath('../../_skbuild/linux-x86_64-3.6/cmake-install/src/pyparadiseo/core'))


# from https://github.com/eudoxos/minieigen/blob/master/doc/source/conf.py

def isBoostFunc(what,obj):
	return what=='function' and obj.__repr__().startswith('<Boost.Python.function object at 0x')

def isBoostMethod(what,obj):
	"I don't know how to distinguish boost and non-boost methods..."
	return what=='method' and obj.__repr__().startswith('<unbound method ')

def isBoostStaticMethod(what,obj):
	return what=='method' and obj.__repr__().startswith('<Boost.Python.function object at 0x')

def fixDocstring(app,what,name,obj,options,lines):
	if isBoostFunc(what,obj) or isBoostMethod(what,obj) or isBoostStaticMethod(what,obj):
		l2=boostFuncSignature(name,obj)[1]
		# we must replace lines one by one (in-place) :-|
		# knowing that l2 is always shorter than lines (l2 is docstring with the signature stripped off)
		for i in range(0,len(lines)):
			lines[i]=l2[i] if i<len(l2) else ''

def fixSignature(app, what, name, obj, options, signature, return_annotation):
	print(what,obj)
	if what in ('attribute','class'): return signature,None
	elif isBoostFunc(what,obj):
		sig=boostFuncSignature(name,obj)[0] or ' (wrapped c++ function)'
		return sig,None
	elif isBoostMethod(what,obj):
		sig=boostFuncSignature(name,obj,removeSelf=True)[0]
		return sig,None
	elif isBoostStaticMethod(what,obj):
		sig=boostFuncSignature(name,obj,removeSelf=True)[0]
        #methods are detected as 'STaTic'?
		# sig=boostFuncSignature(name,obj,removeSelf=False)[0]+' [STATIC]'
		return sig,None

def boostFuncSignature(name,obj,removeSelf=False):
	"""Scan docstring of obj, returning tuple of properly formatted boost python signature
	(first line of the docstring) and the rest of docstring (as list of lines).
	The rest of docstring is stripped of 4 leading spaces which are automatically
	added by boost.

	removeSelf will attempt to remove the first argument from the signature.
	"""
	doc=obj.__doc__
	if doc==None: # not a boost method
		return None,None
	nname=name.split('.')[-1]
	docc=doc.split('\n')
	if len(docc)<2: return None,docc
	doc1=docc[1]
	# functions with weird docstring, likely not documented by boost
	if not re.match('^'+nname+r'(.*)->.*$',doc1):
		return None,docc
	if doc1.endswith(':'): doc1=doc1[:-1]
	strippedDoc=doc.split('\n')[2:]
	# check if all lines are padded
	allLinesHave4LeadingSpaces=True
	for l in strippedDoc:
		if l.startswith('    '): continue
		allLinesHave4LeadingSpaces=False; break
	# remove the padding if so
	if allLinesHave4LeadingSpaces: strippedDoc=[l[4:] for l in strippedDoc]
	for i in range(len(strippedDoc)):
		# fix signatures inside docstring (one function with multiple signatures)
		strippedDoc[i],n=re.subn(r'([a-zA-Z_][a-zA-Z0-9_]*\() \(object\)arg1(, |)',r'\1',strippedDoc[i].replace('->','→'))
	# inspect dosctring after mangling
	sig=doc1.split('(',1)[1]
	if removeSelf:
		# remove up to the first comma; if no comma present, then the method takes no arguments
		# if [ precedes the comma, add it to the result (ugly!)
		try:
			ss=sig.split(',',1)
			if ss[0].endswith('['): sig='['+ss[1]
			else: sig=ss[1]
		except IndexError:
			# grab the return value
			try:
				sig=') -> '+sig.split('->')[-1]
			except IndexError:
				sig=')'
	return '('+sig,strippedDoc

def setup(app):
	app.connect('autodoc-process-docstring',fixDocstring)
	app.connect('autodoc-process-signature',fixSignature)




# -- Project information -----------------------------------------------------

project = 'pyparadiseo'
copyright = '2022, Inria, BONUS Team'
author = 'Jan Gmys'

# The full version, including alpha/beta/rc tags
release = '0.2.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.coverage',
    'sphinx.ext.autosummary',
    'sphinx.ext.napoleon',
    "sphinx.ext.viewcode",
    # "numpydoc",
    # "sphinx_design"
]

# autoclass_content = "both"  # include both class docstring and __init__
autosummary_generate = True  # Turn on sphinx.ext.autosummary

# autodoc_class_signature = "separated"
autodoc_typehints = 'description'

autodoc_default_options = {
    'members': True,
    'member-order': 'groupwise',
    # 'special-members': '__init__,__call__',
    'undoc-members': True,
    'private-members': True,
    'inherited-members': True,
    # "show-inheritance": True,
    'exclude-members': '__weakref__,__new__'
}

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


source_suffix = '.rst'
master_doc = 'index'

# -- Napoleon settings ------------------------
napoleon_numpy_docstring = True
napoleon_use_ivar = True
napoleon_use_param = True

# numpydoc_show_class_members=False

# add_module_names = False

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
html_theme = "pydata_sphinx_theme"
# html_theme = "furo"
# html_theme = 'sphinx_rtd_theme'
#html_theme = 'sphinxdoc'


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

html_logo="_static/paradiseo_logo_200px_dark.png"
html_favicon = "_static/paradiseo_logo_200px_dark.png"
# html_favicon = "_static/eo_logo_fronde.svg"

# gives : WARNING: unsupported theme option 'logo' given
# html_theme_options = {
#     "logo": {
#         "image_light": "paradiseo_logo_200px.png",
#         "image_dark": "paradiseo_logo_200px_dark.png",
#     }
# }

html_theme_options = {
    # ...
    # Note we have omitted `theme-switcher` below
    "navbar_end": ["navbar-icon-links"],
    "gitlab_url": "https://gitlab.inria.fr/paradiseo/pyparadiseo",
    "icon_links": [
            {
                "name": "PyPI",
                "url": "https://pypi.org/project/pyparadiseo",
                "icon": "fas fa-box",
                # "icon" : "_static/pypi_logo_small.svg",
                # "type" : "local"
                # "icon" : "https://pypi.org/static/images/logo-small.95de8436.svg",
                # "type" : "url"
            },
            {
                "name": "PyData",
                "url": "https://pydata.org",
                "icon": "_static/pypi_logo_small.png",
                "type": "local",
                "attributes": {"target": "_blank"},
            },
        ],
    # "show_nav_level": 2
    # "pygment_light_style": "tango",
   # ]


}

html_context = {
   # ...
   "default_mode": "light"
}



# def setup(app):
#     # assign the names to classes imported 'as ...', otherwise autodoc won't document these classes,
#     # and will instead just say 'alias of ...'
#     import pyparadiseo
#     import pyparadiseo.operator
#     pyparadiseo.operator.MonOp.__name__ = 'MonOp'
#     pyparadiseo.operator.BinOp.__name__ = 'BinOp'
#     pyparadiseo.operator.QuadOp.__name__ = 'QuadOp'
#
#     import pyparadiseo.evaluator
#     pyparadiseo.evaluator.PopLoopEval.__name__ = 'PopLoopEval'
#
#     import pyparadiseo.initializer
#     pyparadiseo.initializer.Init.__name__ = 'Init'
#
#     import pyparadiseo.eo
#     pyparadiseo.eo.Algo.__name__ = 'Algo'
#     pyparadiseo.eo.Breed.__name__ = 'Breed'
#     pyparadiseo.eo.Continue.__name__ = 'Continue'
#     pyparadiseo.eo.Merge.__name__ = 'Merge'
#     pyparadiseo.eo.Reduce.__name__ = 'Reduce'
#     pyparadiseo.eo.Replacement.__name__ = 'Replacement'
#     pyparadiseo.eo.Select.__name__ = 'Select'
#     pyparadiseo.eo.Transform.__name__ = 'Transform'
#
#     import pyparadiseo.eo.algo
#     pyparadiseo.eo.algo.SGA.__name__ = 'SGA'
#     pyparadiseo.eo.algo.EasyEA.__name__ = 'EasyEA'
#     pyparadiseo.eo.algo.FastGA.__name__ = 'FastGA'
#
#     import pyparadiseo.eo.selector
#     pyparadiseo.eo.selector.SelectOne.__name__ = 'SelectOne'
#     pyparadiseo.eo.selector.DetTournamentSelect.__name__ = 'DetTournamentSelect'
#     pyparadiseo.eo.selector.TruncatedSelectOne.__name__ = 'TruncatedSelectOne'
#     pyparadiseo.eo.selector.RandomSelect.__name__ = 'RandomSelect'
#     pyparadiseo.eo.selector.NoSelect.__name__ = 'NoSelect'
#     pyparadiseo.eo.selector.SequentialSelect.__name__ = 'SequentialSelect'
#     pyparadiseo.eo.selector.EliteSequentialSelect.__name__ = 'EliteSequentialSelect'
#
#
#     pyparadiseo.eo.selector.DetSelect.__name__ = 'DetSelect'
#     pyparadiseo.eo.selector.SelectMany.__name__ = 'SelectMany'
#     pyparadiseo.eo.selector.SelectNumber.__name__ = 'SelectNumber'
#     pyparadiseo.eo.selector.SelectPerc.__name__ = 'SelectPerc'
#     pyparadiseo.eo.selector.TruncSelect.__name__ = 'TruncSelect'
#     pyparadiseo.eo.selector.TruncatedSelectMany.__name__ = 'TruncatedSelectMany'
#
#     import pyparadiseo.eo.continuator
#     pyparadiseo.eo.continuator.EvalContinue.__name__ = 'EvalContinue'
#     pyparadiseo.eo.continuator.GenContinue.__name__ = 'GenContinue'
#     pyparadiseo.eo.continuator.CombinedContinue.__name__ = 'CombinedContinue'
#     pyparadiseo.eo.continuator.SteadyFitContinue.__name__ = 'SteadyFitContinue'
#     pyparadiseo.eo.continuator.SecondsElapsedContinue.__name__ = 'SecondsElapsedContinue'
#
#     import pyparadiseo.eo.transform
#     pyparadiseo.eo.transform.SGATransform.__name__ = 'SGATransform'
