# Minimal makefile for Sphinx documentation
#
SHELL = sh -xv

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?= -E -a
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build


MKDIR=mkdir

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile $(BUILDDIR)
	@$(SPHINXBUILD) -b $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
#	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

clean:
	rm -rf build source/reference/core/_autosummary/* source/reference/eo/_autosummary/* source/reference/mo/_autosummary/* source/reference/operators/_autosummary/* source/reference/moeo/_autosummary/*
