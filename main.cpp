#include <iostream>

#include "OpticsParser.hpp"

int main() {

	std::string inputFile = "D:\\temp\\CLEAR_3.DAT";
	OpticsParser::Parser par(inputFile);
	std::cout << "NFRC ID: " << par.nfrcid() << std::endl;
	std::cout << "Thickness: " << par.thickness() << std::endl;
	std::cout << "Conductivity: " << par.conductivity() << std::endl;
	std::cout << "IR Transmittance: " << par.IRTransmittance() << std::endl;
	std::cout << "Front Emissivity: " << par.frontEmissivity() << std::endl;
	std::cout << "Back Emissivity: " << par.backEmissivity() << std::endl;
	std::cout << par;
	return 0;
}