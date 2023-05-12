///////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// OrbitCamera.h
// Orbital camera class for OpenGL
// 
//////////////////////////////////////////////////////////////////////////////
// winGlCamera.h
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "animUtils.h"

const float RAD2DEG = 180.0f / 3.141593f;
const float EPSILON = 0.00001f;

const float CAM_DIST = 150.0f; // 400.0f; // 5.0f
const float FOV{ 60.0f };  // in degree

class Camera
{
public:
    std::string camName{};
    float Fov{ 0.0f };

    glm::vec3 initViewPos{};
    glm::vec3 initViewTarget{};
    glm::vec3 initEulers{};
    float initFov{};
    float initAngleToLookAt{};
    float angleToLookAt{};

    const glm::vec3 initDeltaPosition{ 0.0f, 0.0f, 0.0f }; // mainly for cam2 viewPos adjustor
    const glm::vec3 initDeltaTarget{ 0.0f, 0.0f, 0.0f }; // mainly for cam2 viewTarget adjustor
    const glm::vec3 initDeltaEulers{ 0.0f, 0.0f, 0.0f }; // mainly for cam2 viewTarget adjustor
    glm::vec3 deltaPosition{ initDeltaPosition }; // mainly for cam2 viewPos adjustor
    glm::vec3 deltaTarget{ initDeltaTarget };//  mainly for cam2 viewTarget adjustor
    glm::vec3 deltaEulers{ initDeltaEulers };// mainly for cam2 euler adjustor

    struct EulerAngle
    {
        float roll{ 0 };
        float pitch{ 0 };
        float yaw{ 0 };

        float rollRad() { return glm::radians(roll); };
        float pitchRad() { return glm::radians(pitch); };
        float yawRad() { return glm::radians(yaw); };
    };

    Camera(int idchild);

	~Camera();

    void update(float frameTime = 0);
    void lookAt(const glm::vec3& pos, const glm::vec3& tar, const glm::vec3& up = glm::vec3());
    void moveTo(const glm::vec3& to, float duration = 0, Gil::AnimationMode mode = Gil::EASE_OUT);
    void moveForward(float delta, float duration = 0, Gil::AnimationMode mode = Gil::EASE_OUT);
    void startForward(float maxSpeed = 1.0f, float accel = 1.0f);
    void stopForward();
    void shiftTo(const glm::vec3& to, float duration = 0, Gil::AnimationMode mode = Gil::EASE_OUT);
    void shift(const glm::vec2& delta, float duration = 0, Gil::AnimationMode mode = Gil::EASE_OUT);
    void startShift(const glm::vec2& shiftVector, float accel = 1.0f);
    void stopShift();
    void rotateTo(const glm::vec3& eulerangle, float duration = 0.0f, Gil::AnimationMode mode = Gil::EASE_OUT);
    void rotateTo(const glm::quat& q, float duration = 0.0f, Gil::AnimationMode mode = Gil::EASE_OUT);
    void rotate(const glm::vec3& delta, float duration = 0.0f, Gil::AnimationMode mode = Gil::EASE_OUT);
    void setViewPos(const glm::vec3& pos);
    void setViewTarget(const glm::vec3& tar);
    void setViewDistance(float d);
    void setRotation(const glm::vec3& eulerDeltaAngle);
    void setRotation(const glm::quat& quate);
    void setLastEulerRoll(int roll) { lastEuler.roll = roll * ANGLE_SCALE; };
    void setLastEulerPitch(int pitch) { lastEuler.pitch = pitch * ANGLE_SCALE; };
    void setLastEulerYaw(int yaw) { lastEuler.yaw = yaw * ANGLE_SCALE; };
    void setCurrentEulerRoll(int roll) { currentEuler.roll = roll * ANGLE_SCALE; };
    void setCurrentEulerPitch(int pitch) { currentEuler.pitch = pitch * ANGLE_SCALE; };
    void setCurrentEulerYaw(int yaw) { currentEuler.yaw = yaw * ANGLE_SCALE; };
    void resetDeltaCam2() { deltaPosition = initDeltaPosition; deltaTarget = initDeltaTarget; deltaEulers = initDeltaEulers; };
    void rotateEuler(EulerAngle deltaEuler);
    void rotateEulerRoll(int roll);
    void rotateEulerPitch(int pitch);
    void rotateEulerYaw(int yaw);
    void zoomCameraDelta(float delta, float x, float y);
    void reset();
    // getters & settres
    float getFov() { return Fov; }
    void  setFov(float fov) { Fov = fov; }
    void setIdChild(int id) { idChild = id; };

