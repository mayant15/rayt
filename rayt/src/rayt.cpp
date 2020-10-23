#include "rayt.h"

Colorf CalculateRayColor(const Ray& r, const std::vector<SRef<RenderObject>>& world, unsigned int bounce);

constexpr unsigned int NR_SAMPLES = 50;
constexpr unsigned int NR_BOUNCES = 5;

constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr unsigned int IMAGE_HEIGHT = 224;
constexpr unsigned int IMAGE_WIDTH = IMAGE_HEIGHT * ASPECT_RATIO;

int main()
{
    std::vector<Colorf> data(IMAGE_HEIGHT * IMAGE_WIDTH);
    std::vector<SRef<RenderObject>> world;

    auto metal = std::make_shared<Material>();
    auto rough = std::make_shared<Material>();
    auto glass = std::make_shared<Material>();

    metal->Albedo = Colorf {0.8, 0.6, 0.2, 1.0f};
    metal->Type = MaterialType::METAL;
    metal->Roughness = 0.0f;

    rough->Albedo = Colorf {0.8f, 0.1f, 0.8f, 1.0f};
    rough->Type = MaterialType::DIFFUSE;
    rough->Roughness = 1.0f;

    glass->Albedo = Colorf {1.0f};
    glass->Type = MaterialType::GLASS;
    glass->IOR = 1.2f;

    // Glass
    world.push_back(std::make_shared<Sphere>(glm::vec3 {-0.7f, 0.0f, -1.0f}, 0.25f, glass));
    
    // Rough
    world.push_back(std::make_shared<Sphere>(glm::vec3 {0.0f, 0.0f, -1.0f}, 0.25f, rough));
    
    // Metal
    world.push_back(std::make_shared<Sphere>(glm::vec3 {0.7f, 0.0f, -1.0f}, 0.25f, metal));

    // Ground
    world.push_back(std::make_shared<Sphere>(glm::vec3 {0.0f, -100.5f, -1}, 100, rough));

    Camera camera;

#pragma omp parallel num_threads(12)
    {
        // (0, 0) is top-left
#pragma omp for
        for (int i = 0; i < IMAGE_HEIGHT; i++)
        {
            std::cout << "Processing row: " << i << std::endl;
            for (int j = 0; j < IMAGE_WIDTH; j++)
            {
                Colorf color {0.0f};
                for (int s = 0; s < NR_SAMPLES; s++)
                {
                    auto u = (j + Random()) / (IMAGE_WIDTH - 1);
                    auto v = 1 - ((i + Random()) / (IMAGE_HEIGHT - 1));
            
                    Ray ray = camera.GetRayTo(u, v);
                    color += CalculateRayColor(ray, world, NR_BOUNCES);
                }
            
                color = color / (float) NR_SAMPLES;
                color = glm::sqrt(color);  // Gamma correction
                data[j + i * IMAGE_WIDTH] = glm::clamp(color, {0.0f}, {0.999f});
            }
        }
    }

    WriteImage("out.png", IMAGE_WIDTH, IMAGE_HEIGHT, data);
}

Colorf CalculateRayColor(const Ray& r, const std::vector<SRef<RenderObject>>& world, unsigned int bounce)
{
    if (bounce <= 0)
    {
        return Colorf { 0.0f, 0.0f, 1.0f, 1.0f };
    }

    // Find the closest object that was hit
    HitRecord minRecord;
    minRecord.t = INF;
    bool hit = false;
    for (auto object : world)
    {
        HitRecord tmpRecord;
        if (object->Hit(r, 0.001, INF, tmpRecord))
        {
            hit = true;
            if (tmpRecord.t < minRecord.t)
            {
                minRecord = tmpRecord;
            }
        }
    }

    if (hit)
    {
        Ray reflected;
        Colorf attenuation;
        //minRecord.Mat->Scatter(r, minRecord, attenuation, reflected);
        // return {attenuation};
        if (minRecord.Mat->Scatter(r, minRecord, attenuation, reflected))
        {
            return attenuation * CalculateRayColor(reflected, world, bounce - 1);
        }
        else
        {
            return Colorf { 0.0f, 1.0f, 0.0f, 1.0f };
        }
    }

    glm::vec3 unitDir = glm::normalize(r.Direction());
    float t = 0.5f * (unitDir.y + 1.0f);
    return (1 - t) * WHITE + t * Colorf {0.5, 0.7, 1.0, 1.0f};
}
