#pragma once

/// @file
/// @brief Class mbe::TileMapComponentSerialiser

#include <memory>
#include <string>

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::TileMapComponent
	/// @details XML format
	/// @code
	/// <Component type="TileMapComponent">
	///		<Position x="int" y="int" />
	///		<MovementSpeedShape>
	///			<Row>float, float, float, ...</Row>
	///			...
	///		</MovementSpeedShape>
	/// </Component>
	/// @endcode
	class TileMapComponentSerialiser : public ComponentSerialser
	{
	public:
		typedef std::shared_ptr<TileMapComponentSerialiser> Ptr;
		typedef std::weak_ptr<TileMapComponentSerialiser> WPtr;
		typedef std::unique_ptr<TileMapComponentSerialiser> UPtr;

	public:
		TileMapComponentSerialiser() = default;
		~TileMapComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;

	private:
		std::vector<float> ParseRow(const std::string& rowString) const;

	};


} // namespace mbe