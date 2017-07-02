APP = runner
PREFIX = mentions

all:
	@make build
	@make pdf
	evince $(PREFIX).pdf &
	@make clean

build:
	@echo Reading file \'$(PREFIX).cfg\'...
	./$(APP) $(PREFIX).cfg

pdf:
	pdflatex --jobname=$(PREFIX) .latex/main.tex
	pdflatex --jobname=$(PREFIX) .latex/main.tex

clean:
	rm *.aux *.log
