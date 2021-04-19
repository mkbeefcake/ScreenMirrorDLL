#include "stdafx.h"


//// Driver types supported
//D3D_DRIVER_TYPE gDriverTypes[] =
//{
//	D3D_DRIVER_TYPE_HARDWARE
//	//D3D_DRIVER_TYPE_SOFTWARE 
//};
//UINT gNumDriverTypes = ARRAYSIZE(gDriverTypes);
//
//// Feature levels supported
//D3D_FEATURE_LEVEL gFeatureLevels[] =
//{
//	D3D_FEATURE_LEVEL_11_0,
//	D3D_FEATURE_LEVEL_10_1,
//	D3D_FEATURE_LEVEL_10_0,
//	D3D_FEATURE_LEVEL_9_1
//};
//
//UINT gNumFeatureLevels = ARRAYSIZE(gFeatureLevels);


	//d3dMonitors.ddMonitors = 0;

	// initialize capture method
	//memset(&monitorCapture, 0, sizeof(MonitorCapture));


	////
	//// Use Desktop Duplication method for getting multiple monitors
	////
	//bRet = DetectMultipleMonitors();
	//if (bRet == false) {
	//	
	//	// use GDI method
	//	monitorCapture.techStatus = GDI_BITBLT;
	//	monitorCapture.windowStatus = MONITOR_CAPTURE;
	//	monitorCapture.monitors.monitorNumber = 0;

	//}
	//else {

	//	// Create desktop duplication handle
	//	bRet = CreateDesktopDuplicationHandle();

	//	if (bRet == false) {

	//		monitorCapture.techStatus = GDI_BITBLT;
	//		monitorCapture.windowStatus = MONITOR_CAPTURE;
	//		monitorCapture.monitors.monitorNumber = 0;
	//	}
	//	else {

	//		monitorCapture.techStatus = DESKTOP_DUPLICATION_API;
	//		monitorCapture.windowStatus = MONITOR_CAPTURE;
	//		monitorCapture.monitors.monitorNumber = 0;
	//	}

	//}



	//if (d3dMonitors.lImmediateContext)
	//	d3dMonitors.lImmediateContext.Release();

	//if (d3dMonitors.lDevice)
	//	d3dMonitors.lDevice.Release();

	//d3dMonitors.lDevice = NULL;
	//d3dMonitors.lImmediateContext = NULL;
	//d3dMonitors.lDeskDupl = NULL;
	//d3dMonitors.lAcquiredDesktopImage = NULL;
	//d3dMonitors.lGDIImage = NULL;
	//d3dMonitors.lDestImage = NULL;

	// initialize capture method
	//memset(&monitorCapture, 0, sizeof(MonitorCapture));


