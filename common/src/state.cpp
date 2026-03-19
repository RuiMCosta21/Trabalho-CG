#include <iostream>

#include "state.h"
#include "tinyxml2.h"

using namespace tinyxml2;

/*
    int width; int height;
    float position_x; float position_y; float position_z;
    float lookAt_x; float lookAt_y; float lookAt_z;
    float up_x; float up_y; float up_z;
    float fov; float near; float far;
    std::vector<Model> models;
*/

// auxiliar para fazer parsing recursivo de nested groups
std::unique_ptr<Group> parseGroup(XMLElement* xml_group){
	auto group = std::make_unique<Group>();
	XMLElement* xml_transforms = xml_group->FirstChildElement("transform");
	if(xml_transforms){
		XMLElement* xml_transform = xml_transforms->FirstChildElement();
		std::cout << "Transforms FOUND\n";
		Matrix4 groupTransform;
		while(xml_transform != nullptr){
			std::string type = xml_transform->Name();
			Matrix4 next;
			if (type == "translate") {
				float x = xml_transform->FloatAttribute("x");
				float y = xml_transform->FloatAttribute("y");
				float z = xml_transform->FloatAttribute("z");
				next = Matrix4::translation(x, y, z);
			}
			else if (type == "rotate") {
				float angle = xml_transform->FloatAttribute("angle");
				float x = xml_transform->FloatAttribute("x");
				float y = xml_transform->FloatAttribute("y");
				float z = xml_transform->FloatAttribute("z");
				next = Matrix4::rotation(angle, x, y, z);
			}
			else if (type == "scale") {
					float x = xml_transform->FloatAttribute("x");
					float y = xml_transform->FloatAttribute("y");
					float z = xml_transform->FloatAttribute("z");
				next = Matrix4::scaling(x, y, z);
			}
			groupTransform = groupTransform.multiply(next);
			group->transformations = groupTransform;
			xml_transform = xml_transform->NextSiblingElement();
		}
	}
	XMLElement* xml_models = xml_group->FirstChildElement("models");
	if (xml_models) {
		XMLElement* xml_model = xml_models->FirstChildElement();
		std::cout << "FOUND\n";
		while (xml_model != nullptr) {
			const char* filename = xml_model->Attribute("file");
			if (filename) {
				Model model;
				if (model.readFromFile(filename) == 0) {
					std::cout << "READ\n";
					group->models.push_back(std::move(model));
				}
			}
			xml_model = xml_model->NextSiblingElement();
		}
	}
	XMLElement* xml_child = xml_group->FirstChildElement("group");
    while (xml_child) {
		std::cout << "New child group\n";
        group->children.push_back(parseGroup(xml_child));
        xml_child = xml_child->NextSiblingElement("group");
    }
    return group;
}

// parsing de um ficheiro config.xml
int State::loadConfig(const char *config_filename){
    XMLDocument doc;
    XMLError err = doc.LoadFile(config_filename);
    if (err != XML_SUCCESS) {
        std::cerr << "Failed to load config.xml\n";
		return 1;
	}
    
    XMLElement* root = doc.FirstChildElement("world");
    if (!root) {
        std::cerr << "Missing 'world' element\n";
        return 1;
    }
	XMLElement* window = root->FirstChildElement("window");
	XMLElement* camera = root->FirstChildElement("camera");
	XMLElement* rootGroup = root->FirstChildElement("group");
	if (!window || !camera || !rootGroup) {
        std::cerr << "Missing required elements\n";
        return 1;
    }
	window->QueryIntAttribute("height", &(this->height));
	window->QueryIntAttribute("width", &(this->width));
	XMLElement* position = camera->FirstChildElement("position");
	if (position) {
		position->QueryFloatAttribute("x", &(this->position_x));
		position->QueryFloatAttribute("y", &(this->position_y));
		position->QueryFloatAttribute("z", &(this->position_z));
	}
	XMLElement* lookAt = camera->FirstChildElement("lookAt");
	if (lookAt) {
		lookAt->QueryFloatAttribute("x", &(this->lookAt_x));
		lookAt->QueryFloatAttribute("y", &(this->lookAt_y));
		lookAt->QueryFloatAttribute("z", &(this->lookAt_z));
	}
	XMLElement* up = camera->FirstChildElement("up");
	if (up) {
		up->QueryFloatAttribute("x", &(this->up_x));
		up->QueryFloatAttribute("y", &(this->up_y));
		up->QueryFloatAttribute("z", &(this->up_z));
	}
	XMLElement* projection = camera->FirstChildElement("projection");
	if (projection) {
		projection->QueryFloatAttribute("fov", &(this->fov));
		projection->QueryFloatAttribute("near", &(this->near));
		projection->QueryFloatAttribute("far", &(this->far));
	}
	
	this->rootGroup = parseGroup(rootGroup);
	return 0;
}

void State::print() {
	std::cout << "{ 'State', \n{ 'Window', { 'Height', " << this->height << " }, { 'Width', " << this->width
		<< " } },\n{ 'Camera', { 'Position', { 'X', " << this->position_x << " }, { 'Y', " << this->position_y <<  " }, { 'Z', " << this->position_z
		<< " } }, { 'LookAt', { 'X', " << this->lookAt_x << " }, { 'Y', " << this->lookAt_y <<  " }, { 'Z', " << this->lookAt_z
		<< " } }, { 'Up', { 'X', " << this->up_x << " }, { 'Y', " << this->up_y <<  " }, { 'Z', " << this->up_z
		<< " } }, { 'Projection', { 'Fov', " << this->fov << " }, { 'Near', " << this->near <<  " }, { 'Z', " << this->far
		<< " } }\n}\n";
}