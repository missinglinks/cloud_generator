#ifndef CLOUDGEN_H
#define CLOUDGEN_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <OpenSimplexNoise.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <Ref.hpp>
#include <MeshInstance.hpp>
#include <RandomNumberGenerator.hpp>

namespace godot {

class CloudGen : public Spatial {
    GODOT_CLASS(CloudGen, Spatial)

private:
    static const int MAX_ELEMENTS = 100;

    ResourceLoader* loader = ResourceLoader::get_singleton();
    Ref<PackedScene> cloud_element_mesh;
    OpenSimplexNoise* noise = OpenSimplexNoise::_new();
    RandomNumberGenerator* rng = RandomNumberGenerator::_new();

    // cloud data container
    struct CloudElement {
        MeshInstance* instance = nullptr;
        bool visible = false;
    };
    CloudElement cloud_elements[MAX_ELEMENTS][MAX_ELEMENTS];

    // cloud generation setup variables -> TODO: export to godot inspector
    int resolution = 80; // max: MAX_ELEMENTS (100)
    int area_size = 100; // square; no. of units of the sides
    float threshold = 0.5; // lower: bigger clouds; higher: smaller clouds
    float element_size = 20;
    float variation = 1;
    float speed = 0.3;
    int update_frame = 3;

    // helper variables
    float step;
    float x_origin, y_origin;
    Vector3 origin;
    float time_passed;
    int current_frame = 0;

private:
    void create_instance(int i, int j);
    void update_cloud_elements();

public:
    static void _register_methods();

    CloudGen();
    ~CloudGen();

    void _init(); 
    void _process(float delta);
};

}

#endif

