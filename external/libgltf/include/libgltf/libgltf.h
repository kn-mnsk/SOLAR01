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

#ifndef LIBGLTF_H
#define LIBGLTF_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <istream>

#define LIBGLTF_MAJOR_VERSION    0
#define LIBGLTF_MINOR_VERSION    1
#define LIBGLTF_PATCH_VERSION    9

/* #undef LIBGLTF_WITH_MSVC_MT */
/* #undef LIBGLTF_WITH_GOOGLE_DRACO */

namespace libgltf
{

    struct SGlTF;
    bool operator<<(SGlTF& _gltf, const std::string& _sContent);
    bool operator>>(const SGlTF& _gltf, std::string& _sContent);
    
    /*!
     * struct: SObject
     */
    struct SObject
    {
        SObject();
        std::string schemaType;
    };

    // declare all types
    struct SAccessor;
    struct SAccessorSparseIndices;
    struct SAccessorSparse;
    struct SAccessorSparseValues;
    struct SAnimationChannel;
    struct SAnimationChannelTarget;
    struct SAnimationSampler;
    struct SAnimation;
    struct SAsset;
    struct SBuffer;
    struct SBufferView;
    struct SCameraOrthographic;
    struct SCameraPerspective;
    struct SCamera;
    struct SExtension;
    struct SExtras;
    struct SGlTF;
    struct SGlTFChildofRootProperty;
    struct SGlTFId;
    struct SGlTFProperty;
    struct SImage;
    struct SMaterialNormalTextureInfo;
    struct SMaterialOcclusionTextureInfo;
    struct SMaterialPBRMetallicRoughness;
    struct SMaterial;
    struct SMeshPrimitive;
    struct SMesh;
    struct SNode;
    struct SSampler;
    struct SScene;
    struct SSkin;
    struct STexture;
    struct STextureInfo;
    struct SKHR_draco_mesh_compressionextension;
    struct SKHR_lights_punctualglTFextension;
    struct SLight;
    struct SLightspot;
    struct SKHR_lights_punctualnodeextension;
    struct SKHR_materials_clearcoatglTFextension;
    struct SKHR_materials_iorglTFextension;
    struct SKHR_materials_sheenglTFextension;
    struct SKHR_materials_specularglTFextension;
    struct SKHR_materials_transmissionglTFextension;
    struct SKHR_materials_unlitglTFextension;
    struct SKHR_materials_variantsglTFextension;
    struct SKHR_materials_variantsmeshprimitiveextension;
    struct SKHR_materials_volumeglTFextension;
    struct SKHR_texture_transformtextureInfoextension;
    struct SADOBE_materials_thin_transparencyglTFextension;
    struct SArticulation;
    struct SArticulationStage;
    struct SAGI_articulationsglTFextension;
    struct SAGI_articulationsglTFNodeextension;
    struct SAGI_stk_metadataglTFextension;
    struct SAGI_stk_metadataglTFNodeextension;
    struct SSolarPanelGroup;
    struct SCESIUM_primitive_outlineglTFprimitiveextension;
    struct SEXT_mesh_gpu_instancingglTFextension;
    struct SEXT_texture_webpglTFextension;
    struct SMSFT_lodglTFextension;
    struct SMSFT_texture_ddsextension;

    /*!
     * struct: SGlTFProperty
     */
    struct SGlTFProperty : SObject
    {
        SGlTFProperty();

        // Check valid
        operator bool() const;

        std::shared_ptr<SExtension> extensions;
        std::shared_ptr<SExtras> extras;
    };

    /*!
     * struct: SGlTFChildofRootProperty
     */
    struct SGlTFChildofRootProperty : SGlTFProperty
    {
        SGlTFChildofRootProperty();

        // Check valid
        operator bool() const;

        // The user-defined name of this object.
        std::string name;
    };

    /*!
     * struct: SAccessor
     * A typed view into a buffer view that contains raw binary data.
     */
    struct SAccessor : SGlTFChildofRootProperty
    {
        SAccessor();

        // Check valid
        operator bool() const;

        // The index of the bufferView.
        std::shared_ptr<SGlTFId> bufferView;
        // The offset relative to the start of the buffer view in bytes.
        int32_t byteOffset;
        // The datatype of the accessor's components.
        int32_t componentType;
        // Specifies whether integer data values are normalized before usage.
        bool normalized;
        // The number of elements referenced by this accessor.
        int32_t count;
        // Specifies if the accessor's elements are scalars, vectors, or matrices.
        std::string type;
        // Maximum value of each component in this accessor.
        std::vector<float> max;
        // Minimum value of each component in this accessor.
        std::vector<float> min;
        // Sparse storage of elements that deviate from their initialization value.
        std::shared_ptr<SAccessorSparse> sparse;
    };

    /*!
     * struct: SAccessorSparseIndices
     * An object pointing to a buffer view containing the indices of deviating accessor values. The number of indices is equal to `accessor.sparse.count`. Indices **MUST** strictly increase.
     */
    struct SAccessorSparseIndices : SGlTFProperty
    {
        SAccessorSparseIndices();

        // Check valid
        operator bool() const;

        // The index of the buffer view with sparse indices. The referenced buffer view **MUST NOT** have its `target` or `byteStride` properties defined. The buffer view and the optional `byteOffset` **MUST** be aligned to the `componentType` byte length.
        std::shared_ptr<SGlTFId> bufferView;
        // The offset relative to the start of the buffer view in bytes.
        int32_t byteOffset;
        // The indices data type.
        int32_t componentType;
    };

    /*!
     * struct: SAccessorSparse
     * Sparse storage of accessor values that deviate from their initialization value.
     */
    struct SAccessorSparse : SGlTFProperty
    {
        SAccessorSparse();

        // Check valid
        operator bool() const;