//bool ScreenMirrorWrapper::CreateDesktopDuplicationHandle()
//{
//	HRESULT hr;
//	D3D_FEATURE_LEVEL lFeatureLevel;
//	for (UINT DriverTypeIndex = 0; DriverTypeIndex < gNumDriverTypes; ++DriverTypeIndex)
//	{
//		hr = D3D11CreateDevice(
//			nullptr,
//			gDriverTypes[DriverTypeIndex],
//			nullptr,
//			0,
//			gFeatureLevels,
//			gNumFeatureLevels,
//			D3D11_SDK_VERSION,
//			&d3dMonitors.lDevice,
//			&lFeatureLevel,
//			NULL);
//
//		if (SUCCEEDED(hr))
//		{
//			// Device creation success, no need to loop anymore
//			break;
//		}
//
//		d3dMonitors.lDevice.Release();
//	}
//
//	if (FAILED(hr))
//	{
//		PrintLog("ScreenMirrorWrapper::Initialize() - Failed to D3D11CreateDevice() 0x%x...\n", hr);
//		return false;
//	}
//
//	::Sleep(100);
//
//	if (d3dMonitors.lDevice == nullptr)
//		return false;
//
//	d3dMonitors.lDevice->GetImmediateContext(&d3dMonitors.lImmediateContext);
//
//	// Get DXGI device
//	CComPtrCustom<IDXGIDevice> lDxgiDevice;
//
//	hr = d3dMonitors.lDevice->QueryInterface(IID_PPV_ARGS(&lDxgiDevice));
//
//	if (FAILED(hr)) {
//		PrintLog("ScreenMirrorWrapper::Initialize() - Failed to query IDXGIDevice instance: 0x%x...\n", hr);
//		return false;
//	}
//
//	// Get DXGI adapter
//	CComPtrCustom<IDXGIAdapter> lDxgiAdapter;
//	hr = lDxgiDevice->GetParent(
//		__uuidof(IDXGIAdapter),
//		reinterpret_cast<void**>(&lDxgiAdapter));
//
//	if (FAILED(hr)) {
//		PrintLog("ScreenMirrorWrapper::Initialize() - Failed to query IDXGIAdapter instance: 0x%x...\n", hr);
//		return false;
//	}
//
//	lDxgiDevice.Release();
//
//	UINT Output = 0;
//
//	// Get output
//	CComPtrCustom<IDXGIOutput> lDxgiOutput;
//	hr = lDxgiAdapter->EnumOutputs(
//		Output,
//		&lDxgiOutput);
//
//	if (FAILED(hr)) {
//		PrintLog("ScreenMirrorWrapper::Initialize() - Failed to query IDXGIOutput instance: 0x%x...\n", hr);
//		return false;
//	}
//
//	lDxgiAdapter.Release();
//
//	hr = lDxgiOutput->GetDesc(
//		&d3dMonitors.lOutputDesc);
//
//	if (FAILED(hr))
//		return false;
//
//	// QI for Output 1
//	CComPtrCustom<IDXGIOutput1> lDxgiOutput1;
//
//	hr = lDxgiOutput->QueryInterface(IID_PPV_ARGS(&lDxgiOutput1));
//
//	if (FAILED(hr)) {
//		PrintLog("ScreenMirrorWrapper::Initialize() - Failed to query IDXGIOutput1 instance: 0x%x...\n", hr);
//		return false;
//	}
//
//	lDxgiOutput.Release();
//
//	// Create desktop duplication
//	hr = lDxgiOutput1->DuplicateOutput(
//		d3dMonitors.lDevice,
//		&d3dMonitors.lDeskDupl);
//
//	if (FAILED(hr)) {
//		PrintLog("ScreenMirrorWrapper::Initialize() - Failed to query IDXGIOutputDuplication instance: 0x%x...\n", hr);
//		return false;
//	}
//
//	lDxgiOutput1.Release();
//
//	// Create GUI drawing texture
//	d3dMonitors.lDeskDupl->GetDesc(&d3dMonitors.lOutputDuplDesc);
//
//	D3D11_TEXTURE2D_DESC desc;
//
//	// Create CPU access texture
//
//	desc.Width = d3dMonitors.lOutputDuplDesc.ModeDesc.Width;
//	desc.Height = d3dMonitors.lOutputDuplDesc.ModeDesc.Height;
//	desc.Format = d3dMonitors.lOutputDuplDesc.ModeDesc.Format;
//	desc.ArraySize = 1;
//	desc.BindFlags = 0;
//	desc.MiscFlags = 0;
//	desc.SampleDesc.Count = 1;
//	desc.SampleDesc.Quality = 0;
//	desc.MipLevels = 1;
//
//	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
//	desc.Usage = D3D11_USAGE_STAGING;
//
//	hr = d3dMonitors.lDevice->CreateTexture2D(&desc, NULL, &d3dMonitors.lDestImage);
//
//	if (FAILED(hr)) {
//		PrintLog("ScreenMirrorWrapper::Initialize() - Failed to query Texture2D instance: 0x%x...\n", hr);
//		return false;
//	}
//
//	if (d3dMonitors.lDestImage == nullptr)
//		return false;
//
//	return true;
//}

