#include <MBE/Graphics/TopDownInformationComponent.h>

#include <MBE/Serialisation/TopDownInformationComponentSerialiser.h>

using namespace mbe;

void TopDownInformationComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData)
{
	using namespace tinyxml2;

	float logicalBottomOffset;

	// Load logical bottom offset
	const auto logicalBottomOffsetElement = componentData.FirstChildElement("LogicalBottomOffset");
	if (logicalBottomOffsetElement == nullptr)
		throw std::runtime_error("Load top-down information component: Failed to parse LogicalBottomOffset");
	if (logicalBottomOffsetElement->QueryFloatText(&logicalBottomOffset) != XML_SUCCESS)
		throw std::runtime_error("Load top-down information component: Failed to parse LogicalBottomOffset value");

	entity.AddComponent<TopDownInformationComponent>().SetLogicalBottomOffset(logicalBottomOffset);
}

void TopDownInformationComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData)
{
	// The entity must have a top-down information component (this should be the case when this function is called from the EntitySerialiser)
	if (entity.HasComponent<TopDownInformationComponent>() == false)
		throw std::runtime_error("Store top-down information component: The entity must have an mbe::TopDownInformationComponent");

	const auto& topDownInformationComponent = entity.GetComponent<TopDownInformationComponent>();

	// Store logical bottom offset
	auto logicalBottomOffsetElement = document.NewElement("LogicalBottomOffset");
	logicalBottomOffsetElement->SetText(topDownInformationComponent.GetLogicalBottomOffset());
	componentData.InsertEndChild(logicalBottomOffsetElement);
}