        // Number of deviating accessor values stored in the sparse array.
        int32_t count;
        // An object pointing to a buffer view containing the indices of deviating accessor values. The number of indices is equal to `count`. Indices **MUST** strictly increase.
        std::shared_ptr<SAccessorSparseIndices> indices;
        // An object pointing to a buffer view containing the deviating accessor values.
        std::shared_ptr<SAccessorSparseValues> values;
    };

    /*!
     * struct: SAccessorSparseValues
     * An object pointing to a buffer view containing the deviating accessor values. The number of elements is equal to `accessor.sparse.count` times number of components. The elements have the same component type as the base accessor. The elements are tightly packed. Data **MUST** be aligned following the same rules as the base accessor.
     */
    struct SAccessorSparseValues : SGlTFProperty
    {
        SAccessorSparseValues();

        // Check valid
        operator bool() const;

        // The index of the bufferView with sparse values. The referenced buffer view **MUST NOT** have its `target` or `byteStride` properties defined.
        std::shared_ptr<SGlTFId> bufferView;
        // The offset relative to the start of the bufferView in bytes.
        int32_t byteOffset;
    };

    /*!
     * struct: SAnimationChannel
     * An animation channel combines an animation sampler with a target property being animated.
     */
    struct SAnimationChannel : SGlTFProperty
    {
        SAnimationChannel();

        // Check valid
        operator bool() const;

        // The index of a sampler in this animation used to compute the value for the target.
        std::shared_ptr<SGlTFId> sampler;
        // The descriptor of the animated property.
        std::shared_ptr<SAnimationChannelTarget> target;
    };

    /*!
     * struct: SAnimationChannelTarget
     * The descriptor of the animated property.
     */
    struct SAnimationChannelTarget : SGlTFProperty
    {
        SAnimationChannelTarget();

        // Check valid
        operator bool() const;

        // The index of the node to animate. When undefined, the animated object **MAY** be defined by an extension.
        std::shared_ptr<SGlTFId> node;
        // The name of the node's TRS property to animate, or the `"weights"` of the Morph Targets it instantiates. For the `"translation"` property, the values that are provided by the sampler are the translation along the X, Y, and Z axes. For the `"rotation"` property, the values are a quaternion in the order (x, y, z, w), where w is the scalar. For the `"scale"` property, the values are the scaling factors along the X, Y, and Z axes.
        std::string path;
    };

    /*!
     * struct: SAnimationSampler
     * An animation sampler combines timestamps with a sequence of output values and defines an interpolation algorithm.
     */
    struct SAnimationSampler : SGlTFProperty
    {
        SAnimationSampler();

        // Check valid
        operator bool() const;

        // The index of an accessor containing keyframe timestamps.
        std::shared_ptr<SGlTFId> input;
        // Interpolation algorithm.
        std::string interpolation;
        // The index of an accessor, containing keyframe output values.
        std::shared_ptr<SGlTFId> output;
    };

    /*!
     * struct: SAnimation
     * A keyframe animation.
     */
    struct SAnimation : SGlTFChildofRootProperty
    {
        SAnimation();

        // Check valid
        operator bool() const;

        // An array of animation channels. An animation channel combines an animation sampler with a target property being animated. Different channels of the same animation **MUST NOT** have the same targets.
        std::vector<std::shared_ptr<SAnimationChannel>> channels;
        // An array of animation samplers. An animation sampler combines timestamps with a sequence of output values and defines an interpolation algorithm.
        std::vector<std::shared_ptr<SAnimationSampler>> samplers;
    };

    /*!
     * struct: SAsset
     * Metadata about the glTF asset.
     */
    struct SAsset : SGlTFProperty
    {
        SAsset();

        // Check valid
        operator bool() const;

        // A copyright message suitable for display to credit the content creator.
        std::string copyright;
        // Tool that generated this glTF model.  Useful for debugging.
        std::string generator;
        // The glTF version in the form of `<major>.<minor>` that this asset targets.
        std::string version;
        // The minimum glTF version in the form of `<major>.<minor>` that this asset targets. This property **MUST NOT** be greater than the asset version.
        std::string minVersion;
    };

    /*!
     * struct: SBuffer
     * A buffer points to binary geometry, animation, or skins.
     */
    struct SBuffer : SGlTFChildofRootProperty
    {
        SBuffer();

        // Check valid
        operator bool() const;

        // The URI (or IRI) of the buffer.
        std::string uri;
        // The length of the buffer in bytes.
        int32_t byteLength;
    };

    /*!
     * struct: SBufferView
     * A view into a buffer generally representing a subset of the buffer.
     */
    struct SBufferView : SGlTFChildofRootProperty
    {
        SBufferView();

        // Check valid
        operator bool() const;

        // The index of the buffer.
        std::shared_ptr<SGlTFId> buffer;
        // The offset into the buffer in bytes.
        int32_t byteOffset;
        // The length of the bufferView in bytes.
        int32_t byteLength;
        // The stride, in bytes.
        int32_t byteStride;
        // The hint representing the intended GPU buffer type to use with this buffer view.
        int32_t target;
    };

    /*!
     * struct: SCameraOrthographic
     * An orthographic camera containing properties to create an orthographic projection matrix.
     */
    struct SCameraOrthographic : SGlTFProperty
    {
        SCameraOrthographic();

        // Check valid
        operator bool() const;

        // The floating-point horizontal magnification of the view. This value **MUST NOT** be equal to zero. This value **SHOULD NOT** be negative.
        float xmag;
        // The floating-point vertical magnification of the view. This value **MUST NOT** be equal to zero. This value **SHOULD NOT** be negative.
        float ymag;
        // The floating-point distance to the far clipping plane. This value **MUST NOT** be equal to zero. `zfar` **MUST** be greater than `znear`.
        float zfar;
        // The floating-point distance to the near clipping plane.
        float znear;
    };

    /*!
     * struct: SCameraPerspective
     * A perspective camera containing properties to create a perspective projection matrix.
     */
    struct SCameraPerspective : SGlTFProperty
    {
        SCameraPerspective();

