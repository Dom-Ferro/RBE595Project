to change which flock type, change project.bzz:81 to "type_flock = HEX" or "type_flock = GRID" or "type_flock = CELL"

to compile the loop functions, in the project directory:
	mkdir build
	cd build/
	cmake ..
	make

then run argos from the top of the project directory
	argos -c project.argos
