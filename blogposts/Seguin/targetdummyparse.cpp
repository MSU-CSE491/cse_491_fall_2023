// Generated with ChatGPT: https://chat.openai.com/share/61b56d0c-6599-4926-8e01-4a4d26a7eb27

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// Define a target dummy with health
class TargetDummy {
public:
    int health;

    TargetDummy(int initialHealth) : health(initialHealth) {}
    bool isAlive() { return health > 0; }
    void takeDamage(int damage) {
        health -= damage;
    }
};

class Ability {
public:
    double damage;
    double damageInterval;
    double durationSeconds;
    double cooldown;
    int priority;
    std::string name;
    double lastCastTime;
    double lastDamageTime;
    bool isActive;

    // Define a small epsilon value for comparisons
    static const double epsilon;

    Ability(double dmg, double interval, double duration, double cd, int pri, const std::string& n)
        : damage(dmg), damageInterval(interval), durationSeconds(duration), cooldown(cd), priority(pri), name(n),
          lastCastTime(-cd), lastDamageTime(-interval), isActive(false) {}

    bool canCast(double elapsedTime, double sharedCooldown) {
        return !isActive && (elapsedTime - lastCastTime >= cooldown - epsilon) && (sharedCooldown <= epsilon);
    }

    bool canDealDamage(double elapsedTime) {
        return isActive && (elapsedTime - lastDamageTime >= damageInterval - epsilon);
    }

    void castAbility(double elapsedTime) {
        lastCastTime = elapsedTime;
        isActive = true;
    }

    void updateActivity(double elapsedTime) {
        if (elapsedTime - lastCastTime >= durationSeconds - epsilon) {
            isActive = false;
        }
    }
};

const double Ability::epsilon = 1e-5;
double ticksPerSecond = 10; // Adjust the tick rate as needed

// Comparison function for sorting abilities by priority
bool compareAbilities(const Ability& a, const Ability& b) {
    return a.priority > b.priority;
}

int main() {
    double sharedCooldownValue = 1.0; // Shared cooldown value
    double sharedCooldown = 0.0; // Initialize shared cooldown

    TargetDummy dummy(250); // Adjust initial health as needed

    std::vector<Ability> abilities;
    abilities.push_back(Ability(15.0, 10.0, 30.0, 0.0, 2, "Fireball"));
    abilities.push_back(Ability(10.0, 5.0, 20.0, 0.0, 1, "Frostbolt"));
    abilities.push_back(Ability(20.0, 15.0, 10.0, 0.0, 3, "Lightning Strike"));
    abilities.push_back(Ability(5.0, 0.0, 0.0, 0.0, 0, "Instant Damage")); // Adjusted priority

    // Sort abilities by priority, from highest to lowest
    std::sort(abilities.begin(), abilities.end(), compareAbilities);

    int ticks = 0;
    double elapsedTimeSeconds = 0.0;

    while (dummy.isAlive()) {
        for (Ability& ability : abilities) {
            // Update ability activity status
            ability.updateActivity(elapsedTimeSeconds);

            // Check if the ability can be cast
            if (ability.canCast(elapsedTimeSeconds, sharedCooldown)) {
                ability.castAbility(elapsedTimeSeconds);
                sharedCooldown = sharedCooldownValue; // Set shared cooldown to 1 second after casting
            }

            // Check if the ability is active and can deal damage
            if (ability.canDealDamage(elapsedTimeSeconds)) {
                dummy.takeDamage(static_cast<int>(ability.damage));
                ability.lastDamageTime = elapsedTimeSeconds;
                std::cout << "Ability: " << ability.name << " Priority: " << ability.priority
                          << " Used at Tick " << ticks << " (Time: " << elapsedTimeSeconds << "s) "
                          << "Dealt " << ability.damage << " damage. Dummy Health: " << dummy.health << std::endl;
            }
        }

        elapsedTimeSeconds += 1.0 / ticksPerSecond;
        sharedCooldown -= 1.0 / ticksPerSecond; // Decrement shared cooldown
        ticks++;
    }

    // Calculate and display the time taken to kill the dummy in seconds
    double totalElapsedTimeSeconds = static_cast<double>(ticks - 1) / ticksPerSecond;
    std::cout << "Target dummy killed in " << totalElapsedTimeSeconds << " seconds." << std::endl;

    return 0;
}