// Rei Hernandez
//
#include <vector>
#include <iostream>
#include <sstream>
#include "CSVBusSystem.h"


struct SImplementation;
std::unique_ptr< SImplementation > DImplementation;

CCSVBusSystem(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr< CDSVReader > routesrc);

CCSVBusSystem::~CCSVBusSystem()= default;

std::size_t StopCount() const noexcept override;
std::size_t RouteCount() const noexcept override;
std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept override;
std::shared_ptr<SStop> StopByID(TStopID id) const noexcept override;
std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept override;
std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept override;