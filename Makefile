# makefile for cpphtmltags

# builds the sample programs

doc:
	doxygen Doxyfile
	xdg-open html/index.html