        // Check valid
        operator bool() const;

        // The floating-point aspect ratio of the field of view.
        float aspectRatio;
        // The floating-point vertical field of view in radians. This value **SHOULD** be less than π.
        float yfov;
        // The floating-point distance to the far clipping plane.
        float zfar;
        // The floating-point distance to the near clipping plane.
        float znear;
    };

    /*!
     * struct: SCamera
     * A camera's projection.  A node **MAY** reference a camera to apply a transform to place the camera in the scene.
     */
    struct SCamera : SGlTFChildofRootProperty
    {
        SCamera();

        // Check valid
        operator bool() const;

        // An orthographic camera containing properties to create an orthographic projection matrix. This property **MUST NOT** be defined when `perspective` is defined.
        std::shared_ptr<SCameraOrthographic> orthographic;
        // A perspective camera containing properties to create a perspective projection matrix. This property **MUST NOT** be defined when `orthographic` is defined.
        std::shared_ptr<SCameraPerspective> perspective;
        // Specifies if the camera uses a perspective or orthographic projection.
        std::string type;
    };

    /*!
     * struct: SExtension
     * JSON object with extension-specific objects.
     */
    struct SExtension : SObject
    {
        SExtension();

        // Check valid
        operator bool() const;

        // Manual code lines
        std::map<std::string, std::shared_ptr<struct SObject>> properties;
    };

    /*!
     * struct: SExtras
     * Application-specific data.
     */
    struct SExtras : SObject
    {
        SExtras();

        // Check valid
        operator bool() const;
    };

    /*!
     * struct: SGlTF
     * The root object for a glTF asset.
     */
    struct SGlTF : SGlTFProperty
    {
        SGlTF();

        // Check valid
        operator bool() const;

        // Names of glTF extensions used in this asset.
        std::vector<std::string> extensionsUsed;
        // Names of glTF extensions required to properly load this asset.
        std::vector<std::string> extensionsRequired;
        // An array of accessors.
        std::vector<std::shared_ptr<SAccessor>> accessors;
        // An array of keyframe animations.
        std::vector<std::shared_ptr<SAnimation>> animations;
        // Metadata about the glTF asset.
        std::shared_ptr<SAsset> asset;
        // An array of buffers.
        std::vector<std::shared_ptr<SBuffer>> buffers;
        // An array of bufferViews.
        std::vector<std::shared_ptr<SBufferView>> bufferViews;
        // An array of cameras.
        std::vector<std::shared_ptr<SCamera>> cameras;
        // An array of images.
        std::vector<std::shared_ptr<SImage>> images;
        // An array of materials.
        std::vector<std::shared_ptr<SMaterial>> materials;
        // An array of meshes.
        std::vector<std::shared_ptr<SMesh>> meshes;
        // An array of nodes.
        std::vector<std::shared_ptr<SNode>> nodes;
        // An array of samplers.
        std::vector<std::shared_ptr<SSampler>> samplers;
        // The index of the default scene.
        std::shared_ptr<SGlTFId> scene;
        // An array of scenes.
        std::vector<std::shared_ptr<SScene>> scenes;
        // An array of skins.
        std::vector<std::shared_ptr<SSkin>> skins;
        // An array of textures.
        std::vector<std::shared_ptr<STexture>> textures;
    };

    /*!
     * struct: SGlTFId
     */
    struct SGlTFId : SObject
    {
        SGlTFId();

        // Check valid
        operator bool() const;

        operator int32_t() const;

        int32_t int32_tValue;
    };

    /*!
     * struct: SImage
     * Image data used to create a texture. Image **MAY** be referenced by an URI (or IRI) or a buffer view index.
     */
    struct SImage : SGlTFChildofRootProperty
    {
        SImage();

        // Check valid
        operator bool() const;

        // The URI (or IRI) of the image.
        std::string uri;
        // The image's media type. This field **MUST** be defined when `bufferView` is defined.
        std::string mimeType;
        // The index of the bufferView that contains the image. This field **MUST NOT** be defined when `uri` is defined.
        std::shared_ptr<SGlTFId> bufferView;
    };

    /*!
     * struct: STextureInfo
     * Reference to a texture.
     */
    struct STextureInfo : SGlTFProperty
    {
        STextureInfo();

        // Check valid
        operator bool() const;

        // The index of the texture.
        std::shared_ptr<SGlTFId> index;
        // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
        int32_t texCoord;
    };

    /*!
     * struct: SMaterialNormalTextureInfo
     */
    struct SMaterialNormalTextureInfo : STextureInfo
    {
        SMaterialNormalTextureInfo();

        // Check valid
        operator bool() const;

        // The scalar parameter applied to each normal vector of the normal texture.
        float scale;
    };

    /*!
     * struct: SMaterialOcclusionTextureInfo
     */
    struct SMaterialOcclusionTextureInfo : STextureInfo
    {
        SMaterialOcclusionTextureInfo();

        // Check valid
        operator bool() const;

        // A scalar multiplier controlling the amount of occlusion applied.
        float strength;
    };

    /*!
     * struct: SMaterialPBRMetallicRoughness
     * A set of parameter values that are used to define the metallic-roughness material model from Physically-Based Rendering (PBR) methodology.
     */
    struct SMaterialPBRMetallicRoughness : SGlTFProperty
    {
        SMaterialPBRMetallicRoughness();

        // Check valid
        operator bool() const;

        // The factors for the base color of the material.
        std::vector<float> baseColorFactor;
        // The base color texture.
        std::shared_ptr<STextureInfo> baseColorTexture;
        // The factor for the metalness of the material.
        float metallicFactor;
        // The factor for the roughness of the material.
        float roughnessFactor;
        // The metallic-roughness texture.
        std::shared_ptr<STextureInfo> metallicRoughnessTexture;
    };