    std::string getCameName() { return camName; }
    const glm::vec3& getViewPos() const { return viewPos; }
    const glm::vec3& getViewTarget() const { return viewTarget; }
    const glm::vec3& getViewEuler() const { return eulers; }
    const glm::mat4& getViewMatrix() const { return viewMatrix; }
    const glm::mat4& getRotationMatrix() const { return viewRotationMatrix; }
    const float getViewDistance() const { return viewDistance; }
    const glm::quat& getViewQuaternion() const { return viewQuaternion; }
    glm::vec3 getLeftAxis() const;
    glm::vec3 getUpAxis() const;
    glm::vec3 getForwardAxis() const;



protected:

private:
    void updateMove(float frameTime);
    void updateShift(float frameTime);
    void updateForward(float frameTime);
    void updateTurn(float frameTime);

    // member variables
    glm::vec3 viewPos{ 0.0f };                   // camera viewPos at world space
    glm::vec3 viewTarget{ 0.0f };                     // camera focal(lookat) viewPos at world space
    float   viewDistance{ 0.0f };                   // viewDistance between viewPos and viewTarget
    glm::vec3 eulers{ 0.0f };
    glm::vec3 eulersRad{ 0.0f };                      // eulers in radian around the viewTarget (pitch, heading, roll)
    glm::mat4 viewMatrix{ 1.0f };                     // 4x4 viewMatrix combined rotation and translation
    glm::mat4 viewRotationMatrix{ 1.0f };             // rotation only
    //glm::mat<4, 4, float, glm::defaultp> const& rotationMatrix;             // rotation only
    const  glm::mat4& rotationMatrix{};             // rotation only
    glm::quat viewQuaternion{ 1.0f, 0.0f, 0.0f, 0.0f };         // viewQuaternion for rotations

    // for viewPos movement
    glm::vec3 movingFrom{ 0.0f };                 // camera starting viewPos
    glm::vec3 movingTo{ 0.0f };                   // camera destination viewPos 
    glm::vec3 movingVector{ 0.0f };               // normalized direction vector
    float   movingTime{ 0.0f };                 // animation elapsed time (sec)
    float   movingDuration{ 0.0f };             // animation duration (sec)
    bool    moving{ false };                     // flag to start/stop animation
    Gil::AnimationMode movingMode{};      // interpolation mode

    // for viewTarget movement (shift)
    glm::vec3 shiftingFrom{ 0.0f };               // camera starting viewTarget
    glm::vec3 shiftingTo{ 0.0f };                 // camera destination viewTarget
    glm::vec3 shiftingVector{ 0.0f };             // normalized direction vector
    float   shiftingTime{ 0.0f };               // animation elapsed time (sec)
    float   shiftingDuration{ 0.0f };           // animation duration (sec)
    float   shiftingSpeed{ 0.0f };              // current velocity of shift vector
    float   shiftingAccel{ 0.0f };              // acceleration, units per second squared
    float   shiftingMaxSpeed{ 0.0f };           // max velocity of shift vector
    bool    shifting{ false };                   // flag to start/stop animation
    Gil::AnimationMode shiftingMode{};    // interpolation mode

    // for forwarding using viewDistance between viewPos and viewTarget
    float forwardingFrom{ 0.0f };               // starting viewDistance
    float forwardingTo{ 0.0f };                 // ending viewDistance
    float forwardingTime{ 0.0f };               // animation elapsed time (sec)
    float forwardingDuration{ 0.0f };           // animation duration (sec)
    float forwardingSpeed{ 0.0f };              // current velocity of moving forward
    float forwardingAccel{ 0.0f };              // acceleration, units per second squared
    float forwardingMaxSpeed{ 0.0f };           // max velocity of moving forward
    bool  forwarding{ false };                   // flag to start/stop forwarding
    Gil::AnimationMode forwardingMode{};  // interpolation mode

