/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef REIBULLET_H
#define REIBULLET_H

#include "irrlicht.h"
#include "btBulletDynamicsCommon.h"

namespace reib {

    class BulletDebugDrawer : public btIDebugDraw {
    public:
        irr::video::SMaterial material;

        BulletDebugDrawer(irr::IrrlichtDevice* const device);
        virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
        virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
        virtual void reportErrorWarning(const char* warningString);
        virtual void draw3dText(const btVector3& location, const char* textString);
        virtual void setDebugMode(int debugMode);
        virtual int getDebugMode() const;
    private:
        irr::IrrlichtDevice* device;
        irr::video::IVideoDriver* driver;
        int debugMode;
    };
};

#endif // REIBULLET_H