    /*!
     * struct: SMaterial
     * The material appearance of a primitive.
     */
    struct SMaterial : SGlTFChildofRootProperty
    {
        SMaterial();

        // Check valid
        operator bool() const;

        // A set of parameter values that are used to define the metallic-roughness material model from Physically Based Rendering (PBR) methodology. When undefined, all the default values of `pbrMetallicRoughness` **MUST** apply.
        std::shared_ptr<SMaterialPBRMetallicRoughness> pbrMetallicRoughness;
        // The tangent space normal texture.
        std::shared_ptr<SMaterialNormalTextureInfo> normalTexture;
        // The occlusion texture.
        std::shared_ptr<SMaterialOcclusionTextureInfo> occlusionTexture;
        // The emissive texture.
        std::shared_ptr<STextureInfo> emissiveTexture;
        // The factors for the emissive color of the material.
        std::vector<float> emissiveFactor;
        // The alpha rendering mode of the material.
        std::string alphaMode;
        // The alpha cutoff value of the material.
        float alphaCutoff;
        // Specifies whether the material is double sided.
        bool doubleSided;
    };

    /*!
     * struct: SMeshPrimitive
     * Geometry to be rendered with the given material.
     */
    struct SMeshPrimitive : SGlTFProperty
    {
        SMeshPrimitive();

        // Check valid
        operator bool() const;

        // A plain JSON object, where each key corresponds to a mesh attribute semantic and each value is the index of the accessor containing attribute's data.
        std::map<std::string, std::shared_ptr<SGlTFId>> attributes;
        // The index of the accessor that contains the vertex indices.
        std::shared_ptr<SGlTFId> indices;
        // The index of the material to apply to this primitive when rendering.
        std::shared_ptr<SGlTFId> material;
        // The topology type of primitives to render.
        int32_t mode;
        // An array of morph targets.
        std::vector<std::map<std::string, std::shared_ptr<SGlTFId>>> targets;
    };

    /*!
     * struct: SMesh
     * A set of primitives to be rendered.  Its global transform is defined by a node that references it.
     */
    struct SMesh : SGlTFChildofRootProperty
    {
        SMesh();

        // Check valid
        operator bool() const;

        // An array of primitives, each defining geometry to be rendered.
        std::vector<std::shared_ptr<SMeshPrimitive>> primitives;
        // Array of weights to be applied to the morph targets. The number of array elements **MUST** match the number of morph targets.
        std::vector<float> weights;
    };

    /*!
     * struct: SNode
     * A node in the node hierarchy.  When the node contains `skin`, all `mesh.primitives` **MUST** contain `JOINTS_0` and `WEIGHTS_0` attributes.  A node **MAY** have either a `matrix` or any combination of `translation`/`rotation`/`scale` (TRS) properties. TRS properties are converted to matrices and postmultiplied in the `T * R * S` order to compose the transformation matrix; first the scale is applied to the vertices, then the rotation, and then the translation. If none are provided, the transform is the identity. When a node is targeted for animation (referenced by an animation.channel.target), `matrix` **MUST NOT** be present.
     */
    struct SNode : SGlTFChildofRootProperty
    {
        SNode();

        // Check valid
        operator bool() const;

        // The index of the camera referenced by this node.
        std::shared_ptr<SGlTFId> camera;
        // The indices of this node's children.
        std::vector<std::shared_ptr<SGlTFId>> children;
        // The index of the skin referenced by this node.
        std::shared_ptr<SGlTFId> skin;
        // A floating-point 4x4 transformation matrix stored in column-major order.
        std::vector<float> matrix;
        // The index of the mesh in this node.
        std::shared_ptr<SGlTFId> mesh;
        // The node's unit quaternion rotation in the order (x, y, z, w), where w is the scalar.
        std::vector<float> rotation;
        // The node's non-uniform scale, given as the scaling factors along the x, y, and z axes.
        std::vector<float> scale;
        // The node's translation along the x, y, and z axes.
        std::vector<float> translation;
        // The weights of the instantiated morph target. The number of array elements **MUST** match the number of morph targets of the referenced mesh. When defined, `mesh` **MUST** also be defined.
        std::vector<float> weights;
    };

    /*!
     * struct: SSampler
     * Texture sampler properties for filtering and wrapping modes.
     */
    struct SSampler : SGlTFChildofRootProperty
    {
        SSampler();

        // Check valid
        operator bool() const;

        // Magnification filter.
        int32_t magFilter;
        // Minification filter.
        int32_t minFilter;
        // S (U) wrapping mode.
        int32_t wrapS;
        // T (V) wrapping mode.
        int32_t wrapT;
    };

    /*!
     * struct: SScene
     * The root nodes of a scene.
     */
    struct SScene : SGlTFChildofRootProperty
    {
        SScene();

        // Check valid
        operator bool() const;

        // The indices of each root node.
        std::vector<std::shared_ptr<SGlTFId>> nodes;
    };

    /*!
     * struct: SSkin
     * Joints and matrices defining a skin.
     */
    struct SSkin : SGlTFChildofRootProperty
    {
        SSkin();

        // Check valid
        operator bool() const;

        // The index of the accessor containing the floating-point 4x4 inverse-bind matrices.
        std::shared_ptr<SGlTFId> inverseBindMatrices;
        // The index of the node used as a skeleton root.
        std::shared_ptr<SGlTFId> skeleton;
        // Indices of skeleton nodes, used as joints in this skin.
        std::vector<std::shared_ptr<SGlTFId>> joints;
    };

    /*!
     * struct: STexture
     * A texture and its sampler.
     */
    struct STexture : SGlTFChildofRootProperty
    {
        STexture();

        // Check valid
        operator bool() const;

        // The index of the sampler used by this texture. When undefined, a sampler with repeat wrapping and auto filtering **SHOULD** be used.
        std::shared_ptr<SGlTFId> sampler;
        // The index of the image used by this texture. When undefined, an extension or other mechanism **SHOULD** supply an alternate texture source, otherwise behavior is undefined.
        std::shared_ptr<SGlTFId> source;
    };