    // for rotation
    glm::vec3 turningAngleFrom{ 0.0f };           // starting angles
    glm::vec3 turningAngleTo{ 0.0f };             // ending angles
    glm::quat turningQuaternionFrom{};   // starting viewQuaternion
    glm::quat turningQuaternionTo{  };      // ending viewQuaternion
    float   turningTime{ 0.0f };                // animation elapsed time (sec)
    float   turningDuration{ 0.0f };            // animation duration (sec)
    bool    turning{ false };                    // flag to start/stop rotation
    bool    quaternionUsed{ false };             // flag to use viewQuaternion
    Gil::AnimationMode turningMode{ Gil::AnimationMode::LINEAR };     // interpolation mode

    // Euler Angle
    const float ANGLE_SCALE = 0.1f;
    EulerAngle currentEuler{};
    EulerAngle lastEuler{};

    int idChild;
};

inline Camera::Camera(int idchild) :
    movingTime(0), movingDuration(0), moving(false),
    shiftingTime(0), shiftingDuration(0), shiftingSpeed(0),
    shiftingAccel(0), shiftingMaxSpeed(0), shifting(false),
    forwardingTime(0), forwardingDuration(0), forwardingSpeed(0),
    forwardingAccel(0), forwardingMaxSpeed(0), forwarding(false),
    turningTime(0), turningDuration(0), turning(false),
    quaternionUsed(false)
{

    setIdChild(idchild);

    switch (idChild) // two child windows as 0 or 1 in this application
    {
    case 0: // look at the whole solar system
    {
        camName = "Cam" + std::to_string(idChild);
        initViewPos = glm::vec3(-100.0f, CAM_DIST * 0.05f, CAM_DIST * 0.3f);
        initViewTarget = glm::vec3{ 0.0f, 0.0f, 10.0f };
        initFov = FOV;
        angleToLookAt = initAngleToLookAt = 0.0f; // no use for this cam
        break;
    }
    case 1: //  look at a paticular planet or sattelite
    {
        camName = "Cam" + std::to_string(idChild);
        initViewPos = glm::vec3{ 0.0f, 0.0f, 30.0f };
        initViewTarget = glm::vec3{ 0.0f, 0.0f, 0.0f };
        initFov = FOV;
        angleToLookAt = initAngleToLookAt = 45.0f;//  in degree
        break;
    }
    }


    reset();

}

inline Camera::~Camera()
{
}


///////////////////////////////////////////////////////////////////////////////
// update each frame, frame time is sec
///////////////////////////////////////////////////////////////////////////////
inline void Camera::update(float frameTime)
{
    if (moving)
        updateMove(frameTime);
    if (shifting || shiftingSpeed != 0)
        updateShift(frameTime);
    if (forwarding || forwardingSpeed != 0)
        updateForward(frameTime);
    if (turning)
        updateTurn(frameTime);
}

