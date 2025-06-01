#include "HerbivoreSiteMemory.hpp"

HerbivoreSiteMemory::HerbivoreSiteMemory() : Behaviour() {

    throttleCounter = 0;
	capacity = 20;
	siteRadius = 6.0f;
	minSiteDistance = 3.0f;

}


Vec3 HerbivoreSiteMemory::quantizeVec3(Vec3 position, float gridSize)
{
    return Vec3(
        std::round(position.x / gridSize) * gridSize,
        std::round(position.y / gridSize) * gridSize,
        std::round(position.z / gridSize) * gridSize
    );
}


void HerbivoreSiteMemory::lookAround() {

    Vec3 herdPos = getEntity().getComponent<Transform>().getPosition();

    makeSnapshot(herdPos);

    makeSnapshot(herdPos + Vec3(5.0f, 0.0f, 0.0f));
    makeSnapshot(herdPos + Vec3(-5.0f, 0.0f, 0.0f));
    makeSnapshot(herdPos + Vec3(0.0f, 0.0f, 5.0f));
    makeSnapshot(herdPos + Vec3(0.0f, 0.0f, -5.0f));

    makeSnapshot(herdPos + Vec3(3.5f, 0.0f, 3.5f));
    makeSnapshot(herdPos + Vec3(-3.5f, 0.0f, 3.5f));
    makeSnapshot(herdPos + Vec3(3.5f, 0.0f, -3.5f));
    makeSnapshot(herdPos + Vec3(-3.5f, 0.0f, -3.5f));

}


void HerbivoreSiteMemory::purgeEmptySites()
{

    std::vector<Vec3> toRemove;

    for (const auto& [sitePos, info] : sites)
        if (foodDepleted(sitePos))
            toRemove.push_back(sitePos);

    for (const Vec3& pos : toRemove)
        eraseSite(pos);

}


void HerbivoreSiteMemory::makeSnapshot(Vec3 position) {

    float siteValue = 0.0f;

    Vec3 snappedPosition = quantizeVec3(position, 0.2);

    // Iterate through the registered plants, increase site value when instances are found.
    for (const auto& [label, preference] : plantPreferences) {

        auto plants = Labels::getAll(label);
        for (auto* plant : plants) {

            Vec3 plantPosition = plant->getComponent<Transform>().getPosition();

            if (glm::distance(snappedPosition, plantPosition) < siteRadius && plant->getComponent<EdiblePlant>().isConsumable()) {
                siteValue += preference;
            }

        }

    }

    if (siteValue == 0)
        return;

    // Check if a nearby site already exists and whether the new one is better
    bool betterThanAll = true;
    std::vector<Vec3> toRemove;

    for (const auto& [otherPos, info] : sites)
    {
        float dist = glm::distance(snappedPosition, otherPos);

        if (dist < minSiteDistance)
        {
            if (siteValue <= info.value)
            {
                betterThanAll = false;
                break;
            }
            else
            {
                toRemove.push_back(otherPos);
            }
        }
    }


    // Replace nearby sites if the new one is better
    if (betterThanAll)
    {
        for (const Vec3& pos : toRemove)
            eraseSite(pos);

        addSite(snappedPosition, siteValue);
    }


    // Remove the weakest site if we exceed capacity.
    if (sites.size() > static_cast<std::size_t>(capacity))
    {
        auto weakest = std::min_element(
            sites.begin(), sites.end(),
            [](const auto& a, const auto& b)
            {
                return a.second.value < b.second.value;
            });

        if (weakest != sites.end())
            eraseSite(weakest->first);
    }

}


bool HerbivoreSiteMemory::isThereAValidSite() {
    //logDebug(sites.empty() ? "No valid sites." : "There is at least one valid site.");
    return !sites.empty();
}


Vec3 HerbivoreSiteMemory::pickSite() {

    if (sites.empty()) {
        //logDebug("We don't have any valid site in our memory yet.");
        return Vec3(randf(-20, +20), 0, randf(-20, +20));
    }

    glm::vec3 herdPosition = getEntity().getComponent<Transform>().getPosition();
    
    // Picking the maximum distance from the site list.
    // It will be kind of a reference to calculate a modifier from.
    float maxDistance = 0.0f;
    for (const auto& [sitePosition, _] : sites) {
        float dist = glm::distance(herdPosition, sitePosition);
        if (dist > maxDistance) {
            maxDistance = dist;
        }
    }

    // Picking the maximum site value from the site list.
    // It will be kind of a reference to calculate a modifier from.
    float maxSiteValue = 0.0f;
    for (const auto& entry : sites)
    {
        if (entry.second.value > maxSiteValue)
            maxSiteValue = entry.second.value;
    }

    glm::vec3 bestSite = sites.begin()->first;
    float bestScore = -1.0f;

    for (const auto& [sitePosition, info] : sites) {

        float distance = glm::distance(herdPosition, sitePosition);
        float distanceScore = maxDistance > 0.0f
            ? 1 - (distance / maxDistance)
            : 0.0f;

        float siteValueScore = maxSiteValue > 0.0f
            ? info.value / maxSiteValue
            : 0.0f;

        // Cooking the final comparator value.
        float combinedScore = siteValueScore * 0.2f + distanceScore * 0.8f;

        if (combinedScore > bestScore) {
            bestScore = combinedScore;
            bestSite = sitePosition;
        }

    }

    return bestSite;

}


