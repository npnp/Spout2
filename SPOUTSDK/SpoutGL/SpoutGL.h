/*

	SpoutGL.h
	
	Base class for OpenGL SpoutSDK
	See also Sender and Receiver wrapper classes.

	Copyright (c) 2021, Lynn Jarvis. All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, 
	are permitted provided that the following conditions are met:

		1. Redistributions of source code must retain the above copyright notice, 
		   this list of conditions and the following disclaimer.

		2. Redistributions in binary form must reproduce the above copyright notice, 
		   this list of conditions and the following disclaimer in the documentation 
		   and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"	AND ANY 
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE	ARE DISCLAIMED. 
	IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
*/
#pragma once

#ifndef __spoutGL__
#define __spoutGL__

// Change the path as required
#include "SpoutCommon.h" // for dll build and utilities
#include "SpoutSenderNames.h" // for sender creation and update
#include "SpoutDirectX.h" // for DX11 shared textures
#include "SpoutFrameCount.h" // for mutex lock and new frame signal
#include "SpoutCopy.h" // for pixel copy
#include "SpoutUtils.h" // Registry utiities
#include "SpoutGLextensions.h" // include last due to redefinition problems with OpenCL

#include <direct.h> // for _getcwd
#include <TlHelp32.h> // for PROCESSENTRY32
#include <tchar.h> // for _tcsicmp

using namespace spoututils;

class SPOUT_DLLEXP spoutGL {

	public:

	spoutGL();
    ~spoutGL();

	//
	// OpenGL shared texture access
	//

	// Bind OpenGL shared texture
	bool BindSharedTexture();
	// Un-bind OpenGL shared texture
	bool UnBindSharedTexture();
	// OpenGL shared texture ID
	GLuint GetSharedTextureID();

	//
	// Graphics compatibility
	//

	// Get auto GPU/CPU share depending on compatibility
	bool GetAutoShare();
	// Set auto GPU/CPU share depending on compatibility
	void SetAutoShare(bool bAuto = true);
	// OpenGL texture share compatibility
	bool IsGLDXready();

	//
	// User settings recorded by "SpoutSettings"
	//
	
	// Get user buffering mode
	bool GetBufferMode();
	// Set application buffering mode
	void SetBufferMode(bool bActive = true);
	// Get user number of pixel buffers
	int GetBuffers();
	// Set application number of pixel buffers
	void SetBuffers(int nBuffers);
	// Get user Maximum senders allowed
	int GetMaxSenders();
	// Set user Maximum senders allowed
	void SetMaxSenders(int maxSenders);

	//
	// 2.006 compatibility
	//

	// Get user DX9 mode
	bool GetDX9();
	// Set user DX9 mode
	bool SetDX9(bool bDX9 = true);
	// Get user memory share mode
	bool GetMemoryShareMode();
	// Set user memory share mode
	bool SetMemoryShareMode(bool bMem = true);
	// Get user CPU mode
	bool GetCPUmode();
	// Set user CPU mode
	bool SetCPUmode(bool bCPU);
	// Get user share mode
	//  0 - texture, 1 - memory, 2 - CPU
	int GetShareMode();
	// Set user share mode
	//  0 - texture, 1 - memory, 2 - CPU
	void SetShareMode(int mode);

	//
	// Information
	//

	// The path of the host that produced the sender
	bool GetHostPath(const char *sendername, char *hostpath, int maxchars);
	// Vertical sync status
	int GetVerticalSync();
	// Lock to monitor vertical sync
	bool SetVerticalSync(bool bSync = true);
	// Get Spout version
	int GetSpoutVersion();

	//
	// Utility
	//

	// Copy OpenGL texture with optional invert
	//   Textures must be the same size
	bool CopyTexture(GLuint SourceID, GLuint SourceTarget, GLuint DestID, GLuint DestTarget,
		unsigned int width, unsigned int height, bool bInvert = false, GLuint HostFBO = 0);
	// Correct for image stride
	void RemovePadding(const unsigned char *source, unsigned char *dest,
		unsigned int width, unsigned int height, unsigned int stride, GLenum glFormat = GL_RGBA);

	// DX11 texture read
	//  o Copy from the shared DX11 texture to a DX11 texture
	bool ReadTexture(ID3D11Texture2D** texture);
	// DX11 texture write
	//  o Copy a DX11 texture to the shared DX11 texture
	bool WriteTexture(ID3D11Texture2D** texture);
	// DX11 texture write with readback to OpenGL texture
	//   o Copy a DX11 texture to the DX11 shared texture
	//   o Copy the linked OpenGL texture back to an OpenGL texture
	bool WriteTextureReadback(ID3D11Texture2D** texture, GLuint TextureID, GLuint TextureTarget,
		unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO);

