```mermaid
graph TD
    %% Package definitions
    xmlParser[xmlParser<br/><font color='red'>v1.0.5</font>]
    nlohmann_json_fileparse[nlohmann_json<br/><font color='red'>v3.12.0</font>]
    nlohmann_json[nlohmann_json<br/><font color='red'>v3.12.0</font>]
    FileParse[FileParse<br/><font color='red'>rework/bcs</font>]
    BSDFXMLParser[BSDFXMLParser<br/><font color='red'>rework/bcs</font>]
    OpticalMeasurementParser[OpticalMeasurementParser<br/><font color='red'>v2.3.11</font>]

    %% Package relations
    OpticalMeasurementParser --> nlohmann_json
    OpticalMeasurementParser --> BSDFXMLParser
    BSDFXMLParser --> FileParse
    FileParse --> xmlParser
    FileParse --> nlohmann_json_fileparse
```
