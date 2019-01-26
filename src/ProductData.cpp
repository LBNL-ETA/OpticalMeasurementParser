#include "ProductData.hpp"

OpticsParser::WLData::WLData(double wavelength, double T, double frontR, double backR) :
    wavelength(wavelength),
    T(T),
    frontR(frontR),
    backR(backR)
{}




OpticsParser::ProductData::ProductData(std::string const & productName,
                                       std::string const & productType,
                                       int nfrcid,
                                       double thickness,
                                       double conductivity,
                                       double IRTransmittance,
                                       double frontEmissivity,
                                       double backEmissivity,
                                       std::vector<WLData> const & measurements) :
    productName(productName),
    productType(productType),
    nfrcid(nfrcid),
    thickness(thickness),
    conductivity(conductivity),
    IRTransmittance(IRTransmittance),
    frontEmissivity(frontEmissivity),
    backEmissivity(backEmissivity),
    measurements(measurements)
{}