    /*!
     * struct: SKHR_draco_mesh_compressionextension
     */
    struct SKHR_draco_mesh_compressionextension : SGlTFProperty
    {
        SKHR_draco_mesh_compressionextension();

        // Check valid
        operator bool() const;

        // The index of the bufferView.
        std::shared_ptr<SGlTFId> bufferView;
        // A dictionary object, where each key corresponds to an attribute and its unique attribute id stored in the compressed geometry.
        std::map<std::string, std::shared_ptr<SGlTFId>> attributes;
    };

    /*!
     * struct: SKHR_lights_punctualglTFextension
     */
    struct SKHR_lights_punctualglTFextension : SGlTFProperty
    {
        SKHR_lights_punctualglTFextension();

        // Check valid
        operator bool() const;

        std::vector<std::shared_ptr<SLight>> lights;
    };

    /*!
     * struct: SLight
     * A directional, point, or spot light.
     */
    struct SLight : SGlTFChildofRootProperty
    {
        SLight();

        // Check valid
        operator bool() const;

        // Color of the light source.
        std::vector<float> color;
        // Intensity of the light source. `point` and `spot` lights use luminous intensity in candela (lm/sr) while `directional` lights use illuminance in lux (lm/m^2)
        float intensity;
        std::shared_ptr<SLightspot> spot;
        // Specifies the light type.
        std::string type;
        // A distance cutoff at which the light's intensity may be considered to have reached zero.
        float range;
    };

    /*!
     * struct: SLightspot
     */
    struct SLightspot : SGlTFProperty
    {
        SLightspot();

        // Check valid
        operator bool() const;

        // Angle in radians from centre of spotlight where falloff begins.
        float innerConeAngle;
        // Angle in radians from centre of spotlight where falloff ends.
        float outerConeAngle;
    };

    /*!
     * struct: SKHR_lights_punctualnodeextension
     */
    struct SKHR_lights_punctualnodeextension : SGlTFProperty
    {
        SKHR_lights_punctualnodeextension();

        // Check valid
        operator bool() const;

        // The id of the light referenced by this node.
        std::shared_ptr<SGlTFId> light;
    };

    /*!
     * struct: SKHR_materials_clearcoatglTFextension
     * glTF extension that defines the clearcoat material layer.
     */
    struct SKHR_materials_clearcoatglTFextension : SGlTFProperty
    {
        SKHR_materials_clearcoatglTFextension();

        // Check valid
        operator bool() const;

        // The clearcoat layer intensity.
        float clearcoatFactor;
        // The clearcoat layer intensity texture.
        std::shared_ptr<STextureInfo> clearcoatTexture;
        // The clearcoat layer roughness.
        float clearcoatRoughnessFactor;
        // The clearcoat layer roughness texture.
        std::shared_ptr<STextureInfo> clearcoatRoughnessTexture;
        // The clearcoat normal map texture.
        std::shared_ptr<SMaterialNormalTextureInfo> clearcoatNormalTexture;
    };

    /*!
     * struct: SKHR_materials_iorglTFextension
     * glTF extension that defines the index of refraction of a material.
     */
    struct SKHR_materials_iorglTFextension : SGlTFProperty
    {
        SKHR_materials_iorglTFextension();

        // Check valid
        operator bool() const;

        // The index of refraction.
        float ior;
    };

    /*!
     * struct: SKHR_materials_sheenglTFextension
     * glTF extension that defines the sheen material model.
     */
    struct SKHR_materials_sheenglTFextension : SGlTFProperty
    {
        SKHR_materials_sheenglTFextension();

        // Check valid
        operator bool() const;

        // Color of the sheen layer (in linear space).
        std::vector<float> sheenColorFactor;
        // The sheen color (RGB) texture.
        std::shared_ptr<STextureInfo> sheenColorTexture;
        // The sheen layer roughness.
        float sheenRoughnessFactor;
        // The sheen roughness (Alpha) texture.
        std::shared_ptr<STextureInfo> sheenRoughnessTexture;
    };

    /*!
     * struct: SKHR_materials_specularglTFextension
     * glTF extension that defines the strength of the specular reflection.
     */
    struct SKHR_materials_specularglTFextension : SGlTFProperty
    {
        SKHR_materials_specularglTFextension();

        // Check valid
        operator bool() const;

        // The strength of the specular reflection.
        float specularFactor;
        // A texture that defines the specular factor in the alpha channel.
        std::shared_ptr<STextureInfo> specularTexture;
        // The F0 RGB color of the specular reflection.
        std::vector<float> specularColorFactor;
        // A texture that defines the F0 color of the specular reflection.
        std::shared_ptr<STextureInfo> specularColorTexture;
    };

    /*!
     * struct: SKHR_materials_transmissionglTFextension
     * glTF extension that defines the optical transmission of a material.
     */
    struct SKHR_materials_transmissionglTFextension : SGlTFProperty
    {
        SKHR_materials_transmissionglTFextension();

        // Check valid
        operator bool() const;

        // The base percentage of light transmitted through the surface.
        float transmissionFactor;
        // A texture that defines the transmission percentage of the surface, sampled from the R channel. These values are linear, and will be multiplied by transmissionFactor.
        std::shared_ptr<STextureInfo> transmissionTexture;
    };

    /*!
     * struct: SKHR_materials_unlitglTFextension
     * glTF extension that defines the unlit material model.
     */
    struct SKHR_materials_unlitglTFextension : SGlTFProperty
    {
        SKHR_materials_unlitglTFextension();

        // Check valid
        operator bool() const;
    };

    /*!
     * struct: SKHR_materials_variantsglTFextension
     * glTF extension that defines a material variations for mesh primivites
     */
    struct SKHR_materials_variantsglTFextension : SGlTFProperty
    {
        SKHR_materials_variantsglTFextension();

