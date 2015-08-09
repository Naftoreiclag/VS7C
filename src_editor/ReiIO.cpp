/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ReiIO.h"

#include <fstream>

#include "ProjectCompiler.h"

namespace ReiIO {

    void writeU32(std::ofstream& output, const irr::u32 value) {
        char data[4];
        data[0] = (value >> 24) & 0xFF;
        data[1] = (value >> 16) & 0xFF;
        data[2] = (value >> 8) & 0xFF;
        data[3] = value & 0xFF;

        output.write(data, sizeof data);
    }

    void writeU16(std::ofstream& output, const irr::u16 value) {
        char data[2];
        data[0] = (value >> 8) & 0xFF;
        data[1] = value & 0xFF;

        output.write(data, sizeof data);
    }

    void writeU8(std::ofstream& output, const irr::u8 value) {
        output.write((char*) &value, sizeof value);
    }

    void writeF32(std::ofstream& output, const irr::f32 value) {
        char data[4];
        irr::u32& thirtytwo = *((irr::u32*) &value);

        data[0] = (thirtytwo >> 24) & 0xFF;
        data[1] = (thirtytwo >> 16) & 0xFF;
        data[2] = (thirtytwo >> 8) & 0xFF;
        data[3] = thirtytwo & 0xFF;

        output.write(data, sizeof data);
    }

    void writeString(std::ofstream& output, const std::string& value) {
        output.write(value.c_str(), value.length() + 1);
    }

    void writeMatrix(std::ofstream& output, const irr::core::matrix4& value) {
        for(irr::u32 i = 0; i < 16; ++ i) {
            writeF32(output, value[i]);
        }
    }

    void writeMagic(std::ofstream& output) {
		writeU32(output, 0x4E414654);
		writeU32(output, 0x4F524549);
    }

	void writeToFile(std::string filename, const reia::ComplexMeshData& data) {
		std::ofstream ostream;
        ostream.open(filename, std::ios_base::binary);

		writeMagic(ostream);

        writeU16(ostream, 1);
        writeU16(ostream, 1);

        writeU8(ostream, data.numBones);
        for(irr::u8 i = 0; i < data.numBones; ++ i) {
            const reia::Bone& bone = data.bones[i];
            writeString(ostream, bone.name);
            writeMatrix(ostream, bone.inverseBindPose);
            writeU8(ostream, bone.parentId);
        }

        writeU16(ostream, data.numAnims);
        for(irr::u16 i = 0; i < data.numAnims; ++ i) {
            const reia::AnimationData& anim = data.anims[i];

            writeString(ostream, anim.animName);
            writeF32(ostream, anim.duration);
            writeF32(ostream, anim.tps);

            for(irr::u8 j = 0; j < anim.numChannels; ++ j) {
                const reia::ChannelData& channel = anim.channels[j];

                writeU8(ostream, channel.boneId);

                writeU32(ostream, channel.numPositions);
                for(irr::u32 k = 0; k < channel.numPositions; ++ k) {
                    const reia::VectorKey& key = channel.positions[k];

                    writeF32(ostream, key.time);
                    writeF32(ostream, key.value.X);
                    writeF32(ostream, key.value.Y);
                    writeF32(ostream, key.value.Z);
                }

                writeU32(ostream, channel.numRotations);
                for(irr::u32 k = 0; k < channel.numRotations; ++ k) {
                    const reia::QuaternionKey& key = channel.rotations[k];

                    writeF32(ostream, key.time);
                    writeF32(ostream, key.value.W);
                    writeF32(ostream, key.value.X);
                    writeF32(ostream, key.value.Y);
                    writeF32(ostream, key.value.Z);

                }

                writeU32(ostream, channel.numScalings);
                for(irr::u32 k = 0; k < channel.numScalings; ++ k) {
                    const reia::VectorKey& key = channel.scalings[k];

                    writeF32(ostream, key.time);
                    writeF32(ostream, key.value.X);
                    writeF32(ostream, key.value.Y);
                    writeF32(ostream, key.value.Z);
                }
            }
        }

        writeU8(ostream, data.numBuffers);
        for(irr::u8 i = 0; i < data.numBuffers; ++ i) {
            const reia::BufferMetadata& dbuffer = data.buffers[i];
            const irr::scene::SMeshBuffer* ibuffer = (irr::scene::SMeshBuffer*) data.mesh->getMeshBuffer(i);

            writeU8(ostream, dbuffer.usePos);
            writeU8(ostream, dbuffer.useNorm);
            writeU8(ostream, dbuffer.useColor);
            writeU8(ostream, dbuffer.useTexture);
            writeU8(ostream, dbuffer.useBone);

            writeU16(ostream, ibuffer->getVertexCount());
            for(irr::u16 j = 0; j < ibuffer->getVertexCount(); ++ j) {
                const irr::video::S3DVertex& vertex = ibuffer->Vertices[j];
                const reia::VertexMetadata& metadata = dbuffer.verts[j];

                if(dbuffer.usePos) {
                    writeF32(ostream, vertex.Pos.X);
                    writeF32(ostream, vertex.Pos.Y);
                    writeF32(ostream, vertex.Pos.Z);
                }
                if(dbuffer.useNorm) {
                    writeF32(ostream, vertex.Normal.X);
                    writeF32(ostream, vertex.Normal.Y);
                    writeF32(ostream, vertex.Normal.Z);
                }
                if(dbuffer.useColor) {
                    writeU32(ostream, vertex.Color.color);
                }
                if(dbuffer.useTexture) {
                /*
                texture coords here
                */
                }
                if(dbuffer.useBone) {
                    for(irr::u8 k = 0; k < 4; ++ k) {
                        writeU8(ostream, metadata.boneIds[k]);
                        writeF32(ostream, metadata.weights[k]);
                    }
                }
            }
        }

		// (Stream closed on destruction)
	}

    void writePhysics(std::string filename, const ProjectCompiler::FPhysics& data) {
		std::ofstream ostream;
        ostream.open(filename, std::ios_base::binary);

		writeMagic(ostream);

        irr::u8 bType;
        switch(data.type) {
            case ProjectCompiler::SPHERE: {
                bType = 1;
                break;
            }
            case ProjectCompiler::BOX: {
                bType = 2;
                break;
            }
            case ProjectCompiler::CYLINDER: {
                bType = 3;
                break;
            }
            case ProjectCompiler::CAPSULE: {
                bType = 4;
                break;
            }
            case ProjectCompiler::CONE: {
                bType = 5;
                break;
            }
            case ProjectCompiler::MULTI_SPHERE: {
                bType = 6;
                break;
            }
            case ProjectCompiler::CONCAVE_TRIANGLE_MESH: {
                bType = 7;
                break;
            }
            case ProjectCompiler::ARBITRARY_TRIANGLE_MESH: {
                bType = 8;
                break;
            }
            default: {
                bType = 0;
                break;
            }
        }
        writeU8(ostream, bType);
    }
}