	// Initialize OpenGL and DX11
	//     o Load extensions and check for availability and function
	//     o Open DirectX and check for availability
	//     o Compatibility test for use or GL/DX interop
	//     o Optionally re-test compatibility even if already initialized
	bool OpenSpout(bool bRetest = false);
	// Initialize DirectX
	bool OpenDirectX();
	// Set sender DX11 shared texture format
	void SetDX11format(DXGI_FORMAT textureformat);
	// Close DirectX and free resources
	void CloseDirectX();
	// Create an OpenGL window and context for situations where there is none.
	//   Not used if applications already have an OpenGL context.
	//   Always call CloseOpenGL afterwards.
	bool CreateOpenGL();
	// Close OpenGL window
	bool CloseOpenGL();
	// Class initialization status
	bool IsSpoutInitialized();

	// Perform tests for GL/DX interop availability and compatibility
	bool GLDXready();


	// 2.006 compatibility
	bool OpenDirectX11();
	void CleanupDirectX();
	void CleanupDX11();
	void CleanupInterop();

	// OpenGL extensions
	bool LoadGLextensions();
	bool IsGLDXavailable(); // GL/DX interop extensions supported
	bool IsBLITavailable(); // fbo blit extensions available
	bool IsSWAPavailable(); // swap extensions available
	bool IsBGRAavailable(); // bgra extensions available
	bool IsCOPYavailable(); // copy extensions available
	bool IsPBOavailable();  // pbo extensions supported
	bool IsCONTEXTavailable(); // Context extension supported

	// Legacy OpenGL functions
	// See _SpoutCommon.h_ #define legacyOpenGL
#ifdef legacyOpenGL
	void SaveOpenGLstate(unsigned int width, unsigned int height, bool bFitWindow = true);
	void RestoreOpenGLstate();
#endif


	// Public for special use

	// Link a shared DirectX texture to an OpenGL texture
	HANDLE LinkGLDXtextures(void* pDXdevice, void* pSharedTexture, HANDLE dxShareHandle, GLuint glTextureID);
	// Return a handle to the the DX/GL interop device
	HANDLE GetInteropDevice();

	// For external access

	// DirectX 11 texture sharing
	spoutDirectX spoutdx;
	// Pixel buffer copying
	spoutCopy spoutcopy;
	// Spout sender management
	spoutSenderNames sendernames;
	// Frame counting management
	spoutFrameCount frame;

	//
	// Memory sharing - receive only
	//
	// Used if a receiver detects a zero share handle for an existing 2.006 sender.
	// Retained for 2.006 compatibility but may be removed for future release.
	//

	struct {

		SpoutSharedMemory *senderMem;
		unsigned int m_Width;
		unsigned int m_Height;

		// Open an existing named shared memory map
		bool OpenSenderMemory(const char *sendername)
		{
			std::string namestring = sendername;
			// Create a name for the map from the sender name
			namestring += "_map";
			// Create a new shared memory class object for this receiver
			if (!senderMem)
				senderMem = new SpoutSharedMemory();
			// Open the sender's shared memory map.
			// This also creates a mutex for the receiver
			// to lock and unlock the map for reads.
			if (!senderMem->Open(namestring.c_str())) {
				SpoutLogWarning("spoutGL.memoryshare::OpenSenderMemory - open shared memory failed");
				return false;
			}
			return true;
		} // end OpenSenderMemory

		//	Close the sender shared memory map
		void CloseSenderMemory()
		{
			if (senderMem) senderMem->Close();
			senderMem = nullptr;
			m_Width = 0;
			m_Height = 0;
		} // end CloseSenderMemory

		// Lock and unlock memory and retrieve buffer pointer - no size checks
		unsigned char * LockSenderMemory()
		{
			if (!senderMem) return nullptr;
			char *pBuf = senderMem->Lock();
			if (!pBuf) {
				// https://github.com/leadedge/Spout2/issues/15
				// senderMem->Unlock();
				return nullptr;
			}
			return (unsigned char *)pBuf;
		}

		void UnlockSenderMemory()
		{
			if (!senderMem) return;
			senderMem->Unlock();
		}

	} memoryshare;


protected :

	// GL/DX functions
	bool CreateInterop(unsigned int width, unsigned int height, DWORD dwFormat, bool bReceive);
	HRESULT LockInteropObject(HANDLE hDevice, HANDLE *hObject);
	HRESULT UnlockInteropObject(HANDLE hDevice, HANDLE *hObject);
	void CleanupGL(); // Free OpenGL resources

	// OpenGL texture create
	void CheckOpenGLTexture(GLuint &texID, GLenum GLformat, unsigned int width, unsigned int height);
	void InitTexture(GLuint &texID, GLenum GLformat, unsigned int width, unsigned int height);

