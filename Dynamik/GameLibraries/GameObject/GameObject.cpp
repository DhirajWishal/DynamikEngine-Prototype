#include "dmkafx.h"
#include "GameObject.h"

/*
 GameObject data type for Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		19/06/2019	(DEVELOPMENTS ONGOING)
 IDE:		MS Visual Studio Community 2019

 GameObject.cpp file.
 */

 // Dynamik namespace
namespace Dynamik {

	// GameObject(objName, path, xPos, yPos, zPos);  
	GameObject::GameObject(std::string name, std::string path, float x, float y, float z) {
		myName = name;
		objFile.open(path, std::ios::in);
		TRFM->myPosition[TRFM->X] = x;
		TRFM->myPosition[TRFM->Y] = y;
		TRFM->myPosition[TRFM->Z] = z;

		if (!objFile.is_open())
			ERROR_DEFINE("The specified file is unavailable.");	// ERROR: The specified file is unavailable.
	}

	// GameObject("objName", "path", xPos, yPos, zPos);  
	GameObject::GameObject(const char* name, const  char* path, float x, float y, float z) {
		myName = name;
		objFile.open(path, std::ios::in);
		TRFM->myPosition[TRFM->X] = x;
		TRFM->myPosition[TRFM->Y] = y;
		TRFM->myPosition[TRFM->Z] = z;

		if (!objFile.is_open())
			ERROR_DEFINE("The specified file is unavailable.");	// ERROR: The specified file is unavailable.
	}

	// GameObject distructor  
	GameObject::~GameObject() {
		delete TRFM;
		delete CMR;
		delete LGHT;
		delete FLARE;
		delete AUDIO;
	}

	// initialize the GameObject  
	void GameObject::init() {
	}

	// draw/ render the object
	void GameObject::draw() {
	}

	// bindGameObject()  
	void GameObject::bindGameObject() {
	}

	// unbindGameObject();  
	void GameObject::unbindGameObject() {
	}

	/* ---------- ########## \\\\\\\\\\ TRANSFORM ////////// ########## ---------- */
	// initTransform(position[3], rotation[3], scale[3]);  
	void GameObject::initTransform(float* pos[3], float* rot[3], float* scl[3]) {
		TRFM->myPosition[TRFM->X] = *pos[TRFM->X];
		TRFM->myPosition[TRFM->Y] = *pos[TRFM->Y];
		TRFM->myPosition[TRFM->Z] = *pos[TRFM->Z];

		TRFM->myRotation[TRFM->X] = *rot[TRFM->X];
		TRFM->myRotation[TRFM->Y] = *rot[TRFM->Y];
		TRFM->myRotation[TRFM->Z] = *rot[TRFM->Z];

		TRFM->myScale[TRFM->X] = *scl[TRFM->X];
		TRFM->myScale[TRFM->Y] = *scl[TRFM->Y];
		TRFM->myScale[TRFM->Z] = *scl[TRFM->Z];
	}

	// setPosition(xPos, yPos, zPos); 
	void GameObject::setTransformPosition(float* x, float* y, float* z) {
		TRFM->myPosition[TRFM->X] = *x;
		TRFM->myPosition[TRFM->Y] = *y;
		TRFM->myPosition[TRFM->Z] = *z;
	}

	// setRotation(xPos, yPos, zPos);  
	void GameObject::setTransformRotation(float* x, float* y, float* z) {
		TRFM->myRotation[TRFM->X] = *x;
		TRFM->myRotation[TRFM->Y] = *y;
		TRFM->myRotation[TRFM->Z] = *z;
	}

	// setScale(xPos, yPos, zPos);  
	void GameObject::setTransformScale(float* x, float* y, float* z) {
		TRFM->myScale[TRFM->X] = *x;
		TRFM->myScale[TRFM->Y] = *y;
		TRFM->myScale[TRFM->Z] = *z;
	}

	/* ---------- ########## \\\\\\\\\\ CAMERA ////////// ########## ---------- */
	// initCamera(flags, backgroundColor[4], mask, projection, fov, depth, HDR, OcclusionCulling);  
	void GameObject::initCamera(uint8* flg, float* bgcl[4], uint8* msk,
		uint8* prj, int* fov, float* dpth, bool* hdr, bool* oc) {
		CMR->flags = *flg;

		CMR->background[CMR->RED] = *bgcl[CMR->RED];
		CMR->background[CMR->GREEN] = *bgcl[CMR->GREEN];
		CMR->background[CMR->BLUE] = *bgcl[CMR->BLUE];
		CMR->background[CMR->ALPHA] = *bgcl[CMR->ALPHA];

		CMR->mask = *msk;
		CMR->projection = *prj;
		CMR->FOV = *fov;
		CMR->depth = *dpth;
		CMR->isHDR = hdr;
		CMR->isOcclusionCulling = oc;
	}

	// setCameraFlags(flag); 
	void GameObject::setCameraFlags(uint8* flg) {
		CMR->flags = *flg;
	}

	// setBackgroundColor(r, g, b, a); 
	void GameObject::setCameraBackgroundColor(float* red, float* green, float* blue, float* alpha) {
		CMR->background[CMR->RED] = *red;
		CMR->background[CMR->GREEN] = *green;
		CMR->background[CMR->BLUE] = *blue;
		CMR->background[CMR->ALPHA] = *alpha;
	}

