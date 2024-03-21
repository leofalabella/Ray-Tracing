#include <SFML/Graphics.hpp>
#include "general.h"
#include "pixels.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "bvh.h"
#include "quad.h"
#include "constant_medium.h"
#include <vector>

void quads(hittable_list& world) {

    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));
}

void random_spheres(hittable_list& world) {

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));
}

void simple_light(hittable_list& world) {

    world.add(make_shared<sphere>(point3(0,-1000, 0), 1000, make_shared<lambertian>(color(0.5, 0.5, 0.5))));
    world.add(make_shared<sphere>(point3(0,2, 0), 2, make_shared<lambertian>(color(0.5, 0.5, 0.5))));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));
}

void teste(hittable_list& world) {

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
}

void metalBox_spheres(hittable_list& world) {

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    auto box_material = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<quad>(point3(-1,-1,-1), vec3(2, 0, 0), vec3(0, 2, 0), box_material));
    world.add(make_shared<quad>(point3(-1,-1,-1), vec3(0, 0, 2), vec3(0, 2, 0), box_material));
    world.add(make_shared<quad>(point3( 1,-1,-1), vec3(0, 0, 2), vec3(0, 2, 0), box_material));
    world.add(make_shared<quad>(point3(-1,-1, 1), vec3(2, 0, 0), vec3(0, 2, 0), box_material));

    for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            auto choose_mat = random_double();
            point3 center(i + 0.9*random_double(), 0.2, j + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
}

void final_scene(hittable_list& world) {

    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    world.add(make_shared<bvh_node>(boxes1));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    world.add(make_shared<rotate_y>(make_shared<bvh_node>(boxes2), 15));
}

void balls(hittable_list& world) {
    
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));

    world.add(make_shared<sphere>(point3(0,-1000.5, -1), 1000, make_shared<lambertian>(color(0.5, 0.5, 0.5))));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.3, 0.7, 0.5))));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<dielectric>(1.3)));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<metal>(color(0.8, 0.6, 0.2), 0.7)));

    shared_ptr<hittable> box1 = box(point3(0,0,-2), point3(3,2,-5), white);
    box1 = make_shared<rotate_y>(box1, 30);
    shared_ptr<hittable> box2 = box(point3(0,0,-2), point3(-2,2,-5), white);
    box2 = make_shared<rotate_y>(box2, -15);

    world.add(make_shared<constant_medium>(box1, 0.5, color(1,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.5, color(0,0,1)));
    
    auto difflight = make_shared<diffuse_light>(color(7,7,7));
    world.add(make_shared<quad>(point3(0,10,0), vec3(20,0,0), vec3(0,0,20), difflight));
}

void cornell_smoke(hittable_list& world) {
    
    hittable_list boxes1;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto blue   = make_shared<lambertian>(color(.2, .1, .7));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto mirror = make_shared<metal>(color(1, 1, 1), 0.5);

    int boxes_per_side = 5;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 55*2;
            auto x0 = 55 + i*w;
            auto z0 = 55 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(0,1);
            auto z1 = z0 + w;

            boxes1.add(make_shared<sphere>(point3(x0, y1, z0),90 , mirror));
        }
    }
    world.add(make_shared<bvh_node>(boxes1));
    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(100,150,100), point3(165,250,150), blue);
    box1 = make_shared<rotate_y>(box1, 30);

    shared_ptr<hittable> box2 = box(point3(450,50,300), point3(520,165,400), blue);
    box2 = make_shared<rotate_y>(box2, -18);

    world.add(box1);
    world.add(box2);
}