        // Check valid
        operator bool() const;

        std::vector<std::shared_ptr<SGlTFChildofRootProperty>> variants;
    };

    /*!
     * struct: SKHR_materials_variantsmeshprimitiveextension
     */
    struct SKHR_materials_variantsmeshprimitiveextension : SGlTFProperty
    {
        SKHR_materials_variantsmeshprimitiveextension();

        // Check valid
        operator bool() const;

        // A list of material to variant mappings
        std::vector<std::shared_ptr<SGlTFProperty>> mappings;
    };

    /*!
     * struct: SKHR_materials_volumeglTFextension
     * glTF extension that defines the parameters for the volume of a material.
     */
    struct SKHR_materials_volumeglTFextension : SGlTFProperty
    {
        SKHR_materials_volumeglTFextension();

        // Check valid
        operator bool() const;

        // Thickness of the volume.
        float thicknessFactor;
        // Texture that defines the thickness of the volume, stored in the G channel.
        std::shared_ptr<STextureInfo> thicknessTexture;
        // Average distance that light travels in the medium before interacting with a particle.
        float attenuationDistance;
        // Color that white light turns into due to absorption when reaching the attenuation distance.
        std::vector<float> attenuationColor;
    };

    /*!
     * struct: SKHR_texture_transformtextureInfoextension
     * glTF extension that enables shifting and scaling UV coordinates on a per-texture basis
     */
    struct SKHR_texture_transformtextureInfoextension : SGlTFProperty
    {
        SKHR_texture_transformtextureInfoextension();

        // Check valid
        operator bool() const;

        // The offset of the UV coordinate origin as a factor of the texture dimensions.
        std::vector<float> offset;
        // Rotate the UVs by this many radians counter-clockwise around the origin.
        float rotation;
        // The scale factor applied to the components of the UV coordinates.
        std::vector<float> scale;
        // Overrides the textureInfo texCoord value if supplied, and if this extension is supported.
        int32_t texCoord;
    };

    /*!
     * struct: SADOBE_materials_thin_transparencyglTFextension
     * glTF extension that defines properties to model physically plausible optical transparency.
     */
    struct SADOBE_materials_thin_transparencyglTFextension : SGlTFProperty
    {
        SADOBE_materials_thin_transparencyglTFextension();

        // Check valid
        operator bool() const;

        // The base percentage of light transmitted through the surface.
        float transmissionFactor;
        // A greyscale texture that defines the transmission percentage of the surface. This will be multiplied by transmissionFactor.
        std::shared_ptr<STextureInfo> transmissionTexture;
        // The index of refraction of the material.
        float ior;
    };

    /*!
     * struct: SArticulation
     * A model articulation definition.
     */
    struct SArticulation : SGlTFProperty
    {
        SArticulation();

        // Check valid
        operator bool() const;

        // The name of this articulation.  The articulation name must be unique within this model.  Articulation names may not contain spaces.
        std::string name;
        // An array of stages, each of which defines a degree of freedom of movement.
        std::vector<std::shared_ptr<SArticulationStage>> stages;
        // The local forward vector for the associated node, for the purpose of pointing at a target or other object.
        std::vector<float> pointingVector;
    };

    /*!
     * struct: SArticulationStage
     * One stage of a model articulation definition.
     */
    struct SArticulationStage : SGlTFProperty
    {
        SArticulationStage();

        // Check valid
        operator bool() const;

        // The name of this articulation stage.  The articulation stage name must be unique only within the containing articulation.  Articulation Stage names may not contain spaces.
        std::string name;
        // The type of motion applied by this articulation stage.
        std::string type;
        // The minimum value for the range of motion of this articulation stage.
        float minimumValue;
        // The maximum value for the range of motion of this articulation stage.
        float maximumValue;
        // The initial value for this articulation stage.
        float initialValue;
    };

    /*!
     * struct: SAGI_articulationsglTFextension
     * glTF Extension that defines metadata for applying external analysis or effects to a model.
     */
    struct SAGI_articulationsglTFextension : SGlTFProperty
    {
        SAGI_articulationsglTFextension();

        // Check valid
        operator bool() const;

        // An array of articulations.
        std::vector<std::shared_ptr<SArticulation>> articulations;
    };

    /*!
     * struct: SAGI_articulationsglTFNodeextension
     * glTF Extension for an individual node in a glTF model, to associate it with the model's root AGI_articulations object.
     */
    struct SAGI_articulationsglTFNodeextension : SGlTFProperty
    {
        SAGI_articulationsglTFNodeextension();

        // Check valid
        operator bool() const;

        // Set to true to indicate that this node's origin and orientation act as an attach point for external objects, analysis, or effects.
        bool isAttachPoint;
        // The name of an Articulation that applies to this node.
        std::string articulationName;
    };

    /*!
     * struct: SAGI_stk_metadataglTFextension
     * glTF Extension that defines metadata for use with STK (Systems Tool Kit).
     */
    struct SAGI_stk_metadataglTFextension : SGlTFProperty
    {
        SAGI_stk_metadataglTFextension();

        // Check valid
        operator bool() const;

        // An array of solar panel groups.
        std::vector<std::shared_ptr<SSolarPanelGroup>> solarPanelGroups;
    };

    /*!
     * struct: SAGI_stk_metadataglTFNodeextension
     * glTF Extension for an individual node in a glTF model, to associate it with the model's root AGI_stk_metadata object.
     */
    struct SAGI_stk_metadataglTFNodeextension : SGlTFProperty
    {
        SAGI_stk_metadataglTFNodeextension();

        // Check valid
        operator bool() const;

        // The name of a Solar Panel Group that includes this node.
        std::string solarPanelGroupName;
        // Set to true to indicate that this node's geometry does not obscure any sensors' view in the STK Sensor Obscuration tool.
        bool noObscuration;
    };

