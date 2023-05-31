#pragma once

#ifndef ABSTRACT_SENSOR_H
#define ABSTRACT_SENSOR_H

class AbstractSens {
      public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void debug() = 0;
};

#endif  // ABSTRACT_SENSOR_H