#include "cloudgen.h"
#include <string>

using namespace godot;

void CloudGen::_register_methods() {
    register_method("_process", &CloudGen::_process);
}

CloudGen::CloudGen() {
}

CloudGen::~CloudGen() {
}

void CloudGen::_init() {

    // resolution cannot be more then MAX_ELEMENTS
    if (resolution > MAX_ELEMENTS)
        resolution = MAX_ELEMENTS;


    // load cloud element scene
    cloud_element_mesh = loader->load("res://CloudElement/CloudElement.tscn");

    // setup simplex noise
    noise->set_seed(45343);
    noise->set_octaves(1);
    noise->set_period(15.0);
    noise->set_persistence(0.8);

    // setup position helpers
    origin = get_transform().get_origin();
    step = (float) area_size / (float) resolution;
    x_origin = origin.x - area_size / 2;
    y_origin = origin.y - area_size / 2;
    
    
    // setup cloud instances
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            create_instance(i, j);
        }
    }

    time_passed = 0.0;
    update_cloud_elements();
}


void CloudGen::_process(float delta) {
    time_passed += delta * speed;

    current_frame++;
    if (current_frame == update_frame) {
        update_cloud_elements();        
        current_frame = 0;
    }
}


void CloudGen::create_instance(int i, int j) {
    float x = x_origin + i * step + rng->randf_range(-variation, variation);
    float y = y_origin + j * step + rng->randf_range(-variation, variation);

    Node* element = cloud_element_mesh->instance();
    MeshInstance* el = Object::cast_to<MeshInstance>(element);
    add_child(el);

    el->set_transform(Transform(Basis(), Vector3(x, 0.0, y)));
    el->set_visible(false);
    cloud_elements[i][j].instance = el;
}


void CloudGen::update_cloud_elements() {
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {

            float noise_value = (noise->get_noise_2d(i + time_passed, j + time_passed) + 1.0) / 2.0;
            
            // show/hide/rescale cloud elements based on the noise value
            if (noise_value > threshold) {
                float scale = (noise_value - threshold) * element_size;
                if (cloud_elements[i][j].visible == false) {
                    cloud_elements[i][j].instance->set_visible(true);
                    cloud_elements[i][j].visible = true;
                }
                cloud_elements[i][j].instance->set_scale(Vector3(scale, scale, scale));
            }
            else {
                cloud_elements[i][j].instance->set_visible(false);
                cloud_elements[i][j].visible = false;
            }

        }
    }
}