#ifndef OOP_PROJECT_ROBOT_FACTORY_H
#define OOP_PROJECT_ROBOT_FACTORY_H

#include "Robot.h"
#include <vector>
class Robot_factory {
private:
    std::size_t level;
    Robot unit;
    std::size_t factory_output;

public:
    Robot_factory(const Robot &unit, std::size_t factory_output, std::size_t level);

    void set_level(std::size_t level);
    void set_unit(const Robot &unit);
    void set_factory_output(std::size_t factory_output);

    std::size_t get_level() const;
    Robot get_unit() const;
    std::size_t get_factory_output() const;

    Robot create() const;

    std::vector<Robot> produce();
};


#endif