///////////////////////////////////////////////////////////////////////////////
// set transform viewMatrix equivalent to gluLookAt()
// 1. Mt: Translate scene to camera viewPos inversely, (-x, -y, -z)
// 2. Mr: Rotate scene inversly so camera looks at the scene
// 3. Find viewMatrix = Mr * Mt
//       Mr               Mt
// |r0  r4  r8  0|   |1  0  0 -x|   |r0  r4  r8  r0*-x + r4*-y + r8 *-z|
// |r1  r5  r9  0| * |0  1  0 -y| = |r1  r5  r9  r1*-x + r5*-y + r9 *-z|
// |r2  r6  r10 0|   |0  0  1 -z|   |r2  r6  r10 r2*-x + r6*-y + r10*-z|
// |0   0   0   1|   |0  0  0  1|   |0   0   0   1                     |
///////////////////////////////////////////////////////////////////////////////
inline void Camera::lookAt(const glm::vec3& pos, const glm::vec3& tar, const glm::vec3& up )
{
    // remeber the camera posision & viewTarget viewPos
    viewPos = pos;
    viewTarget = tar;

    // if pos and viewTarget are same, only translate camera to viewPos without rotation
    if (pos == tar)
    {
        viewMatrix = glm::mat4{ 1.0f };
        viewMatrix = glm::translate(viewMatrix, -pos);
        viewRotationMatrix = glm::mat4{ 1.0f };
        eulers = glm::vec3{ 0.0f }; // degree
        eulersRad = glm::radians(eulers); // radian
        viewQuaternion = glm::quat{ 1.0f, eulersRad };
        return;
    }

    glm::vec3 left, Up, forward;  // 3 axis of viewMatrix for scene

    // first, compute the forward vector of rotation viewMatrix
    // NOTE: the direction is reversed (viewTarget to camera pos) because of camera transform
    forward = pos - tar;
    viewDistance = glm::length(forward);  // remember the viewDistance
    // normalize
    forward = glm::normalize(forward);

    // compute temporal up vector based on the forward vector
    // watch out when look up/down at 90 degree
    // for example, forward vector is on the Y axis
    if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
    {
        // forward vector is pointing +Y axis
        if (forward.y > 0)
        {
            Up = glm::vec3(0.0f, 0.0f, -1.0f);
        }
        // forward vector is pointing -Y axis
        else
        {
            Up = glm::vec3(0.0f, 0.0f, 1.0f);
        }
    }
    // in general, up vector is straight up
    else
    {
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // compute the left vector of rotation viewMatrix
    left = glm::cross(Up, forward);   // cross product
    left = glm::normalize(left);

    // re-calculate the orthonormal up vector
    Up = glm::cross(forward, left);   // cross product
    Up = glm::normalize(Up);

    if (up != glm::vec3())
    {
        Up = up;
    }

    viewMatrix = glm::lookAt(pos, tar, Up);  // recalculate camera viewMatrix
    viewRotationMatrix = glm::mat4(glm::mat3(viewMatrix)); // refer to https://stackoverflow.com/questions/10546320/remove-rotation-from-a-4x4-homogeneous-transformation-viewMatrix?msclkid=3f88e171bc5b11ec9338ecf94a5943f1

    // extract euler anlge and set viewQuaternion
    glm::extractEulerAngleXYZ(viewMatrix, eulersRad.x, eulersRad.y, eulersRad.z);  // angle in radian
    //eulers.y = -eulers.y;  // why revwerse here ???
    viewQuaternion = glm::quat{ eulersRad };
    eulers = glm::degrees(eulersRad); // radian to degree
}

// move the camera viewPos to the destination
// if duration(sec) is greater than 0, it will animate for the given duration
// otherwise, it will set the viewPos immediately
// use moveForward() to move the camera forward/backward
// NOTE: you must call update() before getting the delta movement per frame
///////////////////////////////////////////////////////////////////////////////
// move the camera viewPos with the given duration
///////////////////////////////////////////////////////////////////////////////
inline void Camera::moveTo(const glm::vec3& to, float duration, Gil::AnimationMode mode)
{
    if (duration <= 0.0f)
    {
        setViewPos(to);
    }
    else
    {
        movingFrom = viewPos;
        movingTo = to;
        movingVector = movingTo - movingFrom;
        movingVector = glm::normalize(movingVector);
        movingTime = 0;
        movingDuration = duration;
        movingMode = mode;
        moving = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
// zoom in/out the camera viewPos with the given delta movement and duration
// it actually moves the camera forward or backward.
// positive delta means moving forward (decreasing viewDistance)
///////////////////////////////////////////////////////////////////////////////
inline void Camera::moveForward(float delta, float duration, Gil::AnimationMode mode )
{
    if (duration <= 0.0f)
    {
        setViewDistance(viewDistance - delta);
    }
    else
    {
        forwardingFrom = viewDistance;
        forwardingTo = viewDistance - delta;
        forwardingTime = 0;
        forwardingDuration = duration;
        forwardingMode = mode;
        forwarding = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
   // start accelerating to move forward
   // It takes maximum speed per sec and acceleration per squared sec.
   // positive speed means moving forward (decreasing viewDistance).
   // acceleration should be always positive.
   ///////////////////////////////////////////////////////////////////////////////
inline void Camera::startForward(float maxSpeed, float accel)
{
    forwardingSpeed = 0;
    forwardingMaxSpeed = maxSpeed;
    forwardingAccel = accel;
    forwardingTime = 0;
    forwardingDuration = 0;
    forwarding = true;
}
inline void Camera::stopForward()
{
    forwarding = false;
};

///////////////////////////////////////////////////////////////////////////////
    // pan the camera viewTarget left/right/up/down with the given duration
    // the camera viewPos will be shifted after transform
inline void Camera::shiftTo(const glm::vec3& to, float duration, Gil::AnimationMode mode)
{
    if (duration <= 0.0f)
    {
        setViewTarget(to);
    }
    else
    {
        shiftingFrom = viewTarget;
        shiftingTo = to;
        shiftingVector = shiftingTo - shiftingFrom;
        //shiftingVector.normalize();
        shiftingVector = glm::normalize(shiftingVector);
        shiftingTime = 0;
        shiftingDuration = duration;
        shiftingMode = mode;
        shifting = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
// shift the camera viewPos and viewTarget left/right/up/down
///////////////////////////////////////////////////////////////////////////////
inline void Camera::shift(const glm::vec2& delta, float duration, Gil::AnimationMode mode)
{
    // get left & up vectors of camera
    glm::vec3 cameraLeft(-viewMatrix[0]);
    glm::vec3 cameraUp(-viewMatrix[1]);

    // compute delta movement
    glm::vec3 deltaMovement = delta.x * cameraLeft;
    deltaMovement += -delta.y * cameraUp;   // reverse up direction

    // find new viewTarget viewPos
    glm::vec3 newTarget = viewTarget + deltaMovement;

    shiftTo(newTarget, duration, mode);
}

///////////////////////////////////////////////////////////////////////////////
// start accelerating to shift camera
// It takes shift direction vector and acceleration per squared sec.
// acceleration should be always positive.
///////////////////////////////////////////////////////////////////////////////
inline void Camera::startShift(const glm::vec2& shiftVector, float accel)
{
    // get left & up vectors of camera
    glm::vec3 cameraLeft(-viewMatrix[0]);
    glm::vec3 cameraUp(-viewMatrix[1]);

    // compute new viewTarget vector
    glm::vec3 vector = shiftVector.x * cameraLeft;
    vector += -shiftVector.y * cameraUp;   // reverse up direction

    shiftingMaxSpeed = (float)shiftVector.length();
    shiftingVector = vector;
    shiftingVector = glm::normalize(shiftingVector);
    shiftingSpeed = 0;
    shiftingAccel = accel;
    shiftingTime = 0;
    shiftingDuration = 0;
    shifting = true;
}

inline void Camera::stopShift()
{
    shifting = false;
}

///////////////////////////////////////////////////////////////////////////////
// rotate camera to the given eulers with duration
///////////////////////////////////////////////////////////////////////////////
inline void Camera::rotateTo(const glm::vec3& eulerangle, float duration, Gil::AnimationMode mode)
{
    quaternionUsed = false;
    if (duration <= 0.0f)
    {
        setRotation(eulerangle);
    }
    else
    {
        turningAngleFrom = eulers;
        turningAngleTo = eulerangle;
        turningTime = 0;
        turningDuration = duration;
        turningMode = mode;
        turning = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
// rotate camera to the given viewQuaternion with duration
///////////////////////////////////////////////////////////////////////////////
inline void Camera::rotateTo(const glm::quat& q, float duration, Gil::AnimationMode mode)
{
    quaternionUsed = true;
    if (duration <= 0.0f)
    {
        setRotation(q);
    }
    else
    {
        turningQuaternionFrom = this->viewQuaternion;
        turningQuaternionTo = q;
        turningTime = 0;
        turningDuration = duration;
        turningMode = mode;
        turning = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
// rotate camera with delta eulers
// NOTE: delta eulers must be negated already
///////////////////////////////////////////////////////////////////////////////
inline void Camera::rotate(const glm::vec3& delta, float duration, Gil::AnimationMode mode)
{
    // delta in degree
    rotateTo(eulers + delta, duration, mode);
}

// setters
    ///////////////////////////////////////////////////////////////////////////////
    // set viewPos of camera, set transform viewMatrix as well
    // ///////////////////////////////////////////////////////////////////////////////
    // set transform viewMatrix equivalent to gluLookAt()
    // 1. Mt: Translate scene to camera viewPos inversely, (-x, -y, -z)
    // 2. Mr: Rotate scene inversly so camera looks at the scene
    // 3. Find viewMatrix = Mr * Mt
    //       Mr               Mt
    // |r0  r4  r8  0|   |1  0  0 -x|   |r0  r4  r8  r0*-x + r4*-y + r8 *-z|
    // |r1  r5  r9  0| * |0  1  0 -y| = |r1  r5  r9  r1*-x + r5*-y + r9 *-z|
    // |r2  r6  r10 0|   |0  0  1 -z|   |r2  r6  r10 r2*-x + r6*-y + r10*-z|
    // |0   0   0   1|   |0  0  0  1|   |0   0   0   1                     |
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
inline void Camera::setViewPos(const glm::vec3& pos)
{
    // update
    lookAt(pos, viewTarget);
}

///////////////////////////////////////////////////////////////////////////////
    // set viewTarget of camera, then rebuild viewMatrix
    // rotation parts are not changed, but translation part must be recalculated
    // And, viewPos is also shifted
    ///////////////////////////////////////////////////////////////////////////////
inline void Camera::setViewTarget(const glm::vec3& tar)
{
    // forward vector of camera
    glm::mat4 mat = glm::transpose(viewMatrix); // to world
    glm::vec3 forward(-mat[2]);//  mat[2] as z axis in world
    glm::vec3 pos = tar - (viewDistance * forward);

     // update
    lookAt(pos, tar);

}

///////////////////////////////////////////////////////////////////////////////
   // set viewDistance of camera, then recompute camera viewPos
   ///////////////////////////////////////////////////////////////////////////////
inline void Camera::setViewDistance(float d)
{
    viewDistance = d;

    // reconstruct camera viewMatrix
    //glm::mat4 mat = glm::transpose(viewMatrix); // to world
    glm::mat4 mat = glm::transpose(viewRotationMatrix); // to world
    glm::vec3 forward{ -mat[2] };//  mat[2] as z axis in world
    glm::vec3 up = glm::normalize(mat[1]); // as y axis
    viewPos = viewTarget - (viewDistance * forward);

    viewMatrix = glm::lookAt(viewPos, viewTarget, up);
    //lookAt(pos, viewTarget);

}

inline void Camera::setRotation(const glm::vec3& eulerDeltaAngle)
{
    // remember angles
    eulers += eulerDeltaAngle;  // degree
    eulersRad = glm::radians(eulers);  // degree

    // remember viewQuaternion value
    glm::quat qDeltaX = glm::angleAxis(glm::radians(eulerDeltaAngle).y, glm::vec3{ 1.0f, 0.0f, 0.0f });
    glm::mat4 mat4DeltaX = glm::toMat4(qDeltaX);
    glm::quat qDeltaY = glm::angleAxis(glm::radians(eulerDeltaAngle).x, glm::vec3{ 0.0f, 1.0f, 0.0f });
    glm::mat4 mat4DeltaY = glm::toMat4(qDeltaY);
    //viewRotationMatrix = mat4DeltaY * mat4DeltaX * viewRotationMatrix;

    glm::mat4 mat4Delta = glm::orientate4(glm::radians(eulerDeltaAngle));
    //glm::mat4 mat4Delta = glm::yawPitchRoll(glm::radians(eulerDeltaAngle).y, glm::radians(eulerDeltaAngle).y, 0.0f );
    viewRotationMatrix = mat4Delta * viewRotationMatrix;

    glm::quat quatDelta = qDeltaX * qDeltaY;
    viewQuaternion = quatDelta * viewQuaternion;
    // recompute rotation viewMatrix with  eulerDeltaAngle
    //viewRotationMatrix = glm::toMat4(quatDelta) * viewRotationMatrix;

    //viewRotationMatrix = glm::toMat4(viewQuaternion);
    //viewQuaternion = glm::
    // reconstruct camera viewMatrix
    glm::mat4 mat = glm::transpose(viewRotationMatrix);
    glm::vec3 forward = glm::normalize(-mat[2]);//  mat[2] as z axis
    glm::vec3 up = glm::normalize(mat[1]); // as y axis
    viewTarget = viewPos + (viewDistance * forward);
    viewMatrix = glm::lookAt(viewPos, viewTarget, up);

    // Win::log("Camera SetRotationNew: %s, viewPos=(%f, %f, %f), eulers=(%f, %f, %f)", camName.c_str(), viewPos.x, viewPos.y, viewPos.z, eulers.x, eulers.y, eulers.z);

}

//////////////////////////////////////////////////////////////////////////////
// set rotation with new viewQuaternion
// NOTE: viewQuaternion value is for viewMatrix, so matrixToAngle() will reverse yaw.
///////////////////////////////////////////////////////////////////////////////
inline void Camera::setRotation(const glm::quat& quate)
{
    // remember the current viewQuaternion
    viewQuaternion = quate;

    eulersRad = glm::eulerAngles(viewQuaternion);
    eulers = glm::degrees(eulersRad);
    setRotation(eulers);

}

inline void Camera::rotateEuler(EulerAngle deltaEuler)
{
    glm::mat4 mat{ 1.0f };
    glm::vec3 forward{}, up{}, right{};
    glm::quat quatRoll{}, quatPitch{}, quatYaw{};
    // reconstruct camera frame axis
    mat = glm::transpose(viewRotationMatrix);
    forward = glm::normalize(-mat[2]);//  mat[2] as z axis
    up = glm::normalize(mat[1]); // as y axis
    right = glm::normalize(glm::cross(up, forward)); // as x axis

    quatRoll = glm::angleAxis(deltaEuler.rollRad(), forward);
    quatPitch = glm::angleAxis(deltaEuler.pitchRad(), right);
    quatYaw = glm::angleAxis(deltaEuler.yawRad(), up);

    glm::mat4 mat4Delta = glm::toMat4(quatYaw * quatPitch * quatRoll);

    // rotation matrix
    viewRotationMatrix = mat4Delta * viewRotationMatrix;
    // quternion
    viewQuaternion = quatYaw * quatPitch * quatRoll * viewQuaternion;
    // reconstruct camera viewMatrix
    mat = glm::transpose(viewRotationMatrix);
    forward = glm::normalize(-mat[2]);//  mat[2] as z axis
    up = glm::normalize(mat[1]); // as y axis
    viewTarget = viewPos + (viewDistance * forward);
    viewMatrix = glm::lookAt(viewPos, viewTarget, up);
}

inline void Camera::rotateEulerRoll(int roll)
{
    // remember angles
    setCurrentEulerRoll(roll);

    EulerAngle deltaEuler = { currentEuler.roll - lastEuler.roll ,0.0f, 0.0f };

    setLastEulerRoll(roll);

    rotateEuler(deltaEuler);

}

inline void Camera::rotateEulerPitch(int pitch)
{
    // remember angles
    setCurrentEulerPitch(pitch);

    EulerAngle deltaEuler = { 0.0f , currentEuler.pitch - lastEuler.pitch , 0.0f };

    setLastEulerPitch(pitch);


    rotateEuler(deltaEuler);

}

inline void Camera::rotateEulerYaw(int yaw)
{
    // remember angles
    setCurrentEulerYaw(yaw);

    EulerAngle deltaEuler = { 0.0f , 0.0f , currentEuler.yaw - lastEuler.yaw };

    setLastEulerYaw(yaw);

    rotateEuler(deltaEuler);

}

inline void Camera::zoomCameraDelta(float delta, float x, float y)
{

    float ZOOM_SCALE = 5.0f;// 0.1f;
    float MIN_DIST = 1.0f;
    float distance = getViewDistance();
    distance -= (delta / WHEEL_DELTA) * ZOOM_SCALE;

    //distance = glm::max(distance, MIN_DIST); // error
    distance = glm::max(distance, MIN_DIST);

    setViewDistance(distance);

}

//////////////////////////////////////////////////////////////////////////////
// reset / back to initial values for viewPos, viewTarget, fov
///////////////////////////////////////////////////////////////////////////////
inline void Camera::reset()
{
    // reset viewPos and viewTarget
    //lookAt(initViewPos, initViewTarget);
    viewPos = initViewPos;
    viewTarget = initViewTarget;
    Fov = initFov;
    angleToLookAt = initAngleToLookAt;
    glm::vec3 left, up, forward;  // 3 axis of viewMatrix for scene
    forward = glm::normalize(viewPos - viewTarget);
    left = glm::cross(up, forward);
    up = glm::vec3{ 0.0f, 1.0f, 0.0f };
    viewDistance = glm::length(viewPos - viewTarget);
    currentEuler = lastEuler = EulerAngle{ 0.0f, 0.0f, 0.0f };
    viewMatrix = glm::lookAt(viewPos, viewTarget, up);
    //viewRotationMatrix = glm::mat4{ 1.0f };
    viewRotationMatrix = glm::mat4(glm::mat3(viewMatrix)); // refer to https://stackoverflow.com/questions/10546320/remove-rotation-from-a-4x4-homogeneous-transformation-matrix?msclkid=3f88e171bc5b11ec9338ecf94a5943f1
    viewQuaternion = glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f };

    if (idChild == 1)
    {
        deltaPosition = initDeltaPosition; 
        deltaTarget = initDeltaTarget; 
        deltaEulers = initDeltaEulers;
    }

    // fov
    /*Fov = initFov;
    angleToLookAt = initAngleToLookAt;*/
    //resetDeltaCam2();  // for point of camera
};

///////////////////////////////////////////////////////////////////////////////
// return left, up, forward axis
///////////////////////////////////////////////////////////////////////////////
inline glm::vec3 Camera::getLeftAxis() const
{
    //return glm::vec3(-viewMatrix[0]);// , -viewMatrix[4], -viewMatrix[8]);
    glm::mat4 mat = glm::transpose(viewMatrix);
    glm::vec3 left{ -mat[0] }; // as x axis
    return left;
}

inline glm::vec3 Camera::getUpAxis() const
{
    // return glm::vec3(viewMatrix[1]);// , viewMatrix[5], viewMatrix[9]);
    glm::mat4 mat = glm::transpose(viewMatrix);
    glm::vec3 up{ mat[1] }; // as y axis
    return up;
}

inline glm::vec3 Camera::getForwardAxis() const
{

    glm::mat4 mat = glm::transpose(viewMatrix);
    glm::vec3 forward{ -mat[2] }; // as x axis
    return forward;
}

///////////////////////////////////////////////////////////////////////////////
   // update viewPos movement only
   ///////////////////////////////////////////////////////////////////////////////
inline void Camera::updateMove(float frameTime)
{
    movingTime += frameTime;
    if (movingTime >= movingDuration)
    {
        setViewPos(movingTo);
        moving = false;
    }
    else
    {
        glm::vec3 p = Gil::interpolate(movingFrom, movingTo,
            movingTime / movingDuration, movingMode);
        setViewPos(p);
    }
};

///////////////////////////////////////////////////////////////////////////////
    // update viewTarget movement only
    ///////////////////////////////////////////////////////////////////////////////
inline void Camera::updateShift(float frameTime)
{
    shiftingTime += frameTime;

    // shift with duration
    if (shiftingDuration > 0)
    {
        if (shiftingTime >= shiftingDuration)
        {
            setViewTarget(shiftingTo);
            shifting = false;
        }
        else
        {
            glm::vec3 p = Gil::interpolate(shiftingFrom, shiftingTo,
                shiftingTime / shiftingDuration, shiftingMode);
            setViewTarget(p);
        }
    }
    // shift with acceleration
    else
    {
        shiftingSpeed = Gil::accelerate(shifting, shiftingSpeed,
            shiftingMaxSpeed, shiftingAccel, frameTime);
        setViewTarget(viewTarget + (shiftingVector * shiftingSpeed * frameTime));
    }
};

///////////////////////////////////////////////////////////////////////////////
   // update forward movement only
   ///////////////////////////////////////////////////////////////////////////////
inline void Camera::updateForward(float frameTime)
{
    forwardingTime += frameTime;

    // move forward for duration
    if (forwardingDuration > 0)
    {
        if (forwardingTime >= forwardingDuration)
        {
            setViewDistance(forwardingTo);
            forwarding = false;
        }
        else
        {
            float d = Gil::interpolate(forwardingFrom, forwardingTo,
                forwardingTime / forwardingDuration, forwardingMode);
            setViewDistance(d);
        }
    }

    // move forward with acceleration
    else
    {
        forwardingSpeed = Gil::accelerate(forwarding, forwardingSpeed,
            forwardingMaxSpeed, forwardingAccel, frameTime);
        setViewDistance(viewDistance - forwardingSpeed * frameTime);
    }
}

///////////////////////////////////////////////////////////////////////////////
// update rotation only
///////////////////////////////////////////////////////////////////////////////
inline void Camera::updateTurn(float frameTime)
{
    turningTime += frameTime;
    if (turningTime >= turningDuration)
    {
        if (quaternionUsed)
            setRotation(turningQuaternionTo);
        else
            setRotation(turningAngleTo);
        turning = false;
    }
    else
    {
        if (quaternionUsed)
        {
            glm::quat q = Gil::slerp(turningQuaternionFrom, turningQuaternionTo,
                turningTime / turningDuration, turningMode);
            setRotation(q);
        }
        else
        {
            glm::vec3 p = Gil::interpolate(turningAngleFrom, turningAngleTo,
                turningTime / turningDuration, turningMode);
            setRotation(p);
        }
    }
}