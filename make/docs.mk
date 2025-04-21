.PHONY: docs

DOXYGEN_CFG = docs/Doxyfile
DOXYGEN     = doxygen

docs:
	@echo "Generating documentation using ${DOXYGEN_CFG}..."
	@${DOXYGEN} ${DOXYGEN_CFG}
	@if [ -d "docs/latex" ]; then \
		echo "Building PDF documentation..."; \
		$(MAKE) -C docs/latex > /dev/null; \
	fi
	@echo ""
	@echo "Documentation generated:"
	@echo " - HTML: docs/html/index.html"
	@echo " - PDF:  docs/latex/refman.pdf"

