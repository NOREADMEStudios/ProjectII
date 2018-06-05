#include "ModuleCinematics.h"
#include "Cinematic.h"
#include "ModuleInput.h"
#include <sstream>
#include "ModuleRender.h"
#include "ModuleAudio.h"

ModuleCinematics::ModuleCinematics()
{
	name = "cinematics";
}


ModuleCinematics::~ModuleCinematics()
{
}

bool ModuleCinematics::Awake(pugi::xml_node & config)
{

	assetsPath = ASSETS_ROOT;
	assetsPath.append(config.attribute("folder").as_string());
	
	return true;
}

bool ModuleCinematics::Start()
{
	PlayVideo("drop.avi");
	return true;
}

bool ModuleCinematics::PreUpdate()
{
	if (IsPlaying())
		App->input->BlockAllInput();

	return true;
}

bool ModuleCinematics::Update(float dt)
{
	if (currentCinematic != nullptr) {
		if (!currentCinematic->Update(dt)) {
			Utils::Release(currentCinematic);
			currentCinematic = nullptr;
		}
	}
	return true;
}

bool ModuleCinematics::PostUpdate()
{
	if (videoPlaying) {
		GetNextFrame();
	}
	else {
		//App->audio->PlayMusic("Assets/Audio/BGM/Character_Selection.ogg");
	}
	return true;
}

bool ModuleCinematics::CleanUp(pugi::xml_node &)
{
	if (currentCinematic != nullptr) {
		Utils::Release(currentCinematic);
		currentCinematic = nullptr;
	}
	
	return true;
}

void ModuleCinematics::StartCinematic(Cinematic * c)
{
	currentCinematic = c;
}

float ModuleCinematics::GetCurrentCinematicTime()
{
	if (currentCinematic != nullptr)
		return currentCinematic->GetDuration();
	else return 0.f;
}

float ModuleCinematics::GetCurrentCinematicTimeLeft()
{
	if (currentCinematic != nullptr)
		return currentCinematic->GetDurationLeft();
	else return 0.f;
}

iPoint ModuleCinematics::GetInitialCameraPosition()
{
	if (currentCinematic != nullptr)
		return currentCinematic->GetCurrentTransition()->initialFrame->cameraPosition;
	else return iPoint(-1, -1);
}

bool ModuleCinematics::PlayCurrentCinematic() //No use really
{
	return false;
}

bool ModuleCinematics::PlayVideo(const char* path)
{
	std::stringstream _path;
	_path << assetsPath.c_str() << path;

	AVIFileInit();

	long res = AVIStreamOpenFromFile(&videoStream, _path.str().c_str(), streamtypeVIDEO, 0, OF_READ, nullptr);

	if (res != 0) {
		LOG("Error opening video stream");
		return false;
	}

	AVISTREAMINFO psi;

	AVIStreamInfo(videoStream, &psi, sizeof(psi));

	videoWidth = psi.rcFrame.right - psi.rcFrame.left;
	videoHeight = psi.rcFrame.bottom - psi.rcFrame.top;
	videoLastFrame = AVIStreamLength(videoStream);
	videoTimeLength = AVIStreamLengthTime(videoStream);

	videoFrame = AVIStreamGetFrameOpen(videoStream, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);

	if (videoFrame == nullptr) {
		LOG("Could not read the stream");
		return false;
	}

	videoPlaying = true;
}

void ModuleCinematics::GetNextFrame()
{
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(videoFrame, videoCurrentFrame++);
	void* pixelData = lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);
	
	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(pixelData, videoWidth, videoHeight, lpbi->biBitCount, videoWidth * 3, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(App->render->renderer, surf);

	if (tex == nullptr) {
		LOG("Error rendering video: %s", SDL_GetError());
		return;
	}

	App->render->Blit(tex, 0, 0, nullptr, 0.0f);

	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);

	if (videoCurrentFrame >= videoLastFrame) {
		videoPlaying = false;
		videoCurrentFrame = 0;
		CloseVideo();
	}
}

void ModuleCinematics::CloseVideo()
{
	AVIStreamGetFrameClose(videoFrame);
	AVIStreamRelease(videoStream);
	AVIFileExit();
}