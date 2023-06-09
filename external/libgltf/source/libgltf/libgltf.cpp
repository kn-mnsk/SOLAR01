/*
 * This software is released under the MIT license.
 * 
 * Copyright (c) 2017-2022 Code 4 Game, Org. All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "libgltf/libgltf.h"

namespace libgltf
{
    SObject::SObject()
        : schemaType("")
    {
        //
    }

    SGlTFProperty::SGlTFProperty()
        : SObject()
        , extensions(nullptr)
        , extras(nullptr)
    {
        //
    }

    SGlTFProperty::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTFChildofRootProperty::SGlTFChildofRootProperty()
        : SGlTFProperty()
        , name("")
    {
        //
    }

    SGlTFChildofRootProperty::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessor::SAccessor()
        : SGlTFChildofRootProperty()
        , bufferView(nullptr)
        , byteOffset(0)
        , componentType(0)
        , normalized(false)
        , count(0)
        , type("")
        , max()
        , min()
        , sparse(nullptr)
    {
        //
    }

    SAccessor::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessorSparseIndices::SAccessorSparseIndices()
        : SGlTFProperty()
        , bufferView(nullptr)
        , byteOffset(0)
        , componentType(0)
    {
        //
    }

    SAccessorSparseIndices::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessorSparse::SAccessorSparse()
        : SGlTFProperty()
        , count(0)
        , indices(nullptr)
        , values(nullptr)
    {
        //
    }

    SAccessorSparse::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessorSparseValues::SAccessorSparseValues()
        : SGlTFProperty()
        , bufferView(nullptr)
        , byteOffset(0)
    {
        //
    }

    SAccessorSparseValues::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimationChannel::SAnimationChannel()
        : SGlTFProperty()
        , sampler(nullptr)
        , target(nullptr)
    {
        //
    }

    SAnimationChannel::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimationChannelTarget::SAnimationChannelTarget()
        : SGlTFProperty()
        , node(nullptr)
        , path("")
    {
        //
    }

    SAnimationChannelTarget::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimationSampler::SAnimationSampler()
        : SGlTFProperty()
        , input(nullptr)
        , interpolation("LINEAR")
        , output(nullptr)
    {
        //
    }

    SAnimationSampler::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimation::SAnimation()
        : SGlTFChildofRootProperty()
        , channels()
        , samplers()
    {
        //
    }

    SAnimation::operator bool() const
    {
        //TODO:
        return true;
    }

    SAsset::SAsset()
        : SGlTFProperty()
        , copyright("")
        , generator("")
        , version("")
        , minVersion("")
    {
        //
    }

    SAsset::operator bool() const
    {
        //TODO:
        return true;
    }

    SBuffer::SBuffer()
        : SGlTFChildofRootProperty()
        , uri("")
        , byteLength(0)
    {
        //
    }

    SBuffer::operator bool() const
    {
        //TODO:
        return true;
    }

    SBufferView::SBufferView()
        : SGlTFChildofRootProperty()
        , buffer(nullptr)
        , byteOffset(0)
        , byteLength(0)
        , byteStride(0)
        , target(0)
    {
        //
    }

    SBufferView::operator bool() const
    {
        //TODO:
        return true;
    }

    SCameraOrthographic::SCameraOrthographic()
        : SGlTFProperty()
        , xmag(0.0f)
        , ymag(0.0f)
        , zfar(0.0f)
        , znear(0.0f)
    {
        //
    }

    SCameraOrthographic::operator bool() const
    {
        //TODO:
        return true;
    }

    SCameraPerspective::SCameraPerspective()
        : SGlTFProperty()
        , aspectRatio(0.0f)
        , yfov(0.0f)
        , zfar(0.0f)
        , znear(0.0f)
    {
        //
    }

    SCameraPerspective::operator bool() const
    {
        //TODO:
        return true;
    }

    SCamera::SCamera()
        : SGlTFChildofRootProperty()
        , orthographic(nullptr)
        , perspective(nullptr)
        , type("")
    {
        //
    }

    SCamera::operator bool() const
    {
        //TODO:
        return true;
    }

    SExtension::SExtension()
        : SObject()
        // Manual code lines
        , properties()
    {
        //
    }

    SExtension::operator bool() const
    {
        //TODO:
        return true;
    }

    SExtras::SExtras()
        : SObject()
    {
        //
    }

    SExtras::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTF::SGlTF()
        : SGlTFProperty()
        , extensionsUsed()
        , extensionsRequired()
        , accessors()
        , animations()
        , asset(nullptr)
        , buffers()
        , bufferViews()
        , cameras()
        , images()
        , materials()
        , meshes()
        , nodes()
        , samplers()
        , scene(nullptr)
        , scenes()
        , skins()
        , textures()
    {
        //
    }

    SGlTF::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTFId::SGlTFId()
        : SObject()
        , int32_tValue(0)
    {
        //
    }

    SGlTFId::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTFId::operator int32_t() const
    {
        return int32_tValue;
    }

    SImage::SImage()
        : SGlTFChildofRootProperty()
        , uri("")
        , mimeType("")
        , bufferView(nullptr)
    {
        //
    }

    SImage::operator bool() const
    {
        //TODO:
        return true;
    }

    STextureInfo::STextureInfo()
        : SGlTFProperty()
        , index(nullptr)
        , texCoord(0)
    {
        //
    }

    STextureInfo::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterialNormalTextureInfo::SMaterialNormalTextureInfo()
        : STextureInfo()
        , scale(1.000000f)
    {
        //
    }

    SMaterialNormalTextureInfo::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterialOcclusionTextureInfo::SMaterialOcclusionTextureInfo()
        : STextureInfo()
        , strength(1.000000f)
    {
        //
    }

    SMaterialOcclusionTextureInfo::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterialPBRMetallicRoughness::SMaterialPBRMetallicRoughness()
        : SGlTFProperty()
        , baseColorFactor({ 1.000000f, 1.000000f, 1.000000f, 1.000000f })
        , baseColorTexture(nullptr)
        , metallicFactor(1.000000f)
        , roughnessFactor(1.000000f)
        , metallicRoughnessTexture(nullptr)
    {
        //
    }

    SMaterialPBRMetallicRoughness::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterial::SMaterial()
        : SGlTFChildofRootProperty()
        , pbrMetallicRoughness(nullptr)
        , normalTexture(nullptr)
        , occlusionTexture(nullptr)
        , emissiveTexture(nullptr)
        , emissiveFactor({ 0.000000f, 0.000000f, 0.000000f })
        , alphaMode("OPAQUE")
        , alphaCutoff(0.500000f)
        , doubleSided(false)
    {
        //
    }

    SMaterial::operator bool() const
    {
        //TODO:
        return true;
    }

    SMeshPrimitive::SMeshPrimitive()
        : SGlTFProperty()
        , attributes()
        , indices(nullptr)
        , material(nullptr)
        , mode(4)
        , targets()
    {
        //
    }

    SMeshPrimitive::operator bool() const
    {
        //TODO:
        return true;
    }

    SMesh::SMesh()
        : SGlTFChildofRootProperty()
        , primitives()
        , weights()
    {
        //
    }

    SMesh::operator bool() const
    {
        //TODO:
        return true;
    }

    SNode::SNode()
        : SGlTFChildofRootProperty()
        , camera(nullptr)
        , children()
        , skin(nullptr)
        , matrix({ 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f })
        , mesh(nullptr)
        , rotation({ 0.000000f, 0.000000f, 0.000000f, 1.000000f })
        , scale({ 1.000000f, 1.000000f, 1.000000f })
        , translation({ 0.000000f, 0.000000f, 0.000000f })
        , weights()
    {
        //
    }

    SNode::operator bool() const
    {
        //TODO:
        return true;
    }

    SSampler::SSampler()
        : SGlTFChildofRootProperty()
        , magFilter(0)
        , minFilter(0)
        , wrapS(10497)
        , wrapT(10497)
    {
        //
    }

    SSampler::operator bool() const
    {
        //TODO:
        return true;
    }

    SScene::SScene()
        : SGlTFChildofRootProperty()
        , nodes()
    {
        //
    }

    SScene::operator bool() const
    {
        //TODO:
        return true;
    }

    SSkin::SSkin()
        : SGlTFChildofRootProperty()
        , inverseBindMatrices(nullptr)
        , skeleton(nullptr)
        , joints()
    {
        //
    }

    SSkin::operator bool() const
    {
        //TODO:
        return true;
    }

    STexture::STexture()
        : SGlTFChildofRootProperty()
        , sampler(nullptr)
        , source(nullptr)
    {
        //
    }

    STexture::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_draco_mesh_compressionextension::SKHR_draco_mesh_compressionextension()
        : SGlTFProperty()
        , bufferView(nullptr)
        , attributes()
    {
        //
    }

    SKHR_draco_mesh_compressionextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_lights_punctualglTFextension::SKHR_lights_punctualglTFextension()
        : SGlTFProperty()
        , lights()
    {
        //
    }

    SKHR_lights_punctualglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SLight::SLight()
        : SGlTFChildofRootProperty()
        , color({ 1.000000f, 1.000000f, 1.000000f })
        , intensity(1.000000f)
        , spot(nullptr)
        , type("")
        , range(0.0f)
    {
        //
    }

    SLight::operator bool() const
    {
        //TODO:
        return true;
    }

    SLightspot::SLightspot()
        : SGlTFProperty()
        , innerConeAngle(0.000000f)
        , outerConeAngle(0.785398f)
    {
        //
    }

    SLightspot::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_lights_punctualnodeextension::SKHR_lights_punctualnodeextension()
        : SGlTFProperty()
        , light(nullptr)
    {
        //
    }

    SKHR_lights_punctualnodeextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_clearcoatglTFextension::SKHR_materials_clearcoatglTFextension()
        : SGlTFProperty()
        , clearcoatFactor(0.000000f)
        , clearcoatTexture(nullptr)
        , clearcoatRoughnessFactor(0.000000f)
        , clearcoatRoughnessTexture(nullptr)
        , clearcoatNormalTexture(nullptr)
    {
        //
    }

    SKHR_materials_clearcoatglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_iorglTFextension::SKHR_materials_iorglTFextension()
        : SGlTFProperty()
        , ior(1.500000f)
    {
        //
    }

    SKHR_materials_iorglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_sheenglTFextension::SKHR_materials_sheenglTFextension()
        : SGlTFProperty()
        , sheenColorFactor({ 0.000000f, 0.000000f, 0.000000f })
        , sheenColorTexture(nullptr)
        , sheenRoughnessFactor(0.000000f)
        , sheenRoughnessTexture(nullptr)
    {
        //
    }

    SKHR_materials_sheenglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_specularglTFextension::SKHR_materials_specularglTFextension()
        : SGlTFProperty()
        , specularFactor(1.000000f)
        , specularTexture(nullptr)
        , specularColorFactor({ 1.000000f, 1.000000f, 1.000000f })
        , specularColorTexture(nullptr)
    {
        //
    }

    SKHR_materials_specularglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_transmissionglTFextension::SKHR_materials_transmissionglTFextension()
        : SGlTFProperty()
        , transmissionFactor(0.000000f)
        , transmissionTexture(nullptr)
    {
        //
    }

    SKHR_materials_transmissionglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_unlitglTFextension::SKHR_materials_unlitglTFextension()
        : SGlTFProperty()
    {
        //
    }

    SKHR_materials_unlitglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_variantsglTFextension::SKHR_materials_variantsglTFextension()
        : SGlTFProperty()
        , variants()
    {
        //
    }

    SKHR_materials_variantsglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_variantsmeshprimitiveextension::SKHR_materials_variantsmeshprimitiveextension()
        : SGlTFProperty()
        , mappings()
    {
        //
    }

    SKHR_materials_variantsmeshprimitiveextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_volumeglTFextension::SKHR_materials_volumeglTFextension()
        : SGlTFProperty()
        , thicknessFactor(0.000000f)
        , thicknessTexture(nullptr)
        , attenuationDistance(0.0f)
        , attenuationColor({ 1.000000f, 1.000000f, 1.000000f })
    {
        //
    }

    SKHR_materials_volumeglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_texture_transformtextureInfoextension::SKHR_texture_transformtextureInfoextension()
        : SGlTFProperty()
        , offset({ 0.000000f, 0.000000f })
        , rotation(0.000000f)
        , scale({ 1.000000f, 1.000000f })
        , texCoord(0)
    {
        //
    }

    SKHR_texture_transformtextureInfoextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SADOBE_materials_thin_transparencyglTFextension::SADOBE_materials_thin_transparencyglTFextension()
        : SGlTFProperty()
        , transmissionFactor(1.000000f)
        , transmissionTexture(nullptr)
        , ior(1.330000f)
    {
        //
    }

    SADOBE_materials_thin_transparencyglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SArticulation::SArticulation()
        : SGlTFProperty()
        , name("")
        , stages()
        , pointingVector()
    {
        //
    }

    SArticulation::operator bool() const
    {
        //TODO:
        return true;
    }

    SArticulationStage::SArticulationStage()
        : SGlTFProperty()
        , name("")
        , type("")
        , minimumValue(0.0f)
        , maximumValue(0.0f)
        , initialValue(0.0f)
    {
        //
    }

    SArticulationStage::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_articulationsglTFextension::SAGI_articulationsglTFextension()
        : SGlTFProperty()
        , articulations()
    {
        //
    }

    SAGI_articulationsglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_articulationsglTFNodeextension::SAGI_articulationsglTFNodeextension()
        : SGlTFProperty()
        , isAttachPoint(false)
        , articulationName("")
    {
        //
    }

    SAGI_articulationsglTFNodeextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_stk_metadataglTFextension::SAGI_stk_metadataglTFextension()
        : SGlTFProperty()
        , solarPanelGroups()
    {
        //
    }

    SAGI_stk_metadataglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_stk_metadataglTFNodeextension::SAGI_stk_metadataglTFNodeextension()
        : SGlTFProperty()
        , solarPanelGroupName("")
        , noObscuration(false)
    {
        //
    }

    SAGI_stk_metadataglTFNodeextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SSolarPanelGroup::SSolarPanelGroup()
        : SGlTFProperty()
        , name("")
        , efficiency(0.0f)
    {
        //
    }

    SSolarPanelGroup::operator bool() const
    {
        //TODO:
        return true;
    }

    SCESIUM_primitive_outlineglTFprimitiveextension::SCESIUM_primitive_outlineglTFprimitiveextension()
        : SGlTFProperty()
        , indices(0)
    {
        //
    }

    SCESIUM_primitive_outlineglTFprimitiveextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SEXT_mesh_gpu_instancingglTFextension::SEXT_mesh_gpu_instancingglTFextension()
        : SGlTFProperty()
        , attributes()
    {
        //
    }

    SEXT_mesh_gpu_instancingglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SEXT_texture_webpglTFextension::SEXT_texture_webpglTFextension()
        : SGlTFProperty()
        , source(nullptr)
    {
        //
    }

    SEXT_texture_webpglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SMSFT_lodglTFextension::SMSFT_lodglTFextension()
        : SGlTFProperty()
        , ids()
    {
        //
    }

    SMSFT_lodglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SMSFT_texture_ddsextension::SMSFT_texture_ddsextension()
        : SGlTFProperty()
        , source(nullptr)
    {
        //
    }

    SMSFT_texture_ddsextension::operator bool() const
    {
        //TODO:
        return true;
    }

}