    /*!
     * struct: SSolarPanelGroup
     * A solar panel group definition.
     */
    struct SSolarPanelGroup : SGlTFProperty
    {
        SSolarPanelGroup();

        // Check valid
        operator bool() const;

        // The name of this solar panel group.  The group name must be unique within this model, and may not contain spaces.
        std::string name;
        // The percentage, from 0.0 to 100.0, of how efficiently the solar cells convert solar to electrical energy.
        float efficiency;
    };

    /*!
     * struct: SCESIUM_primitive_outlineglTFprimitiveextension
     * glTF extension for indicating that some edges of a primitive's triangles should be outlined.
     */
    struct SCESIUM_primitive_outlineglTFprimitiveextension : SGlTFProperty
    {
        SCESIUM_primitive_outlineglTFprimitiveextension();

        // Check valid
        operator bool() const;

        // The index of the accessor providing the list of highlighted lines at the edge of this primitive's triangles.
        int32_t indices;
    };

    /*!
     * struct: SEXT_mesh_gpu_instancingglTFextension
     * glTF extension defines instance attributes for a node with a mesh.
     */
    struct SEXT_mesh_gpu_instancingglTFextension : SGlTFProperty
    {
        SEXT_mesh_gpu_instancingglTFextension();

        // Check valid
        operator bool() const;

        // A dictionary object, where each key corresponds to instance attribute and each value is the index of the accessor containing attribute's data. Attributes TRANSLATION, ROTATION, SCALE define instance transformation. For "TRANSLATION" the values are FLOAT_VEC3's specifying translation along the x, y, and z axes. For "ROTATION" the values are VEC4's specifying rotation as a quaternion in the order (x, y, z, w), where w is the scalar, with component type `FLOAT` or normalized integer. For "SCALE" the values are FLOAT_VEC3's specifying scaling factors along the x, y, and z axes.
        std::map<std::string, std::shared_ptr<SGlTFId>> attributes;
    };

    /*!
     * struct: SEXT_texture_webpglTFextension
     * glTF extension to specify textures using the WebP image format.
     */
    struct SEXT_texture_webpglTFextension : SGlTFProperty
    {
        SEXT_texture_webpglTFextension();

        // Check valid
        operator bool() const;

        // The index of the images node which points to a WebP image.
        std::shared_ptr<SGlTFId> source;
    };

    /*!
     * struct: SMSFT_lodglTFextension
     * glTF extension for specifying levels of detail (LOD).
     */
    struct SMSFT_lodglTFextension : SGlTFProperty
    {
        SMSFT_lodglTFextension();

        // Check valid
        operator bool() const;

        // Array containing the indices of progressively lower LOD nodes.
        std::vector<int32_t> ids;
    };

    /*!
     * struct: SMSFT_texture_ddsextension
     * glTF extension to specify textures using the DirectDraw Surface file format (DDS).
     */
    struct SMSFT_texture_ddsextension : SGlTFProperty
    {
        SMSFT_texture_ddsextension();

        // Check valid
        operator bool() const;

        // The index of the images node which points to a DDS texture file.
        std::shared_ptr<SGlTFId> source;
    };

    enum class EAccessorComponentType : uint32_t
    {
        NONE,
        BYTE,
        UNSIGNED_BYTE,
        SHORT,
        UNSIGNED_SHORT,
        INT,
        UNSIGNED_INT,
        FLOAT,
        MAX,
    };
    
    template<typename TType>
    class TComponentData
    {
    public:
        typedef TType value_type;

    public:
        operator EAccessorComponentType() const
        {
            return EAccessorComponentType::NONE;
        }
    };

#define LIBGLTF_ACCESSORCOMPONENTDATA(TType, EType)\
    template<>\
    class TComponentData<TType>\
    {\
    public:\
        operator EAccessorComponentType() const\
        {\
            return EType;\
        }\
        template<typename TTypeAnother>\
        bool operator==(TComponentData<TTypeAnother> _Another) const\
        {\
            return (EAccessorComponentType(_Another) == EType);\
        }\
    }
    LIBGLTF_ACCESSORCOMPONENTDATA(int8_t    , EAccessorComponentType::BYTE          );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint8_t   , EAccessorComponentType::UNSIGNED_BYTE );
    LIBGLTF_ACCESSORCOMPONENTDATA(int16_t   , EAccessorComponentType::SHORT         );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint16_t  , EAccessorComponentType::UNSIGNED_SHORT);
    LIBGLTF_ACCESSORCOMPONENTDATA(int32_t   , EAccessorComponentType::INT           );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint32_t  , EAccessorComponentType::UNSIGNED_INT  );
    LIBGLTF_ACCESSORCOMPONENTDATA(float     , EAccessorComponentType::FLOAT         );

    struct SAccessorComponentType
    {
        size_t value;
        size_t size;
    };

    const SAccessorComponentType GSAccessorComponentTypes[uint32_t(EAccessorComponentType::MAX)] = {
        SAccessorComponentType{ 0   , 0                },
        SAccessorComponentType{ 5120, sizeof(int8_t)   },
        SAccessorComponentType{ 5121, sizeof(uint8_t)  },
        SAccessorComponentType{ 5122, sizeof(int16_t)  },
        SAccessorComponentType{ 5123, sizeof(uint16_t) },
        SAccessorComponentType{ 5124, sizeof(int32_t)  },
        SAccessorComponentType{ 5125, sizeof(uint32_t) },
        SAccessorComponentType{ 5126, sizeof(float)    }
    };

    enum class EAccessorType : uint8_t
    {
        NONE,
        SCALAR,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
        MAX,
    };

    struct SAccessorType
    {
        std::string text;
        size_t dimension;
    };

    const SAccessorType GSAccessorTypes[uint8_t(EAccessorType::MAX)] = {
        SAccessorType{ ""         , 0  },
        SAccessorType{ "SCALAR"   , 1  },
        SAccessorType{ "VEC2"     , 2  },
        SAccessorType{ "VEC3"     , 3  },
        SAccessorType{ "VEC4"     , 4  },
        SAccessorType{ "MAT2"     , 4  },
        SAccessorType{ "MAT3"     , 9  },
        SAccessorType{ "MAT4"     , 16 }
    };

    int32_t AccessorComponentTypeToInt32(EAccessorComponentType _eType);
    EAccessorComponentType Int32ToAccessorComponentType(int32_t _iValue);
    const std::string& AccessorTypeToText(EAccessorType _eType);
    EAccessorType TextToAccessorType(const std::string& _eText, bool _bCaseCensitive = true);
    size_t SizeOfAccessorComponentType(EAccessorComponentType _eType);
    size_t DimensionOfAccessorType(EAccessorType _eType);
    size_t SizeOfAccessor(EAccessorComponentType _eAccessorComponentType, size_t _iCount, EAccessorType _eAccessorType);

    struct SBufferData
    {
        SBufferData();

        const uint8_t* buffer;
        size_t bufferSize;
        size_t bufferStride;
    };