	// setCameraMask(mask); 
	void GameObject::setCameraMask(uint8* msk) {
		CMR->mask = *msk;
	}

	// setCameraProjection(peojection); 
	void GameObject::setCameraProjection(uint8* prj) {
		CMR->projection = *prj;
	}

	// setCameraFOV(FOVangle); 
	void GameObject::setCameraFOV(int* fov) {
		CMR->FOV = *fov;
	}

	// setCameraDepth(depth); 
	void GameObject::setCameraDepth(float* dpth) {
		CMR->depth = *dpth;
	}

	// setCameraHDR(HDR); 
	void GameObject::setCameraHDR(bool* hdr) {
		CMR->isHDR = *hdr;
	}

	// setCameraOcclusionCulling(cameraOcclusionCulling); 
	void GameObject::setCameraOcclusionCulling(bool* oc) {
		CMR->isOcclusionCulling = *oc;
	}

	/* ---------- ########## \\\\\\\\\\ AUDIO ////////// ########## ---------- */
		// initAudio(path, outputMode, priority, volume, pitch, stereoPan, mute, bypassEffects,\
					bypassListnerEffects, bypassReverbZones, playOnAwake, loop); 
	void GameObject::initAudio(std::string& path, uint8* out, uint8* pri, uint8* vol,
		uint8* pit, int8* sp, bool* mute, bool* be, bool* ble, bool* brz, bool* poa, bool* loop) {
		AUDIO->clipPath = path;
		AUDIO->output = *out;
		AUDIO->priority = *pri;
		AUDIO->volume = *vol;
		AUDIO->pitch = *pit;
		AUDIO->stereoPan = *sp;
		AUDIO->isMute = *mute;
		AUDIO->isBypassEffects = *be;
		AUDIO->isBypassListnerEffects = *ble;
		AUDIO->isBypassReverbZones = *brz;
		AUDIO->isPlayOnAwake = *poa;
		AUDIO->loop = *loop;
	}

	// setAudioClipPath(path); 
	void GameObject::setAudioClipPath(std::string& path) {
		AUDIO->clipPath = path;
	}

	// setAudioOutput(output); 
	void GameObject::setAudioOutput(uint8* out) {
		AUDIO->output = *out;
	}

	// setAudioPriority(priority); 
	void GameObject::setAudioPriority(uint8* pri) {
		AUDIO->priority = *pri;
	}

	// setAudioVolume(volume); 
	void GameObject::setAudioVolume(uint8* vol) {
		AUDIO->volume = *vol;
	}

	// setAudioPitch(pitch); 
	void GameObject::setAudioPitch(uint8* pit) {
		AUDIO->pitch = *pit;
	}

	// setAudioStereoPan(stereoPan); 
	void GameObject::setAudioStereoPan(int8* sp) {
		AUDIO->stereoPan = *sp;
	}

	// setAudioMute(mute); 
	void GameObject::setAudioMute(bool* mute) {
		AUDIO->isMute = *mute;
	}

	// setAudioBypassEffects(bypassStatus); 
	void GameObject::setAudioBypassEffects(bool* be) {
		AUDIO->isBypassEffects = *be;
	}

	// setAudioBypassListnerEffects(bypassStatus); 
	void GameObject::setAudioBypassListnerEffects(bool* ble) {
		AUDIO->isBypassListnerEffects = *ble;
	}

	// setAudioBypassReverbZones(bypassStatus); 
	void GameObject::setAudioBypassReverbZones(bool* brz) {
		AUDIO->isBypassReverbZones = *brz;
	}

	// setAudioPlayOnAwake(status); 
	void GameObject::setAudioPlayOnAwake(bool* poa) {
		AUDIO->isPlayOnAwake = *poa;
	}

	// setLoop(status); 
	void GameObject::setLoop(bool* loop) {
		AUDIO->loop = *loop;
	}

	// clear();
	void GameObject::clear() {
		TRFM->myPosition[TRFM->X] = DEFAULT_POSITION_X;
		TRFM->myPosition[TRFM->Y] = DEFAULT_POSITION_Y;
		TRFM->myPosition[TRFM->Z] = DEFAULT_POSITION_Z;

		TRFM->myRotation[TRFM->X] = DEFAULT_ROTATION_X;
		TRFM->myRotation[TRFM->Y] = DEFAULT_ROTATION_Y;
		TRFM->myRotation[TRFM->Z] = DEFAULT_ROTATION_Z;

		TRFM->myScale[TRFM->X] = DEFAULT_SCALE_X;
		TRFM->myScale[TRFM->Y] = DEFAULT_SCALE_Y;
		TRFM->myScale[TRFM->Z] = DEFAULT_SCALE_Z;
	}

	// EXCLUSIVELY FOR DYNAMIK ENGINE  
#if defined(DYNAMIK_ENGINE) || defined(TEST_BUILD) || defined(ENGINE_API) || defined(GRAPHICS_API)
	std::vector<float> GameObject::getObjectVertex() {
		return { 0.0f };
	}

	// ----------
#endif

}