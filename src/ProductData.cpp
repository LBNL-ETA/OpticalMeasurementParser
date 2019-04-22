#include "ProductData.hpp"

#include <sstream>

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
                                       std::string fileName,
                                       std::string unitSystem,
                                       std::string wavelengthUnit,
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
    fileName(fileName),
    unitSystem(unitSystem),
    wavelengthUnit(wavelengthUnit),
    measurements(measurements)
{}

void OpticsParser::to_json(nlohmann::json & j, OpticsParser::WLData const& wl)
{
    j = nlohmann::json{{"w", wl.wavelength}, {"tf", wl.T}, {"rf", wl.frontR}, {"rb", wl.backR}};
}

void OpticsParser::to_json(nlohmann::json & j, OpticsParser::ProductData const& p)
{
    nlohmann::json bulk_properties{{"thermal_conductivity", p.conductivity}};
    nlohmann::json angle_block{{"incidence_angle", 0}, {"number_wavelengths", p.measurements.size()}, {"solar_optical_spectral_data", p.measurements}};
    nlohmann::json spectral_data{{"unit", p.wavelengthUnit}, {"number_incidence_angles", 1}, {"angle_block", std::vector<nlohmann::json>{angle_block}}};

    nlohmann::json measured_data{{"thickness", p.thickness}, {"material_bulk_properties_overrides", bulk_properties}, {"tir_front", p.IRTransmittance}, {"emissivity_front", p.frontEmissivity}, {"emissivity_back", p.backEmissivity}, {"spectral_data", spectral_data}};

    nlohmann::json coating_properties{{"coating_name", p.coatingName}, {"coated_side", p.coatedSide}};
    
    j = nlohmann::json{{"name", p.productName}, {"filename", p.fileName}, {"token", p.fileName}, {"token_version", 1}, {"unit_system", p.unitSystem}, {"appearance", p.appearance}, {"acceptance", p.acceptance}, {"nfrc_id", p.nfrcid}, {"owner", p.manufacturer}, {"manufacturer", p.manufacturer}, {"type", "Glazing"}, {"specularity", "Specular"}, {"product_type", p.productType}, {"coating_properties", coating_properties}, {"measured_data", measured_data}};
}

