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
                                       std::string frontEmissivitySource,
                                       std::string backEmissivitySource,
                                       std::string manufacturer,
                                       std::string material,
                                       std::string coatingName,
                                       std::string coatedSide,
                                       std::string substrateFilename,
                                       std::string appearance,
                                       std::string acceptance,
                                       std::vector<WLData> const & measurements) :
    productName(productName),
    productType(productType),
    nfrcid(nfrcid),
    thickness(thickness),
    conductivity(conductivity),
    IRTransmittance(IRTransmittance),
    frontEmissivity(frontEmissivity),
    backEmissivity(backEmissivity),
    frontEmissivitySource(frontEmissivitySource),
    backEmissivitySource(backEmissivitySource),
    manufacturer(manufacturer),
    material(material),
    coatingName(coatingName),
    coatedSide(coatedSide),
    substrateFilename(substrateFilename),
    appearance(appearance),
    acceptance(acceptance),
    measurements(measurements)
{}