//bool ScreenMirrorWrapper::DetectMultipleMonitors()
//{
//	HRESULT hr = S_OK;
//
//	CComPtrCustom<IDXGIFactory1> factory;
//	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
//
//	if (FAILED(hr))
//	{
//		PrintLog("ScreenMirrorWrapper::Initialize() - CreateDXGIFactory1() failed: 0x%x...\n", hr);
//		return false;
//	}
//
//	int id = 0;
//	CComPtrCustom<IDXGIAdapter1> adapter;
//	for (int i = 0; (factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND); ++i)
//	{
//		CComPtrCustom<IDXGIOutput> output;
//		for (int j = 0; (adapter->EnumOutputs(j, &output) != DXGI_ERROR_NOT_FOUND); ++j)
//		{
//			id++;
//		}
//	}
//
//	d3dMonitors.ddMonitors = id;
//	return true;
//}









	//do {

	//	CComPtrCustom<IDXGIResource> lDesktopResource;
	//	DXGI_OUTDUPL_FRAME_INFO lFrameInfo;

	//	int lTryCount = 10;

	//	hr = d3dMonitors.lDeskDupl->ReleaseFrame();
	//	if (hr == DXGI_ERROR_INVALID_CALL)
	//	{
	//		PrintLog("ScreenMirrorWrapper::GetNextScreenFrame() - Already released IDXGIOutputDuplication frame. go on ...\n");
	//	}
	//	else if (hr == DXGI_ERROR_ACCESS_LOST)
	//	{
	//		PrintLog("ScreenMirrorWrapper::GetNextScreenFrame() - ERROR with DXGI_ERROR_ACCESS_LOST...\n");
	//		globalMutex.unlock();
	//		return -1;
	//	}
	//	else if (hr == DXGI_ERROR_WAIT_TIMEOUT)
	//	{
	//		::Sleep(100);
	//		PrintLog("ScreenMirrorWrapper::GetNextScreenFrame() - Wait Timeout (ReleaseFrame)...\n");
	//		continue;
	//	}

	//	do
	//	{
	//		Sleep(100);

	//		// Get new frame
	//		hr = d3dMonitors.lDeskDupl->AcquireNextFrame(
	//			100,
	//			&lFrameInfo,
	//			&lDesktopResource);

	//		if (SUCCEEDED(hr))
	//			break;
	//		
	//		if (hr == DXGI_ERROR_WAIT_TIMEOUT)
	//		{
	//			PrintLog("ScreenMirrorWrapper::GetNextScreenFrame() - Wait timeout (AcquireNextFrame)...\n");
	//			continue;
	//		}
	//		else if (FAILED(hr))
	//			break;

	//	} while (--lTryCount > 0);

	//	if (FAILED(hr)) {
	//		break;
	//	}

	//	// QI for ID3D11Texture2D

	//	hr = lDesktopResource->QueryInterface(IID_PPV_ARGS(&d3dMonitors.lAcquiredDesktopImage));

	//	if (FAILED(hr)) {
	//		PrintLog("ScreenMirrorWrapper::GetNextScreenFrame() - Failed to query AcquireDesktopImage ...\n");
	//		break;
	//	}

	//	lDesktopResource.Release();

	//	if (d3dMonitors.lAcquiredDesktopImage == nullptr)
	//		break;

	//	// Copy image into GDI drawing texture
	//	d3dMonitors.lImmediateContext->CopyResource(d3dMonitors.lDestImage, d3dMonitors.lAcquiredDesktopImage);

	//	// Copy from CPU access texture to bitmap buffer
	//	D3D11_MAPPED_SUBRESOURCE resource;
	//	UINT subresource = 0; //D3D11CalcSubresource(0, 0, 0);
	//	d3dMonitors.lImmediateContext->Map(d3dMonitors.lDestImage, subresource, D3D11_MAP_READ_WRITE, 0, &resource);

	//	UINT lBmpRowPitch = d3dMonitors.lOutputDuplDesc.ModeDesc.Width * 4;
	//	BYTE* sptr = reinterpret_cast<BYTE*>(resource.pData);

	//	if ((screenBufferSize >= lBmpRowPitch * d3dMonitors.lOutputDuplDesc.ModeDesc.Height) &&
	//		(screenBuffer != NULL))
	//	{			

	//		BYTE* dptr = (BYTE*)screenBuffer + d3dMonitors.lOutputDuplDesc.ModeDesc.Width
	//			* d3dMonitors.lOutputDuplDesc.ModeDesc.Height * 4 - lBmpRowPitch;

	//		UINT lRowPitch = std::min<UINT>(lBmpRowPitch, resource.RowPitch);
	//		for (size_t h = 0; h < d3dMonitors.lOutputDuplDesc.ModeDesc.Height; ++h)
	//		{
	//			memcpy_s(dptr, lBmpRowPitch, sptr, lRowPitch);
	//			sptr += resource.RowPitch;
	//			dptr -= lBmpRowPitch;
	//		}

	//		size = lBmpRowPitch * d3dMonitors.lOutputDuplDesc.ModeDesc.Height;

	//	}

	//	d3dMonitors.lImmediateContext->Unmap(d3dMonitors.lDestImage, subresource);

	//} while (false);

