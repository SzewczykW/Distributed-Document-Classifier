.PHONY: docs

DOXYGEN             = doxygen
DOXYGEN_CFG         = docs/Doxyfile
DOXYGEN_THEME_PATH  = docs/themes/doxygen-awesome-css

docs: $(DOXYGEN_THEME_PATH)
	@echo "Generating documentation using ${DOXYGEN_CFG}..."
	@${DOXYGEN} ${DOXYGEN_CFG} > /dev/null
	@if [ -d "docs/latex" ]; then \
		echo "Building PDF documentation..."; \
		$(MAKE) -C docs/latex > /dev/null; \
        rm -f docs/latex/ddc_docs.pdf || true; \
        ln -s docs/latex/refman.pdf docs/latex/ddc_docs.pdf; \
	fi
	@echo ""
    @echo "Documentation generated:"
	@echo " - HTML: docs/html/index.html"
	@echo " - PDF:  docs/latex/ddc_docs.pdf"

$(DOXYGEN_THEME_PATH):
	git submodule update --init --recursive