	// OpenGL texture copy
	bool WriteGLDXtexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert = true, GLuint HostFBO = 0);
	bool ReadGLDXtexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert = false, GLuint HostFBO = 0);
	bool SetSharedTextureData(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO);
	bool GetSharedTextureData(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO);
	
	// OpenGL pixel copy
	bool WriteGLDXpixels(const unsigned char* pixels, unsigned int width, unsigned int height, GLenum glFormat = GL_RGBA, bool bInvert = false, GLuint HostFBO = 0);
	bool ReadGLDXpixels(unsigned char* pixels, unsigned int width, unsigned int height, GLenum glFormat = GL_RGBA, bool bInvert = false, GLuint HostFBO = 0);
	
	// PBOs for OpenGL pixel copy
	GLuint m_pbo[4];
	int PboIndex;
	int NextPboIndex;
	int m_nBuffers;
	bool UnloadTexturePixels(GLuint TextureID, GLuint TextureTarget,
		unsigned int width, unsigned int height, unsigned int pitch,
		unsigned char* data, GLenum glFormat = GL_RGBA,
		bool bInvert = false, GLuint HostFBO = 0);
	
	// OpenGL <-> DX11
	bool WriteDX11texture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO);
	bool ReadDX11texture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO);
	bool ReadTextureData(GLuint SourceID, GLuint SourceTarget, unsigned int width, unsigned int height, unsigned int pitch, unsigned char* dest, GLenum GLformat, bool bInvert, GLuint HostFBO);
	
	// Pixels <-> DX11
	bool WriteDX11pixels(const unsigned char* pixels, unsigned int width, unsigned int height, GLenum glFormat = GL_RGBA, bool bInvert = false);
	bool ReadDX11pixels(unsigned char * pixels, unsigned int width, unsigned int height, GLenum glFormat = GL_RGBA, bool bInvert = false);
	bool WritePixelData(const unsigned char* pixels, ID3D11Texture2D* pStagingTexture, unsigned int width, unsigned int height, GLenum glFormat, bool bInvert);
	bool ReadPixelData(ID3D11Texture2D* pStagingTexture, unsigned char* pixels, unsigned int width, unsigned int height, GLenum glFormat, bool bInvert);

	// Staging textures for DX11 CPU copy
	ID3D11Texture2D* m_pStaging[2];
	int m_Index;
	int m_NextIndex;
	bool CheckStagingTextures(unsigned int width, unsigned int height, int nTextures);

	// Shared memory - receive only
	bool ReadMemory(const char* sendername, GLuint TexID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert = false, GLuint HostFBO = 0);
	bool ReadMemoryPixels(const char* sendername, unsigned char* pixels, unsigned int width, unsigned int height, GLenum glFormat = GL_RGBA, bool bInvert = false);

	// Set host path to sender information
	bool SetHostPath(const char *sendername); 
	// Set GL/DX compatibility to sender information
	bool SetSenderCPUmode(const char *sendername, bool bCPU);

	// Utility
	bool OpenDeviceKey(const char* key, int maxsize, char* description, char* version);
	void trim(char* s);

	// Errors
	void PrintFBOstatus(GLenum status);
	bool GLerror();

	//
	// Class globals
	//

	// Sender/Receiver
	char m_SenderName[256];
	char m_SenderNameSetup[256];
	unsigned int m_Width;
	unsigned int m_Height;

	// General
	GLuint m_fbo; // Fbo used for OpenGL functions
	GLuint m_TexID; // Class texture used for invert copy
	unsigned int m_TexWidth;
	unsigned int m_TexHeight;
	DWORD m_TexFormat;
	GLuint m_glTexture; // OpenGL shared texture
	ID3D11Texture2D* m_pSharedTexture; // DirectX shared texture
	HANDLE m_dxShareHandle; // DirectX shared texture handle
	DWORD m_dwFormat; // DirectX shared texture format
	DXGI_FORMAT m_DX11format; // DirectX 11 texture format

	// GL/DX interop
	HANDLE m_hInteropDevice; // Handle to the DX/GL interop device
	HANDLE m_hInteropObject; // Handle to the DX/GL interop object (the shared texture)
	HWND m_hWnd; // OpenGL window
	int m_SpoutVersion; // Spout version

	// For CreateOpenGL and CloseOpenGL
	HDC m_hdc = NULL;
	HWND m_hwndButton = NULL;
	HGLRC m_hRc = NULL;

	// Status flags
	bool m_bConnected;
	bool m_bNewFrame;
	bool m_bUpdated;
	bool m_bInitialized;
	bool m_bMirror;  // Mirror image (used for SpoutCam)
	bool m_bSwapRB;  // RGB <> BGR (used for SpoutCam)

	// Sharing modes
	bool m_bAuto; // Auto share mode
	bool m_bUseGLDX; // GL/DX interop compatibility
	bool m_bSenderCPUmode; // Sender GL/DX compatibility

	bool m_bSpoutPanelOpened;
	bool m_bSpoutPanelActive;
	SHELLEXECUTEINFOA m_ShExecInfo;

	// OpenGL extensions
	unsigned int m_caps;
	bool m_bGLDXavailable;
	bool m_bFBOavailable;
	bool m_bBLITavailable;
	bool m_bPBOavailable;
	bool m_bSWAPavailable;
	bool m_bBGRAavailable;
	bool m_bCOPYavailable;
	bool m_bCONTEXTavailable;
	bool m_bExtensionsLoaded;

};

#endif
