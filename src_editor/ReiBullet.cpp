/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ReiBullet.h"

reib::BulletDebugDrawer::BulletDebugDrawer(irr::IrrlichtDevice* const device)
: device(device),
debugMode(btIDebugDraw::DBG_DrawWireframe) {
    driver = device->getVideoDriver();
    material.Lighting = false;
    material.Thickness += 2;
}

void reib::BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    driver->draw3DLine(
        irr::core::vector3df(from.getX(), from.getY(), from.getZ()),
        irr::core::vector3df(to.getX(), to.getY(), to.getZ()),
        irr::video::SColor(color.getX(), color.getY(), color.getZ(), 255.0f));
}

void reib::BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
}
void reib::BulletDebugDrawer::reportErrorWarning(const char* warningString) {
}
void reib::BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString) {
}
void reib::BulletDebugDrawer::setDebugMode(int debugMode) {
    this->debugMode = debugMode;
}
int reib::BulletDebugDrawer::getDebugMode() const {
    return debugMode;
}
