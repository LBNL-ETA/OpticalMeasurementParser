#pragma once

#include <string>
#include <vector>
#include <ostream>

namespace OpticsParser
{
    struct WLData
    {
    public:
        WLData(double wavelength, double T, double frontR, double backR);
        friend std::ostream & operator<<(std::ostream & os, const WLData & data);

        double wavelength;
        double T;
        double frontR;
        double backR;
    };
    class Parser
    {
    public:
        explicit Parser(const std::string & inputFile);
        friend std::ostream & operator<<(std::ostream & os, const Parser & parser);

        double thickness() const;
		double conductivity() const;
		double IRTransmittance() const;
        double frontEmissivity() const;
        double backEmissivity() const;
		int nfrcid() const;
		std::vector<WLData> measurements() const;
	private:
        void parseHeaderLine(const std::string & line);
        void parseMeasurementLine(const std::string & line);
        void parsePropertyAtTheEnd( const std::string & searchString,
									const std::string & behind,
									const std::string & line, double & property );
        void parseEmissivities(const std::string & line);
        void parseNFRCID(const std::string & line);
        double m_Thickness;
        double m_Conductivity;
        double m_IRTransmittance;
        double m_FrontEmissivity;
        double m_BackEmissivity;
        int m_NFRCID;
        std::vector<WLData> m_WLData;
    };

}   // namespace OpticsParser
