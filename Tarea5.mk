Results_hw5.pdf: parametros.dat
	python Plots.py

	pdflatex Results_hw5.tex


parametros.dat: RadialVelocities.dat
	gcc CurvaRotacion.c -lm
	./ejec.x > datos_mcmc

	tail -1 datos_mcmc > parametros.dat
	cat parametros.dat
