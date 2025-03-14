#include "bsdfxml.hpp"

#include <bsdfdata/Enumerators.hpp>

namespace OpticsParser
{
    namespace Helper
    {
        BSDF convert(const BSDFData::WavelengthDataBlock & data)
        {
            BSDF result;

            result.rowAngleBasisName = data.rowAngleBasis.value_or("");
            result.columnAngleBasisName = data.columnAngleBasis.value_or("");
            result.data = data.scatteringData;

            return result;
        }

        enum class Range
        {
            Unknown,
            Solar,
            Visible
        };

        std::string toLower(std::string s)
        {
#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4244)
#endif
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
#ifdef _MSC_VER
#    pragma warning(pop)
#endif
            return s;
        }

        Range getRange(const BSDFData::Wavelength & data)
        {
            if(toLower(data.value) == "solar")
            {
                return Range::Solar;
            }
            else if(toLower(data.value) == "visible")
            {
                return Range::Visible;
            }

            throw std::runtime_error("XML error: Unsupported wavelength range");
        }

        std::map<Helper::Range, WavelengthBSDFs>
          convert(const std::vector<BSDFData::WavelengthData> & data)
        {
            std::map<Helper::Range, WavelengthBSDFs> result{
              {Helper::Range::Solar, WavelengthBSDFs{}},
              {Helper::Range::Visible, WavelengthBSDFs{}}};

            for(const auto & value : data)
            {
                if(value.wavelength.has_value())
                {
                    auto range = Helper::getRange(value.wavelength.value());
                    auto bsdf = Helper::convert(value.block);

                    std::map<BSDFData::WavelengthDataDirection, BSDF *> mapping{
                      {BSDFData::WavelengthDataDirection::TransmissionFront, &result.at(range).tf},
                      {BSDFData::WavelengthDataDirection::TransmissionBack, &result.at(range).tb},
                      {BSDFData::WavelengthDataDirection::ReflectionFront, &result.at(range).rf},
                      {BSDFData::WavelengthDataDirection::ReflectionBack, &result.at(range).rb}};

                    *mapping.at(value.block.wavelengthDataDirection.value()) = bsdf;
                }
            }

            return result;
        }

        inline std::shared_ptr<LouveredGeometry>
          createLouveredGeometryIfLouvered(const BSDFData::WindowElement & data,
                                           const ProductData & product)
        {
            // Check if it is a louvered shutter and if geometry is provided
            if(product.deviceType.has_value()
               && BSDFData::DeviceTypeFromString(product.deviceType.value())
                    == BSDFData::DeviceType::LouveredShutter
               && data.optical.layer.geometry.has_value())
            {
                // Build the geometry
                LouveredGeometry geometry{data.optical.layer.geometry->width.value_or(0),
                                          data.optical.layer.geometry->height.value_or(0),
                                          data.optical.layer.geometry->spacing.value_or(0),
                                          data.optical.layer.geometry->angle.value_or(0)};

                // Return it in a shared_ptr
                return std::make_shared<LouveredGeometry>(geometry);
            }

            // Otherwise, return null (no geometry)
            return nullptr;
        }

    }   // namespace Helper

    ProductData convert(const BSDFData::WindowElement & data)
    {
        ProductData product;

        if(data.optical.layer.material.has_value())
        {
            auto & matData = data.optical.layer.material.value();

            product.productName = matData.name;
            product.manufacturer = matData.manufacturer.value_or("");

            if(matData.thickness.has_value())
            {
                product.thicknessUnit = BSDFData::LengthUnitToString(matData.thickness->unit);
                product.thickness = matData.thickness->value;
            }
            product.frontEmissivity = matData.emissivityFront;
            product.backEmissivity = matData.emissivityBack;
            product.IRTransmittance = matData.TIR;
            product.conductivity = matData.thermalConductivity;

            product.permeabilityFactor = matData.permeabilityFactor;

            if(!product.permeabilityFactor.has_value())
            {
                product.permeabilityFactor = matData.effectiveOpennessFraction;
            }

            const auto bsdfs = Helper::convert(data.optical.layer.wavelengthData);

            if(bsdfs.count(Helper::Range::Solar) && bsdfs.count(Helper::Range::Visible))
            {
                product.measurements =
                  DualBandBSDF{bsdfs.at(Helper::Range::Solar), bsdfs.at(Helper::Range::Visible)};
            }

            product.deviceType =
              DeviceTypeToString(matData.deviceType.value_or(BSDFData::DeviceType::Unknown));

            product.geometry = Helper::createLouveredGeometryIfLouvered(data, product);
        }

        return product;
    }
}   // namespace OpticsParser