Entity* HerbivoreSiteMemory::pickFoodSourceAtSite(Vec3 siteCenter)
{
    struct Candidate
    {
        Entity* plant;
        float   weight;
    };

    std::vector<Candidate> candidatePlants;

    // Collect plants within the site radius.
    for (const auto& [label, preference] : plantPreferences)
    {
        auto plants = Labels::getAll(label);
        for (Entity* plant : plants)
        {
            Vec3 plantPos = plant->getComponent<Transform>().getPosition();
            if (glm::distance(siteCenter, plantPos) <= siteRadius)          
            {
                candidatePlants.push_back({ plant, preference });
            }
        }
    }

    if (candidatePlants.empty()) {
        //logDebug("The site is empty. It should NOT be the case.");
        return nullptr;
    }

    while (!candidatePlants.empty())
    {
        // Compute total weight.
        float totalWeight = 0.0f;
        for (const Candidate& c : candidatePlants) totalWeight += c.weight;

        // Weighted random pick.
        float randomValue = randf(0.0f, totalWeight);
        float accumulatedWeight = 0.0f;
        int   chosenIndex = 0;

        // Kinda interesting way to choose a random plant with weights taken into consideration, but it
        // seems good. We'll see.
        for (int i = 0; i < static_cast<int>(candidatePlants.size()); ++i)
        {
            accumulatedWeight += candidatePlants[i].weight;
            if (accumulatedWeight >= randomValue)
            {
                chosenIndex = i;
                break;
            }
        }

        Entity* chosenPlant = candidatePlants[chosenIndex].plant;

        if (chosenPlant->getComponent<EdiblePlant>().isConsumable()) {
            return chosenPlant;
        }

        candidatePlants.erase(candidatePlants.begin() + chosenIndex);
    }

    // Absolutely no food. We shouldn't really get here, because we will run the less heavy foodDepleted()
    // method before querying.
    //logDebug("No food at the site: foodDepleted() lied, or (more likely) I messed up something.");
    return nullptr;

}


std::optional<Vec3> HerbivoreSiteMemory::pickWaterSourceAtSite(Vec3 position)
{

    Entity& baseGrid = Labels::getUnique("global");
    Grid& grid = baseGrid.getComponent<Grid>();
    const auto& waterTiles = grid.getAllOfType("water");
    if (waterTiles.empty())
        return std::nullopt;
    float tileSize = grid.getTileSize();
    Vec3 origin = grid.getPosition();

    std::vector<Ivec2> valid;
    for (const Ivec2& tile : waterTiles)
    {

        float minX = origin.x + tile.x * tileSize;
        float maxX = origin.x + (tile.x + 1) * tileSize;
        float minZ = origin.z + tile.y * tileSize;
        float maxZ = origin.z + (tile.y + 1) * tileSize;

        float dx = std::max({ minX - position.x, 0.f, position.x - maxX });
        float dz = std::max({ minZ - position.z, 0.f, position.z - maxZ });

        if (std::sqrt(dx * dx + dz * dz) <= siteRadius * 2)
            valid.push_back(tile);

    }

    if (valid.empty())
        return std::nullopt;

    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> pick(0, static_cast<int>(valid.size()) - 1);
    Ivec2 chosenTile = valid[pick(rng)];

    float x = randf(chosenTile.x * tileSize, (chosenTile.x + 1) * tileSize);
    float z = randf(chosenTile.y * tileSize, (chosenTile.y + 1) * tileSize);

    return origin + Vec3(x, 0.f, z);

}



bool HerbivoreSiteMemory::foodDepleted(Vec3 position) {

    bool foodLeft = false;

    // Iterate through the registered plants, checks if there is still available in the area.
    for (const auto& [label, preference] : plantPreferences) {

        auto plants = Labels::getAll(label);
        for (auto* plant : plants) {

            glm::vec3 plantPosition = plant->getComponent<Transform>().getPosition();

            if (glm::distance(position, plantPosition) < siteRadius && plant->getComponent<EdiblePlant>().isConsumable()) {
                foodLeft = true;
                break;
            }

        }

        if (foodLeft) break;

    }

    return !foodLeft;

}


void HerbivoreSiteMemory::addPlantPreference(const std::string& label, float preference) {
    plantPreferences[label] = preference;
}


void HerbivoreSiteMemory::removePlantPreference(const std::string& label) {
    plantPreferences.erase(label);
}


void HerbivoreSiteMemory::start(Entity& entity) { }

void HerbivoreSiteMemory::update(Entity& entity)
{

    if (throttleCounter++ % 20 == 0) {
        lookAround();
        purgeEmptySites();
    }

    /*char buf[64];
    std::snprintf(buf, sizeof(buf), "Site count (update): %zu", sites.size());
    logDebug(buf);*/

}


void HerbivoreSiteMemory::addSite(Vec3 position, float siteValue)
{
    Vec3 key = quantizeVec3(position, 0.2f);

    Entity& marker = Factory::createSiteMarker(position);
    marker.getComponent<Sprite>().setOpacity(1);// DEBUG
    sites[key] = { siteValue, &marker };

}


bool HerbivoreSiteMemory::eraseSite(Vec3 position)
{
    Vec3 key = quantizeVec3(position, 0.2f);

    auto it = sites.find(key);
    if (it == sites.end())
        return false;

    if (Entity* site = it->second.marker)
        site->destroy();

    sites.erase(it);
    return true;
}

