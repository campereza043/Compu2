# Makefile Wrapper para CMake y utilidades del examen

BUILD_DIR = build
RESULTS_DIR = results
DOCS_DIR = documents

.PHONY: all run clean clean_all docs

all: $(BUILD_DIR)
	@echo "Configurando y construyendo con CMake..."
	cd $(BUILD_DIR) && cmake .. && make

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	@echo "Ejecutando simulacion y graficando..."
	cd $(BUILD_DIR) && make run_pipeline

docs:
	@echo "Generando documentacion..."
	mkdir -p $(DOCS_DIR)
	# 1. Crear Doxyfile si no existe (igual que antes)
	@if [ ! -f Doxyfile ]; then \
		doxygen -g; \
		sed -i 's/PROJECT_NAME.*/PROJECT_NAME = "Flashing Ratchet"/' Doxyfile; \
		sed -i 's/OUTPUT_DIRECTORY.*/OUTPUT_DIRECTORY = $(DOCS_DIR)\/doxygen/' Doxyfile; \
		sed -i 's/RECURSIVE.*/RECURSIVE = YES/' Doxyfile; \
	fi
	# 2. Correr Doxygen
	doxygen Doxyfile
	# 3. Crear reporte LaTeX con las 3 imagenes separadas
	@echo "\\documentclass{article}" > $(DOCS_DIR)/reporte.tex
	@echo "\\usepackage{graphicx}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\usepackage{float}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\usepackage[utf8]{inputenc}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\title{Examen 2: Motor Molecular Flashing Ratchet}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\author{Grupo 2}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\begin{document}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\maketitle" >> $(DOCS_DIR)/reporte.tex
	
	@echo "\\section{Introduccion}" >> $(DOCS_DIR)/reporte.tex
	@echo "Este documento presenta la simulacion de un Flashing Ratchet implementado en C++." >> $(DOCS_DIR)/reporte.tex
	
	@echo "\\section{Resultados}" >> $(DOCS_DIR)/reporte.tex
	@echo "A continuacion se presentan las graficas obtenidas de la simulacion." >> $(DOCS_DIR)/reporte.tex
	
	@echo "\\begin{figure}[H]" >> $(DOCS_DIR)/reporte.tex
	@echo "\\centering" >> $(DOCS_DIR)/reporte.tex
	@echo "\\includegraphics[width=0.9\\textwidth]{../results/trayectoria.png}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\caption{Trayectoria de la particula mostrando el transporte dirigido (rectificacion).} " >> $(DOCS_DIR)/reporte.tex
	@echo "\\end{figure}" >> $(DOCS_DIR)/reporte.tex
	
	@echo "\\begin{figure}[H]" >> $(DOCS_DIR)/reporte.tex
	@echo "\\centering" >> $(DOCS_DIR)/reporte.tex
	@echo "\\includegraphics[width=0.9\\textwidth]{../results/energia.png}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\caption{Evolucion de la energia total del sistema. Note los saltos cuando el potencial se enciende o apaga.} " >> $(DOCS_DIR)/reporte.tex
	@echo "\\end{figure}" >> $(DOCS_DIR)/reporte.tex

	@echo "\\begin{figure}[H]" >> $(DOCS_DIR)/reporte.tex
	@echo "\\centering" >> $(DOCS_DIR)/reporte.tex
	@echo "\\includegraphics[width=0.9\\textwidth]{../results/estados.png}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\caption{Ciclo quimico (ON/OFF) que impulsa el motor.} " >> $(DOCS_DIR)/reporte.tex
	@echo "\\end{figure}" >> $(DOCS_DIR)/reporte.tex

	@echo "\\begin{figure}[H]" >> $(DOCS_DIR)/reporte.tex
	@echo "\\centering" >> $(DOCS_DIR)/reporte.tex
	@echo "\\includegraphics[width=0.9\\textwidth]{../results/desplazamiento_neto.png}" >> $(DOCS_DIR)/reporte.tex
	@echo "\\caption{Desplazamiento neto de la particula a lo largo de toda la simulacion. La pendiente representa la velocidad de transporte.} " >> $(DOCS_DIR)/reporte.tex
	@echo "\\end{figure}" >> $(DOCS_DIR)/reporte.tex
	
	@echo "\\end{document}" >> $(DOCS_DIR)/reporte.tex
	@echo "Documentacion generada en $(DOCS_DIR)/reporte.tex"

clean:
	rm -rf $(BUILD_DIR)

clean_all:
	rm -rf $(BUILD_DIR) $(RESULTS_DIR) $(DOCS_DIR) Doxyfile
	rm -f html latex # Limpieza de carpetas default de doxygen si se crearon mal