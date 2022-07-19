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
import sys
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

# -- Project information -----------------------------------------------------

project = 'pyparadiseo'
copyright = '2022, Jan Gmys'
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
    'sphinx.ext.napoleon'
]

autosummary_generate = True  # Turn on sphinx.ext.autosummary

autodoc_class_signature = "separated"
autodoc_typehints = 'description'

autodoc_default_options = {
    'members': True,
    'member-order': 'bysource',
    'special-members': '__init__,__call__',
    'undoc-members': True,
    'private-members': True,
    'inherited-members': True,
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


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
html_theme = 'sphinx_rtd_theme'
#html_theme = 'sphinxdoc'


# html_theme_options = {
#     'analytics_id': 'G-XXXXXXXXXX',  #  Provided by Google in your dashboard
#     'analytics_anonymize_ip': False,
#     'logo_only': False,
#     'display_version': True,
#     'prev_next_buttons_location': 'bottom',
#     'style_external_links': False,
#     'vcs_pageview_mode': '',
#     'style_nav_header_background': 'white',
#     # Toc options
#     'collapse_navigation': True,
#     'sticky_navigation': True,
#     'navigation_depth': 4,
#     'includehidden': True,
#     'titles_only': False
# }

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


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