int main()
{
    hittable_list world;

    // Create an enum to represent the currently selected input box
    enum class SelectedInput
    {
        None,
        Width,
        Height,
        SamplesPerPixel,
        MaxDepth,
        LookFromX,
        LookFromY,
        LookFromZ,
        LookAtX,
        LookAtY,
        LookAtZ,
        Fov,
        WorldSelection
    };

    // Initialize the currently selected input box to None
    SelectedInput selectedInput = SelectedInput::None;

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int windowPosX = (desktopMode.width - 800) / 2;
    unsigned int windowPosY = (desktopMode.height - 600) / 2;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ray Tracing Configuration Tool");

    window.setPosition(sf::Vector2i(windowPosX, windowPosY));

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf"))
    {
        return EXIT_FAILURE;
    }

    // Create a rectangle shape to represent the input width
    sf::RectangleShape inputBox_Width(sf::Vector2f(200, 50));
    inputBox_Width.setFillColor(sf::Color::White);
    inputBox_Width.setPosition(100, 100);

    // Create a text object to represent the input width
    sf::Text inputText_Width;
    inputText_Width.setFont(font);
    inputText_Width.setCharacterSize(24);
    inputText_Width.setFillColor(sf::Color::Black);
    inputText_Width.setPosition(105, 105);

    sf::Text Label_Width;
    Label_Width.setFont(font);
    Label_Width.setString("Width");
    Label_Width.setCharacterSize(24);
    Label_Width.setFillColor(sf::Color::Red);
    Label_Width.setPosition(100, 70);

    // Create a rectangle shape to represent the input height
    sf::RectangleShape inputBox_Height(sf::Vector2f(200, 50));
    inputBox_Height.setFillColor(sf::Color::White);
    inputBox_Height.setPosition(100, 200);

    // Create a text object to represent the input height
    sf::Text inputText_Height;
    inputText_Height.setFont(font);
    inputText_Height.setCharacterSize(24);
    inputText_Height.setFillColor(sf::Color::Black);
    inputText_Height.setPosition(105, 205);

    sf::Text Label_Height;
    Label_Height.setFont(font);
    Label_Height.setString("Height");
    Label_Height.setCharacterSize(24);
    Label_Height.setFillColor(sf::Color::Red);
    Label_Height.setPosition(100, 170);

    // Create a rectangle shape to represent the input samples per pixel
    sf::RectangleShape inputBox_SamplesPerPixel(sf::Vector2f(200, 50));
    inputBox_SamplesPerPixel.setFillColor(sf::Color::White);
    inputBox_SamplesPerPixel.setPosition(100, 300);

    // Create a text object to represent the input samples per pixel
    sf::Text inputText_SamplesPerPixel;
    inputText_SamplesPerPixel.setFont(font);
    inputText_SamplesPerPixel.setCharacterSize(24);
    inputText_SamplesPerPixel.setFillColor(sf::Color::Black);
    inputText_SamplesPerPixel.setPosition(105, 305);

    sf::Text Label_SamplesPerPixel;
    Label_SamplesPerPixel.setFont(font);
    Label_SamplesPerPixel.setString("Samples Per Pixel");
    Label_SamplesPerPixel.setCharacterSize(24);
    Label_SamplesPerPixel.setFillColor(sf::Color::Red);
    Label_SamplesPerPixel.setPosition(100, 270);

    // Create a rectangle shape to represent the input max depth
    sf::RectangleShape inputBox_MaxDepth(sf::Vector2f(200, 50));
    inputBox_MaxDepth.setFillColor(sf::Color::White);
    inputBox_MaxDepth.setPosition(100, 400);

    // Create a text object to represent the input max depth
    sf::Text inputText_MaxDepth;
    inputText_MaxDepth.setFont(font);
    inputText_MaxDepth.setCharacterSize(24);
    inputText_MaxDepth.setFillColor(sf::Color::Black);
    inputText_MaxDepth.setPosition(105, 405);

    sf::Text Label_MaxDepth;
    Label_MaxDepth.setFont(font);
    Label_MaxDepth.setString("Max Depth");
    Label_MaxDepth.setCharacterSize(24);
    Label_MaxDepth.setFillColor(sf::Color::Red);
    Label_MaxDepth.setPosition(100, 370);

    // Create a rectangle shape to represent the input look from x
    sf::RectangleShape inputBox_LookFromX(sf::Vector2f(50, 50));
    inputBox_LookFromX.setFillColor(sf::Color::White);
    inputBox_LookFromX.setPosition(500, 100);

    // Create a text object to represent the input look from x
    sf::Text inputText_LookFromX;
    inputText_LookFromX.setFont(font);
    inputText_LookFromX.setCharacterSize(24);
    inputText_LookFromX.setFillColor(sf::Color::Black);
    inputText_LookFromX.setPosition(505, 105);

    // Create a rectangle shape to represent the input look from y
    sf::RectangleShape inputBox_LookFromY(sf::Vector2f(50, 50));
    inputBox_LookFromY.setFillColor(sf::Color::White);
    inputBox_LookFromY.setPosition(560, 100);

    // Create a text object to represent the input look from y
    sf::Text inputText_LookFromY;
    inputText_LookFromY.setFont(font);
    inputText_LookFromY.setCharacterSize(24);
    inputText_LookFromY.setFillColor(sf::Color::Black);
    inputText_LookFromY.setPosition(565, 105);

    // Create a rectangle shape to represent the input look from z
    sf::RectangleShape inputBox_LookFromZ(sf::Vector2f(50, 50));
    inputBox_LookFromZ.setFillColor(sf::Color::White);
    inputBox_LookFromZ.setPosition(620, 100);

    // Create a text object to represent the input look from z
    sf::Text inputText_LookFromZ;
    inputText_LookFromZ.setFont(font);
    inputText_LookFromZ.setCharacterSize(24);
    inputText_LookFromZ.setFillColor(sf::Color::Black);
    inputText_LookFromZ.setPosition(625, 105);

    sf::Text Label_LookFrom;
    Label_LookFrom.setFont(font);
    Label_LookFrom.setString("Look From");
    Label_LookFrom.setCharacterSize(24);
    Label_LookFrom.setFillColor(sf::Color::Red);
    Label_LookFrom.setPosition(525, 70);

    // Create a rectangle shape to represent the input look at x
    sf::RectangleShape inputBox_LookAtX(sf::Vector2f(50, 50));
    inputBox_LookAtX.setFillColor(sf::Color::White);
    inputBox_LookAtX.setPosition(500, 200);

    // Create a text object to represent the input look at x
    sf::Text inputText_LookAtX;
    inputText_LookAtX.setFont(font);
    inputText_LookAtX.setCharacterSize(24);
    inputText_LookAtX.setFillColor(sf::Color::Black);
    inputText_LookAtX.setPosition(505, 205);

    // Create a rectangle shape to represent the input look at y
    sf::RectangleShape inputBox_LookAtY(sf::Vector2f(50, 50));
    inputBox_LookAtY.setFillColor(sf::Color::White);
    inputBox_LookAtY.setPosition(560, 200);

    // Create a text object to represent the input look at y
    sf::Text inputText_LookAtY;
    inputText_LookAtY.setFont(font);
    inputText_LookAtY.setCharacterSize(24);
    inputText_LookAtY.setFillColor(sf::Color::Black);
    inputText_LookAtY.setPosition(565, 205);

    // Create a rectangle shape to represent the input look at z
    sf::RectangleShape inputBox_LookAtZ(sf::Vector2f(50, 50));
    inputBox_LookAtZ.setFillColor(sf::Color::White);
    inputBox_LookAtZ.setPosition(620, 200);

    // Create a text object to represent the input look at z
    sf::Text inputText_LookAtZ;
    inputText_LookAtZ.setFont(font);
    inputText_LookAtZ.setCharacterSize(24);
    inputText_LookAtZ.setFillColor(sf::Color::Black);
    inputText_LookAtZ.setPosition(625, 205);

    sf::Text Label_LookAt;
    Label_LookAt.setFont(font);
    Label_LookAt.setString("Look At");
    Label_LookAt.setCharacterSize(24);
    Label_LookAt.setFillColor(sf::Color::Red);
    Label_LookAt.setPosition(525, 170);

    // Create a rectangle shape to represent the input fov
    sf::RectangleShape inputBox_Fov(sf::Vector2f(200, 50));
    inputBox_Fov.setFillColor(sf::Color::White);
    inputBox_Fov.setPosition(500, 300);

    // Create a text object to represent the input fov
    sf::Text inputText_Fov;
    inputText_Fov.setFont(font);
    inputText_Fov.setCharacterSize(24);
    inputText_Fov.setFillColor(sf::Color::Black);
    inputText_Fov.setPosition(505, 305);

    sf::Text Label_Fov;
    Label_Fov.setFont(font);
    Label_Fov.setString("FoV");
    Label_Fov.setCharacterSize(24);
    Label_Fov.setFillColor(sf::Color::Red);
    Label_Fov.setPosition(500, 270);

    // Create a rectangle shape to represent the input world selection
    sf::RectangleShape inputBox_WorldSelection(sf::Vector2f(200, 50));
    inputBox_WorldSelection.setFillColor(sf::Color::White);
    inputBox_WorldSelection.setPosition(500, 400);

    sf::Text inputText_WorldSelection;
    inputText_WorldSelection.setFont(font);
    inputText_WorldSelection.setCharacterSize(24);
    inputText_WorldSelection.setFillColor(sf::Color::Black);
    inputText_WorldSelection.setPosition(505, 405);

    sf::Text Label_WorldSelection;
    Label_WorldSelection.setFont(font);
    Label_WorldSelection.setString("World Selection");
    Label_WorldSelection.setCharacterSize(24);
    Label_WorldSelection.setFillColor(sf::Color::Red);
    Label_WorldSelection.setPosition(500, 370);

    // Create a rectangle shape to represent the button
    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setFillColor(sf::Color::Green);
    button.setPosition(340, 500);

    // Create a text object to represent the button label
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("Render");
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(345, 505);

    std::string width_inserted;
    std::string height_inserted;
    std::string samplesPerPixel;
    std::string maxDepth;
    std::string lookFromX;
    std::string lookFromY;
    std::string lookFromZ;
    std::string lookAtX;
    std::string lookAtY;
    std::string lookAtZ;
    std::string fov;
    std::string world_selection;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle mouse click events
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Check if the mouse click is within the bounds of the input boxes
                    if (inputBox_Width.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::Width;

                    else if (inputBox_Height.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::Height;

                    else if (inputBox_SamplesPerPixel.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::SamplesPerPixel;

                    else if (inputBox_MaxDepth.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::MaxDepth;

                    else if (inputBox_LookFromX.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookFromX;

                    else if (inputBox_LookFromY.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookFromY;

                    else if (inputBox_LookFromZ.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookFromZ;

                    else if (inputBox_LookAtX.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookAtX;

                    else if (inputBox_LookAtY.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookAtY;

                    else if (inputBox_LookAtZ.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookAtZ;

                    else if (inputBox_Fov.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::Fov;

                    else if (inputBox_WorldSelection.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::WorldSelection;

                    else if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        window.close();

                    else
                        selectedInput = SelectedInput::None;
                }
            }

            // Handle text input events
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    // Update the text for the currently selected input box
                    switch (selectedInput)
                    {
                    case SelectedInput::Width:
                        width_inserted += static_cast<char>(event.text.unicode);
                        inputText_Width.setString(width_inserted);
                        break;
                    case SelectedInput::Height:
                        height_inserted += static_cast<char>(event.text.unicode);
                        inputText_Height.setString(height_inserted);
                        break;
                    case SelectedInput::SamplesPerPixel:
                        samplesPerPixel += static_cast<char>(event.text.unicode);
                        inputText_SamplesPerPixel.setString(samplesPerPixel);
                        break;
                    case SelectedInput::MaxDepth:
                        maxDepth += static_cast<char>(event.text.unicode);
                        inputText_MaxDepth.setString(maxDepth);
                        break;
                    case SelectedInput::LookFromX:
                        lookFromX += static_cast<char>(event.text.unicode);
                        inputText_LookFromX.setString(lookFromX);
                        break;
                    case SelectedInput::LookFromY:
                        lookFromY += static_cast<char>(event.text.unicode);
                        inputText_LookFromY.setString(lookFromY);
                        break;
                    case SelectedInput::LookFromZ:
                        lookFromZ += static_cast<char>(event.text.unicode);
                        inputText_LookFromZ.setString(lookFromZ);
                        break;
                    case SelectedInput::LookAtX:
                        lookAtX += static_cast<char>(event.text.unicode);
                        inputText_LookAtX.setString(lookAtX);
                        break;
                    case SelectedInput::LookAtY:
                        lookAtY += static_cast<char>(event.text.unicode);
                        inputText_LookAtY.setString(lookAtY);
                        break;
                    case SelectedInput::LookAtZ:
                        lookAtZ += static_cast<char>(event.text.unicode);
                        inputText_LookAtZ.setString(lookAtZ);
                        break;
                    case SelectedInput::Fov:
                        fov += static_cast<char>(event.text.unicode);
                        inputText_Fov.setString(fov);
                        break;
                    case SelectedInput::WorldSelection:
                        world_selection += static_cast<char>(event.text.unicode);
                        inputText_WorldSelection.setString(world_selection);
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        window.clear();

        window.draw(inputBox_Width);
        window.draw(inputText_Width);
        window.draw(Label_Width);

        window.draw(inputBox_Height);
        window.draw(inputText_Height);
        window.draw(Label_Height);

        window.draw(inputBox_SamplesPerPixel);
        window.draw(inputText_SamplesPerPixel);
        window.draw(Label_SamplesPerPixel);

        window.draw(inputBox_MaxDepth);
        window.draw(inputText_MaxDepth);
        window.draw(Label_MaxDepth);

        window.draw(button);
        window.draw(buttonText);

        window.draw(inputBox_LookFromX);
        window.draw(inputText_LookFromX);
        window.draw(inputBox_LookFromY);
        window.draw(inputText_LookFromY);
        window.draw(inputBox_LookFromZ);
        window.draw(inputText_LookFromZ);
        window.draw(Label_LookFrom);

        window.draw(inputBox_LookAtX);
        window.draw(inputText_LookAtX);
        window.draw(inputBox_LookAtY);
        window.draw(inputText_LookAtY);
        window.draw(inputBox_LookAtZ);
        window.draw(inputText_LookAtZ);
        window.draw(Label_LookAt);

        window.draw(inputBox_Fov);
        window.draw(inputText_Fov);
        window.draw(Label_Fov);

        window.draw(inputBox_WorldSelection);
        window.draw(inputText_WorldSelection);
        window.draw(Label_WorldSelection);

        window.display();
    }

    camera cam;

    unsigned width = std::stoul(width_inserted);
    unsigned height = std::stoul(height_inserted);
    int samples_Pixel = std::stoi(samplesPerPixel);

    cam.image_width = width;
    cam.image_height = height;
    cam.samples_per_pixel = samples_Pixel;
    cam.max_depth = std::stoi(maxDepth);
    cam.background = color(0, 0, 0);
    //cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = std::stof(fov);

    cam.lookfrom = point3(std::stof(lookFromX), std::stof(lookFromY), std::stof(lookFromZ));
    cam.lookat = point3(std::stof(lookAtX), std::stof(lookAtY), std::stof(lookAtZ));
    cam.vup = vec3(0, 1, 0);

    int world_selected = std::stoi(world_selection);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    switch (world_selected){
        case 1: random_spheres(world); break;
        case 2: quads(world); break;
        case 3: simple_light(world); break;
        case 4: metalBox_spheres(world); break;
        case 5: final_scene(world); break;
        case 6: teste(world); break;
        case 7: balls(world); break;
        case 8: cornell_smoke(world); break;
    }

    Pixels pixels{width, height};

    unsigned int window2PosX = (desktopMode.width - width) / 2;
    unsigned int window2PosY = (desktopMode.height - height) / 2;

    sf::RenderWindow window2(sf::VideoMode(width, height), "Ray Tracing",
                            sf::Style::Titlebar | sf::Style::Close);
    window2.setPosition(sf::Vector2i(window2PosX, window2PosY));
    sf::Texture tex;
    sf::Sprite sprite;

    if (!tex.create(width, height))
    {
        std::cerr << "Couldn't create texture!" << std::endl;
        return 1;
    }

    tex.setSmooth(false);

    sprite.setTexture(tex);

    cam.render(world, pixels, window2, sprite, tex);

    while (window2.isOpen()) {
        sf::Event event;
        while (window2.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window2.close();
        }
    }

    tex.copyToImage().saveToFile("out.png");

    return 0;
}