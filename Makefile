all:

	g++ -w Globals.cpp cubieboard_serial_vision.cpp SerialUtilities.cpp DepthHeightCalculation.cpp -o test `pkg-config --cflags --libs opencv`