#define LIBGLTF_ACCESSORCOMPONENT_CASE(TType, EType)\
    case EType: {\
        _Vector.Resize(count);\
        for (size_t i = 0; i < count; ++i)\
        {\
            for (size_t j = 0; j < dimension_of_accessor_type; ++j)\
            {\
                _Vector[i][j] = static_cast<typename TVector::TComponent>(*((TType*)bufferData.buffer + i * dimension_of_accessor_type + j));\
            }\
        }\
    } break

    /// help to operate the accessor data
    struct SAccessorData
    {
        SAccessorData();

        EAccessorComponentType componentType;
        size_t                 count;
        EAccessorType          type;
        size_t                 bufferStride;
        SBufferData            bufferData;

        template<typename TVector>
        bool operator>>(TVector& _Vector) const
        {
            const size_t dimension_of_accessor_type = DimensionOfAccessorType(type);
            // not allow to convert to another with the different dimension
            if (dimension_of_accessor_type != TVector::Dimension) return false;

            const size_t sizeof_data = SizeOfAccessor(componentType, 1, type);
            if (bufferStride != 0 && bufferStride != sizeof_data) return false;

            const size_t sizeof_accessor = sizeof_data * count;
            if (sizeof_accessor > bufferData.bufferSize) return false;

            if (count > 0)
            {
                EAccessorComponentType v_component_type = TComponentData<typename TVector::TComponent>();
                if (componentType == v_component_type)
                {
                    _Vector.Resize(count);
                    ::memcpy(_Vector.Data(), bufferData.buffer, sizeof_accessor);
                }
                else
                {
                    switch (componentType)
                    {
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int8_t   , EAccessorComponentType::BYTE          );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint8_t  , EAccessorComponentType::UNSIGNED_BYTE );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int16_t  , EAccessorComponentType::SHORT         );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint16_t , EAccessorComponentType::UNSIGNED_SHORT);
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int32_t  , EAccessorComponentType::INT           );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint32_t , EAccessorComponentType::UNSIGNED_INT  );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(float    , EAccessorComponentType::FLOAT         );

                    default:
                        // not support
                        return false;
                    }
                }
            }
            return true;
        }
    };

    class IAccessorStream
    {
    public:
        virtual bool operator<<(const SAccessorData& accessor_data) = 0;
    };

    /// a dimension vertex that supports to construct some vertex variables like vec2, vec3, vec4, etc
    template<size_t VDim, typename TType>
    class TVertex
    {
    public:
        static const size_t Dimension = VDim;

    public:
        const TType& operator[](size_t index) const
        {
            return m_aData[index];
        }

        TType& operator[](size_t index)
        {
            return m_aData[index];
        }

    private:
        TType m_aData[Dimension];
    };

    /// a vector, contains some vertex datas
    template<size_t VDim, typename TType>
    class TVertexList : public std::vector<TVertex<VDim, TType> >
    {
    public:
        typedef std::vector<TVertex<VDim, TType> > TSuper;
        typedef typename TSuper::value_type        TValue;
        typedef TType                              TComponent;

    public:
        static const size_t Dimension = TValue::Dimension;

    public:
        void Resize(size_t new_size)
        {
            TSuper::resize(new_size);
        }

        const TValue* Data() const
        {
            return TSuper::data();
        }

        TValue* Data()
        {
            return TSuper::data();
        }
    };

    /// help to pass the vector
    template<typename TVector>
    class TAccessorStream : public libgltf::IAccessorStream
    {
    public:
        explicit TAccessorStream(TVector& _Vector)
            : m_Vector(_Vector)
        {
            //
        }

    public:
        virtual bool operator<<(const libgltf::SAccessorData& accessor_data) override
        {
            return (accessor_data >> m_Vector);
        }

    private:
        TVector& m_Vector;
    };

    /// gltf loader
    class IglTFLoader
    {
    public:
        static std::shared_ptr<IglTFLoader> Create(std::function<std::shared_ptr<std::istream>(const std::string&)> _reader);

    public:
        /// get the glTF structure
        virtual const std::unique_ptr<SGlTF>& glTF() const = 0;

        /// load the indices data
        virtual bool LoadMeshPrimitiveIndicesData(size_t _mesh_index, size_t _primitive_index, std::shared_ptr<IAccessorStream> _accessor_stream) = 0;

        /// load the attribute data like position, normal, texcoord, etc
        virtual bool LoadMeshPrimitiveAttributeData(size_t _mesh_index, size_t _primitive_index, const std::string& _attribute, std::shared_ptr<IAccessorStream> _accessor_stream) = 0;

        /// load the image data and type
        virtual bool LoadImageData(size_t _index, std::vector<uint8_t>& _data, std::string& _type) = 0;
    };

}
#endif // LIBGLTF_H

