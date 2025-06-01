#pragma once

#include <vector>
#include "../Factory.hpp"
#include "Utils.hpp"
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "Transform.hpp"
#include "../EdiblePlants/EdiblePlant.hpp"
#include "../Sprite.hpp"
#include "../Grid.hpp"


class HerbivoreSiteMemory : public Behaviour {

public:

    // Dot map node data: site value (for weighting) and reference to the visual marker.
    struct SiteInfo               
    {
        float   value{ 0.f };      
        Entity* marker{ nullptr };
    };


    // ------------------------------
    // | Constructors
    // ------------------------------

    HerbivoreSiteMemory();


    // ------------------------------
    // | Public methods
    // ------------------------------

    Vec3 quantizeVec3(Vec3 position, float gridSize);

    void lookAround();

    void purgeEmptySites();

    /// <summary>
    /// Makes a snapshot of the herd's surrondings, assessing its food and water availability.
    /// </summary>
    void makeSnapshot(Vec3 position);

    /// <summary>
    /// Checks if there is a valid site the herd can head to.
    /// </summary>
    /// <returns>A bool value denoting if there was a valid site to visit.</returns>
    bool isThereAValidSite();

    /// <summary>
    /// Picks the best site based on preferences and distance.
    /// </summary>
    /// <returns>The position of the best available site.</returns>
    Vec3 pickSite();

    /// <summary>
    /// Picks a valid food source at the site.
    /// </summary>
    /// <returns>A valid food source at the site.</returns>
    Entity* pickFoodSourceAtSite(Vec3 position);

    /// <summary>
    /// Picks a valid water source at the site.
    /// </summary>
    /// <returns>A valid water source at the site if any was found.</returns>
    std::optional<Vec3> pickWaterSourceAtSite(Vec3 position);


    /// <summary>
    /// Checks the surronding area to see if all food sources are depleted.
    /// </summary>
    bool foodDepleted(Vec3 position);

    void addPlantPreference(const std::string& label, float preference);

    void removePlantPreference(const std::string& label);


    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    inline int getCapacity() const { return capacity; }
    inline void setCapacity(int newCapacity) { capacity = newCapacity; }

    inline float getSiteRadius() const { return siteRadius; }
    inline void setSiteRadius(float newSiteRadius) { siteRadius = newSiteRadius; }

    inline float getMinSiteDistance() const { return minSiteDistance; }
    inline void setMinSiteDistance(float newMinSiteDistance) { minSiteDistance = newMinSiteDistance; }


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    void addSite(Vec3 position, float siteValue);
    bool eraseSite(Vec3 position);
    void createSiteMark(Vec3 position);
    bool hideSiteMark(Vec3 position); // Need to destroy instead
    bool destroySiteMark(Vec3 position); // Need to destroy instead


    // ------------------------------
    // | Fields
    // ------------------------------
    
    int throttleCounter;

    std::unordered_map<Vec3, SiteInfo> sites;

    //std::unordered_map<Vec3, float> sites;
    std::unordered_map<std::string, float> plantPreferences;

    int capacity;
    float siteRadius;
    float minSiteDistance;

    //std::unordered_map<Vec3, Entity*> siteMarks;

    
};

