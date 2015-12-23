#include "ProjectCompiler.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "json/json.h"

#include "ReiIO.h"
#include "ReiAssimp.h"

namespace ProjectCompiler {


const Path pNull = "null";
const Path pMain = "content-pack.json";
const Path pOutputRoot = "content-packs";

std::map<Path, std::string> occupiedNames;
Path pRoot;

Json::Value toJson(btVector3 vector) {
    Json::Value ret = Json::arrayValue;

    ret[0] = vector.getX();
    ret[1] = vector.getY();
    ret[2] = vector.getZ();

    return ret;
}
btVector3 toBullet(Json::Value& jValue) {
    if(jValue.isArray()) {
        btVector3 retVal(0, 0, 0);

        Json::Value& jx = jValue[0];
        Json::Value& jy = jValue[1];
        Json::Value& jz = jValue[2];

        if(jx.isDouble()) {
            retVal.setX(jx.asDouble());
        }
        if(jy.isDouble()) {
            retVal.setY(jy.asDouble());
        }
        if(jz.isDouble()) {
            retVal.setZ(jz.asDouble());
        }

        return retVal;
    }
    if(jValue.isDouble()) {
        btDouble n = jValue.asDouble();
        return btVector3(n, n, n);
    } else {
        return btVector3(0, 0, 0);
    }
}
std::string toString(const Json::Value& value, const std::string& defaultVal = pNull) {
    if(value.isNull()) {
        return defaultVal;
    }
    else {
        return value.asString();
    }
}

Path getDirectory(const Path& filename) {
    return filename.substr(0, filename.find_last_of("\\/"));
}
std::string getExtension(const Path& filename) {
    return filename.substr(filename.find_last_of(".") + 1);
}
Path shaveExtension(const Path& filename) {
    return filename.substr(0, filename.find_last_of("."));
}

// Try determine a path relative to the root of the project, based off a given context file
Path absolutify(Path path, Path context) {


    Path testPath;
    {
        testPath = context + "/" + path;
        std::ifstream test(pRoot + "/" + testPath);
        if(test.is_open()) {
            return testPath;
        }
    }
    {
        testPath = path;
        std::ifstream test(pRoot + "/" + testPath);
        if(test.is_open()) {
            return testPath;
        }
    }

    std::cout << "Error! Could not find file referred to by " << path << " in context " << context << std::endl;

    return pNull;
}

std::string qualify(Path path) {
    std::stringstream ss;

    for(std::size_t i = 0; i < path.size(); ++ i) {
        char c = path[i];

        if(c == '/') {
            ss << '.';
            continue;
        }
        if(c == ' ') {
            ss << '-';
            continue;
        }

        ss << c;
    }

    return ss.str();
}


Referen::Referen()
: absPath(pNull),
qualifiedName(qualify(absPath)) {
}

Referen::Referen(Path path, Path context)
: absPath(absolutify(path, context)),
qualifiedName(qualify(absPath)) {
}

Referen::Referen(Path path)
: absPath(path),
qualifiedName(qualify(absPath)) {
}


void compileProject(Path rootPath, irr::scene::ISceneManager* smgr) {

    pRoot = rootPath;

    FMain fMain;
    std::vector<FObject> fObjects;
    std::vector<FResource> fResources;

    // Main file
    {
        std::cout << "Loading main file..." << std::endl;
        Json::Value jrMain;
        // Read
        {
            std::ifstream rMain(pRoot + "/" + pMain);
            if(!rMain.is_open()) {
                std::cout << "Aborted! Could not read main file!" << std::endl;
                return;
            }
            rMain >> jrMain;
        }

        fMain.name = toString(jrMain["name"], "Unnamed Content Pack");
        fMain.namesp = toString(jrMain["namespace"], "anon");
        fMain.desc = toString(jrMain["description"], "No description available.");

        // Load authors
        {
            const Json::Value& jrMainObjects = jrMain["authors"];
            if(jrMainObjects.isArray()) {
                for(Json::Value::iterator it = jrMainObjects.begin(); it != jrMainObjects.end(); ++ it) {
                    fMain.authors.push_back(toString(*it));
                }
            }
            else {
                fMain.authors.push_back(toString(jrMainObjects, "Anonymous"));
            }
        }

        // Load object reference list
        {
            const Json::Value& jrMainObjects = jrMain["objects"];
            if(jrMainObjects.isArray()) {
                for(Json::Value::iterator it = jrMainObjects.begin(); it != jrMainObjects.end(); ++ it) {
                    Path objPath = toString(*it);
                    fMain.refObjects.push_back(Referen(objPath));
                }
            }
            else if(jrMainObjects.isString()) {
                Path objPath = jrMainObjects.asString();
                fMain.refObjects.push_back(Referen(objPath));
            }
            else {
                std::cout << "Aborted! Cannot parse objects list!" << std::endl;
                return;
            }
        }
    }

    // Object files
    {
        std::cout << "Loading object files...";
        for(std::vector<Referen>::iterator it = fMain.refObjects.begin(); it != fMain.refObjects.end(); ++ it) {

            const Referen& refObject = *it;
            Path refContext = getDirectory(refObject.absPath);

            FObject fObject;

            Json::Value jrObject;
            {
                std::ifstream rObject(pRoot + "/" + refObject.absPath);
                if(!rObject.is_open()) {
                    std::cout << "Aborted! Could not read " + refObject.absPath + "!" << std::endl;
                    return;
                }
                rObject >> jrObject;
            }

            fObject.id = toString(jrObject["id"]);

            Path pModel = toString(jrObject["model"]);
            fObject.refModel = Referen(pModel, refContext);
            FResource fModel;
            fModel.refSelf = fObject.refModel;
            fModel.type = MODEL;
            fResources.push_back(fModel);


            Path pPhysics = toString(jrObject["physics"]);
            fObject.refPhysics = Referen(pPhysics, refContext);
            FResource fPhys;
            fPhys.refSelf = fObject.refPhysics;
            fPhys.type = PHYSICS;
            fResources.push_back(fPhys);

            fObject.physOffset = toBullet(jrObject["physics-offset"]);

            fObject.refSelf = refObject;
            fObjects.push_back(fObject);
        }
    }

    std::cout << "Compiling entire project..." << std::endl;

    // Main file
    {
        std::cout << "Compiling main file..." << std::endl;

        // Construct new json file
        Json::Value jwMain;

        // Simple strings
        jwMain["name"] = fMain.name;
        jwMain["namespace"] = fMain.namesp;
        jwMain["description"] = fMain.desc;

        // Authors
        {
            Json::Value& jwMainAuthors = jwMain["authors"];
            for(std::vector<std::string>::iterator it = fMain.authors.begin(); it != fMain.authors.end(); ++ it) {
                jwMainAuthors.append(*it);
            }
        }

        // Objects
        {
            Json::Value& jwMainAuthors = jwMain["objects"];
            for(std::vector<Referen>::iterator it = fMain.refObjects.begin(); it != fMain.refObjects.end(); ++ it) {
                const Referen& referen = *it;
                jwMainAuthors.append(referen.qualifiedName);
            }
        }

        // Also clean the folder
        {
            std::string winCmd;
            winCmd = "del /q \"" + pOutputRoot + "\\" + fMain.name + "\\*.*\"";
            std::system(winCmd.c_str());
            winCmd = "mkdir \"" + pOutputRoot + "/" + fMain.name + "\"";
            std::system(winCmd.c_str());
        }

        std::ofstream wMain(pOutputRoot + "/" + fMain.name + "/content-pack.json");
        if(!wMain.is_open()) {
            std::cout << "Aborted! Could not write to main file!" << std::endl;
            return;
        }
        wMain << jwMain;
    }

    // Object files
    {
        std::cout << "Compiling object definition files..." << std::endl;
        for(std::vector<FObject>::iterator it = fObjects.begin(); it != fObjects.end(); ++ it) {
            const FObject& fObject = *it;

            // Construct new json file
            Json::Value jwObject;

            // Simple strings
            jwObject["id"] = fObject.id;
            jwObject["model"] = fObject.refModel.qualifiedName;
            jwObject["physics"] = fObject.refPhysics.qualifiedName;
            jwObject["physics-offset"] = toJson(fObject.physOffset);

            std::ofstream wObject(pOutputRoot + "/" + fMain.name + "/" + fObject.refSelf.qualifiedName);
            if(!wObject.is_open()) {
                std::cout << "Aborted! Could not write to object file " << fObject.refSelf.qualifiedName << std::endl;
                return;
            }
            wObject << jwObject;
        }
    }

    // Resource files
    {
        std::cout << "Compiling resources" << std::endl;
        for(std::vector<FResource>::iterator it = fResources.begin(); it != fResources.end(); ++ it) {
            const FResource& fResource = *it;



            Path output = pOutputRoot + "/" + fMain.name + "/" + fResource.refSelf.qualifiedName;

            switch(fResource.type) {
                /*
                case MODEL: {
                }
                */
                case PHYSICS: {
                    std::ifstream rResource(pRoot + "/" + fResource.refSelf.absPath);
                    if(!rResource.is_open()) {
                        std::cout << "Aborted! Could not read " << fResource.refSelf.absPath << "!" << std::endl;
                        return;
                    }
                    std::string extension = getExtension(fResource.refSelf.absPath);

                    FPhysics physShape;
                    if(extension == "json") {
                        Json::Value jwPhysics;
                        rResource >> jwPhysics;

                        std::string strType = jwPhysics["type"].asString();

                        if(strType == "sphere") {
                            physShape.type = SPHERE;
                            physShape.radius = jwPhysics["radius"].asDouble();
                        }
                        else if(strType == "box") {
                            physShape.type = BOX;
                            physShape.dimensions = toBullet(jwPhysics["size"]);
                        }
                        else if(strType == "cylinder") {
                            physShape.type = CYLINDER;
                            physShape.dimensions = toBullet(jwPhysics["size"]);
                        }
                        else if(strType == "capsule") {
                            physShape.type = CAPSULE;
                            physShape.radius = jwPhysics["radius"].asDouble();
                            physShape.height = jwPhysics["height"].asDouble();
                        }
                        else if(strType == "cone") {
                            physShape.type = CONE;
                            physShape.radius = jwPhysics["radius"].asDouble();
                            physShape.height = jwPhysics["height"].asDouble();
                        }
                    }
                    else {

                    }

                    output = shaveExtension(output);
                    ReiIO::savePhysics(output + ".physics", physShape);

                    break;
                }
                case MODEL: {

                    reia::ComplexMeshData* meshData = reia::loadUsingAssimp(smgr, pRoot + "/" + fResource.refSelf.absPath);

                    reia::ComplexMeshSceneNode* meshNode = reia::addNodeFromMesh(smgr, meshData);
                    delete meshNode;

                    output = shaveExtension(output);
                    ReiIO::saveComplexMesh(output + ".model", *meshData);
                    delete meshData;
                    break;
                }

                default: {

                }
            }
        }
    }
    occupiedNames.clear();
}

}
