#include "ProductData.hpp"

#include <sstream>

OpticsParser::WLData::WLData(double wavelength,
                             MeasurementComponent directComponent,
                             std::optional<MeasurementComponent> diffuseComponent) :
    wavelength(wavelength), directComponent(directComponent), diffuseComponent(diffuseComponent)
{}

OpticsParser::WLData::WLData(double wavelength, double tDirect, double rfDirect, double rbDiffuse) :
    wavelength(wavelength),
    directComponent(MeasurementComponent{tDirect, tDirect, rfDirect, rbDiffuse}),
    diffuseComponent(std::optional<MeasurementComponent>())
{}

OpticsParser::WLData::WLData(double wavelength,
                             double tfDirect,
                             double tfDiffuse,
                             double tbDirect,
                             double tbDiffuse,
                             double rfDirect,
                             double rfDiffuse,
                             double rbDirect,
                             double rbDiffuse) :
    wavelength(wavelength),
    directComponent(MeasurementComponent{tfDirect, tbDirect, rfDirect, rbDirect}),
    diffuseComponent(MeasurementComponent{tfDiffuse, tbDiffuse, rfDiffuse, rbDiffuse})
{}


OpticsParser::ProductData::ProductData(std::string const & productName,
                                       std::string const & productType,
                                       std::string const & manufacturer) :
    productName(productName), productType(productType), manufacturer(manufacturer)
{}

OpticsParser::ProductData::ProductData(std::string const & productName,
                                       std::string const & productType,
                                       std::string const & subtype,
                                       std::string const & manufacturer) :
    productName(productName), productType(productType), subtype(subtype), manufacturer(manufacturer)
{}

OpticsParser::ProductData::ProductData(std::string const & productName,
                                       std::string const & productType,
                                       std::string const & subtype,
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
    subtype(subtype),
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

std::shared_ptr<OpticsParser::ProductData> OpticsParser::ProductData::composedProduct()
{
    return shared_from_this();
}

std::shared_ptr<OpticsParser::ProductData> OpticsParser::ComposedProductData::composedProduct()
{
    return compositionInformation->material;
}

void OpticsParser::to_json(nlohmann::json & j, OpticsParser::WLData const & wl)
{
    j = nlohmann::json{{"w", wl.wavelength},
                       {"tf", wl.directComponent.tf},
                       {"rf", wl.directComponent.rf},
                       {"rb", wl.directComponent.rb}};
}

std::string convert_product_type(std::string const & optics_product_type)
{
    std::map<std::string, std::string> optics_types_to_new_types;
    optics_types_to_new_types["Monolithic"] = "monolithic";
    optics_types_to_new_types["Coated"] = "coated-glass";
    optics_types_to_new_types["Laminate"] = "laminate";

    std::map<std::string, std::string>::iterator itr =
      optics_types_to_new_types.find(optics_product_type);

    if(itr != optics_types_to_new_types.end())
    {
        return itr->second;
    }
    else
    {
        std::stringstream err_msg;
        err_msg << "Unknown type in optics file: " << optics_product_type;
        throw std::runtime_error(err_msg.str());
    }
}

template<typename T>
void add_optional(nlohmann::json & j,
                  std::string const & field_name,
                  std::optional<T> const & value)
{
    if(value)
    {
        j[field_name] = value.value();
    }
}

void OpticsParser::to_json(nlohmann::json & j, OpticsParser::ProductData const & p)
{
    nlohmann::json spectral_data;

    if(p.measurements)
    {
        nlohmann::json angle_block{{"incidence_angle", 0},
                                   {"number_wavelengths", p.measurements.value().size()},
                                   {"wavelength_data", p.measurements.value()}};
        spectral_data["number_incidence_angles"] = 1;
        spectral_data["angle_block"] = std::vector<nlohmann::json>{angle_block};

        add_optional(spectral_data, "unit", p.wavelengthUnit);
    }
    nlohmann::json measured_data;
    add_optional(measured_data, "thickness", p.thickness);
    add_optional(measured_data, "tir_front", p.IRTransmittance);
    add_optional(measured_data, "tir_back", p.IRTransmittance);
    add_optional(measured_data, "emissivity_front", p.frontEmissivity);
    add_optional(measured_data, "emissivity_back", p.backEmissivity);
    measured_data["spectral_data"] = spectral_data;
    add_optional(measured_data, "is_specular", p.specularity);


    if(p.conductivity)
    {
        nlohmann::json bulk_properties{{"thermal_conductivity", p.conductivity.value()}};
        measured_data["bulk_properties_override"] = bulk_properties;
    }

    nlohmann::json coating_properties{{"coating_name", p.coatingName.value_or("None")},
                                      {"coated_side", p.coatedSide.value_or("None")}};

    std::string product_type = convert_product_type(p.productType);

    j = nlohmann::json{{"product_name", p.productName},
                       {"version", 1},
                       {"owner", p.manufacturer},
                       {"manufacturer", p.manufacturer},
                       {"type", "Glazing"},
                       {"specularity", "Specular"},
                       {"product_type", product_type},
                       {"coating_properties", coating_properties},
                       {"measured_data", measured_data},
                       {"active", true}};


    add_optional(j, "nfrc_id", p.nfrcid);
    add_optional(j, "filename", p.fileName);
    add_optional(j, "token", p.fileName);
    add_optional(j, "unit_system", p.unitSystem);
    add_optional(j, "appearance", p.appearance);
    add_optional(j, "acceptance", p.acceptance);
}

OpticsParser::ComposedProductData::ComposedProductData(
  ProductData const & product, std::shared_ptr<CompositionInformation> composition) :
    ProductData(product), compositionInformation(composition)
{}

OpticsParser::VenetianGeometry::VenetianGeometry(
  double slatWidth, double slatSpacing, double slatCurvature, double slatTilt, int numberSegments) :
    slatWidth(slatWidth),
    slatSpacing(slatSpacing),
    slatCurvature(slatCurvature),
    slatTilt(slatTilt),
    numberSegments(numberSegments)
{}

OpticsParser::WovenGeometry::WovenGeometry(double threadDiameter,
                                           double threadSpacing,
                                           double shadeThickness) :
    threadDiameter(threadDiameter), threadSpacing(threadSpacing), shadeThickness(shadeThickness)
{}

OpticsParser::PerforatedGeometry::PerforatedGeometry(double spacingX,
                                                     double spacingY,
                                                     double dimensionX,
                                                     double dimensionY,
                                                     std::string perforationType) :
    spacingX(spacingX),
    spacingY(spacingY),
    dimensionX(dimensionX),
    dimensionY(dimensionY),
    perforationType(perforationType)
{}
